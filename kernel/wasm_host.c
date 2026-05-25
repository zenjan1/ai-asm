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

static wasm_registry_entry_t wasm_registry[] = {
    { "init",           NULL, 0 },
    { "shell",          NULL, 0 },
    { "test",           NULL, 0 },
    { "proc_monitor",   NULL, 0 },
    { "syslog",         NULL, 0 },
    { "filemgr",        NULL, 0 },
    { "settings",       NULL, 0 },
    { "user",           NULL, 0 },
    { "devmgr",         NULL, 0 },
    { "httpd",          NULL, 0 },
    { "dns_resolver",   NULL, 0 },
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

/* host_print(offset, len) — print WASM linear memory to UART */
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
    (void)runtime; (void)_ctx; (void)_mem;

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
    int32_t *ret = (int32_t *)(_sp);
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

    uart_puts_raw("ok\n");
    *ret_val = (int32_t)module_table[slot].id;
    return m3Err_none;
}

/* host_fs_open(path_ptr, path_len) — open file on RAM disk */
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
    int fd = ramdisk_open(path, path_len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)fd;
    return m3Err_none;
}

/* host_fs_read(fd, buf_ptr, len) — read from file */
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

    int n = ramdisk_read(fd, mem + buf_off, len);

    int32_t *ret = (int32_t *)(_sp);
    *ret = (int32_t)n;
    return m3Err_none;
}

/* host_fs_close(fd) — close file */
static const void *host_fs_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd = (int32_t)(int64_t)*(_sp + 1);
    ramdisk_close(fd);
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

/* host_fs_write(fd, buf_off, len) — write to open file */
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

    extern int fs_file_write(int fd, const uint8_t *buf, uint32_t len);
    int rc = fs_file_write(fd, mem + buf_off, len);

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

    if (argc_out + 4 <= mem_size) *(uint32_t *)(mem + argc_out) = 0;
    if (argv_buf_out + 4 <= mem_size) *(uint32_t *)(mem + argv_buf_out) = 0;

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

/* host_net_close(sock) — close socket */
static const void *host_net_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t sock = (int32_t)(int64_t)*(_sp + 1);

    extern int net_close(int sock);
    net_close(sock);

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

/* pipe_create(read_fd_off, write_fd_off) — create pipe, store fds */
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

/* pipe_read(fd, buf_off, len) — read from pipe */
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

/* pipe_write(fd, buf_off, len) — write to pipe */
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

/* pipe_close(fd) — close pipe */
static const void *host_pipe_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int fd = (int)*(int64_t*)(_sp + 1);
    (void)_ctx; (void)_mem;

    extern int pipe_close(int fd);
    int rc = pipe_close(fd);
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
    { "host", "fs_open",     "i(ii)",  &host_fs_open     },
    { "host", "fs_read",     "i(iii)", &host_fs_read     },
    { "host", "fs_close",    "v(i)",   &host_fs_close    },
    { "host", "fs_list",     "i(ii)",  &host_fs_list     },
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
    { "wasi_snapshot_preview1", "environ_sizes_get","i(ii)",  &wasi_environ_sizes_get },
    /* GUI */
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
    wasm_registry[8].wasm_bytes = devmgr_module_start;
    wasm_registry[8].wasm_size = devmgr_module_size;
    wasm_registry[9].wasm_bytes = httpd_module_start;
    wasm_registry[9].wasm_size = httpd_module_size;
    wasm_registry[10].wasm_bytes = dns_resolver_module_start;
    wasm_registry[10].wasm_size = dns_resolver_module_size;

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
