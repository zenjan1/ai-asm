/*
 * aiasm-aarch64/kernel/wasm_host.c
 * Wasm3 host integration v0.5: multi-module runtime, RAM disk filesystem,
 * module spawning, TAR parsing, aligned alloc/free.
 *
 * Multi-module design:
 *   - Single shared IM3Environment, per-module IM3Runtime
 *   - Module table (MAX_MODULES slots) tracks state, runtimes, entries
 *   - host_spawn registers a module (parse + link + compile), marks MOD_READY
 *   - host_exit marks module MOD_EXITED (does NOT halt CPU)
 *   - Kernel runs MOD_READY modules after previous module exits
 */

#include "wasm3.h"
#include "m3_env.h"
#include "m3_compile.h"
#include "module_runtime.h"

/* -------------------------------------------------------------------------- */
/* JIT Cache integration                                                      */
/* -------------------------------------------------------------------------- */

extern void jit_cache_init(void);
extern void *jit_cache_lookup(uint32_t name_hash, const char *name);
extern int jit_cache_store(uint32_t name_hash, const char *name,
                            const void *code, uint32_t size);

static uint32_t crc32_hash(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFF;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320 : 0);
    }
    return crc ^ 0xFFFFFFFF;
}

/* -------------------------------------------------------------------------- */
/* VFS integration externs (v11.0 — per-process fd tables)                    */
/* -------------------------------------------------------------------------- */
extern void vfs_init(void);
extern int  vfs_alloc_fd(int pid, int type, int flags);
extern int  vfs_free_fd(int pid, int fd);
extern int  vfs_set_ops(int pid, int fd, void *ops);
extern void *vfs_get_ops(int pid, int fd);
extern int  vfs_get_type(int pid, int fd);
extern int  vfs_read(int pid, int fd, void *buf, int len);
extern int  vfs_write(int pid, int fd, const void *buf, int len);
extern int  vfs_close(int pid, int fd);
extern int  vfs_poll(int pid, int fd);
extern void vfs_free_all(int pid);

/* Pipe externs (v15.0 — used in host_getc/host_exit) */
extern int pipe_create(int *, int *);
extern int pipe_read(int, uint8_t *, uint32_t);
extern int pipe_write(int, const uint8_t *, uint32_t);
extern int pipe_close(int);

/* Helper: get current module PID from IM3Runtime */
static int get_current_pid(IM3Runtime runtime)
{
    /* Scan module_table for matching runtime */
    for (int i = 0; i < 16; i++) {
        if (module_table[i].runtime == runtime && module_table[i].state != MOD_FREE)
            return (int)module_table[i].id;
    }
    return 0;  /* fallback to proc 0 */
}

/* -------------------------------------------------------------------------- */
/* UART helpers                                                               */
/* -------------------------------------------------------------------------- */

#define UART_BASE_ADDR 0x09000000UL
#define UART_DR   (*(volatile unsigned int *)(UART_BASE_ADDR + 0x000))
#define UART_FR   (*(volatile unsigned int *)(UART_BASE_ADDR + 0x018))
#define UART_FR_TXFF (1 << 5)
#define UART_FR_RXFE (1 << 4)

void uart_putc_raw(char c)
{
    while (UART_FR & UART_FR_TXFF)
        ;
    UART_DR = (unsigned int)c;
}

void uart_puts_raw(const char *s)
{
    while (*s)
        uart_putc_raw(*s++);
}

/* -------------------------------------------------------------------------- */
/* Kernel log: JSON error/info messages via UART                              */
/* -------------------------------------------------------------------------- */

static void kernel_log(const char *level, const char *event, const char *data)
{
    uart_puts_raw("{\"ts\":0,\"level\":\"");
    uart_puts_raw(level);
    uart_puts_raw("\",\"event\":\"");
    uart_puts_raw(event);
    if (data && *data) {
        uart_puts_raw("\",\"data\":\"");
        uart_puts_raw(data);
    }
    uart_puts_raw("\"}\n");
}

#define LOG_ERROR(event, data)  kernel_log("ERROR", (event), (data))
#define LOG_INFO(event, data)   kernel_log("INFO",  (event), (data))

/* -------------------------------------------------------------------------- */
/* String helpers (freestanding)                                              */
/* -------------------------------------------------------------------------- */

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b) ? 1 : 0;
}

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

IM3Environment g_shared_env = NULL;
wasm_module_slot_t module_table[MAX_MODULES];
uint32_t module_count = 0;
uint32_t next_module_id = 1;
uint32_t current_module_id = 0;

/* Preempt flag — set by IRQ handler (timer tick), cleared by scheduler loop */
extern volatile uint8_t preempt_pending;

ramdisk_file_t ramdisk_files[MAX_RAMDISK_FILES];
uint32_t ramdisk_file_count = 0;

static open_file_t open_files[MAX_OPEN_FILES];

/* -------------------------------------------------------------------------- */
/* Process isolation: per-module permission levels                            */
/* -------------------------------------------------------------------------- */

#define MAX_NAME_LEN        32

/* Per-module argv storage (v16.0) */
static char module_argv[MAX_MODULES][128];
static uint32_t module_argc[MAX_MODULES];  /* stored argv length */
static char pending_argv[128];
static uint32_t pending_argc = 0;

/* Permission levels: 0=root, 1=admin, 2=user, 3=guest */
static uint8_t module_perm_level[MAX_MODULES];

/* Get current running module's permission level */
static uint8_t get_current_perm(void)
{
    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == current_module_id &&
            module_table[i].state == 3 /* MOD_RUNNING */) {
            return module_perm_level[i];
        }
    }
    return 3; /* default: guest if not found */
}

/* Permission check for file operations: 0=read, 1=write, 2=delete, 3=create */
static int perm_check_file(int op)
{
    uint8_t level = get_current_perm();
    switch (level) {
        case 0: return 0; /* root: allow all */
        case 1: return (op == 2) ? -1 : 0; /* admin: deny delete */
        case 2: return (op >= 2) ? -1 : 0; /* user: deny delete/create */
        default: return (op == 0) ? 0 : -1; /* guest: read only */
    }
}

/* Permission check for network operations */
static int perm_check_net(void)
{
    uint8_t level = get_current_perm();
    return (level <= 2) ? 0 : -1; /* guest: no network */
}

/* Permission check for GUI operations (drawing) */
static int perm_check_gui_draw(void)
{
    uint8_t level = get_current_perm();
    return (level <= 2) ? 0 : -1; /* guest: no drawing */
}

/* -------------------------------------------------------------------------- */
/* Embedded WASM registry symbols (from ramdisk.asm)                          */
/* -------------------------------------------------------------------------- */

extern const uint8_t init_module_start[], init_module_end[];
extern const uint32_t init_module_size;
extern const uint8_t shell_module_start[], shell_module_end[];
extern const uint32_t shell_module_size;
extern const uint8_t test_module_start[], test_module_end[];
extern const uint32_t test_module_size;
extern const uint8_t proc_monitor_module_start[], proc_monitor_module_end[];
extern const uint32_t proc_monitor_module_size;
extern const uint8_t syslog_module_start[], syslog_module_end[];
extern const uint32_t syslog_module_size;
extern const uint8_t filemgr_module_start[], filemgr_module_end[];
extern const uint32_t filemgr_module_size;
extern const uint8_t settings_module_start[], settings_module_end[];
extern const uint32_t settings_module_size;
extern const uint8_t user_module_start[], user_module_end[];
extern const uint32_t user_module_size;
extern const uint8_t devmgr_module_start[], devmgr_module_end[];
extern const uint32_t devmgr_module_size;
extern const uint8_t httpd_module_start[], httpd_module_end[];
extern const uint32_t httpd_module_size;
extern const uint8_t dns_resolver_module_start[], dns_resolver_module_end[];
extern const uint32_t dns_resolver_module_size;
extern const uint8_t shmem_test_module_start[], shmem_test_module_end[];
extern const uint32_t shmem_test_module_size;
extern const uint8_t grep_module_start[], grep_module_end[];
extern const uint32_t grep_module_size;
extern const uint8_t cat_module_start[], cat_module_end[];
extern const uint32_t cat_module_size;
extern const uint8_t echo_module_start[], echo_module_end[];
extern const uint32_t echo_module_size;
extern const uint8_t tee_module_start[], tee_module_end[];
extern const uint32_t tee_module_size;
extern const uint8_t wc_module_start[], wc_module_end[];
extern const uint32_t wc_module_size;
extern const uint8_t head_module_start[], head_module_end[];
extern const uint32_t head_module_size;
extern const uint8_t tail_module_start[], tail_module_end[];
extern const uint32_t tail_module_size;
extern const uint8_t sort_module_start[], sort_module_end[];
extern const uint32_t sort_module_size;
extern const uint8_t uniq_module_start[], uniq_module_end[];
extern const uint32_t uniq_module_size;
extern const uint8_t tr_module_start[], tr_module_end[];
extern const uint32_t tr_module_size;
extern const uint8_t cut_module_start[], cut_module_end[];
extern const uint32_t cut_module_size;
extern const uint8_t sed_module_start[], sed_module_end[];
extern const uint32_t sed_module_size;
extern const uint8_t xargs_module_start[], xargs_module_end[];
extern const uint32_t xargs_module_size;
extern const uint8_t date_module_start[], date_module_end[];
extern const uint32_t date_module_size;
extern const uint8_t awk_module_start[], awk_module_end[];
extern const uint32_t awk_module_size;
extern const uint8_t ls_module_start[], ls_module_end[];
extern const uint32_t ls_module_size;
extern const uint8_t pwd_module_start[], pwd_module_end[];
extern const uint32_t pwd_module_size;
extern const uint8_t env_module_start[], env_module_end[];
extern const uint32_t env_module_size;
extern const uint8_t printf_module_start[], printf_module_end[];
extern const uint32_t printf_module_size;
extern const uint8_t sleep_module_start[], sleep_module_end[];
extern const uint32_t sleep_module_size;
extern const uint8_t yes_module_start[], yes_module_end[];
extern const uint32_t yes_module_size;
extern const uint8_t rev_module_start[], rev_module_end[];
extern const uint32_t rev_module_size;
extern const uint8_t cmp_module_start[], cmp_module_end[];
extern const uint32_t cmp_module_size;
extern const uint8_t diff_module_start[], diff_module_end[];
extern const uint32_t diff_module_size;
extern const uint8_t basename_module_start[], basename_module_end[];
extern const uint32_t basename_module_size;
extern const uint8_t dirname_module_start[], dirname_module_end[];
extern const uint32_t dirname_module_size;
extern const uint8_t nl_module_start[], nl_module_end[];
extern const uint32_t nl_module_size;
extern const uint8_t shuf_module_start[], shuf_module_end[];
extern const uint32_t shuf_module_size;
extern const uint8_t comm_module_start[], comm_module_end[];
extern const uint32_t comm_module_size;
extern const uint8_t paste_module_start[], paste_module_end[];
extern const uint32_t paste_module_size;
extern const uint8_t fold_module_start[], fold_module_end[];
extern const uint32_t fold_module_size;
extern const uint8_t tac_module_start[], tac_module_end[];
extern const uint32_t tac_module_size;
extern const uint8_t cksum_module_start[], cksum_module_end[];
extern const uint32_t cksum_module_size;
extern const uint8_t sum_module_start[], sum_module_end[];
extern const uint32_t sum_module_size;
extern const uint8_t touch_module_start[], touch_module_end[];
extern const uint32_t touch_module_size;
extern const uint8_t true_module_start[], true_module_end[];
extern const uint32_t true_module_size;
extern const uint8_t false_module_start[], false_module_end[];
extern const uint32_t false_module_size;
extern const uint8_t id_module_start[], id_module_end[];
extern const uint32_t id_module_size;
extern const uint8_t uptime_module_start[], uptime_module_end[];
extern const uint32_t uptime_module_size;
extern const uint8_t whoami_module_start[], whoami_module_end[];
extern const uint32_t whoami_module_size;
extern const uint8_t hostname_module_start[], hostname_module_end[];
extern const uint32_t hostname_module_size;
extern const uint8_t uname_module_start[], uname_module_end[];
extern const uint32_t uname_module_size;
extern const uint8_t df_module_start[], df_module_end[];
extern const uint32_t df_module_size;
extern const uint8_t free_module_start[], free_module_end[];
extern const uint32_t free_module_size;
extern const uint8_t ps_module_start[], ps_module_end[];
extern const uint32_t ps_module_size;
extern const uint8_t kill_module_start[], kill_module_end[];
extern const uint32_t kill_module_size;
extern const uint8_t mkdir_module_start[], mkdir_module_end[];
extern const uint32_t mkdir_module_size;
extern const uint8_t rmdir_module_start[], rmdir_module_end[];
extern const uint32_t rmdir_module_size;
extern const uint8_t ln_module_start[], ln_module_end[];
extern const uint32_t ln_module_size;
extern const uint8_t rm_module_start[], rm_module_end[];
extern const uint32_t rm_module_size;
extern const uint8_t cp_module_start[], cp_module_end[];
extern const uint32_t cp_module_size;
extern const uint8_t mv_module_start[], mv_module_end[];
extern const uint32_t mv_module_size;
extern const uint8_t chmod_module_start[], chmod_module_end[];
extern const uint32_t chmod_module_size;
extern const uint8_t tty_module_start[], tty_module_end[];
extern const uint32_t tty_module_size;
extern const uint8_t who_module_start[], who_module_end[];
extern const uint32_t who_module_size;
extern const uint8_t stty_module_start[], stty_module_end[];
extern const uint32_t stty_module_size;
extern const uint8_t tput_module_start[], tput_module_end[];
extern const uint32_t tput_module_size;
extern const uint8_t readlink_module_start[], readlink_module_end[];
extern const uint32_t readlink_module_size;
extern const uint8_t cal_module_start[], cal_module_end[];
extern const uint32_t cal_module_size;
extern const uint8_t find_module_start[], find_module_end[];
extern const uint32_t find_module_size;
extern const uint8_t du_module_start[], du_module_end[];
extern const uint32_t du_module_size;
extern const uint8_t mount_module_start[], mount_module_end[];
extern const uint32_t mount_module_size;
extern const uint8_t umount_module_start[], umount_module_end[];
extern const uint32_t umount_module_size;
extern const uint8_t ifconfig_module_start[], ifconfig_module_end[];
extern const uint32_t ifconfig_module_size;
extern const uint8_t ping_module_start[], ping_module_end[];
extern const uint32_t ping_module_size;
extern const uint8_t netstat_module_start[], netstat_module_end[];
extern const uint32_t netstat_module_size;
extern const uint8_t route_module_start[], route_module_end[];
extern const uint32_t route_module_size;
extern const uint8_t traceroute_module_start[], traceroute_module_end[];
extern const uint32_t traceroute_module_size;
extern const uint8_t nslookup_module_start[], nslookup_module_end[];
extern const uint32_t nslookup_module_size;
extern const uint8_t wget_module_start[], wget_module_end[];
extern const uint32_t wget_module_size;
extern const uint8_t curl_module_start[], curl_module_end[];
extern const uint32_t curl_module_size;
extern const uint8_t ssh_module_start[], ssh_module_end[];
extern const uint32_t ssh_module_size;
extern const uint8_t scp_module_start[], scp_module_end[];
extern const uint32_t scp_module_size;
extern const uint8_t tar_module_start[], tar_module_end[];
extern const uint32_t tar_module_size;
extern const uint8_t gzip_module_start[], gzip_module_end[];
extern const uint32_t gzip_module_size;
extern const uint8_t gunzip_module_start[], gunzip_module_end[];
extern const uint32_t gunzip_module_size;
extern const uint8_t zcat_module_start[], zcat_module_end[];
extern const uint32_t zcat_module_size;
extern const uint8_t bzip2_module_start[], bzip2_module_end[];
extern const uint32_t bzip2_module_size;
extern const uint8_t bunzip2_module_start[], bunzip2_module_end[];
extern const uint32_t bunzip2_module_size;
extern const uint8_t lzma_module_start[], lzma_module_end[];
extern const uint32_t lzma_module_size;
extern const uint8_t unlzma_module_start[], unlzma_module_end[];
extern const uint32_t unlzma_module_size;
extern const uint8_t xz_module_start[], xz_module_end[];
extern const uint32_t xz_module_size;
extern const uint8_t unxz_module_start[], unxz_module_end[];
extern const uint32_t unxz_module_size;
extern const uint8_t sha256sum_module_start[], sha256sum_module_end[];
extern const uint32_t sha256sum_module_size;
extern const uint8_t md5sum_module_start[], md5sum_module_end[];
extern const uint32_t md5sum_module_size;
extern const uint8_t base64_module_start[], base64_module_end[];
extern const uint32_t base64_module_size;
extern const uint8_t base64d_module_start[], base64d_module_end[];
extern const uint32_t base64d_module_size;
extern const uint8_t od_module_start[], od_module_end[];
extern const uint32_t od_module_size;
extern const uint8_t hexdump_module_start[], hexdump_module_end[];
extern const uint32_t hexdump_module_size;
extern const uint8_t strings_module_start[], strings_module_end[];
extern const uint32_t strings_module_size;
extern const uint8_t file_module_start[], file_module_end[];
extern const uint32_t file_module_size;
extern const uint8_t xxd_module_start[], xxd_module_end[];
extern const uint32_t xxd_module_size;
extern const uint8_t less_module_start[], less_module_end[];
extern const uint32_t less_module_size;
extern const uint8_t more_module_start[], more_module_end[];
extern const uint32_t more_module_size;
extern const uint8_t join_module_start[], join_module_end[];
extern const uint32_t join_module_size;
extern const uint8_t split_module_start[], split_module_end[];
extern const uint32_t split_module_size;
extern const uint8_t patch_module_start[], patch_module_end[];
extern const uint32_t patch_module_size;
extern const uint8_t diff3_module_start[], diff3_module_end[];
extern const uint32_t diff3_module_size;
extern const uint8_t tsort_module_start[], tsort_module_end[];
extern const uint32_t tsort_module_size;
extern const uint8_t csplit_module_start[], csplit_module_end[];
extern const uint32_t csplit_module_size;
extern const uint8_t fmt_module_start[], fmt_module_end[];
extern const uint32_t fmt_module_size;
extern const uint8_t pr_module_start[], pr_module_end[];
extern const uint32_t pr_module_size;
extern const uint8_t column_module_start[], column_module_end[];
extern const uint32_t column_module_size;
extern const uint8_t expand_module_start[], expand_module_end[];
extern const uint32_t expand_module_size;
extern const uint8_t unexpand_module_start[], unexpand_module_end[];
extern const uint32_t unexpand_module_size;

static wasm_registry_entry_t wasm_registry[] = {
    { "init",           NULL, 0 },
    { "shell",          NULL, 0 },
    { "test",           NULL, 0 },
    { "proc_monitor",   NULL, 0 },
    { "syslog",         NULL, 0 },
    { "filemgr",        NULL, 0 },
    { "settings",       NULL, 0 },
    { "user",           NULL, 0 },
    { "awk",            NULL, 0 },
    { "date",           NULL, 0 },
    { "devmgr",         NULL, 0 },
    { "httpd",          NULL, 0 },
    { "dns_resolver",   NULL, 0 },
    { "shmem_test",     NULL, 0 },
    { "grep",           NULL, 0 },
    { "cat",            NULL, 0 },
    { "echo",           NULL, 0 },
    { "tee",            NULL, 0 },
    { "wc",             NULL, 0 },
    { "head",           NULL, 0 },
    { "tail",           NULL, 0 },
    { "sort",           NULL, 0 },
    { "uniq",           NULL, 0 },
    { "tr",             NULL, 0 },
    { "cut",            NULL, 0 },
    { "sed",            NULL, 0 },
    { "xargs",          NULL, 0 },
    { "ls",             NULL, 0 },
    { "pwd",            NULL, 0 },
    { "env",            NULL, 0 },
    { "printf",         NULL, 0 },
    { "sleep",          NULL, 0 },
    { "yes",            NULL, 0 },
    { "rev",            NULL, 0 },
    { "cmp",            NULL, 0 },
    { "diff",           NULL, 0 },
    { "basename",       NULL, 0 },
    { "dirname",        NULL, 0 },
    { "nl",             NULL, 0 },
    { "shuf",           NULL, 0 },
    { "comm",           NULL, 0 },
    { "paste",          NULL, 0 },
    { "fold",           NULL, 0 },
    { "tac",            NULL, 0 },
    { "cksum",          NULL, 0 },
    { "sum",            NULL, 0 },
    { "touch",          NULL, 0 },
    { "true",           NULL, 0 },
    { "false",          NULL, 0 },
    { "id",             NULL, 0 },
    { "uptime",         NULL, 0 },
    { "whoami",         NULL, 0 },
    { "hostname",       NULL, 0 },
    { "uname",          NULL, 0 },
    { "df",             NULL, 0 },
    { "free",           NULL, 0 },
    { "ps",             NULL, 0 },
    { "kill",           NULL, 0 },
    { "mkdir",          NULL, 0 },
    { "rmdir",          NULL, 0 },
    { "ln",             NULL, 0 },
    { "rm",             NULL, 0 },
    { "cp",             NULL, 0 },
    { "mv",             NULL, 0 },
    { "chmod",          NULL, 0 },
    { "tty",            NULL, 0 },
    { "who",            NULL, 0 },
    { "stty",           NULL, 0 },
    { "tput",           NULL, 0 },
    { "readlink",       NULL, 0 },
    { "cal",            NULL, 0 },
    { "find",           NULL, 0 },
    { "du",             NULL, 0 },
    { "mount",          NULL, 0 },
    { "umount",         NULL, 0 },
    { "ifconfig",       NULL, 0 },
    { "ping",           NULL, 0 },
    { "netstat",        NULL, 0 },
    { "route",          NULL, 0 },
    { "traceroute",     NULL, 0 },
    { "nslookup",       NULL, 0 },
    { "wget",           NULL, 0 },
    { "curl",           NULL, 0 },
    { "ssh",            NULL, 0 },
    { "scp",            NULL, 0 },
    { "tar",            NULL, 0 },
    { "gzip",           NULL, 0 },
    { "gunzip",         NULL, 0 },
    { "zcat",           NULL, 0 },
    { "bzip2",          NULL, 0 },
    { "bunzip2",        NULL, 0 },
    { "lzma",           NULL, 0 },
    { "unlzma",         NULL, 0 },
    { "xz",             NULL, 0 },
    { "unxz",           NULL, 0 },
    { "sha256sum",      NULL, 0 },
    { "md5sum",         NULL, 0 },
    { "base64",         NULL, 0 },
    { "base64d",        NULL, 0 },
    { "od",             NULL, 0 },
    { "hexdump",        NULL, 0 },
    { "strings",        NULL, 0 },
    { "file",           NULL, 0 },
    { "xxd",            NULL, 0 },
    { "less",           NULL, 0 },
    { "more",           NULL, 0 },
    { "join",           NULL, 0 },
    { "split",          NULL, 0 },
    { "patch",          NULL, 0 },
    { "diff3",          NULL, 0 },
    { "tsort",          NULL, 0 },
    { "csplit",         NULL, 0 },
    { "fmt",            NULL, 0 },
    { "pr",             NULL, 0 },
    { "column",         NULL, 0 },
    { "expand",         NULL, 0 },
    { "unexpand",       NULL, 0 },
};
#define WASM_REGISTRY_COUNT (sizeof(wasm_registry) / sizeof(wasm_registry[0]))

/* -------------------------------------------------------------------------- */
/* RAM disk TAR parser                                                        */
/* -------------------------------------------------------------------------- */

static unsigned int octal_to_uint(const char *s, unsigned int len)
{
    unsigned int result = 0;
    for (unsigned int i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '7') break;
        result = (result << 3) | (s[i] - '0');
    }
    return result;
}

void ramdisk_init(const uint8_t *data, uint32_t size)
{
    ramdisk_files[0].name[0] = '\0';
    ramdisk_file_count = 0;
    open_files[0].active = 0;

    if (!data || size < 512) return;

    uint32_t pos = 0;
    int file_num = 0;
    while (pos + 512 <= size) {
        const uint8_t *hdr = data + pos;

        /* Check for end-of-archive (two zero blocks) */
        int is_zero = 1;
        for (int i = 0; i < 512; i++) {
            if (hdr[i] != 0) { is_zero = 0; break; }
        }
        if (is_zero) {
            break;
        }

        /* Parse USTAR header */

        /* Parse USTAR header */
        char name[101];
        unsigned int namelen = 0;
        for (unsigned int i = 0; i < 100; i++) {
            if (hdr[i] == '\0') break;
            name[namelen++] = (char)hdr[i];
        }
        name[namelen] = '\0';

        char typeflag = (char)hdr[156];
        if (typeflag != '0' && typeflag != '\0') {
            /* Not a regular file, skip */
            uint32_t fsize = octal_to_uint((const char *)(hdr + 124), 12);
            uint32_t blocks = (fsize + 511) / 512;
            pos += 512 + blocks * 512;
            continue;
        }

        uint32_t fsize = octal_to_uint((const char *)(hdr + 124), 12);

        if (ramdisk_file_count < MAX_RAMDISK_FILES) {
            ramdisk_file_t *f = &ramdisk_files[ramdisk_file_count];
            unsigned int slen = namelen < (MAX_PATH_LEN - 1) ? namelen : (MAX_PATH_LEN - 1);
            for (unsigned int i = 0; i < slen; i++) f->name[i] = name[i];
            f->name[slen] = '\0';
            f->offset = pos + 512;  /* data starts after header */
            f->size = fsize;
            ramdisk_file_count++;
        }

        uint32_t blocks = (fsize + 511) / 512;
        pos += 512 + blocks * 512;
    }

    LOG_INFO("ramdisk", "files indexed");
}

int ramdisk_open(const char *path, uint32_t path_len)
{
    /* Null-terminate path */
    char tmp[MAX_PATH_LEN];
    if (path_len >= MAX_PATH_LEN) path_len = MAX_PATH_LEN - 1;
    for (uint32_t i = 0; i < path_len; i++) tmp[i] = path[i];
    tmp[path_len] = '\0';

    /* Find file */
    for (uint32_t i = 0; i < ramdisk_file_count; i++) {
        if (my_strcmp(ramdisk_files[i].name, tmp) == 0) {
            /* Allocate fd slot */
            for (int j = 0; j < MAX_OPEN_FILES; j++) {
                if (!open_files[j].active) {
                    open_files[j].active = 1;
                    open_files[j].file_index = (int)i;
                    open_files[j].position = 0;
                    return j;
                }
            }
            return -2;  /* no free fd */
        }
    }
    return -1;  /* not found */
}

int ramdisk_read(int fd, uint8_t *buf, uint32_t len)
{
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd].active)
        return -1;

    int fi = open_files[fd].file_index;
    ramdisk_file_t *f = &ramdisk_files[fi];

    extern const uint8_t ramdisk_start[];
    uint32_t remaining = f->size - open_files[fd].position;
    if (len > remaining) len = remaining;
    if (len == 0) return 0;

    const uint8_t *src = ramdisk_start + f->offset + open_files[fd].position;
    for (uint32_t i = 0; i < len; i++)
        buf[i] = src[i];

    open_files[fd].position += len;
    return (int)len;
}

void ramdisk_close(int fd)
{
    if (fd >= 0 && fd < MAX_OPEN_FILES)
        open_files[fd].active = 0;
}

int ramdisk_list(uint8_t *buf, uint32_t max_len)
{
    uint32_t written = 0;
    for (uint32_t i = 0; i < ramdisk_file_count; i++) {
        const char *name = ramdisk_files[i].name;
        for (unsigned int j = 0; name[j]; j++) {
            if (written >= max_len) return (int)written;
            buf[written++] = (uint8_t)name[j];
        }
        if (written < max_len)
            buf[written++] = '\0';  /* null separator */
    }
    return (int)written;
}

/* -------------------------------------------------------------------------- */
/* Module management                                                          */
/* -------------------------------------------------------------------------- */

static int find_module_slot(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].state == MOD_FREE) return i;
    }
    return -1;
}

static wasm_registry_entry_t *find_registry_entry(const char *name)
{
    for (unsigned int i = 0; i < WASM_REGISTRY_COUNT; i++) {
        if (my_strcmp(wasm_registry[i].name, name) == 0)
            return &wasm_registry[i];
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Forward declarations                                                       */
/* -------------------------------------------------------------------------- */

static const char *load_module_internal(int slot_idx, const char *name,
                                         const uint8_t *wasm_bytes, uint32_t wasm_size);

/* -------------------------------------------------------------------------- */
/* Host function implementations                                              */
/* -------------------------------------------------------------------------- */

/* host_print(offset, len) — print WASM linear memory to UART or pipe stdout */
static const void *host_print(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t offset = (uint32_t)*_sp;
    uint32_t len    = (uint32_t)*(_sp + 1);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (offset + len > mem_size) {
        LOG_ERROR("wasm_print", "out of bounds memory access");
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Check if current module has stdout redirected to a pipe */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        wasm_module_slot_t *slot = &module_table[idx];
        if (slot->stdout_pipe_fd >= 0 && slot->runtime == runtime) {
            /* Write to pipe stdout */
            uint8_t *p = mem + offset;
            extern int pipe_write(int, const uint8_t *, uint32_t);
            pipe_write(slot->stdout_pipe_fd, p, len);
            return m3Err_none;
        }
    }

    /* Default: print to UART */
    uint8_t *p = mem + offset;
    for (uint32_t i = 0; i < len; i++)
        uart_putc_raw((char)p[i]);

    return m3Err_none;
}

/* host_get_tick() — return ARM CNTVCT_EL0 (i64) */
static const void *host_get_tick(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint64_t *ret = (uint64_t *)_sp++;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(*ret));
    return m3Err_none;
}

/* host_exit(code) — mark module exited (does NOT halt CPU) */
static const void *host_exit(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t code = (int32_t)(int64_t)*_sp++;

    if (current_module_id > 0 && current_module_id < (uint32_t)(MAX_MODULES + 1)) {
        uint32_t idx = current_module_id - 1;
        if (idx < MAX_MODULES && module_table[idx].state != MOD_FREE) {
            /* VFS: release all fds for this process (v12.0) */
            vfs_free_all((int)module_table[idx].id);

            /* Close pipe fds for stdin/stdout redirection (v15.0) */
            if (module_table[idx].stdin_pipe_fd >= 0) {
                extern int pipe_close(int);
                pipe_close(module_table[idx].stdin_pipe_fd);
                module_table[idx].stdin_pipe_fd = -1;
            }
            if (module_table[idx].stdout_pipe_fd >= 0) {
                pipe_close(module_table[idx].stdout_pipe_fd);
                module_table[idx].stdout_pipe_fd = -1;
            }

            module_table[idx].exit_code = (int)code;
            module_table[idx].state = MOD_EXITED;

            uart_puts_raw("\n[WASM exit ");
            if (code == 0) uart_puts_raw("ok");
            else { uart_puts_raw("code="); if (code < 0) { uart_putc_raw('-'); code = -code; } char buf[12]; int i = 0; do { buf[i++] = (char)('0' + (code % 10)); code /= 10; } while (code > 0); while (i > 0) uart_putc_raw(buf[--i]); }
            uart_puts_raw("]\n");
        }
    } else {
        /* No valid current module, halt */
        uart_puts_raw("\n[WASM exit ");
        if (code == 0) uart_puts_raw("ok");
        else { uart_puts_raw("code="); if (code < 0) { uart_putc_raw('-'); code = -code; } char buf[12]; int i = 0; do { buf[i++] = (char)('0' + (code % 10)); code /= 10; } while (code > 0); while (i > 0) uart_putc_raw(buf[--i]); }
        uart_puts_raw("]\n");
        while (1) __asm__ volatile("wfi");
    }

    return m3Err_none;
}

/* host_alloc(size, align) — allocate from WASM linear memory */
static const void *host_alloc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t size  = (uint32_t)*(_sp + 1);
    uint32_t align = (uint32_t)*(_sp + 2);
    (void)align;  /* WASM memory is already page-aligned */

    /* Allocate from the end of WASM linear memory (growing downward) */
    uint32_t mem_size = m3_GetMemorySize(runtime);
    uint32_t *heap_pos_ptr = (uint32_t *)((uint8_t *)_mem + mem_size - 4);
    uint32_t heap_pos = *heap_pos_ptr;
    if (heap_pos == 0) {
        /* First allocation: initialize heap pointer to end of memory */
        heap_pos = mem_size;
    }
    if (heap_pos < size) {
        uint32_t *ret = (uint32_t *)(_sp);
        *ret = 0;  /* OOM */
        return m3Err_none;
    }
    heap_pos -= size;
    heap_pos &= ~(uint32_t)15;  /* 16-byte align */
    *heap_pos_ptr = heap_pos;

    uint32_t *ret = (uint32_t *)(_sp);
    *ret = heap_pos;  /* return offset within WASM memory */
    return m3Err_none;
}

/* host_free(ptr) — no-op (WASM memory is managed by runtime) */
static const void *host_free(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    /* No-op: WASM linear memory is managed by the runtime, not the kernel */
    return m3Err_none;
}

/* host_sleep(ms) — busy-wait delay, yields on timer IRQ */
static const void *host_sleep(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t ms = (uint32_t)(uint64_t)*_sp++;

    /* Use timer_delay_ms from kernel — it uses cntvct_el0 polling */
    /* In freestanding mode, we call it via inline asm since it's in memory.asm */
    uint64_t start;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(start));

    /* Get frequency */
    uint64_t freq;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    uint64_t target = start + (ms * freq / 1000);
    uint64_t now;
    do {
        __asm__ volatile("wfi");
        __asm__ volatile("mrs %0, cntvct_el0" : "=r"(now));
        /* If preempt_pending is set, yield back to scheduler */
        if (preempt_pending) {
            /* Clear preempt and return early — scheduler will resume us */
            preempt_pending = 0;
            break;
        }
    } while (now < target);

    return m3Err_none;
}

/* host_yield() — return control to scheduler immediately */
static const void *host_yield(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;

    /* Set preempt flag so scheduler loop will run next module */
    preempt_pending = 1;

    /* Wait until scheduler clears the flag (next tick) */
    while (preempt_pending)
        __asm__ volatile("wfi");

    return m3Err_none;
}

/* host_log(level, msg) — log message to kernel JSON log */
static const void *host_log(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t level_off = (uint32_t)*_sp++;
    uint32_t level_len = (uint32_t)*_sp++;
    uint32_t msg_off   = (uint32_t)*_sp++;
    uint32_t msg_len   = (uint32_t)*_sp++;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (level_off + level_len > mem_size || msg_off + msg_len > mem_size)
        return m3Err_trapOutOfBoundsMemoryAccess;

    char level_saved = (char)mem[level_off + level_len];
    char msg_saved   = (char)mem[msg_off   + msg_len];
    mem[level_off + level_len] = '\0';
    mem[msg_off   + msg_len]   = '\0';

    kernel_log((const char *)(mem + level_off), "wasm_log",
               (const char *)(mem + msg_off));

    mem[level_off + level_len] = level_saved;
    mem[msg_off   + msg_len]   = msg_saved;

    return m3Err_none;
}

/* host_getc() — non-blocking read one character from UART (yields on preempt) */
static const void *host_getc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    int32_t *ret = (int32_t *)(_sp);

    /* Check if current module has stdin redirected to a pipe */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        wasm_module_slot_t *slot = &module_table[idx];
        if (slot->stdin_pipe_fd >= 0 && slot->runtime == runtime) {
            /* Read from pipe stdin */
            uint8_t ch;
            uint32_t mem_size = m3_GetMemorySize(runtime);
            if (mem_size < 16) {
                *ret = -1;
                return m3Err_none;
            }
            int n = pipe_read(slot->stdin_pipe_fd, &ch, 1);
            if (n > 0) {
                *ret = (int32_t)ch;
            } else {
                /* Pipe empty — check if writer is still active (v19.0) */
                extern int pipe_is_ready(int);
                if (pipe_is_ready(slot->stdin_pipe_fd)) {
                    *ret = 0;  /* no data yet, caller should retry */
                } else {
                    *ret = -1;  /* EOF — writer closed */
                }
            }
            return m3Err_none;
        }
    }

    /* Default: read from UART */
    /* Check preemption flag — timer IRQ may have fired */
    if (preempt_pending) {
        /* Wait for next IRQ (scheduler loop will clear preempt_pending) */
        while (preempt_pending)
            __asm__ volatile("wfi");
    }

    while (UART_FR & UART_FR_RXFE) {
        /* Re-check preempt during wait */
        if (preempt_pending) {
            while (preempt_pending)
                __asm__ volatile("wfi");
        }
        __asm__ volatile("wfi");
    }

    uint8_t ch = (uint8_t)(UART_DR & 0xff);
    *ret = (int32_t)ch;
    return m3Err_none;
}

/* host_spawn(name_ptr, name_len) — register a module, return module_id */
static const void *host_spawn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t name_off = (uint32_t)*(_sp + 1);
    uint32_t name_len = (uint32_t)*(_sp + 2);
    int32_t *ret_val = (int32_t *)(_sp);
    _sp += 3;  /* consume return slot + 2 params */

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size) {
        *ret_val = -1;
        return m3Err_none;
    }

    /* Copy name to temp buffer */
    char name_buf[64];
    if (name_len >= 64) name_len = 63;
    for (uint32_t i = 0; i < name_len; i++) name_buf[i] = (char)mem[name_off + i];
    name_buf[name_len] = '\0';

    uart_puts_raw("[spawn] ");
    uart_puts_raw(name_buf);
    uart_puts_raw("... ");

    /* Look up in registry */
    wasm_registry_entry_t *entry = find_registry_entry(name_buf);
    if (!entry) {
        uart_puts_raw("unknown\n");
        *ret_val = -1;
        return m3Err_none;
    }

    /* Find free slot */
    int slot = find_module_slot();
    if (slot < 0) {
        uart_puts_raw("no slot\n");
        *ret_val = -2;
        return m3Err_none;
    }

    /* Load module into slot */
    const char *err = load_module_internal(slot, entry->name, entry->wasm_bytes, entry->wasm_size);
    if (err) {
        uart_puts_raw("error: ");
        uart_puts_raw(err);
        uart_puts_raw("\n");
        *ret_val = -3;
        return m3Err_none;
    }

    /* Save argv for the spawned module (v16.0 / v17.0) */
    if (pending_argc > 0) {
        /* Use pending argv set by host_set_argv (shell pipe mode) */
        uint32_t argv_len = pending_argc;
        if (argv_len >= 127) argv_len = 127;
        for (uint32_t i = 0; i < argv_len; i++)
            module_argv[slot][i] = pending_argv[i];
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
        pending_argc = 0;  /* clear pending */
    } else {
        /* Use module name as argv (direct spawn mode) */
        uint32_t argv_len = name_len;
        if (argv_len >= 127) argv_len = 127;
        for (uint32_t i = 0; i < argv_len; i++)
            module_argv[slot][i] = (char)mem[name_off + i];
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
    }

    uart_puts_raw("ok\n");
    *ret_val = (int32_t)module_table[slot].id;
    return m3Err_none;
}

/* host_spawn_redirect(name_ptr, name_len, stdin_fd, stdout_fd)
 * Spawn a module and set its stdin/stdout pipe fds for pipeline support (v15.0)
 * stdin_fd: pipe fd to read from (-1 = UART)
 * stdout_fd: pipe fd to write to (-1 = UART)
 * Returns: module_id or negative error
 */
static const void *host_spawn_redirect(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t name_off   = (uint32_t)*(_sp + 1);
    uint32_t name_len   = (uint32_t)*(_sp + 2);
    int32_t stdin_fd    = (int32_t)(int64_t)*(_sp + 3);
    int32_t stdout_fd   = (int32_t)(int64_t)*(_sp + 4);
    int32_t *ret_val    = (int32_t *)(_sp);
    _sp += 5;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size) {
        *ret_val = -1;
        return m3Err_none;
    }

    /* Copy name to temp buffer */
    char name_buf[64];
    if (name_len >= 64) name_len = 63;
    for (uint32_t i = 0; i < name_len; i++) name_buf[i] = (char)mem[name_off + i];
    name_buf[name_len] = '\0';

    /* Look up in registry */
    wasm_registry_entry_t *entry = find_registry_entry(name_buf);
    if (!entry) {
        *ret_val = -1;
        return m3Err_none;
    }

    /* Find free slot */
    int slot = find_module_slot();
    if (slot < 0) {
        *ret_val = -2;
        return m3Err_none;
    }

    /* Load module into slot */
    const char *err = load_module_internal(slot, entry->name, entry->wasm_bytes, entry->wasm_size);
    if (err) {
        *ret_val = -3;
        return m3Err_none;
    }

    /* Save argv for the spawned module (v16.0 / v17.0) */
    if (pending_argc > 0) {
        uint32_t argv_len = pending_argc;
        if (argv_len >= 127) argv_len = 127;
        for (uint32_t i = 0; i < argv_len; i++)
            module_argv[slot][i] = pending_argv[i];
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
        pending_argc = 0;
    } else {
        uint32_t argv_len = name_len;
        if (argv_len >= 127) argv_len = 127;
        for (uint32_t i = 0; i < argv_len; i++)
            module_argv[slot][i] = (char)mem[name_off + i];
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
    }

    /* Set stdin/stdout pipe fds for redirection */
    module_table[slot].stdin_pipe_fd = (int)stdin_fd;
    module_table[slot].stdout_pipe_fd = (int)stdout_fd;

    *ret_val = (int32_t)module_table[slot].id;
    return m3Err_none;
}

/* host_get_argv(buf_off, max_len) — copy current module's argv to WASM memory
 * Returns: actual length copied, or -1 if no argv
 */
static const void *host_get_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx;
    uint32_t buf_off  = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t max_len  = (uint32_t)*(uint64_t*)(_sp + 2);
    int32_t *ret      = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + 1 > mem_size) {
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Find current module slot */
    int slot_idx = -1;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == current_module_id && module_table[i].state != MOD_FREE) {
            slot_idx = i;
            break;
        }
    }

    if (slot_idx < 0 || module_argc[slot_idx] == 0) {
        *ret = -1;
        return m3Err_none;
    }

    uint32_t len = module_argc[slot_idx];
    if (len > max_len) len = max_len;
    if (len >= 128) len = 127;

    for (uint32_t i = 0; i < len; i++)
        mem[buf_off + i] = (uint8_t)module_argv[slot_idx][i];
    mem[buf_off + len] = '\0';

    *ret = (int32_t)len;
    return m3Err_none;
}

/* host_set_argv(buf_off, buf_len) — set argv for the next spawned module (v17.0)
 * Stores the full command string so the spawned module can parse its args.
 */
static const void *host_set_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx;
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t buf_len = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + buf_len > mem_size) {
        return m3Err_none;
    }
    if (buf_len < 128) {
        for (uint32_t i = 0; i < buf_len; i++)
            pending_argv[i] = (char)mem[buf_off + i];
        pending_argv[buf_len] = '\0';
        pending_argc = buf_len;
    }
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* VFS path resolution (v13.0)                                                */
/* -------------------------------------------------------------------------- */

#define VFS_FLAG_READ   1
#define VFS_FLAG_WRITE  2

extern int net_close(int);
extern int net_connect(unsigned int, unsigned int, unsigned int);
extern int net_listen_impl(unsigned int);
extern void ramdisk_close(int);
extern int ramdisk_open(const char *, unsigned int);

/* Simple string prefix check */
static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (a[i] != b[i]) return 1;
        if (a[i] == '\0') return 1;
    }
    return 0;
}

/* Parse a decimal number from string, return value and advance pointer */
static unsigned int my_parse_uint(const char *s, unsigned int *out_pos, unsigned int max_len)
{
    unsigned int pos = *out_pos;
    unsigned int val = 0;
    while (pos < max_len && s[pos] >= '0' && s[pos] <= '9') {
        val = val * 10 + (s[pos] - '0');
        pos++;
    }
    *out_pos = pos;
    return val;
}

/* _vfs_path_open: C helper called from vfs_open in assembly
 * Matches path prefix and creates appropriate fd type
 * Called from assembly: _vfs_path_open(pid, path_ptr, path_len, flags)
 */
int _vfs_path_open(int pid, const char *path, unsigned int path_len, int flags)
{
    if (!path || path_len == 0) return -1;

    /* Check "/dev/pipe" (9 chars) */
    if (path_len >= 9 && my_strncmp(path, "/dev/pipe", 9) == 0) {
        int rfd = 0, wfd = 0;
        extern int pipe_create(int *, int *);
        if (pipe_create(&rfd, &wfd) != 0) return -1;

        /* Register read_fd in VFS */
        int vfs_rfd = vfs_alloc_fd(pid, 1 /* VFS_TYPE_PIPE */, flags | VFS_FLAG_READ);
        if (vfs_rfd < 0) { pipe_close(rfd); return -1; }

        /* Register write_fd in VFS (if flags include write) */
        if (flags & VFS_FLAG_WRITE) {
            int vfs_wfd = vfs_alloc_fd(pid, 1 /* VFS_TYPE_PIPE */, flags | VFS_FLAG_WRITE);
            if (vfs_wfd >= 0) {
                vfs_set_ops(pid, vfs_wfd, (void *)(intptr_t)wfd);
            }
        }

        vfs_set_ops(pid, vfs_rfd, (void *)(intptr_t)rfd);
        return vfs_rfd;
    }

    /* Check "/net/connect/" (13 chars) */
    if (path_len >= 13 && my_strncmp(path, "/net/connect/", 13) == 0) {
        /* Format: /net/connect/ip.port.proto  (dots as separators) */
        const char *p = path + 13;
        unsigned int rem = path_len - 13;
        unsigned int pos = 0;

        /* Parse IP (4 octets separated by dots) */
        unsigned int ip = 0;
        for (int i = 0; i < 4; i++) {
            unsigned int octet = my_parse_uint(p, &pos, rem);
            ip = (ip << 8) | octet;
            if (pos < rem && p[pos] == '.') pos++;  /* skip dot */
        }

        /* Parse port */
        unsigned int port = my_parse_uint(p, &pos, rem);
        if (pos < rem && p[pos] == '.') pos++;

        /* Parse proto */
        unsigned int proto = my_parse_uint(p, &pos, rem);

        extern int net_connect(unsigned int, unsigned int, unsigned int);
        int sock = net_connect(ip, port, proto);
        if (sock < 0) return -1;

        int vfs_fd = vfs_alloc_fd(pid, 2 /* VFS_TYPE_SOCKET */, flags);
        if (vfs_fd < 0) { net_close(sock); return -1; }
        vfs_set_ops(pid, vfs_fd, (void *)(intptr_t)sock);
        return vfs_fd;
    }

    /* Check "/net/listen/" (12 chars) */
    if (path_len >= 12 && my_strncmp(path, "/net/listen/", 12) == 0) {
        const char *p = path + 12;
        unsigned int rem = path_len - 12;
        unsigned int pos = 0;
        unsigned int port = my_parse_uint(p, &pos, rem);

        extern int net_listen_impl(unsigned int);
        int sock = net_listen_impl(port);
        if (sock < 0) return -1;

        int vfs_fd = vfs_alloc_fd(pid, 2 /* VFS_TYPE_SOCKET */, flags);
        if (vfs_fd < 0) { net_close(sock); return -1; }
        vfs_set_ops(pid, vfs_fd, (void *)(intptr_t)sock);
        return vfs_fd;
    }

    /* Default: regular file via ramdisk */
    int fd = ramdisk_open(path, path_len);
    if (fd < 0) return -1;

    int vfs_fd = vfs_alloc_fd(pid, 0 /* VFS_TYPE_FILE */, flags);
    if (vfs_fd < 0) { ramdisk_close(fd); return -1; }
    vfs_set_ops(pid, vfs_fd, (void *)(intptr_t)fd);
    return vfs_fd;
}

/* host_vfs_open(path_ptr, path_len, flags) → fd or -1 (unified path-based open) */
static const void *host_vfs_open(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t path_off = (uint32_t)*(_sp + 1);
    uint32_t path_len = (uint32_t)*(_sp + 2);
    int32_t flags     = (int32_t)(int64_t)*(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (path_off + path_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    int pid = get_current_pid(runtime);
    const char *path = (const char *)(mem + path_off);
    int fd = _vfs_path_open(pid, path, path_len, (int)flags);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)fd;
    return m3Err_none;
}

/* host_fs_open(path_ptr, path_len) — open file via VFS (per-process) */
static const void *host_fs_open(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t path_off = (uint32_t)*(_sp + 1);
    uint32_t path_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (path_off + path_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    const char *path = (const char *)(mem + path_off);

    /* VFS: allocate fd slot for current process */
    int pid = get_current_pid(runtime);
    int vfs_fd = vfs_alloc_fd(pid, 0 /* VFS_TYPE_FILE */, 1 /* READ */);
    if (vfs_fd < 0) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    int fd = ramdisk_open(path, path_len);
    if (fd < 0) {
        vfs_free_fd(pid, vfs_fd);
        int32_t *ret = (int32_t *)(_sp);
        *ret = (int32_t)fd;
        return m3Err_none;
    }

    /* Store ramdisk fd in ops_ptr for later use */
    vfs_set_ops(pid, vfs_fd, (void *)(intptr_t)fd);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)vfs_fd;
    return m3Err_none;
}

/* host_fs_read(fd, buf_ptr, len) — read from file via VFS (per-process) */
static const void *host_fs_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd     = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len   = (uint32_t)*(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    int pid = get_current_pid(runtime);

    /* VFS: get ops_ptr (ramdisk fd) and call ramdisk_read directly */
    void *ops = vfs_get_ops(pid, fd);
    int ramdisk_fd = (int)(intptr_t)ops;
    if (ramdisk_fd < 0) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    int n = ramdisk_read(ramdisk_fd, mem + buf_off, len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)n;
    return m3Err_none;
}

/* host_fs_close(fd) — close file via VFS (per-process) */
static const void *host_fs_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd = (int32_t)(int64_t)*(_sp + 1);

    int pid = get_current_pid(runtime);

    /* VFS: get ramdisk fd from ops, close it, then free VFS slot */
    void *ops = vfs_get_ops(pid, fd);
    if (ops) {
        int ramdisk_fd = (int)(intptr_t)ops;
        ramdisk_close(ramdisk_fd);
    }
    vfs_free_fd(pid, fd);
    return m3Err_none;
}

/* host_fs_list(buf_ptr, max_len) — list all files */
static const void *host_fs_list(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*(_sp + 1);
    uint32_t max_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = 0;
        return m3Err_none;
    }

    int n = ramdisk_list(mem + buf_off, max_len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)n;
    return m3Err_none;
}

/* host_blk_read(sector, buf_ptr, count) — read sectors from VirtIO-Block */
static const void *host_blk_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t sector_lo = (uint32_t)*(_sp + 1);
    uint32_t sector_hi = (uint32_t)*(_sp + 2);
    uint32_t buf_off   = (uint32_t)*(_sp + 3);
    uint32_t count     = (uint32_t)*(_sp + 4);
    (void)sector_hi; /* 32-bit sector space is enough for QEMU virt */

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);
    if (buf_off + (count << 9) > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Call virtio_blk_read from kernel — identity-mapped, callable from C */
    extern int virtio_blk_read(uint32_t sector, uint8_t *buf, uint32_t count);
    int rc = virtio_blk_read((uint32_t)sector_lo, mem + buf_off, count);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_blk_write(sector, buf_ptr, count) — write sectors to VirtIO-Block */
static const void *host_blk_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t sector_lo = (uint32_t)*(_sp + 1);
    uint32_t sector_hi = (uint32_t)*(_sp + 2);
    uint32_t buf_off   = (uint32_t)*(_sp + 3);
    uint32_t count     = (uint32_t)*(_sp + 4);
    (void)sector_hi;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);
    if (buf_off + (count << 9) > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern int virtio_blk_write(uint32_t sector, uint8_t *buf, uint32_t count);
    int rc = virtio_blk_write((uint32_t)sector_lo, mem + buf_off, count);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_fs_write(fd, buf_off, len) — write to open file via VFS (per-process) */
static const void *host_fs_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd       = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len     = (uint32_t)*(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    int pid = get_current_pid(runtime);

    /* VFS: get ramdisk fd from ops */
    void *ops = vfs_get_ops(pid, fd);
    int ramdisk_fd = (int)(intptr_t)ops;
    if (ramdisk_fd < 0) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    extern int fs_file_write(int fd, const uint8_t *buf, uint32_t len);
    int rc = fs_file_write(ramdisk_fd, mem + buf_off, len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_fs_create(name_off, name_len) — create a new empty file */
static const void *host_fs_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t name_off = (uint32_t)*(_sp + 1);
    uint32_t name_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Null-terminate */
    char tmp[128];
    if (name_len >= 128) name_len = 127;
    for (uint32_t i = 0; i < name_len; i++) tmp[i] = (char)mem[name_off + i];
    tmp[name_len] = '\0';

    extern int fs_create_file(const char *name);
    int rc = fs_create_file(tmp);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_fs_delete(name_off, name_len) — delete a file */
static const void *host_fs_delete(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t name_off = (uint32_t)*(_sp + 1);
    uint32_t name_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    char tmp[128];
    if (name_len >= 128) name_len = 127;
    for (uint32_t i = 0; i < name_len; i++) tmp[i] = (char)mem[name_off + i];
    tmp[name_len] = '\0';

    extern int fs_delete_file(const char *name);
    int rc = fs_delete_file(tmp);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_get_cwd(buf_off, max_len) — get current working directory */
static const void *host_get_cwd(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*(_sp + 1);
    uint32_t max_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    const char *cwd = "/";
    unsigned int len = 0;
    while (cwd[len]) len++;
    if (len >= max_len) len = max_len - 1;
    for (unsigned int i = 0; i < len; i++) mem[buf_off + i] = (uint8_t)cwd[i];
    mem[buf_off + len] = '\0';

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)len;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Environment variables                                                      */
/* -------------------------------------------------------------------------- */

#define MAX_ENV_VARS 64
#define MAX_ENV_KEY  64
#define MAX_ENV_VAL  256

static struct {
    char key[MAX_ENV_KEY];
    char val[MAX_ENV_VAL];
    int  active;
} env_table[MAX_ENV_VARS];

/* Simple string copy (freestanding) */
static void env_strcpy(char *dst, const char *src, unsigned int max)
{
    unsigned int i = 0;
    while (src[i] && i < max - 1) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int env_strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static int env_find(const char *key)
{
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (env_table[i].active && env_strcmp(env_table[i].key, key) == 0)
            return i;
    }
    return -1;
}

/* host_env_set(key_off, key_len, val_off, val_len) — set environment variable */
static const void *host_env_set(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t key_off = (uint32_t)*(_sp + 1);
    uint32_t key_len = (uint32_t)*(_sp + 2);
    uint32_t val_off = (uint32_t)*(_sp + 3);
    uint32_t val_len = (uint32_t)*(_sp + 4);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (key_off + key_len > mem_size || val_off + val_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    char key[MAX_ENV_KEY];
    char val[MAX_ENV_VAL];
    if (key_len >= MAX_ENV_KEY) key_len = MAX_ENV_KEY - 1;
    if (val_len >= MAX_ENV_VAL) val_len = MAX_ENV_VAL - 1;
    for (uint32_t i = 0; i < key_len; i++) key[i] = (char)mem[key_off + i];
    key[key_len] = '\0';
    for (uint32_t i = 0; i < val_len; i++) val[i] = (char)mem[val_off + i];
    val[val_len] = '\0';

    /* Find existing or new slot */
    int slot = env_find(key);
    if (slot >= 0) {
        env_strcpy(env_table[slot].val, val, MAX_ENV_VAL);
    } else {
        for (int i = 0; i < MAX_ENV_VARS; i++) {
            if (!env_table[i].active) {
                env_strcpy(env_table[i].key, key, MAX_ENV_KEY);
                env_strcpy(env_table[i].val, val, MAX_ENV_VAL);
                env_table[i].active = 1;
                slot = i;
                break;
            }
        }
    }

    int32_t *ret = (int32_t *)(_sp);
    *ret = (slot >= 0) ? 0 : -1;
    return m3Err_none;
}

/* host_env_get(key_off, key_len, buf_off, max_len) — get environment variable */
static const void *host_env_get(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t key_off = (uint32_t)*(_sp + 1);
    uint32_t key_len = (uint32_t)*(_sp + 2);
    uint32_t buf_off = (uint32_t)*(_sp + 3);
    uint32_t max_len = (uint32_t)*(_sp + 4);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (key_off + key_len > mem_size || buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    char key[MAX_ENV_KEY];
    if (key_len >= MAX_ENV_KEY) key_len = MAX_ENV_KEY - 1;
    for (uint32_t i = 0; i < key_len; i++) key[i] = (char)mem[key_off + i];
    key[key_len] = '\0';

    int slot = env_find(key);
    if (slot < 0) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_none;
    }

    const char *val = env_table[slot].val;
    unsigned int vlen = 0;
    while (val[vlen]) vlen++;
    if (vlen >= max_len) vlen = max_len - 1;
    for (unsigned int i = 0; i < vlen; i++) mem[buf_off + i] = (uint8_t)val[i];
    mem[buf_off + vlen] = '\0';

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)vlen;
    return m3Err_none;
}

/* host_env_list(buf_off, max_len) — list all environment variables */
static const void *host_env_list(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*(_sp + 1);
    uint32_t max_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    uint32_t written = 0;
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (!env_table[i].active) continue;

        const char *key = env_table[i].key;
        const char *val = env_table[i].val;

        /* Write KEY=VALUE\0 */
        while (*key && written < max_len - 1) {
            mem[buf_off + written++] = (uint8_t)*key++;
        }
        if (written < max_len - 1)
            mem[buf_off + written++] = '=';
        while (*val && written < max_len - 1) {
            mem[buf_off + written++] = (uint8_t)*val++;
        }
        if (written < max_len)
            mem[buf_off + written++] = '\0';
    }

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)written;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* WASI host functions (Phase 6补全)                                          */
/* -------------------------------------------------------------------------- */

#define WASI_ESUCCESS 0
#define WASI_EBADF    8
#define WASI_EINVAL   28
#define WASI_ENOSYS   52
#define WASI_ENOTSUP  58

/* wasi_fd_write(fd, iovs_off, iovs_len, nwritten_off) */
static const void *wasi_fd_write(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t fd         = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t iovs_off   = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t iovs_len   = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t nwritten_off = (uint32_t)*(uint64_t*)(_sp + 4);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    /* stdout/stderr -> serial output */
    if (fd == 1 || fd == 2) {
        uint32_t total = 0;
        for (uint32_t i = 0; i < iovs_len; i++) {
            if (iovs_off + i * 8 + 8 > mem_size) {
                int32_t *ret = (int32_t *)_sp;
                *ret = WASI_EINVAL;
                return m3Err_trapOutOfBoundsMemoryAccess;
            }
            uint32_t buf_off = *(uint32_t *)(mem + iovs_off + i * 8);
            uint32_t buf_len = *(uint32_t *)(mem + iovs_off + i * 8 + 4);
            if (buf_off + buf_len > mem_size) {
                int32_t *ret = (int32_t *)_sp;
                *ret = WASI_EINVAL;
                return m3Err_trapOutOfBoundsMemoryAccess;
            }
            uart_puts_raw((const char *)(mem + buf_off));
            total += buf_len;
        }
        if (nwritten_off + 4 <= mem_size)
            *(uint32_t *)(mem + nwritten_off) = total;
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_ESUCCESS;
        return m3Err_none;
    }

    /* regular file -> ramdisk (read-only, write unsupported) */
    if (nwritten_off + 4 <= mem_size)
        *(uint32_t *)(mem + nwritten_off) = 0;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ENOTSUP;
    return m3Err_none;
}

/* wasi_fd_read(fd, iovs_off, iovs_len, nread_off) */
static const void *wasi_fd_read(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t fd       = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t iovs_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t iovs_len = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t nread_off = (uint32_t)*(uint64_t*)(_sp + 4);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    /* stdin -> serial rx */
    if (fd == 0) {
        if (iovs_off + 8 > mem_size) {
            int32_t *ret = (int32_t *)_sp;
            *ret = WASI_EINVAL;
            return m3Err_trapOutOfBoundsMemoryAccess;
        }
        uint32_t buf_off = *(uint32_t *)(mem + iovs_off);
        /* Read one byte from UART */
        while (UART_FR & UART_FR_RXFE) {
            if (preempt_pending) { preempt_pending = 0; break; }
            __asm__ volatile("wfi");
        }
        if (nread_off + 4 <= mem_size) {
            mem[buf_off] = (uint8_t)(UART_DR & 0xff);
            *(uint32_t *)(mem + nread_off) = 1;
        }
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_ESUCCESS;
        return m3Err_none;
    }

    /* regular file -> ramdisk_read */
    if (iovs_off + 8 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EINVAL;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }
    uint32_t buf_off = *(uint32_t *)(mem + iovs_off);
    uint32_t buf_len = *(uint32_t *)(mem + iovs_off + 4);
    if (buf_off + buf_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EINVAL;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Use open file from WASI context (track via thread-local) */
    /* For simplicity, return not supported without file context */
    if (nread_off + 4 <= mem_size)
        *(uint32_t *)(mem + nread_off) = 0;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_EBADF;
    return m3Err_none;
}

/* wasi_clock_time_get(clock_id, precision, time_off) */
static const void *wasi_clock_time_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t clock_id  = (uint32_t)*(uint64_t*)(_sp + 1);
    uint64_t precision = *(uint64_t*)(_sp + 2);
    uint32_t time_off  = (uint32_t)*(uint64_t*)(_sp + 3);
    (void)clock_id; (void)precision;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);
    if (time_off + 8 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EINVAL;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    uint64_t ticks;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(ticks));
    uint64_t freq;
    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(freq));

    /* Convert ticks to nanoseconds */
    uint64_t ns = (ticks * 1000000000ULL) / freq;
    *(uint64_t *)(mem + time_off) = ns;

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_proc_exit(code) */
static const void *wasi_proc_exit(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem;
    int32_t code = (int32_t)*(uint64_t*)(_sp + 1);

    /* Reuse host_exit logic */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        if (idx < MAX_MODULES && module_table[idx].state != MOD_FREE) {
            /* VFS: release all fds for this process (v12.0) */
            vfs_free_all((int)module_table[idx].id);

            module_table[idx].exit_code = (int)code;
            module_table[idx].state = MOD_EXITED;

            uart_puts_raw("\n[WASI exit ");
            if (code == 0) uart_puts_raw("ok");
            else {
                uart_puts_raw("code=");
                if (code < 0) { uart_putc_raw('-'); code = -code; }
                char buf[12]; int i = 0;
                do { buf[i++] = (char)('0' + (code % 10)); code /= 10; } while (code > 0);
                while (i > 0) uart_putc_raw(buf[--i]);
            }
            uart_puts_raw("]\n");
        }
    }

    /* Halt the CPU */
    while (1) __asm__ volatile("wfi");
}

/* wasi_random_get(buf_off, buf_len) */
static const void *wasi_random_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t buf_len = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (buf_off + buf_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EINVAL;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    uint64_t seed;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(seed));
    for (uint32_t i = 0; i < buf_len; i++) {
        seed = seed * 6364136223846793005ULL + 1;
        mem[buf_off + i] = (uint8_t)(seed >> 56);
    }

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_fd_close(fd) */
static const void *wasi_fd_close(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem;
    uint32_t fd = (uint32_t)*(uint64_t*)(_sp + 1);

    /* stdin/stdout/stderr: no-op */
    if (fd <= 2) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_ESUCCESS;
        return m3Err_none;
    }

    /* regular file -> ramdisk_close */
    ramdisk_close((int)fd);
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_path_open(dirfd, dirflags, path_off, path_len, oflags, fs_rights_base, fs_rights_inheriting, fdflags, fd_out) */
static const void *wasi_path_open(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t dirfd          = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t dirflags       = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t path_off       = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t path_len       = (uint32_t)*(uint64_t*)(_sp + 4);
    uint32_t oflags         = (uint32_t)*(uint64_t*)(_sp + 5);
    /* skip fs_rights_base (u64), fs_rights_inheriting (u64), fdflags (u16) */
    uint32_t fd_out_off     = (uint32_t)*(uint64_t*)(_sp + 11);
    (void)dirflags; (void)oflags;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (path_off + path_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EINVAL;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    int fd = ramdisk_open((const char *)(mem + path_off), path_len);
    if (fd < 0) {
        if (fd_out_off + 4 <= mem_size) *(uint32_t *)(mem + fd_out_off) = 0;
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_EBADF;
        return m3Err_none;
    }

    if (fd_out_off + 4 <= mem_size)
        *(uint32_t *)(mem + fd_out_off) = (uint32_t)fd;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_fd_seek(fd, offset, whence, newoffset_off) */
static const void *wasi_fd_seek(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem;
    /* Not supported for RAM disk */
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ENOTSUP;
    return m3Err_none;
}

/* wasi_fd_tell(fd, offset_out) */
static const void *wasi_fd_tell(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ENOSYS;
    return m3Err_none;
}

/* wasi_args_sizes_get(argc_out, argv_buf_size_out) */
static const void *wasi_args_sizes_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t argc_out    = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t argv_buf_out = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    /* Find current module's argv */
    uint32_t argc = 0;
    uint32_t buf_size = 0;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == current_module_id && module_table[i].state != MOD_FREE) {
            if (module_argc[i] > 0) {
                argc = 1;
                buf_size = module_argc[i] + 1;  /* string + null */
            }
            break;
        }
    }

    if (argc_out + 4 <= mem_size) *(uint32_t *)(mem + argc_out) = argc;
    if (argv_buf_out + 4 <= mem_size) *(uint32_t *)(mem + argv_buf_out) = buf_size;

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_args_get(argv, argv_buf) — copy argc/argv to WASM memory (v18.0) */
static const void *wasi_args_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t argv_off     = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t argv_buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (argv_off + 8 > mem_size || argv_buf_off > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = 28; /* EFAULT */
        return m3Err_none;
    }

    /* Find current module's argv */
    int slot_idx = -1;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == current_module_id && module_table[i].state != MOD_FREE) {
            slot_idx = i;
            break;
        }
    }

    if (slot_idx < 0 || module_argc[slot_idx] == 0) {
        *(uint32_t *)(mem + argv_off) = 0;  /* argc = 0 */
        int32_t *ret = (int32_t *)_sp;
        *ret = WASI_ESUCCESS;
        return m3Err_none;
    }

    /* Copy argument string to buffer */
    uint32_t arg_len = module_argc[slot_idx];
    char *arg = module_argv[slot_idx];
    uint32_t buf_pos = argv_buf_off;
    for (uint32_t i = 0; i <= arg_len; i++)
        mem[buf_pos++] = (uint8_t)arg[i];

    /* Write argv pointer array: [ptr_to_string, NULL] */
    *(uint32_t *)(mem + argv_off) = argv_buf_off;
    *(uint32_t *)(mem + argv_off + 4) = 0;

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_environ_get(environ, environ_buf) — return empty environment (v18.0) */
static const void *wasi_environ_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem; (void)_sp;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_fd_fdstat_get(fd, stat_off) — return fd stat (v18.0) */
static const void *wasi_fd_fdstat_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    int32_t fd = (int32_t)*(int64_t*)(_sp + 1);
    uint32_t stat_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (stat_off + 24 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = 8; /* EBADF */
        return m3Err_none;
    }

    /* fdstat: filetype(1) + fdflags(1) + pad(6) + rights_base(8) + rights_inherit(8) = 24 */
    if (fd >= 0 && fd <= 2) {
        mem[stat_off] = 2;  /* __WASI_FILETYPE_CHARACTER_DEVICE */
    } else {
        mem[stat_off] = 0;  /* unknown */
    }
    /* Zero rest */
    for (int i = 1; i < 24; i++) mem[stat_off + i] = 0;

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_environ_sizes_get(environ_count_out, environ_buf_size_out) */
static const void *wasi_environ_sizes_get(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t ec_out = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t eb_out = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (ec_out + 4 <= mem_size) *(uint32_t *)(mem + ec_out) = 0;
    if (eb_out + 4 <= mem_size) *(uint32_t *)(mem + eb_out) = 0;

    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ESUCCESS;
    return m3Err_none;
}

/* wasi_path_removefile — stub */
static const void *wasi_path_removefile(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem;
    int32_t *ret = (int32_t *)_sp;
    *ret = WASI_ENOTSUP;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* GUI host functions (Phase 7)                                               */
/* -------------------------------------------------------------------------- */

/* host_gui_create(x, y, w, h, title_off) — create a window */
static const void *host_gui_create(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t x         = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t y         = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t w         = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t h         = (uint32_t)*(uint64_t*)(_sp + 4);
    uint32_t title_off = (uint32_t)*(uint64_t*)(_sp + 5);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    /* Find title length */
    uint32_t title_len = 0;
    if (title_off < mem_size) {
        while (title_off + title_len < mem_size && mem[title_off + title_len] && title_len < 31)
            title_len++;
    }

    extern int gui_create_window(int x, int y, int w, int h, const char *title);
    int win_id = gui_create_window((int)x, (int)y, (int)w, (int)h,
                                   (const char *)(mem + title_off));

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)win_id;
    return m3Err_none;
}

/* host_gui_draw(win_id, buf_off, x, y, w, h) — blit buffer to window */
static const void *host_gui_draw(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    int32_t win_id  = (int32_t)(int64_t)*(uint64_t*)(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t bx     = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t by     = (uint32_t)*(uint64_t*)(_sp + 4);
    uint32_t bw     = (uint32_t)*(uint64_t*)(_sp + 5);
    uint32_t bh     = (uint32_t)*(uint64_t*)(_sp + 6);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (buf_off + (bw * bh * 4) > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Copy pixels from WASM memory to framebuffer */
    /* For simplicity, draw as rectangles (batch by row) */
    uint32_t *src = (uint32_t *)(mem + buf_off);
    for (uint32_t row = 0; row < bh; row++) {
        for (uint32_t col = 0; col < bw; col++) {
            extern void fb_draw_pixel(int x, int y, uint32_t color);
            fb_draw_pixel((int)(bx + col), (int)(by + row + 20 + (win_id * 10)),
                         src[row * bw + col]);
        }
    }

    int32_t *ret = (int32_t *)_sp;
    *ret = 0;
    return m3Err_none;
}

/* host_gui_flush(win_id) — flush screen */
static const void *host_gui_flush(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem; (void)*_sp;

    extern void gui_flush_screen(void);
    gui_flush_screen();

    return m3Err_none;
}

/* host_gui_blit(dst_x, dst_y, src_x, src_y, w, h, buf_off) — batched blit from WASM memory */
static const void *host_gui_blit(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t dst_x   = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t dst_y   = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t src_x   = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t src_y   = (uint32_t)*(uint64_t*)(_sp + 4);
    uint32_t w       = (uint32_t)*(uint64_t*)(_sp + 5);
    uint32_t h       = (uint32_t)*(uint64_t*)(_sp + 6);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 7);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (buf_off + (w * h * 4) > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern void fb_blit(int dst_x, int dst_y, int src_x, int src_y, int w, int h, uint32_t *src);
    fb_blit((int)dst_x, (int)dst_y, (int)src_x, (int)src_y, (int)w, (int)h, (uint32_t *)(mem + buf_off));

    int32_t *ret = (int32_t *)_sp;
    *ret = 0;
    return m3Err_none;
}

/* host_gui_swap() — swap double buffers (eliminates tearing) */
static const void *host_gui_swap(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)rt; (void)_ctx; (void)_mem; (void)*_sp;

    extern void fb_swap_buffers(void);
    fb_swap_buffers();

    return m3Err_none;
}

/* host_gui_poll_event(win_id, event_buf_off) — poll window event queue */
static const void *host_gui_poll_event(IM3Runtime rt, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t win_id     = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t event_buf  = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(rt);

    if (event_buf + 16 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = 0;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern int gui_poll_event(int win_id, uint8_t *event_buf);
    int rc = gui_poll_event((int)win_id, mem + event_buf);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Network host functions (Phase 5)                                           */
/* -------------------------------------------------------------------------- */

/* net_connect(ip, port, proto) — create socket, return sock_id or -1 */
static const void *host_net_connect(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t ip    = (uint32_t)*(_sp + 1);
    uint32_t port  = (uint32_t)*(_sp + 2);
    uint32_t proto = (uint32_t)*(_sp + 3);

    extern int net_connect(uint32_t ip, uint32_t port, uint32_t proto);
    int sock = net_connect(ip, port, proto);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)sock;
    return m3Err_none;
}

/* host_net_send(sock, buf_off, len) — send data over socket */
static const void *host_net_send(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sock    = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len     = (uint32_t)*(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern int net_send(int sock, const uint8_t *data, uint32_t len);
    int rc = net_send(sock, mem + buf_off, len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_net_recv(sock, buf_off, max_len) — receive data into buffer */
static const void *host_net_recv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sock    = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t max_len = (uint32_t)*(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern int net_recv(int sock, uint8_t *buf, uint32_t max_len);
    int rc = net_recv(sock, mem + buf_off, max_len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_net_close(sock) — close socket via VFS (per-process) */
static const void *host_net_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sock = (int32_t)(int64_t)*(_sp + 1);

    int pid = get_current_pid(runtime);

    extern int net_close(int sock);
    net_close(sock);
    vfs_free_fd(pid, sock);

    return m3Err_none;
}

/* host_net_listen(port) — create TCP listening socket */
static const void *host_net_listen(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t port = (uint32_t)*(_sp + 1);

    extern int net_listen_impl(uint32_t port);
    int sock = net_listen_impl(port);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)sock;
    return m3Err_none;
}

/* host_net_accept(sock) — accept connection on listening socket */
static const void *host_net_accept(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sock = (int32_t)(int64_t)*(_sp + 1);

    extern int net_accept_impl(int sock);
    int client = net_accept_impl(sock);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)client;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* IPC: Pipe and Message Queue host functions                                 */
/* -------------------------------------------------------------------------- */

/* pipe_create(read_fd_off, write_fd_off) — create pipe, store fds via VFS */
static const void *host_pipe_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t read_fd_off  = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t write_fd_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;

    extern int pipe_create(int *read_fd, int *write_fd);
    int rfd = 0, wfd = 0;
    int rc = pipe_create(&rfd, &wfd);

    if (rc == 0) {
        *(int32_t *)(mem + read_fd_off)  = (int32_t)rfd;
        *(int32_t *)(mem + write_fd_off) = (int32_t)wfd;
    }
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* pipe_read(fd, buf_off, len) — read from pipe via VFS */
static const void *host_pipe_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int fd = (int)*(int64_t*)(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t len = (uint32_t)*(uint64_t*)(_sp + 3);
    uint8_t *mem = (uint8_t *)_mem;

    extern int pipe_read(int fd, uint8_t *buf, uint32_t len);
    int rc = pipe_read(fd, mem + buf_off, len);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* pipe_write(fd, buf_off, len) — write to pipe via VFS */
static const void *host_pipe_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int fd = (int)*(int64_t*)(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t len = (uint32_t)*(uint64_t*)(_sp + 3);
    uint8_t *mem = (uint8_t *)_mem;

    extern int pipe_write(int fd, const uint8_t *buf, uint32_t len);
    int rc = pipe_write(fd, mem + buf_off, len);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* pipe_close(fd) — close pipe via VFS (per-process) */
static const void *host_pipe_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int fd = (int)*(int64_t*)(_sp + 1);
    (void)_ctx; (void)_mem;

    int pid = get_current_pid(runtime);

    extern int pipe_close(int fd);
    int rc = pipe_close(fd);
    vfs_free_fd(pid, fd);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* msgq_create() — create message queue, return qid */
static const void *host_msgq_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_sp; (void)_mem;

    extern int msgq_create(void);
    int qid = msgq_create();
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)qid;
    return m3Err_none;
}

/* msgq_send(qid, msg_off) — send message to queue */
static const void *host_msgq_send(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int qid = (int)*(int64_t*)(_sp + 1);
    uint32_t msg_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;

    extern int msgq_send(int qid, const uint8_t *msg);
    int rc = msgq_send(qid, mem + msg_off);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* msgq_recv(qid, buf_off) — receive message from queue */
static const void *host_msgq_recv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int qid = (int)*(int64_t*)(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;

    extern int msgq_recv(int qid, uint8_t *buf);
    int rc = msgq_recv(qid, mem + buf_off);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* msgq_destroy(qid) — destroy message queue */
static const void *host_msgq_destroy(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int qid = (int)*(int64_t*)(_sp + 1);
    (void)_ctx; (void)_mem;

    extern int msgq_destroy(int qid);
    int rc = msgq_destroy(qid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* signal_register(sig, handler_off) — register signal handler for current module */
static const void *host_signal_register(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t sig = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t handler_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint8_t *mem = (uint8_t *)_mem;

    extern int signal_register(int pid, int sig, void *handler);
    int pid = (int)current_module_id;
    int rc = signal_register(pid, (int)sig, (void *)(mem + handler_off));
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* signal_send(pid, sig) — send signal to a process */
static const void *host_signal_send(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int pid = (int)*(int64_t*)(_sp + 1);
    uint32_t sig = (uint32_t)*(uint64_t*)(_sp + 2);
    (void)_ctx; (void)_mem;

    extern int signal_send(int pid, int sig);
    int rc = signal_send(pid, (int)sig);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* IPC: Shared Memory host functions                                          */
/* -------------------------------------------------------------------------- */

/* host_shmem_alloc(pid, size) — allocate shared memory region, return region_id */
static const void *host_shmem_alloc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid  = (int32_t)(int64_t)*(_sp + 1);
    uint32_t size = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int shmem_alloc(int pid, int size);
    int rid = shmem_alloc(pid, (int)size);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rid;
    return m3Err_none;
}

/* host_shmem_free(region_id) — free shared memory region */
static const void *host_shmem_free(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid = (int32_t)(int64_t)*(_sp + 1);

    extern int shmem_free(int region_id);
    int rc = shmem_free(rid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_shmem_attach(region_id, pid) — attach to shared memory */
static const void *host_shmem_attach(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid = (int32_t)(int64_t)*(_sp + 1);
    int32_t pid = (int32_t)(int64_t)*(_sp + 2);

    extern int shmem_attach(int region_id, int pid);
    int rc = shmem_attach(rid, pid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_shmem_detach(region_id) — detach from shared memory */
static const void *host_shmem_detach(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid = (int32_t)(int64_t)*(_sp + 1);

    extern int shmem_detach(int region_id);
    int rc = shmem_detach(rid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_shmem_get_ptr(region_id) — return kernel physical address of data area */
static const void *host_shmem_get_ptr(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid = (int32_t)(int64_t)*(_sp + 1);

    extern void *shmem_get_ptr(int region_id);
    void *ptr = shmem_get_ptr(rid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)(uintptr_t)ptr;
    return m3Err_none;
}

/* host_shmem_set_flag(region_id, flag) — set sync flag */
static const void *host_shmem_set_flag(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid  = (int32_t)(int64_t)*(_sp + 1);
    uint32_t flag = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int shmem_set_flag(int region_id, int flag);
    int rc = shmem_set_flag(rid, (int)flag);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_shmem_get_flag(region_id) — get sync flags */
static const void *host_shmem_get_flag(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid = (int32_t)(int64_t)*(_sp + 1);

    extern int shmem_get_flag(int region_id);
    int flags = shmem_get_flag(rid);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)flags;
    return m3Err_none;
}

/* host_shmem_clear_flag(region_id, flag) — clear sync flag */
static const void *host_shmem_clear_flag(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t rid  = (int32_t)(int64_t)*(_sp + 1);
    uint32_t flag = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int shmem_clear_flag(int region_id, int flag);
    int rc = shmem_clear_flag(rid, (int)flag);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_shmem_write(region_id, buf_off, len) — write data to shared memory */
static const void *host_shmem_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    int32_t rid    = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t len   = (uint32_t)*(uint64_t*)(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern void *shmem_get_ptr(int region_id);
    void *shmem_ptr = shmem_get_ptr(rid);
    if (!shmem_ptr) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -2;
        return m3Err_none;
    }

    extern int shmem_get_flag(int region_id);
    int flags = shmem_get_flag(rid);
    if (flags <= 0) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -3;
        return m3Err_none;
    }

    uint8_t *dst = (uint8_t *)shmem_ptr;
    for (uint32_t i = 0; i < len; i++)
        dst[i] = mem[buf_off + i];

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)len;
    return m3Err_none;
}

/* host_shmem_read(region_id, buf_off, len) — read data from shared memory */
static const void *host_shmem_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    int32_t rid    = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t len   = (uint32_t)*(uint64_t*)(_sp + 3);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern void *shmem_get_ptr(int region_id);
    void *shmem_ptr = shmem_get_ptr(rid);
    if (!shmem_ptr) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -2;
        return m3Err_none;
    }

    extern int shmem_get_flag(int region_id);
    int flags = shmem_get_flag(rid);
    if (flags <= 0) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -3;
        return m3Err_none;
    }

    uint8_t *src = (uint8_t *)shmem_ptr;
    for (uint32_t i = 0; i < len; i++)
        mem[buf_off + i] = src[i];

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)len;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* IPC: Semaphore host functions                                              */
/* -------------------------------------------------------------------------- */

/* host_sem_create(initial_value) — create semaphore, return sem_id or -1 */
static const void *host_sem_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t initial_value = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_create(int initial_value);
    int sid = semaphore_create(initial_value);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)sid;
    return m3Err_none;
}

/* host_sem_wait(sem_id) — P operation: decrement or block */
static const void *host_sem_wait(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sid = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_wait(int sem_id);
    int rc = semaphore_wait(sid);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_sem_try_wait(sem_id) — non-blocking wait, return 0=ok, -2=would block */
static const void *host_sem_try_wait(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sid = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_try_wait(int sem_id);
    int rc = semaphore_try_wait(sid);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_sem_post(sem_id) — V operation: increment */
static const void *host_sem_post(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sid = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_post(int sem_id);
    int rc = semaphore_post(sid);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_sem_destroy(sem_id) — destroy semaphore */
static const void *host_sem_destroy(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sid = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_destroy(int sem_id);
    int rc = semaphore_destroy(sid);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_sem_get_value(sem_id) — get semaphore value */
static const void *host_sem_get_value(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sid = (int32_t)(int64_t)*(_sp + 1);

    extern int semaphore_get_value(int sem_id);
    int val = semaphore_get_value(sid);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)val;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Module hot-reload host functions                                           */
/* -------------------------------------------------------------------------- */

/* Forward declaration */
static const char *load_module_internal(int slot_idx, const char *name,
                                         const uint8_t *wasm_bytes, uint32_t wasm_size);

/* host_module_reload(module_id) — reload an exited module, preserving PID */
static const void *host_module_reload(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t module_id = (int32_t)(int64_t)*(_sp + 1);

    if (module_id <= 0 || module_id > MAX_MODULES) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    uint32_t slot_idx = (uint32_t)module_id - 1;
    if (slot_idx >= MAX_MODULES) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    wasm_module_slot_t *slot = &module_table[slot_idx];
    if (slot->state == MOD_FREE) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -2;
        return m3Err_none;
    }

    /* Find registry entry for this module */
    wasm_registry_entry_t *entry = find_registry_entry(slot->name);
    if (!entry || !entry->wasm_bytes || entry->wasm_size == 0) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -3;
        return m3Err_none;
    }

    /* Preserve the original PID */
    uint32_t preserved_pid = slot->id;

    /* Destroy old runtime before creating new one */
    if (slot->runtime) {
        m3_FreeRuntime(slot->runtime);
        slot->runtime = NULL;
    }
    slot->module = NULL;
    slot->entry = NULL;

    /* Reset state to allow reloading */
    slot->state = MOD_FREE;

    /* Reload module */
    const char *err = load_module_internal((int)slot_idx, slot->name,
                                           entry->wasm_bytes, entry->wasm_size);
    if (err) {
        uart_puts_raw("[reload] error: ");
        uart_puts_raw(err);
        uart_puts_raw("\n");
        int32_t *ret = (int32_t *)_sp;
        *ret = -4;
        return m3Err_none;
    }

    /* Restore original PID */
    module_table[slot_idx].id = preserved_pid;

    uart_puts_raw("[reload] ");
    uart_puts_raw(slot->name);
    uart_puts_raw(" pid=");
    {
        uint32_t pid = module_table[slot_idx].id;
        char buf[12]; int i = 0;
        if (pid == 0) { uart_puts_raw("0"); }
        else { uint32_t v = pid; do { buf[i++] = (char)('0' + (v % 10)); v /= 10; } while (v > 0); while (i > 0) uart_putc_raw(buf[--i]); }
    }
    uart_puts_raw(" ok\n");

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)module_table[slot_idx].id;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Kernel event notification host functions                                   */
/* -------------------------------------------------------------------------- */

/* host_event_subscribe(pid, event_mask) — subscribe to events */
static const void *host_event_subscribe(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid       = (int32_t)(int64_t)*(_sp + 1);
    uint32_t mask     = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int event_subscribe(int pid, int event_mask);
    int rc = event_subscribe(pid, (int)mask);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_event_unsubscribe(pid, event_mask) — unsubscribe from events */
static const void *host_event_unsubscribe(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid       = (int32_t)(int64_t)*(_sp + 1);
    uint32_t mask     = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int event_unsubscribe(int pid, int event_mask);
    int rc = event_unsubscribe(pid, (int)mask);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_event_notify(type, data0, data1) — trigger kernel event */
static const void *host_event_notify(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t type   = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t data0  = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t data1  = (uint32_t)*(uint64_t*)(_sp + 3);

    extern int event_notify(int event_type, int data0, int data1);
    int rc = event_notify((int)type, (int)data0, (int)data1);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_event_poll(pid, buf_off) — poll for events, returns count */
static const void *host_event_poll(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid     = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);

    extern int event_poll(int pid, int buf_off);
    int count = event_poll(pid, (int)buf_off);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)count;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Persistence: flush filesystem to disk (v14.0)                              */
/* -------------------------------------------------------------------------- */

/* host_persist_sync() — flush all dirty filesystem data to disk */
static const void *host_persist_sync(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_sp; (void)_mem;

    extern int persist_sync(void);
    int rc = persist_sync();

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)rc;
    return m3Err_none;
}

/* host_persist_get_sync_count() — return number of syncs performed */
static const void *host_persist_get_sync_count(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_sp; (void)_mem;

    extern int persist_get_sync_count(void);
    int count = persist_get_sync_count();

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)count;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* WASI helper functions (called from wasi.asm)                               */
/* -------------------------------------------------------------------------- */

/* host_fd_write_c(fd, buf, len) — write to regular fd */
int host_fd_write_c(int fd, const uint8_t *buf, uint32_t len)
{
    /* For RAM disk: read-only, so write not supported */
    /* Could route to VirtIO-Block or network in future */
    (void)fd; (void)buf; (void)len;
    return -1;
}

/* host_fd_read_c(fd, buf, len) — read from regular fd */
int host_fd_read_c(int fd, uint8_t *buf, uint32_t len)
{
    return ramdisk_read(fd, buf, len);
}

/* host_fd_close_c(fd) — close regular fd */
int host_fd_close_c(int fd)
{
    ramdisk_close(fd);
    return 0;
}

/* host_path_open_c(dirfd, flags, path, len) — open file by path */
int host_path_open_c(int dirfd, int flags, const char *path, uint32_t len)
{
    (void)dirfd; (void)flags;
    return ramdisk_open(path, len);
}

/* host_proc_exit_c(code) — exit current module */
void host_proc_exit_c(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int32_t code = (int32_t)(int64_t)*_sp++;
    (void)runtime; (void)_ctx; (void)_mem;
    host_exit(runtime, _ctx, _sp, _mem);
    /* Override with code */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        if (idx < MAX_MODULES && module_table[idx].state != MOD_FREE) {
            /* VFS: release all fds for this process (v12.0) */
            vfs_free_all((int)module_table[idx].id);

            module_table[idx].exit_code = (int)code;
            module_table[idx].state = MOD_EXITED;
        }
    }
}

/* host_proc_spawn_c(path_ptr, path_len) — spawn process from path */
int host_proc_spawn_c(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t path_off = (uint32_t)*(_sp + 1);
    uint32_t path_len = (uint32_t)*(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (path_off + path_len > mem_size) {
        int32_t *ret = (int32_t *)(_sp);
        *ret = -1;
        return -1;
    }

    /* For now, delegate to host_spawn with the path as module name */
    char name_buf[64];
    if (path_len >= 64) path_len = 63;
    for (uint32_t i = 0; i < path_len; i++) name_buf[i] = (char)mem[path_off + i];
    name_buf[path_len] = '\0';

    /* Call host_spawn */
    int32_t result;
    host_spawn(runtime, _ctx, _sp, _mem);
    result = *(int32_t *)(_sp);

    int32_t *ret = (int32_t *)(_sp);
    *ret = result;
    return result;
}

/* serial_puts_buf — write buffer with length to UART (called from wasi.asm) */
void serial_puts_buf(const uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
        uart_putc_raw((char)buf[i]);
}

/* host_getc_asm — non-blocking UART read (called from wasi.asm) */
int host_getc_asm(void)
{
    if (UART_FR & UART_FR_RXFE)
        return -1;
    return (int)(UART_DR & 0xff);
}

/* -------------------------------------------------------------------------- */
/* Process management host functions (proc_monitor service)                   */
/* -------------------------------------------------------------------------- */

/* Static iterator for proc_list_next — preserves position across calls */
static uint32_t proc_list_iter = 0;

/* host_proc_list_next() — iterate through modules, return module ID or -1 */
static const void *host_proc_list_next(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;

    for (; proc_list_iter < MAX_MODULES; proc_list_iter++) {
        if (module_table[proc_list_iter].state != MOD_FREE) {
            uint32_t pid = module_table[proc_list_iter].id;
            proc_list_iter++;
            int32_t *ret = (int32_t *)_sp;
            *ret = (int32_t)pid;
            return m3Err_none;
        }
    }
    /* End of list — reset iterator */
    proc_list_iter = 0;
    int32_t *ret = (int32_t *)_sp;
    *ret = -1;
    return m3Err_none;
}

/* host_proc_get_status(pid) — return module state code */
static const void *host_proc_get_status(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid = (int32_t)(int64_t)*(_sp + 1);

    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == (uint32_t)pid) {
            int32_t *ret = (int32_t *)_sp;
            *ret = (int32_t)module_table[i].state;
            return m3Err_none;
        }
    }
    int32_t *ret = (int32_t *)_sp;
    *ret = -1;  /* not found */
    return m3Err_none;
}

/* host_proc_restart(pid) — restart an exited module, return new PID or -1 */
static const void *host_proc_restart(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    int32_t pid = (int32_t)(int64_t)*(_sp + 1);

    /* Find the module slot */
    int slot_idx = -1;
    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == (uint32_t)pid) {
            slot_idx = (int)i;
            break;
        }
    }
    if (slot_idx < 0) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    wasm_module_slot_t *slot = &module_table[slot_idx];

    /* Find WASM registry entry for this module name */
    wasm_registry_entry_t *entry = find_registry_entry(slot->name);
    if (!entry) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -2;
        return m3Err_none;
    }

    /* Reload module from registry */
    const char *err = load_module_internal(slot_idx, entry->name, entry->wasm_bytes, entry->wasm_size);
    if (err) {
        uart_puts_raw("[proc_restart] reload error: ");
        uart_puts_raw(err);
        uart_puts_raw("\n");
        int32_t *ret = (int32_t *)_sp;
        *ret = -3;
        return m3Err_none;
    }

    uart_puts_raw("[proc_restart] ");
    uart_puts_raw(slot->name);
    uart_puts_raw(" -> pid=");
    {
        uint32_t new_pid = module_table[slot_idx].id;
        char buf[12]; int i = 0;
        if (new_pid == 0) { uart_puts_raw("0"); }
        else { uint32_t v = new_pid; do { buf[i++] = (char)('0' + (v % 10)); v /= 10; } while (v > 0); while (i > 0) uart_putc_raw(buf[--i]); }
    }
    uart_puts_raw("\n");

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)module_table[slot_idx].id;
    return m3Err_none;
}

/* host_proc_kill(pid) — terminate a module (mark as EXITED) */
static const void *host_proc_kill(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid = (int32_t)(int64_t)*(_sp + 1);

    terminate_module((uint32_t)pid);

    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Kernel log ring buffer host functions                                      */
/* -------------------------------------------------------------------------- */

/* host_log_read(buf_off, max_len) — read one line from kernel log ring buffer */
static const void *host_log_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t max_len = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + max_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    extern int log_read(char *buf, int max_len);
    int len = log_read((char *)(mem + buf_off), (int)max_len);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)len;
    return m3Err_none;
}

/* host_log_size() — return bytes available in kernel log ring buffer */
static const void *host_log_size(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;

    extern int log_ring_available(void);
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)log_ring_available();
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* User authentication host functions                                         */
/* -------------------------------------------------------------------------- */

/* User table externs from user.asm */
extern uint8_t user_table[100];       /* 4 users x 25 bytes */
extern int32_t user_current;          /* current logged-in uid */
extern int32_t user_count;            /* number of registered users */

static uint32_t user_hash_pass(const char *pass, uint32_t len)
{
    uint32_t h = 0;
    for (uint32_t i = 0; i < len; i++)
        h = h * 31 + (uint8_t)pass[i];
    return h;
}

/* user_login_impl: look up user by name+pass hash, return uid or -1 */
static int user_login_impl(const uint8_t *mem, uint32_t mem_size,
                            uint32_t name_off, uint32_t name_len,
                            uint32_t pass_off, uint32_t pass_len)
{
    if (name_off + name_len > mem_size || pass_off + pass_len > mem_size)
        return -1;

    const char *name = (const char *)(mem + name_off);
    const char *pass = (const char *)(mem + pass_off);
    uint32_t h = user_hash_pass(pass, pass_len);

    int32_t count = user_count;
    for (int32_t i = 0; i < count; i++) {
        uint8_t *u = &user_table[i * 25];
        /* Compare name (first 16 bytes) */
        int match = 1;
        for (int j = 0; j < 16; j++) {
            char tc = (j < (int)name_len) ? name[j] : '\0';
            if (u[j] != (uint8_t)tc) { match = 0; break; }
        }
        if (!match) continue;
        /* Compare password hash */
        uint32_t stored;
        __builtin_memcpy(&stored, u + 16, 4);
        if (stored != h) continue;
        /* Found — set current user */
        user_current = i;
        return i;
    }
    return -1;
}

/* host_user_login(name_off, name_len, pass_off, pass_len) — returns uid or -1 */
static const void *host_user_login(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t name_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t name_len = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t pass_off = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t pass_len = (uint32_t)*(uint64_t*)(_sp + 4);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size || pass_off + pass_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    int uid = user_login_impl(mem, mem_size, name_off, name_len, pass_off, pass_len);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)uid;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Device management host functions                                           */
/* -------------------------------------------------------------------------- */

/* Device table externs from device.asm */
extern uint8_t device_table[512];     /* 16 devices x 32 bytes */
extern int32_t device_count;          /* number of registered devices */

#define DEVICE_ENTRY_SIZE 32
#define MAX_DEVS 16

/* host_device_list(buf_off, max_len) — serialize device list to buffer */
static const void *host_device_list_fn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t max_len = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + 1 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    char *buf = (char *)(mem + buf_off);
    int count = device_count;
    int total = 0;

    for (int i = 0; i < count && i < MAX_DEVS; i++) {
        uint8_t *dev = &device_table[i * DEVICE_ENTRY_SIZE];
        uint32_t id, type, status;
        __builtin_memcpy(&id, dev, 4);
        __builtin_memcpy(&type, dev + 4, 4);
        __builtin_memcpy(&status, dev + 8, 4);

        /* Manual int formatting: "id,type,status\n" */
        char entry[32];
        int n = 0;
        /* id */
        uint32_t v = id;
        char tmp[12]; int ti = 0;
        if (v == 0) tmp[ti++] = '0';
        else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
        for (int j = ti - 1; j >= 0; j--) entry[n++] = tmp[j];
        entry[n++] = ',';
        /* type */
        v = type; ti = 0;
        if (v == 0) tmp[ti++] = '0';
        else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
        for (int j = ti - 1; j >= 0; j--) entry[n++] = tmp[j];
        entry[n++] = ',';
        /* status */
        v = status; ti = 0;
        if (v == 0) tmp[ti++] = '0';
        else { while (v > 0) { tmp[ti++] = (char)('0' + (v % 10)); v /= 10; } }
        for (int j = ti - 1; j >= 0; j--) entry[n++] = tmp[j];
        entry[n++] = '\n';

        if (total + n > (int)max_len) break;
        __builtin_memcpy(buf + total, entry, n);
        total += n;
    }

    buf[total] = '\0';
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)total;
    return m3Err_none;
}

/* host_device_status(device_id) — return device status */
static const void *host_device_status_fn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t device_id = (int32_t)(int64_t)*(_sp + 1);

    if (device_id < 0 || device_id >= device_count) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    uint8_t *dev = &device_table[device_id * DEVICE_ENTRY_SIZE];
    uint32_t status;
    __builtin_memcpy(&status, dev + 8, 4);

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)status;
    return m3Err_none;
}

/* host_device_attach(type_off, type_len) — attach a device, return device_id */
static const void *host_device_attach_fn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx;
    uint32_t type_off = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t type_len = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (type_off + type_len > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    const char *type_str = (const char *)(mem + type_off);
    uint32_t type = 0; /* default: blk */

    if (type_len >= 4) {
        if (type_str[0] == 'n' || type_str[0] == 'N') type = 1;
        else if (type_str[0] == 'g' || type_str[0] == 'G') type = 2;
        else if (type_str[0] == 'i' || type_str[0] == 'I') type = 3;
    }

    int count = device_count;
    if (count >= MAX_DEVS) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -2;
        return m3Err_none;
    }

    uint8_t *dev = &device_table[count * DEVICE_ENTRY_SIZE];
    int32_t id = count;
    uint32_t st = 1;
    __builtin_memcpy(dev, &id, 4);
    __builtin_memcpy(dev + 4, &type, 4);
    __builtin_memcpy(dev + 8, &st, 4);
    device_count++;

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)id;
    return m3Err_none;
}

/* host_device_detach(device_id) — detach a device */
static const void *host_device_detach_fn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t device_id = (int32_t)(int64_t)*(_sp + 1);

    if (device_id < 0 || device_id >= device_count) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    uint32_t st = 0;
    uint8_t *dev = &device_table[device_id * DEVICE_ENTRY_SIZE];
    __builtin_memcpy(dev + 8, &st, 4);

    int32_t *ret = (int32_t *)_sp;
    *ret = 0;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Resource quota host functions (v7.0/v8.0)                                  */
/* -------------------------------------------------------------------------- */

/* Externs from quota.asm */
extern void quota_init(void);
extern void quota_set_default(int pid, int perm_level);
extern void quota_set(int pid, uint32_t cpu_lim, uint32_t mem_lim, uint8_t fd_lim);
extern int quota_check_cpu(int pid);
extern void quota_bump_cpu(int pid);
extern int quota_check_mem(int pid, uint32_t size);
extern void quota_bump_mem(int pid, uint32_t size);
extern int quota_check_fd(int pid);
extern void quota_bump_fd(int pid);
extern void quota_drop_fd(int pid);
extern void quota_get(int pid, uint8_t *buf);

/* host_quota_get(pid, buf_off) — get quota info for process */
static const void *host_quota_get(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    int32_t pid      = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + 20 > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    quota_get(pid, mem + buf_off);

    int32_t *ret = (int32_t *)_sp;
    *ret = 0;
    return m3Err_none;
}

/* host_quota_set(pid, cpu_lim, mem_lim, fd_lim) — set quota limits */
static const void *host_quota_set(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid      = (int32_t)(int64_t)*(_sp + 1);
    uint32_t cpu_lim = (uint32_t)*(uint64_t*)(_sp + 2);
    uint32_t mem_lim = (uint32_t)*(uint64_t*)(_sp + 3);
    uint32_t fd_lim  = (uint32_t)*(uint64_t*)(_sp + 4);

    quota_set(pid, cpu_lim, mem_lim, (uint8_t)fd_lim);

    return m3Err_none;
}

/* host_quota_check(pid, type) — check quota, type: 0=cpu, 1=mem, 2=fd */
static const void *host_quota_check_fn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid  = (int32_t)(int64_t)*(_sp + 1);
    int32_t type = (int32_t)(int64_t)*(_sp + 2);
    int result;

    switch (type) {
        case 0: result = quota_check_cpu(pid); break;
        case 2: result = quota_check_fd(pid); break;
        default: result = 0; break;
    }

    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)result;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Permission host functions                                                  */
/* -------------------------------------------------------------------------- */

/* host_perm_get_level() — return current module's permission level */
static const void *host_perm_get_level(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint8_t level = get_current_perm();
    int32_t *ret = (int32_t *)_sp;
    *ret = (int32_t)level;
    return m3Err_none;
}

/* host_perm_set_level(level) — set current module's permission level */
static const void *host_perm_set_level(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t level = (int32_t)(int64_t)*(_sp + 1);
    if (level < 0 || level > 3) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_none;
    }

    /* Find current module slot */
    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == current_module_id) {
            module_perm_level[i] = (uint8_t)level;
            int32_t *ret = (int32_t *)_sp;
            *ret = 0;
            return m3Err_none;
        }
    }
    int32_t *ret = (int32_t *)_sp;
    *ret = -1;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Kernel audit log host functions (v8.0)                                     */
/* -------------------------------------------------------------------------- */

/* Externs from audit.asm */
extern void audit_init(void);
extern void audit_log(uint16_t event_type, uint16_t event_level,
                      uint32_t proc_id, uint32_t user_id,
                      uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3);
extern int audit_query(uint8_t *buf, int max_events);
extern void audit_flush(void);
extern int audit_get_count(void);

/* Convenience wrappers for auto-audit */
void audit_process_create(int proc_id, int parent_id)
{
    uint32_t uid = 0; /* default root */
    audit_log(0x0004, 0, (uint32_t)proc_id, uid, (uint32_t)parent_id, 0, 0, 0);
}

void audit_quota_exceed(int proc_id, int quota_type)
{
    uint32_t uid = 0;
    audit_log(0x0006, 1, (uint32_t)proc_id, uid, (uint32_t)quota_type, 0, 0, 0);
}

/* host_audit_query(buf_off, max_events) — read audit log into WASM memory */
static const void *host_audit_query(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t buf_off   = (uint32_t)*(uint64_t*)(_sp + 1);
    uint32_t max_events = (uint32_t)*(uint64_t*)(_sp + 2);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (max_events == 0) {
        int32_t *ret = (int32_t *)_sp;
        *ret = 0;
        return m3Err_none;
    }
    if (max_events > 128) max_events = 128; /* AUDIT_MAX */
    if (buf_off + (max_events * 32) > mem_size) {
        int32_t *ret = (int32_t *)_sp;
        *ret = -1;
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    int count = audit_query(mem + buf_off, (int)max_events);

    int32_t *ret = (int32_t *)_sp;
    *ret = count;
    return m3Err_none;
}

/* host_audit_get_count() — return number of events in buffer */
static const void *host_audit_get_count(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t count = audit_get_count();
    int32_t *ret = (int32_t *)_sp;
    *ret = count;
    return m3Err_none;
}

/* host_audit_flush() — clear audit buffer */
static const void *host_audit_flush(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    audit_flush();
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Host function registration table                                           */
/* -------------------------------------------------------------------------- */

typedef struct {
    const char *module;     /* WASM import module name, e.g. "host" */
    const char *field;      /* WASM import field name, e.g. "print"   */
    const char *signature;  /* wasm3 signature string                 */
    void       *function;   /* C function pointer                     */
} host_reg_t;

static const host_reg_t host_registry[] = {
    { "host", "print",       "v(ii)",  &host_print       },
    { "host", "get_tick",    "I()",    &host_get_tick    },
    { "host", "exit",        "v(i)",   &host_exit        },
    { "host", "alloc",       "i(ii)",  &host_alloc       },
    { "host", "free",        "v(i)",   &host_free        },
    { "host", "log",         "v(iiii)",&host_log         },
    { "host", "getc",        "i()",    &host_getc        },
    { "host", "spawn",       "i(ii)",  &host_spawn       },
    { "host", "spawn_redirect","i(iiii)",&host_spawn_redirect },
    { "host", "get_argv",      "i(ii)",  &host_get_argv      },
    { "host", "set_argv",      "v(ii)",  &host_set_argv      },
    { "host", "vfs_open",    "i(iii)", &host_vfs_open    },
    { "host", "fs_open",     "i(ii)",  &host_fs_open     },
    { "host", "fs_read",     "i(iii)", &host_fs_read     },
    { "host", "fs_close",    "v(i)",   &host_fs_close    },
    { "host", "fs_list",     "i(ii)",  &host_fs_list     },
    { "host", "get_cwd",     "i(ii)",  &host_get_cwd     },
    { "host", "env_get",     "i(iiii)", &host_env_get    },
    { "host", "env_set",     "i(iiii)", &host_env_set    },
    { "host", "env_list",    "i(ii)",   &host_env_list   },
    { "host", "sleep",       "v(i)",   &host_sleep       },
    { "host", "yield",       "v()",    &host_yield       },
    { "host", "blk_read",    "i(iiii)",&host_blk_read    },
    { "host", "blk_write",   "i(iiii)",&host_blk_write   },
    { "host", "fs_write",    "i(iii)", &host_fs_write    },
    { "host", "fs_create",   "i(ii)",  &host_fs_create   },
    { "host", "fs_delete",   "i(ii)",  &host_fs_delete   },
    { "host", "net_connect", "i(iii)", &host_net_connect },
    { "host", "net_send",    "i(iii)", &host_net_send    },
    { "host", "net_recv",    "i(iii)", &host_net_recv    },
    { "host", "net_close",   "v(i)",   &host_net_close   },
    { "host", "net_listen",  "i(i)",   &host_net_listen  },
    { "host", "net_accept",  "i(i)",   &host_net_accept  },
    /* IPC: Pipes */
    { "host", "pipe_create", "i(ii)",  &host_pipe_create  },
    { "host", "pipe_read",   "i(iii)", &host_pipe_read    },
    { "host", "pipe_write",  "i(iii)", &host_pipe_write   },
    { "host", "pipe_close",  "v(i)",   &host_pipe_close   },
    /* IPC: Message Queues */
    { "host", "msgq_create", "i()",    &host_msgq_create  },
    { "host", "msgq_send",   "i(ii)",  &host_msgq_send    },
    { "host", "msgq_recv",   "i(ii)",  &host_msgq_recv    },
    { "host", "msgq_destroy","v(i)",   &host_msgq_destroy },
    /* Signals */
    { "host", "signal_register", "i(ii)", &host_signal_register },
    { "host", "signal_send",     "i(ii)", &host_signal_send     },
    /* IPC: Shared Memory */
    { "host", "shmem_alloc",      "i(ii)", &host_shmem_alloc     },
    { "host", "shmem_free",       "v(i)",  &host_shmem_free      },
    { "host", "shmem_attach",     "i(ii)", &host_shmem_attach    },
    { "host", "shmem_detach",     "v(i)",  &host_shmem_detach    },
    { "host", "shmem_get_ptr",    "i(i)",  &host_shmem_get_ptr   },
    { "host", "shmem_set_flag",   "v(ii)", &host_shmem_set_flag  },
    { "host", "shmem_get_flag",   "i(i)",  &host_shmem_get_flag  },
    { "host", "shmem_clear_flag", "v(ii)", &host_shmem_clear_flag },
    { "host", "shmem_write",      "i(iii)", &host_shmem_write    },
    { "host", "shmem_read",       "i(iii)", &host_shmem_read     },
    /* IPC: Semaphores */
    { "host", "sem_create",     "i(i)",  &host_sem_create     },
    { "host", "sem_wait",       "i(i)",  &host_sem_wait       },
    { "host", "sem_try_wait",   "i(i)",  &host_sem_try_wait   },
    { "host", "sem_post",       "i(i)",  &host_sem_post       },
    { "host", "sem_destroy",    "v(i)",  &host_sem_destroy    },
    { "host", "sem_get_value",  "i(i)",  &host_sem_get_value  },
    /* WASI snapshot_preview1 */
    { "wasi_snapshot_preview1", "fd_write",        "i(iiii)", &wasi_fd_write      },
    { "wasi_snapshot_preview1", "fd_read",         "i(iiii)", &wasi_fd_read       },
    { "wasi_snapshot_preview1", "fd_close",        "i(i)",    &wasi_fd_close      },
    { "wasi_snapshot_preview1", "fd_seek",         "i(iIi)",  &wasi_fd_seek       },
    { "wasi_snapshot_preview1", "fd_tell",         "i(ii)",   &wasi_fd_tell       },
    { "wasi_snapshot_preview1", "path_open",       "i(iiiiiIIIi)", &wasi_path_open },
    { "wasi_snapshot_preview1", "path_removefile", "i(ii)",   &wasi_path_removefile },
    { "wasi_snapshot_preview1", "proc_exit",       "v(i)",    &wasi_proc_exit     },
    { "wasi_snapshot_preview1", "clock_time_get",  "i(iiI)",  &wasi_clock_time_get },
    { "wasi_snapshot_preview1", "random_get",      "i(ii)",   &wasi_random_get    },
    { "wasi_snapshot_preview1", "args_sizes_get",  "i(ii)",   &wasi_args_sizes_get },
    { "wasi_snapshot_preview1", "args_get",        "i(ii)",   &wasi_args_get       },
    { "wasi_snapshot_preview1", "environ_sizes_get","i(ii)",  &wasi_environ_sizes_get },
    { "wasi_snapshot_preview1", "environ_get",     "i(ii)",   &wasi_environ_get    },
    { "wasi_snapshot_preview1", "fd_fdstat_get",   "i(ii)",   &wasi_fd_fdstat_get  },
    /* GUI */
    { "host", "gui_blit",   "v(iiiiii)", &host_gui_blit    },
    { "host", "gui_create", "i(iiiii)", &host_gui_create  },
    { "host", "gui_draw",   "i(iiiiii)",&host_gui_draw    },
    { "host", "gui_flush",  "v(i)",     &host_gui_flush   },
    { "host", "gui_poll_event", "i(ii)", &host_gui_poll_event },
    { "host", "gui_swap",   "v()",    &host_gui_swap    },
    /* Process Management */
    { "host", "proc_list_next",  "i()",  &host_proc_list_next  },
    { "host", "proc_get_status", "i(i)", &host_proc_get_status },
    { "host", "proc_restart",    "i(i)", &host_proc_restart    },
    { "host", "proc_kill",       "v(i)", &host_proc_kill       },
    /* Kernel Log Ring Buffer */
    { "host", "log_read",    "i(ii)", &host_log_read    },
    { "host", "log_size",    "i()",   &host_log_size    },
    /* User Authentication */
    { "host", "user_login",  "i(iiii)", &host_user_login },
    /* Device Management */
    { "host", "device_list",     "i(ii)",  &host_device_list_fn    },
    { "host", "device_status",   "i(i)",   &host_device_status_fn  },
    { "host", "device_attach",   "i(ii)",  &host_device_attach_fn  },
    { "host", "device_detach",   "i(i)",   &host_device_detach_fn  },
    /* Permission queries */
    { "host", "perm_get_level", "i()",  &host_perm_get_level },
    { "host", "perm_set_level", "v(i)", &host_perm_set_level },
    /* Resource quotas */
    { "host", "quota_get",     "i(ii)", &host_quota_get },
    { "host", "quota_set",     "v(iiii)", &host_quota_set },
    { "host", "quota_check",   "i(ii)", &host_quota_check_fn },
    /* Audit log */
    { "host", "audit_query",     "i(ii)", &host_audit_query    },
    { "host", "audit_get_count", "i()",   &host_audit_get_count },
    { "host", "audit_flush",     "v()",   &host_audit_flush    },
    /* Module hot-reload */
    { "host", "module_reload",   "i(i)",  &host_module_reload  },
    /* Kernel event notification */
    { "host", "event_subscribe",     "i(ii)", &host_event_subscribe     },
    { "host", "event_unsubscribe",   "i(ii)", &host_event_unsubscribe   },
    { "host", "event_notify",        "v(iii)",&host_event_notify        },
    { "host", "event_poll",          "i(ii)", &host_event_poll          },
    /* Persistence */
    { "host", "persist_sync",         "i()",  &host_persist_sync         },
    { "host", "persist_get_sync_count","i()",  &host_persist_get_sync_count },
};

#define HOST_REG_COUNT (sizeof(host_registry) / sizeof(host_registry[0]))

/* -------------------------------------------------------------------------- */
/* register_host_functions: batch register all host functions                 */
/* -------------------------------------------------------------------------- */

static const char *register_host_functions(IM3Module module)
{
    M3Result result;

    for (unsigned int i = 0; i < HOST_REG_COUNT; i++) {
        const host_reg_t *reg = &host_registry[i];
        result = m3_LinkRawFunctionEx(module, reg->module, reg->field,
                                    reg->signature, reg->function, NULL);
        if (result && result != m3Err_functionLookupFailed) {
            LOG_ERROR("wasm_link", reg->field);
            return result;
        }
    }

    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* load_module_internal: load, link, compile a module into a specific slot    */
/* -------------------------------------------------------------------------- */

const char *load_module_internal(int slot_idx, const char *name,
                                  const uint8_t *wasm_bytes, uint32_t wasm_size)
{
    if (slot_idx < 0 || slot_idx >= MAX_MODULES)
        return "invalid slot";

    /* Validate WASM magic */
    if (wasm_size < 4 || wasm_bytes[0] != 0x00 || wasm_bytes[1] != 0x61 ||
        wasm_bytes[2] != 0x73 || wasm_bytes[3] != 0x6d) {
        LOG_ERROR("wasm_load", "invalid magic");
        return "invalid WASM magic";
    }

    wasm_module_slot_t *slot = &module_table[slot_idx];
    uint32_t id = next_module_id++;

    slot->id = id;
    slot->state = MOD_LOADING;
    slot->exit_code = 0;
    slot->stdin_pipe_fd = -1;
    slot->stdout_pipe_fd = -1;

    /* Copy name */
    unsigned int nlen = my_strlen(name);
    if (nlen >= MAX_NAME_LEN) nlen = MAX_NAME_LEN - 1;
    for (unsigned int i = 0; i < nlen; i++) slot->name[i] = name[i];
    slot->name[nlen] = '\0';

    /* Create runtime */
    slot->runtime = m3_NewRuntime(g_shared_env, 8192, NULL);
    if (!slot->runtime) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_init", "m3_NewRuntime failed");
        return "m3_NewRuntime failed";
    }

    /* Parse module */
    M3Result result = m3_ParseModule(g_shared_env, &slot->module, wasm_bytes, wasm_size);
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_parse", result);
        return result;
    }

    /* Load module into runtime */
    result = m3_LoadModule(slot->runtime, slot->module);
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_load", result);
        return result;
    }

    /* Register host functions */
    result = register_host_functions(slot->module);
    if (result) {
        slot->state = MOD_FREE;
        return result;
    }

    /* Check JIT cache before compiling */
    uint32_t wasm_hash = crc32_hash(wasm_bytes, wasm_size);
    void *cached_code = jit_cache_lookup(wasm_hash, name);
    if (cached_code) {
        /* Use cached compiled code — skip compilation */
        LOG_INFO("wasm_cache", "hit");
    } else {
        /* Compile all functions */
        result = m3_CompileModule(slot->module);
        if (result) {
            slot->state = MOD_FREE;
            LOG_ERROR("wasm_compile", result);
            return result;
        }

        /* Store in JIT cache for future reloads */
        /* Cache the compiled code pointer from the module */
        jit_cache_store(wasm_hash, name, slot->module, wasm_size);
    }

    /* Find entry point */
    result = m3_FindFunction(&slot->entry, slot->runtime, "_start");
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_find", result);
        return result;
    }

    slot->state = MOD_READY;
    module_count++;

    return NULL;  /* success */
}

const char *load_module(const char *name, uint32_t *out_id)
{
    /* Find registry entry */
    wasm_registry_entry_t *entry = find_registry_entry(name);
    if (!entry) return "unknown module name";

    /* Find free slot */
    int slot = find_module_slot();
    if (slot < 0) return "no free module slot";

    const char *err = load_module_internal(slot, name, entry->wasm_bytes, entry->wasm_size);
    if (err) return err;

    *out_id = module_table[slot].id;
    return NULL;
}

const char *run_module(uint32_t module_id)
{
    /* Find slot */
    int slot_idx = -1;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == module_id) {
            slot_idx = i;
            break;
        }
    }
    if (slot_idx < 0) return "module not found";

    wasm_module_slot_t *slot = &module_table[slot_idx];
    if (slot->state != MOD_READY) return "module not ready";

    slot->state = MOD_RUNNING;
    current_module_id = module_id;

    M3Result result = m3_Call(slot->entry, 0, NULL);
    if (result) {
        LOG_ERROR("wasm_run", result);
        return result;
    }

    if (slot->state == MOD_RUNNING) {
        slot->state = MOD_EXITED;
    }

    return NULL;
}

int find_ready_module(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].state == MOD_READY) {
            return (int)module_table[i].id;
        }
    }
    return -1;
}

void terminate_module(uint32_t module_id)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == module_id) {
            module_table[i].state = MOD_EXITED;
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* wasm_host_init_multi: set up shared environment, parse RAM disk            */
/* -------------------------------------------------------------------------- */

const char *wasm_host_init_multi(void)
{
    /* Initialize shared environment */
    g_shared_env = m3_NewEnvironment();
    if (!g_shared_env) {
        LOG_ERROR("wasm_init", "m3_NewEnvironment failed");
        return "m3_NewEnvironment failed";
    }

    /* Zero module table */
    for (int i = 0; i < MAX_MODULES; i++) {
        module_table[i].state = MOD_FREE;
        module_table[i].id = 0;
        module_table[i].name[0] = '\0';
    }
    module_count = 0;
    next_module_id = 1;
    current_module_id = 0;

    /* Zero open files */
    for (int i = 0; i < MAX_OPEN_FILES; i++)
        open_files[i].active = 0;

    /* Resolve embedded WASM registry */
    wasm_registry[0].wasm_bytes = init_module_start;
    wasm_registry[0].wasm_size = init_module_size;
    wasm_registry[1].wasm_bytes = shell_module_start;
    wasm_registry[1].wasm_size = shell_module_size;
    wasm_registry[2].wasm_bytes = test_module_start;
    wasm_registry[2].wasm_size = test_module_size;
    wasm_registry[3].wasm_bytes = proc_monitor_module_start;
    wasm_registry[3].wasm_size = proc_monitor_module_size;
    wasm_registry[4].wasm_bytes = syslog_module_start;
    wasm_registry[4].wasm_size = syslog_module_size;
    wasm_registry[5].wasm_bytes = filemgr_module_start;
    wasm_registry[5].wasm_size = filemgr_module_size;
    wasm_registry[6].wasm_bytes = settings_module_start;
    wasm_registry[6].wasm_size = settings_module_size;
    wasm_registry[7].wasm_bytes = user_module_start;
    wasm_registry[7].wasm_size = user_module_size;
    wasm_registry[8].wasm_bytes = awk_module_start;
    wasm_registry[8].wasm_size = awk_module_size;
    wasm_registry[9].wasm_bytes = date_module_start;
    wasm_registry[9].wasm_size = date_module_size;
    wasm_registry[10].wasm_bytes = devmgr_module_start;
    wasm_registry[10].wasm_size = devmgr_module_size;
    wasm_registry[11].wasm_bytes = httpd_module_start;
    wasm_registry[11].wasm_size = httpd_module_size;
    wasm_registry[12].wasm_bytes = dns_resolver_module_start;
    wasm_registry[12].wasm_size = dns_resolver_module_size;
    wasm_registry[13].wasm_bytes = shmem_test_module_start;
    wasm_registry[13].wasm_size = shmem_test_module_size;
    wasm_registry[14].wasm_bytes = grep_module_start;
    wasm_registry[14].wasm_size = grep_module_size;
    wasm_registry[15].wasm_bytes = cat_module_start;
    wasm_registry[15].wasm_size = cat_module_size;
    wasm_registry[16].wasm_bytes = echo_module_start;
    wasm_registry[16].wasm_size = echo_module_size;
    wasm_registry[17].wasm_bytes = tee_module_start;
    wasm_registry[17].wasm_size = tee_module_size;
    wasm_registry[18].wasm_bytes = wc_module_start;
    wasm_registry[18].wasm_size = wc_module_size;
    wasm_registry[19].wasm_bytes = head_module_start;
    wasm_registry[19].wasm_size = head_module_size;
    wasm_registry[20].wasm_bytes = tail_module_start;
    wasm_registry[20].wasm_size = tail_module_size;
    wasm_registry[21].wasm_bytes = sort_module_start;
    wasm_registry[21].wasm_size = sort_module_size;
    wasm_registry[22].wasm_bytes = uniq_module_start;
    wasm_registry[22].wasm_size = uniq_module_size;
    wasm_registry[23].wasm_bytes = tr_module_start;
    wasm_registry[23].wasm_size = tr_module_size;
    wasm_registry[24].wasm_bytes = cut_module_start;
    wasm_registry[24].wasm_size = cut_module_size;
    wasm_registry[25].wasm_bytes = sed_module_start;
    wasm_registry[25].wasm_size = sed_module_size;
    wasm_registry[26].wasm_bytes = xargs_module_start;
    wasm_registry[26].wasm_size = xargs_module_size;
    wasm_registry[27].wasm_bytes = ls_module_start;
    wasm_registry[27].wasm_size = ls_module_size;
    wasm_registry[28].wasm_bytes = pwd_module_start;
    wasm_registry[28].wasm_size = pwd_module_size;
    wasm_registry[29].wasm_bytes = env_module_start;
    wasm_registry[29].wasm_size = env_module_size;
    wasm_registry[30].wasm_bytes = printf_module_start;
    wasm_registry[30].wasm_size = printf_module_size;
    wasm_registry[31].wasm_bytes = sleep_module_start;
    wasm_registry[31].wasm_size = sleep_module_size;
    wasm_registry[32].wasm_bytes = yes_module_start;
    wasm_registry[32].wasm_size = yes_module_size;
    wasm_registry[33].wasm_bytes = rev_module_start;
    wasm_registry[33].wasm_size = rev_module_size;
    wasm_registry[34].wasm_bytes = cmp_module_start;
    wasm_registry[34].wasm_size = cmp_module_size;
    wasm_registry[35].wasm_bytes = diff_module_start;
    wasm_registry[35].wasm_size = diff_module_size;
    wasm_registry[36].wasm_bytes = basename_module_start;
    wasm_registry[36].wasm_size = basename_module_size;
    wasm_registry[37].wasm_bytes = dirname_module_start;
    wasm_registry[37].wasm_size = dirname_module_size;
    wasm_registry[38].wasm_bytes = nl_module_start;
    wasm_registry[38].wasm_size = nl_module_size;
    wasm_registry[39].wasm_bytes = shuf_module_start;
    wasm_registry[39].wasm_size = shuf_module_size;
    wasm_registry[40].wasm_bytes = comm_module_start;
    wasm_registry[40].wasm_size = comm_module_size;
    wasm_registry[41].wasm_bytes = paste_module_start;
    wasm_registry[41].wasm_size = paste_module_size;
    wasm_registry[42].wasm_bytes = fold_module_start;
    wasm_registry[42].wasm_size = fold_module_size;
    wasm_registry[43].wasm_bytes = tac_module_start;
    wasm_registry[43].wasm_size = tac_module_size;
    wasm_registry[44].wasm_bytes = cksum_module_start;
    wasm_registry[44].wasm_size = cksum_module_size;
    wasm_registry[45].wasm_bytes = sum_module_start;
    wasm_registry[45].wasm_size = sum_module_size;
    wasm_registry[46].wasm_bytes = touch_module_start;
    wasm_registry[46].wasm_size = touch_module_size;
    wasm_registry[47].wasm_bytes = true_module_start;
    wasm_registry[47].wasm_size = true_module_size;
    wasm_registry[48].wasm_bytes = false_module_start;
    wasm_registry[48].wasm_size = false_module_size;
    wasm_registry[49].wasm_bytes = id_module_start;
    wasm_registry[49].wasm_size = id_module_size;
    wasm_registry[50].wasm_bytes = uptime_module_start;
    wasm_registry[50].wasm_size = uptime_module_size;
    wasm_registry[51].wasm_bytes = whoami_module_start;
    wasm_registry[51].wasm_size = whoami_module_size;
    wasm_registry[52].wasm_bytes = hostname_module_start;
    wasm_registry[52].wasm_size = hostname_module_size;
    wasm_registry[53].wasm_bytes = uname_module_start;
    wasm_registry[53].wasm_size = uname_module_size;
    wasm_registry[54].wasm_bytes = df_module_start;
    wasm_registry[54].wasm_size = df_module_size;
    wasm_registry[55].wasm_bytes = free_module_start;
    wasm_registry[55].wasm_size = free_module_size;
    wasm_registry[56].wasm_bytes = ps_module_start;
    wasm_registry[56].wasm_size = ps_module_size;
    wasm_registry[57].wasm_bytes = kill_module_start;
    wasm_registry[57].wasm_size = kill_module_size;
    wasm_registry[58].wasm_bytes = mkdir_module_start;
    wasm_registry[58].wasm_size = mkdir_module_size;
    wasm_registry[59].wasm_bytes = rmdir_module_start;
    wasm_registry[59].wasm_size = rmdir_module_size;
    wasm_registry[60].wasm_bytes = ln_module_start;
    wasm_registry[60].wasm_size = ln_module_size;
    wasm_registry[61].wasm_bytes = rm_module_start;
    wasm_registry[61].wasm_size = rm_module_size;
    wasm_registry[62].wasm_bytes = cp_module_start;
    wasm_registry[62].wasm_size = cp_module_size;
    wasm_registry[63].wasm_bytes = mv_module_start;
    wasm_registry[63].wasm_size = mv_module_size;
    wasm_registry[64].wasm_bytes = chmod_module_start;
    wasm_registry[64].wasm_size = chmod_module_size;
    wasm_registry[65].wasm_bytes = tty_module_start;
    wasm_registry[65].wasm_size = tty_module_size;
    wasm_registry[66].wasm_bytes = who_module_start;
    wasm_registry[66].wasm_size = who_module_size;
    wasm_registry[67].wasm_bytes = stty_module_start;
    wasm_registry[67].wasm_size = stty_module_size;
    wasm_registry[68].wasm_bytes = tput_module_start;
    wasm_registry[68].wasm_size = tput_module_size;
    wasm_registry[69].wasm_bytes = readlink_module_start;
    wasm_registry[69].wasm_size = readlink_module_size;
    wasm_registry[70].wasm_bytes = cal_module_start;
    wasm_registry[70].wasm_size = cal_module_size;
    wasm_registry[71].wasm_bytes = find_module_start;
    wasm_registry[71].wasm_size = find_module_size;
    wasm_registry[72].wasm_bytes = du_module_start;
    wasm_registry[72].wasm_size = du_module_size;
    wasm_registry[73].wasm_bytes = mount_module_start;
    wasm_registry[73].wasm_size = mount_module_size;
    wasm_registry[74].wasm_bytes = umount_module_start;
    wasm_registry[74].wasm_size = umount_module_size;
    wasm_registry[75].wasm_bytes = ifconfig_module_start;
    wasm_registry[75].wasm_size = ifconfig_module_size;
    wasm_registry[76].wasm_bytes = ping_module_start;
    wasm_registry[76].wasm_size = ping_module_size;
    wasm_registry[77].wasm_bytes = netstat_module_start;
    wasm_registry[77].wasm_size = netstat_module_size;
    wasm_registry[78].wasm_bytes = route_module_start;
    wasm_registry[78].wasm_size = route_module_size;
    wasm_registry[79].wasm_bytes = traceroute_module_start;
    wasm_registry[79].wasm_size = traceroute_module_size;
    wasm_registry[80].wasm_bytes = nslookup_module_start;
    wasm_registry[80].wasm_size = nslookup_module_size;
    wasm_registry[81].wasm_bytes = wget_module_start;
    wasm_registry[81].wasm_size = wget_module_size;
    wasm_registry[82].wasm_bytes = curl_module_start;
    wasm_registry[82].wasm_size = curl_module_size;
    wasm_registry[83].wasm_bytes = ssh_module_start;
    wasm_registry[83].wasm_size = ssh_module_size;
    wasm_registry[84].wasm_bytes = scp_module_start;
    wasm_registry[84].wasm_size = scp_module_size;
    wasm_registry[85].wasm_bytes = tar_module_start;
    wasm_registry[85].wasm_size = tar_module_size;
    wasm_registry[86].wasm_bytes = gzip_module_start;
    wasm_registry[86].wasm_size = gzip_module_size;
    wasm_registry[87].wasm_bytes = gunzip_module_start;
    wasm_registry[87].wasm_size = gunzip_module_size;
    wasm_registry[88].wasm_bytes = zcat_module_start;
    wasm_registry[88].wasm_size = zcat_module_size;
    wasm_registry[89].wasm_bytes = bzip2_module_start;
    wasm_registry[89].wasm_size = bzip2_module_size;
    wasm_registry[90].wasm_bytes = bunzip2_module_start;
    wasm_registry[90].wasm_size = bunzip2_module_size;
    wasm_registry[91].wasm_bytes = lzma_module_start;
    wasm_registry[91].wasm_size = lzma_module_size;
    wasm_registry[92].wasm_bytes = unlzma_module_start;
    wasm_registry[92].wasm_size = unlzma_module_size;
    wasm_registry[93].wasm_bytes = xz_module_start;
    wasm_registry[93].wasm_size = xz_module_size;
    wasm_registry[94].wasm_bytes = unxz_module_start;
    wasm_registry[94].wasm_size = unxz_module_size;
    wasm_registry[95].wasm_bytes = sha256sum_module_start;
    wasm_registry[95].wasm_size = sha256sum_module_size;
    wasm_registry[96].wasm_bytes = md5sum_module_start;
    wasm_registry[96].wasm_size = md5sum_module_size;
    wasm_registry[97].wasm_bytes = base64_module_start;
    wasm_registry[97].wasm_size = base64_module_size;
    wasm_registry[98].wasm_bytes = base64d_module_start;
    wasm_registry[98].wasm_size = base64d_module_size;
    wasm_registry[99].wasm_bytes = od_module_start;
    wasm_registry[99].wasm_size = od_module_size;
    wasm_registry[100].wasm_bytes = hexdump_module_start;
    wasm_registry[100].wasm_size = hexdump_module_size;
    wasm_registry[101].wasm_bytes = strings_module_start;
    wasm_registry[101].wasm_size = strings_module_size;
    wasm_registry[102].wasm_bytes = file_module_start;
    wasm_registry[102].wasm_size = file_module_size;
    wasm_registry[103].wasm_bytes = xxd_module_start;
    wasm_registry[103].wasm_size = xxd_module_size;
    wasm_registry[104].wasm_bytes = less_module_start;
    wasm_registry[104].wasm_size = less_module_size;
    wasm_registry[105].wasm_bytes = more_module_start;
    wasm_registry[105].wasm_size = more_module_size;
    wasm_registry[106].wasm_bytes = join_module_start;
    wasm_registry[106].wasm_size = join_module_size;
    wasm_registry[107].wasm_bytes = split_module_start;
    wasm_registry[107].wasm_size = split_module_size;
    wasm_registry[108].wasm_bytes = patch_module_start;
    wasm_registry[108].wasm_size = patch_module_size;
    wasm_registry[109].wasm_bytes = diff3_module_start;
    wasm_registry[109].wasm_size = diff3_module_size;
    wasm_registry[110].wasm_bytes = tsort_module_start;
    wasm_registry[110].wasm_size = tsort_module_size;
    wasm_registry[111].wasm_bytes = csplit_module_start;
    wasm_registry[111].wasm_size = csplit_module_size;
    wasm_registry[112].wasm_bytes = fmt_module_start;
    wasm_registry[112].wasm_size = fmt_module_size;
    wasm_registry[113].wasm_bytes = pr_module_start;
    wasm_registry[113].wasm_size = pr_module_size;
    wasm_registry[114].wasm_bytes = column_module_start;
    wasm_registry[114].wasm_size = column_module_size;
    wasm_registry[115].wasm_bytes = expand_module_start;
    wasm_registry[115].wasm_size = expand_module_size;
    wasm_registry[116].wasm_bytes = unexpand_module_start;
    wasm_registry[116].wasm_size = unexpand_module_size;

    /* Initialize default environment variables */
    env_strcpy(env_table[0].key, "PATH", MAX_ENV_KEY);
    env_strcpy(env_table[0].val, "/bin", MAX_ENV_VAL);
    env_table[0].active = 1;
    env_strcpy(env_table[1].key, "HOME", MAX_ENV_KEY);
    env_strcpy(env_table[1].val, "/", MAX_ENV_VAL);
    env_table[1].active = 1;
    env_strcpy(env_table[2].key, "SHELL", MAX_ENV_KEY);
    env_strcpy(env_table[2].val, "shell", MAX_ENV_VAL);
    env_table[2].active = 1;

    /* Initialize RAM disk */
    extern const uint8_t ramdisk_start[];
    extern const uint32_t ramdisk_size;
    ramdisk_init(ramdisk_start, ramdisk_size);

    LOG_INFO("wasm_init", "multi-module runtime ready");
    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* Legacy compatibility: single-module load/run                               */
/* -------------------------------------------------------------------------- */

const char *wasm_host_load(const uint8_t *wasm_bytes, uint32_t wasm_size)
{
    /* Use slot 0 for legacy single-module loading */
    int slot = find_module_slot();
    if (slot < 0) return "no free module slot";

    return load_module_internal(slot, "legacy", wasm_bytes, wasm_size);
}

const char *wasm_host_run(void)
{
    /* Find the first READY module and run it */
    int ready_id = find_ready_module();
    if (ready_id < 0) return "no ready module";

    return run_module((uint32_t)ready_id);
}

/* -------------------------------------------------------------------------- */
/* run_ready_modules: find and run all READY modules in order                 */
/* -------------------------------------------------------------------------- */

const char *run_ready_modules_c(void)
{
    for (;;) {
        int ready_id = find_ready_module();
        if (ready_id < 0) break;

        const char *err = run_module((uint32_t)ready_id);
        if (err) return err;
    }
    return NULL;
}
