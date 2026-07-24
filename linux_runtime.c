/*
 * aiasm-aarch64/linux_runtime.c
 * Linux-hosted WASM runtime for AI-ASM AArch64
 *
 * Provides the same host_* ABI as the bare-metal kernel, but maps to
 * Linux syscalls. Allows WASM modules to run under proot on Termux
 * without needing QEMU or bare-metal hardware.
 *
 * Build: clang -o linux_runtime linux_runtime.c kernel/wasm3/source/*.c
 *        -Ikernel/wasm3/source -Ikernel -Dd_m3FixedHeap=8388608 ...
 *
 * Usage: ./linux_runtime --modules-dir modules --boot init
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sched.h>
#include <sys/select.h>

/* Wasm3 headers */
#include "wasm3.h"
#include "m3_env.h"
#include "m3_compile.h"

/* -------------------------------------------------------------------------- */
/* Type definitions (from kernel/module_runtime.h)                            */
/* -------------------------------------------------------------------------- */

#define MAX_MODULES         64
#define MAX_RAMDISK_FILES   64
#define MAX_OPEN_FILES      16
#define MAX_NAME_LEN        32
#define MAX_PATH_LEN        128

typedef enum {
    MOD_FREE = 0,
    MOD_LOADING,
    MOD_READY,
    MOD_RUNNING,
    MOD_EXITED
} module_state_t;

typedef struct {
    module_state_t state;
    uint32_t id;
    char name[MAX_NAME_LEN];
    IM3Environment env;
    IM3Runtime runtime;
    IM3Module module;
    IM3Function entry;
    int exit_code;
    int stdin_pipe_fd;
    int stdout_pipe_fd;
} wasm_module_slot_t;

/* -------------------------------------------------------------------------- */
/* Configuration                                                              */
/* -------------------------------------------------------------------------- */

#define MODULES_DIR_MAX  256
#define MODULE_PATH_MAX  512

static char g_modules_dir[MODULES_DIR_MAX] = "modules";
static char g_boot_module[64] = "init";
static volatile uint8_t g_running = 1;

/* -------------------------------------------------------------------------- */
/* Globals (match bare-metal kernel)                                          */
/* -------------------------------------------------------------------------- */

IM3Environment g_shared_env = NULL;
wasm_module_slot_t module_table[MAX_MODULES];
uint32_t module_count = 0;
uint32_t next_module_id = 1;
uint32_t current_module_id = 0;

/* Per-module argv storage */
static char module_argv[MAX_MODULES][128];
static uint32_t module_argc[MAX_MODULES];
static char pending_argv[128];
static uint32_t pending_argc = 0;

/* -------------------------------------------------------------------------- */
/* Module registry: scans modules/ directory for .wasm files                  */
/* -------------------------------------------------------------------------- */

#define MAX_REGISTRY 4096

typedef struct {
    char name[MAX_NAME_LEN];
    char path[MODULE_PATH_MAX];
    uint8_t *wasm_bytes;   /* NULL until loaded on demand */
    uint32_t wasm_size;
} wasm_registry_entry_t;

static wasm_registry_entry_t g_registry[MAX_REGISTRY];
static uint32_t g_registry_count = 0;

/* Load WASM bytes for a single module on demand */
static int load_wasm_bytes(wasm_registry_entry_t *entry)
{
    if (entry->wasm_bytes) return 0;  /* already loaded */

    struct stat st;
    if (stat(entry->path, &st) != 0 || st.st_size == 0) return -1;

    FILE *fp = fopen(entry->path, "rb");
    if (!fp) return -1;

    uint8_t *buf = (uint8_t *)malloc(st.st_size);
    if (!buf) { fclose(fp); return -1; }

    size_t nread = fread(buf, 1, st.st_size, fp);
    fclose(fp);

    if (nread != (size_t)st.st_size) { free(buf); return -1; }

    /* Validate WASM magic */
    if (nread < 4 || buf[0] != 0x00 || buf[1] != 0x61 ||
        buf[2] != 0x73 || buf[3] != 0x6d) {
        free(buf);
        return -1;
    }

    entry->wasm_bytes = buf;
    entry->wasm_size = (uint32_t)st.st_size;
    return 0;
}

static void free_wasm_bytes(wasm_registry_entry_t *entry)
{
    if (entry->wasm_bytes) {
        free(entry->wasm_bytes);
        entry->wasm_bytes = NULL;
        entry->wasm_size = 0;
    }
}

static int scan_modules_dir(const char *dir)
{
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "[runtime] ERROR: cannot open modules dir: %s\n", dir);
        return -1;
    }

    struct dirent *ep;
    while ((ep = readdir(dp)) != NULL) {
        if (g_registry_count >= MAX_REGISTRY) break;

        /* Look for directories containing .wasm files */
        char full_path[MODULE_PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, ep->d_name);

        struct stat st;
        if (stat(full_path, &st) != 0 || !S_ISDIR(st.st_mode)) continue;

        /* Skip hidden directories */
        if (ep->d_name[0] == '.') continue;

        /* Check for .wasm file inside */
        char wasm_path[MODULE_PATH_MAX];
        snprintf(wasm_path, sizeof(wasm_path), "%s/%s.wasm", full_path, ep->d_name);

        if (stat(wasm_path, &st) != 0 || st.st_size == 0) continue;

        wasm_registry_entry_t *entry = &g_registry[g_registry_count];
        strncpy(entry->name, ep->d_name, MAX_NAME_LEN - 1);
        entry->name[MAX_NAME_LEN - 1] = '\0';
        strncpy(entry->path, wasm_path, MODULE_PATH_MAX - 1);
        entry->path[MODULE_PATH_MAX - 1] = '\0';
        entry->wasm_bytes = NULL;  /* lazy load */
        entry->wasm_size = 0;
        g_registry_count++;
    }

    closedir(dp);
    printf("[runtime] total modules registered: %u (lazy load enabled)\n", g_registry_count);
    return (int)g_registry_count;
}

static wasm_registry_entry_t *find_registry_entry(const char *name)
{
    for (uint32_t i = 0; i < g_registry_count; i++) {
        if (strcmp(g_registry[i].name, name) == 0) {
            /* Lazy load WASM bytes if not yet loaded */
            if (load_wasm_bytes(&g_registry[i]) != 0) {
                fprintf(stderr, "[runtime] failed to load WASM for: %s\n", name);
                return NULL;
            }
            return &g_registry[i];
        }
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Module slot management                                                     */
/* -------------------------------------------------------------------------- */

static int find_module_slot(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].state == MOD_FREE) return i;
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
/* String helpers (freestanding-compatible)                                   */
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
/* Logging                                                                    */
/* -------------------------------------------------------------------------- */

static void kernel_log(const char *level, const char *event, const char *data)
{
    fprintf(stderr, "{\"level\":\"%s\",\"event\":\"%s\"", level, event);
    if (data && *data) fprintf(stderr, ",\"data\":\"%s\"", data);
    fprintf(stderr, "}\n");
}

#define LOG_ERROR(event, data)  kernel_log("ERROR", (event), (data))
#define LOG_INFO(event, data)   kernel_log("INFO",  (event), (data))

/* -------------------------------------------------------------------------- */
/* JIT Cache (simplified for Linux: just skip, recompile each time)           */
/* -------------------------------------------------------------------------- */

void jit_cache_init(void) {}
void *jit_cache_lookup(uint32_t name_hash, const char *name) { (void)name_hash; (void)name; return NULL; }
int jit_cache_store(uint32_t name_hash, const char *name, const void *code, uint32_t size)
{ (void)name_hash; (void)name; (void)code; (void)size; return 0; }

/* -------------------------------------------------------------------------- */
/* Preempt flag (Linux: always 0, we use cooperative scheduling)              */
/* -------------------------------------------------------------------------- */

volatile uint8_t preempt_pending = 0;

/* -------------------------------------------------------------------------- */
/* Forward declarations                                                       */
/* -------------------------------------------------------------------------- */

static void register_host_functions_for_module(IM3Module module);
const char *run_module(uint32_t module_id);

/* -------------------------------------------------------------------------- */
/* Module loading                                                             */
/* -------------------------------------------------------------------------- */

static const char *load_module_internal(int slot_idx, const char *name,
                                         const uint8_t *wasm_bytes, uint32_t wasm_size)
{
    if (slot_idx < 0 || slot_idx >= MAX_MODULES)
        return "invalid slot";

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

    unsigned int nlen = my_strlen(name);
    if (nlen >= MAX_NAME_LEN) nlen = MAX_NAME_LEN - 1;
    for (unsigned int i = 0; i < nlen; i++) slot->name[i] = name[i];
    slot->name[nlen] = '\0';

    slot->runtime = m3_NewRuntime(g_shared_env, 65536, NULL);
    if (!slot->runtime) {
        slot->state = MOD_FREE;
        fprintf(stderr, "[runtime] m3_NewRuntime failed for module '%s' (env=%p, stack=65536)\n", name, (void*)g_shared_env);
        LOG_ERROR("wasm_init", "m3_NewRuntime failed");
        return "m3_NewRuntime failed";
    }

    M3Result result = m3_ParseModule(g_shared_env, &slot->module, wasm_bytes, wasm_size);
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_parse", result);
        return result;
    }

    result = m3_LoadModule(slot->runtime, slot->module);
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_load", result);
        return result;
    }

    /* Register host functions */
    register_host_functions_for_module(slot->module);

    /* Compile */
    result = m3_CompileModule(slot->module);
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_compile", result);
        return result;
    }

    /* Find entry point */
    result = m3_FindFunction(&slot->entry, slot->runtime, "_start");
    if (result) {
        slot->state = MOD_FREE;
        LOG_ERROR("wasm_entry", result);
        return result;
    }

    slot->state = MOD_READY;
    module_count++;
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Pipe management (simple in-process pipes using file descriptors)           */
/* -------------------------------------------------------------------------- */

#define MAX_PIPES 32

typedef struct {
    int active;
    int pipe_fds[2];  /* [0]=read, [1]=write */
} pipe_entry_t;

static pipe_entry_t g_pipes[MAX_PIPES];
static int g_next_pipe_fd = 100;  /* start pipe fd numbering at 100 */

static int pipe_create_internal(int *read_fd, int *write_fd)
{
    int fds[2];
    if (pipe(fds) < 0) return -1;
    *read_fd = fds[0];
    *write_fd = fds[1];
    return 0;
}

static int alloc_pipe_fd(int real_fd)
{
    int virtual_fd = g_next_pipe_fd++;
    for (int i = 0; i < MAX_PIPES; i++) {
        if (!g_pipes[i].active) {
            g_pipes[i].active = 1;
            g_pipes[i].pipe_fds[0] = real_fd >= 0 ? real_fd : -1;
            g_pipes[i].pipe_fds[1] = -1;
            return virtual_fd;
        }
    }
    return -1;
}

/* For our Linux runtime, pipes are real file descriptors, but we track them */
/* The virtual fd is the same as the real fd for simplicity */

/* -------------------------------------------------------------------------- */
/* VFS stubs (Linux: use real filesystem, these are placeholders)             */
/* -------------------------------------------------------------------------- */

void vfs_init(void) {}
int  vfs_alloc_fd(int pid, int type, int flags) { (void)pid; (void)type; (void)flags; return -1; }
int  vfs_free_fd(int pid, int fd) { (void)pid; (void)fd; return 0; }
int  vfs_set_ops(int pid, int fd, void *ops) { (void)pid; (void)fd; (void)ops; return 0; }
void *vfs_get_ops(int pid, int fd) { (void)pid; (void)fd; return NULL; }
int  vfs_get_type(int pid, int fd) { (void)pid; (void)fd; return 0; }
int  vfs_read(int pid, int fd, void *buf, int len) { (void)pid; (void)fd; (void)buf; (void)len; return -1; }
int  vfs_write(int pid, int fd, const void *buf, int len) { (void)pid; (void)fd; (void)buf; (void)len; return -1; }
int  vfs_close(int pid, int fd) { (void)pid; (void)fd; return 0; }
int  vfs_poll(int pid, int fd) { (void)pid; (void)fd; return 0; }
void vfs_free_all(int pid) { (void)pid; }

int pipe_create(int *r, int *w) { return pipe_create_internal(r, w); }
int pipe_read(int fd, uint8_t *buf, uint32_t len) {
    ssize_t n = read(fd, buf, len);
    return (int)n;
}
int pipe_write(int fd, const uint8_t *buf, uint32_t len) {
    ssize_t n = write(fd, buf, len);
    return (int)n;
}
int pipe_close(int fd) { return close(fd); }
int pipe_is_ready(int fd) {
    fd_set fds;
    struct timeval tv = {0, 0};
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    return select(fd + 1, &fds, NULL, NULL, &tv) > 0;
}

/* Forward declarations for host function registry */
typedef struct {
    const char *module;
    const char *field;
    const char *signature;
    const void *function;
} host_reg_t;

static const void *host_print(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_exit(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_alloc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_free(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_getc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_yield(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_sleep(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_log(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_get_tick(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_set_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_get_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_proc_list_next(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_proc_get_status(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_spawn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_open(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_fs_list(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_pipe_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_pipe_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_pipe_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);
static const void *host_spawn_redirect(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem);

/* Host function registry */
static const host_reg_t host_registry[] = {
    /* Core */
    { "host", "print",    "v(ii)",  &host_print    },
    { "host", "exit",     "v(i)",   &host_exit     },
    { "host", "alloc",    "i(ii)",  &host_alloc    },
    { "host", "free",     "v(i)",   &host_free     },
    { "host", "getc",     "i()",    &host_getc     },
    { "host", "yield",    "v()",    &host_yield    },
    { "host", "sleep",    "v(i)",   &host_sleep    },
    { "host", "log",      "v(iiii)",&host_log      },
    { "host", "get_tick", "I()",    &host_get_tick },
    /* Process */
    { "host", "set_argv", "v(ii)",  &host_set_argv },
    { "host", "get_argv", "i(ii)",  &host_get_argv },
    { "host", "spawn",    "i(ii)",  &host_spawn    },
    { "host", "proc_list_next",  "i()",  &host_proc_list_next  },
    { "host", "proc_get_status", "i(i)", &host_proc_get_status },
    /* Filesystem */
    { "host", "fs_open",    "i(ii)", &host_fs_open    },
    { "host", "fs_read",    "i(iii)", &host_fs_read   },
    { "host", "fs_write",   "i(iii)", &host_fs_write  },
    { "host", "fs_close",   "v(i)",  &host_fs_close   },
    { "host", "fs_create",  "i(ii)", &host_fs_create  },
    { "host", "fs_list",    "i(ii)", &host_fs_list    },
    /* Pipes */
    { "host", "pipe_create",   "i(ii)", &host_pipe_create },
    { "host", "pipe_read",     "i(iii)", &host_pipe_read  },
    { "host", "pipe_close",    "v(i)",  &host_pipe_close  },
    { "host", "spawn_redirect", "i(iiii)", &host_spawn_redirect },
};

#define HOST_REG_COUNT (sizeof(host_registry) / sizeof(host_registry[0]))

/* Register host functions for a specific module */
static void register_host_functions_for_module(IM3Module module)
{
    for (unsigned int i = 0; i < HOST_REG_COUNT; i++) {
        const host_reg_t *reg = &host_registry[i];
        if (!reg->function) continue;

        M3Result result = m3_LinkRawFunctionEx(module, reg->module, reg->field,
                                               reg->signature, reg->function, NULL);
        if (result && result != m3Err_functionLookupFailed) {
            fprintf(stderr, "[runtime] link warning: %s.%s: %s\n", reg->module, reg->field, result);
        }
    }
}

/* -------------------------------------------------------------------------- */
/* HOST FUNCTION IMPLEMENTATIONS                                              */
/* -------------------------------------------------------------------------- */

/* host_print(offset, len) — write to stdout */
static const void *host_print(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t offset = (uint32_t)*_sp;
    uint32_t len    = (uint32_t)*(_sp + 1);
    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (offset + len > mem_size) {
        fprintf(stderr, "[runtime] print: out of bounds\n");
        return m3Err_trapOutOfBoundsMemoryAccess;
    }

    /* Check stdout pipe redirection */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        wasm_module_slot_t *slot = &module_table[idx];
        if (slot->stdout_pipe_fd >= 0 && slot->runtime == runtime) {
            uint8_t *p = mem + offset;
            ssize_t n = write(slot->stdout_pipe_fd, p, len);
            if (n < 0) {
                /* Pipe broken — treat as EOF for writer */
                slot->stdout_pipe_fd = -1;
            }
            return m3Err_none;
        }
    }

    /* Default: write to stdout */
    fwrite(mem + offset, 1, len, stdout);
    fflush(stdout);
    return m3Err_none;
}

/* host_get_tick() — return monotonic time in microseconds */
static const void *host_get_tick(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint64_t *ret = (uint64_t *)_sp++;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    *ret = (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
    return m3Err_none;
}

/* host_exit(code) — mark module exited */
static const void *host_exit(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    int32_t code = (int32_t)(int64_t)*_sp++;

    if (current_module_id > 0 && current_module_id < (uint32_t)(MAX_MODULES + 1)) {
        uint32_t idx = current_module_id - 1;
        if (idx < MAX_MODULES && module_table[idx].state != MOD_FREE) {
            vfs_free_all((int)module_table[idx].id);

            if (module_table[idx].stdin_pipe_fd >= 0) {
                pipe_close(module_table[idx].stdin_pipe_fd);
                module_table[idx].stdin_pipe_fd = -1;
            }
            if (module_table[idx].stdout_pipe_fd >= 0) {
                pipe_close(module_table[idx].stdout_pipe_fd);
                module_table[idx].stdout_pipe_fd = -1;
            }

            module_table[idx].exit_code = (int)code;
            module_table[idx].state = MOD_EXITED;

            fprintf(stderr, "\n[WASM exit %s]\n", code == 0 ? "ok" : "code");
            if (code != 0) fprintf(stderr, "  exit code: %d\n", code);
        }
    } else {
        g_running = 0;
    }

    return m3Err_none;
}

/* host_alloc(size, align) — allocate from WASM linear memory */
static const void *host_alloc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx;
    uint32_t size  = (uint32_t)*(_sp + 1);
    uint32_t align = (uint32_t)*(_sp + 2);
    (void)align;

    uint32_t mem_size = m3_GetMemorySize(runtime);
    uint32_t *heap_pos_ptr = (uint32_t *)((uint8_t *)_mem + mem_size - 4);
    uint32_t heap_pos = *heap_pos_ptr;
    if (heap_pos == 0) heap_pos = mem_size;

    if (heap_pos < size) {
        uint32_t *ret = (uint32_t *)(_sp);
        *ret = 0;
        return m3Err_none;
    }

    heap_pos -= size;
    heap_pos &= ~(uint32_t)15;
    *heap_pos_ptr = heap_pos;

    uint32_t *ret = (uint32_t *)(_sp);
    *ret = heap_pos;
    return m3Err_none;
}

/* host_free(ptr) — no-op */
static const void *host_free(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    return m3Err_none;
}

/* host_sleep(ms) — usleep */
static const void *host_sleep(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t ms = (uint32_t)(uint64_t)*_sp++;
    usleep(ms * 1000);
    return m3Err_none;
}

/* host_yield() — cooperative scheduler: switch to next READY module */
static int yield_and_switch = 0;
static int yield_switch_from_id = 0;

static const void *host_yield(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;

    /* Signal scheduler to switch to another module */
    if (current_module_id > 0) {
        yield_switch_from_id = current_module_id;
        yield_and_switch = 1;
    }
    return m3Err_none;
}

/* host_log(level, msg) — log to stderr */
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

    char level_buf[32], msg_buf[256];
    unsigned int ll = level_len < 31 ? level_len : 31;
    unsigned int ml = msg_len < 255 ? msg_len : 255;
    memcpy(level_buf, mem + level_off, ll);
    level_buf[ll] = '\0';
    memcpy(msg_buf, mem + msg_off, ml);
    msg_buf[ml] = '\0';

    fprintf(stderr, "[%s] %s\n", level_buf, msg_buf);
    return m3Err_none;
}

/* host_getc() — non-blocking read from stdin */
static const void *host_getc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    int32_t *ret = (int32_t *)(_sp);

    /* Check stdin pipe redirection */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        wasm_module_slot_t *slot = &module_table[idx];
        if (slot->stdin_pipe_fd >= 0 && slot->runtime == runtime) {
            uint8_t ch;
            int n = read(slot->stdin_pipe_fd, &ch, 1);
            if (n > 0) {
                *ret = (int32_t)ch;
            } else {
                if (pipe_is_ready(slot->stdin_pipe_fd)) {
                    *ret = 0;
                } else {
                    *ret = -1;
                }
            }
            return m3Err_none;
        }
    }

    /* Default: read from stdin */
    fd_set fds;
    struct timeval tv = {0, 0};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0) {
        *ret = 0;  /* no data */
        return m3Err_none;
    }

    uint8_t ch;
    int n = read(STDIN_FILENO, &ch, 1);
    if (n > 0) {
        *ret = (int32_t)ch;
    } else {
        *ret = -1;  /* EOF */
    }
    return m3Err_none;
}

/* host_set_argv(buf_off, buf_len) — set argv for next spawn */
static const void *host_set_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*_sp++;
    uint32_t buf_len = (uint32_t)*_sp++;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + buf_len > mem_size) {
        pending_argc = 0;
        return m3Err_none;
    }

    unsigned int n = buf_len < 127 ? buf_len : 127;
    memcpy(pending_argv, mem + buf_off, n);
    pending_argv[n] = '\0';
    pending_argc = n;
    return m3Err_none;
}

/* host_get_argv(buf_off, max_len) — get argv for current module */
static const void *host_get_argv(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*_sp++;
    uint32_t max_len = (uint32_t)*_sp++;
    int32_t *ret = (int32_t *)(_sp);

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (current_module_id > 0 && current_module_id <= MAX_MODULES) {
        uint32_t idx = current_module_id - 1;
        if (idx < MAX_MODULES && module_table[idx].state != MOD_FREE) {
            uint32_t argv_len = module_argc[idx];
            if (argv_len > 0 && argv_len < max_len && buf_off + argv_len < mem_size) {
                memcpy(mem + buf_off, module_argv[idx], argv_len);
                *ret = (int32_t)argv_len;
                return m3Err_none;
            }
        }
    }

    *ret = 0;
    return m3Err_none;
}

/* proc_list iterator */
static uint32_t proc_list_iter = 0;

/* host_proc_list_next() — iterate through modules, return module ID or -1 */
static const void *host_proc_list_next(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t *ret = (int32_t *)_sp;

    for (; proc_list_iter < MAX_MODULES; proc_list_iter++) {
        if (module_table[proc_list_iter].state != MOD_FREE) {
            uint32_t pid = module_table[proc_list_iter].id;
            proc_list_iter++;
            *ret = (int32_t)pid;
            return m3Err_none;
        }
    }
    proc_list_iter = 0;
    *ret = -1;
    return m3Err_none;
}

/* host_proc_get_status(pid) — return module state code */
static const void *host_proc_get_status(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t pid = (int32_t)(int64_t)*(_sp + 1);
    int32_t *ret = (int32_t *)_sp;

    for (uint32_t i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == (uint32_t)pid) {
            *ret = (int32_t)module_table[i].state;
            return m3Err_none;
        }
    }
    *ret = -1;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Filesystem host functions — map to Linux real filesystem                   */
/* -------------------------------------------------------------------------- */

/* Per-module file descriptor table */
#define MOD_MAX_FDS 32
static int module_fds[MAX_MODULES][MOD_MAX_FDS];  /* -1 = unused */
static int fs_list_iter = 0;

static int alloc_fd(uint32_t module_id, int real_fd)
{
    if (module_id == 0 || module_id > MAX_MODULES) return -1;
    uint32_t idx = module_id - 1;
    for (int i = 3; i < MOD_MAX_FDS; i++) {  /* skip 0,1,2 (stdin,stdout,stderr) */
        if (module_fds[idx][i] == -1) {
            module_fds[idx][i] = real_fd;
            return i;
        }
    }
    close(real_fd);
    return -1;
}

static int get_real_fd(uint32_t module_id, int wasm_fd)
{
    if (module_id == 0 || module_id > MAX_MODULES) return -1;
    if (wasm_fd < 0 || wasm_fd >= MOD_MAX_FDS) return -1;
    return module_fds[module_id - 1][wasm_fd];
}

static void free_fd_entry(uint32_t module_id, int wasm_fd)
{
    if (module_id == 0 || module_id > MAX_MODULES) return;
    if (wasm_fd < 0 || wasm_fd >= MOD_MAX_FDS) return;
    int real_fd = module_fds[module_id - 1][wasm_fd];
    if (real_fd >= 0) {
        close(real_fd);
        module_fds[module_id - 1][wasm_fd] = -1;
    }
}

/* Initialize module fd table */
static void init_module_fds(uint32_t module_id)
{
    if (module_id == 0 || module_id > MAX_MODULES) return;
    for (int i = 0; i < MOD_MAX_FDS; i++)
        module_fds[module_id - 1][i] = -1;
    /* Map stdin/stdout/stderr */
    module_fds[module_id - 1][0] = STDIN_FILENO;
    module_fds[module_id - 1][1] = STDOUT_FILENO;
    module_fds[module_id - 1][2] = STDERR_FILENO;
}

/* host_fs_open(path_off, path_len) — open file on real filesystem */
static const void *host_fs_open(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t path_off = (uint32_t)*(_sp + 1);
    uint32_t path_len = (uint32_t)*(_sp + 2);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (path_off + path_len > mem_size) { *ret = -1; return m3Err_none; }

    char path_buf[256];
    unsigned int pl = path_len < 255 ? path_len : 255;
    memcpy(path_buf, mem + path_off, pl);
    path_buf[pl] = '\0';

    int fd = open(path_buf, O_RDONLY);
    if (fd < 0) { *ret = -1; return m3Err_none; }

    *ret = (int32_t)alloc_fd(current_module_id, fd);
    return m3Err_none;
}

/* host_fs_read(fd, buf_off, len) — read from file */
static const void *host_fs_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int32_t fd = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len = (uint32_t)*(_sp + 3);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) { *ret = -1; return m3Err_none; }

    int real_fd = get_real_fd(current_module_id, fd);
    if (real_fd < 0) { *ret = -1; return m3Err_none; }

    ssize_t n = read(real_fd, mem + buf_off, len);
    *ret = (int32_t)n;
    return m3Err_none;
}

/* host_fs_write(fd, buf_off, len) — write to file */
static const void *host_fs_write(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int32_t fd = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len = (uint32_t)*(_sp + 3);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (buf_off + len > mem_size) { *ret = -1; return m3Err_none; }

    int real_fd = get_real_fd(current_module_id, fd);
    if (real_fd < 0) { *ret = -1; return m3Err_none; }

    ssize_t n = write(real_fd, mem + buf_off, len);
    *ret = (int32_t)n;
    return m3Err_none;
}

/* host_fs_close(fd) — close file */
static const void *host_fs_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd = (int32_t)(int64_t)*_sp++;
    free_fd_entry(current_module_id, fd);
    return m3Err_none;
}

/* host_fs_create(path_off, path_len) — create and open file for writing */
static const void *host_fs_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t path_off = (uint32_t)*(_sp + 1);
    uint32_t path_len = (uint32_t)*(_sp + 2);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (path_off + path_len > mem_size) { *ret = -1; return m3Err_none; }

    char path_buf[256];
    unsigned int pl = path_len < 255 ? path_len : 255;
    memcpy(path_buf, mem + path_off, pl);
    path_buf[pl] = '\0';

    int fd = open(path_buf, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { *ret = -1; return m3Err_none; }

    *ret = (int32_t)alloc_fd(current_module_id, fd);
    return m3Err_none;
}

/* host_fs_list(buf_off, max_len) — list files in current directory */
static const void *host_fs_list(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)_ctx; (void)_mem;
    uint32_t buf_off = (uint32_t)*(_sp + 1);
    uint32_t max_len = (uint32_t)*(_sp + 2);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    /* Simple directory listing — just list . in current dir */
    DIR *dp = opendir(".");
    if (!dp) { *ret = -1; return m3Err_none; }

    int total = 0;
    struct dirent *ep;
    while ((ep = readdir(dp)) != NULL) {
        int name_len = (int)strlen(ep->d_name);
        if ((int)(total + name_len + 1) >= (int)max_len) break;
        if (buf_off + total + name_len + 1 > mem_size) break;
        memcpy(mem + buf_off + total, ep->d_name, name_len);
        mem[buf_off + total + name_len] = '\n';
        total += name_len + 1;
    }
    closedir(dp);
    *ret = total;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Pipe host functions                                                        */
/* -------------------------------------------------------------------------- */

static const void *host_pipe_create(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t rfd_off = (uint32_t)*(_sp + 1);
    uint32_t wfd_off = (uint32_t)*(_sp + 2);
    int32_t *ret = (int32_t *)_sp;

    int32_t *ret_rfd = (int32_t *)((uint8_t *)_mem + rfd_off);
    int32_t *ret_wfd = (int32_t *)((uint8_t *)_mem + wfd_off);

    int fds[2];
    if (pipe(fds) < 0) {
        *ret_rfd = -1; *ret_wfd = -1;
        *ret = -1;
        return m3Err_none;
    }

    *ret_rfd = (int32_t)alloc_fd(current_module_id, fds[0]);
    *ret_wfd = (int32_t)alloc_fd(current_module_id, fds[1]);
    *ret = 0;  /* success */
    return m3Err_none;
}

static const void *host_pipe_read(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    int32_t fd = (int32_t)(int64_t)*(_sp + 1);
    uint32_t buf_off = (uint32_t)*(_sp + 2);
    uint32_t len = (uint32_t)*(_sp + 3);
    int32_t *ret = (int32_t *)_sp;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);
    if (buf_off + len > mem_size) { *ret = -1; return m3Err_none; }

    int real_fd = get_real_fd(current_module_id, fd);
    if (real_fd < 0) { *ret = -1; return m3Err_none; }

    /* Non-blocking read */
    fd_set fds; struct timeval tv = {0, 0};
    FD_ZERO(&fds); FD_SET(real_fd, &fds);
    if (select(real_fd + 1, &fds, NULL, NULL, &tv) <= 0) { *ret = 0; return m3Err_none; }

    ssize_t n = read(real_fd, mem + buf_off, len);
    *ret = (int32_t)n;
    return m3Err_none;
}

static const void *host_pipe_close(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t fd = (int32_t)(int64_t)*_sp++;
    /* Just mark as closed, don't actually close (might be shared) */
    if (current_module_id > 0 && current_module_id <= MAX_MODULES && fd >= 0 && fd < MOD_MAX_FDS)
        module_fds[current_module_id - 1][fd] = -1;
    return m3Err_none;
}

/* host_spawn_redirect — spawn with pipe redirection */
static const void *host_spawn_redirect(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t name_off   = (uint32_t)*(_sp + 1);
    uint32_t name_len   = (uint32_t)*(_sp + 2);
    int32_t stdin_fd    = (int32_t)(int64_t)*(_sp + 3);
    int32_t stdout_fd   = (int32_t)(int64_t)*(_sp + 4);
    int32_t *ret_val    = (int32_t *)_sp;

    /* For simplicity: just call regular spawn and set pipe fds on the slot */
    /* First copy name to temp and find entry */
    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);
    if (name_off + name_len > mem_size) { *ret_val = -1; return m3Err_none; }

    char name_buf[64];
    if (name_len >= 64) name_len = 63;
    memcpy(name_buf, mem + name_off, name_len);
    name_buf[name_len] = '\0';

    fprintf(stderr, "[spawn] %s (redirect)... ", name_buf);

    wasm_registry_entry_t *entry = find_registry_entry(name_buf);
    if (!entry) { fprintf(stderr, "unknown\n"); *ret_val = -1; return m3Err_none; }

    int slot = find_module_slot();
    if (slot < 0) { fprintf(stderr, "no slot\n"); *ret_val = -2; return m3Err_none; }

    const char *err = load_module_internal(slot, entry->name, entry->wasm_bytes, entry->wasm_size);
    if (err) { fprintf(stderr, "error: %s\n", err); *ret_val = -3; return m3Err_none; }

    /* Set pipe fds for redirection */
    module_table[slot].stdin_pipe_fd = stdin_fd;
    module_table[slot].stdout_pipe_fd = stdout_fd;

    /* Save argv */
    if (pending_argc > 0) {
        uint32_t al = pending_argc < 127 ? pending_argc : 127;
        memcpy(module_argv[slot], pending_argv, al);
        module_argv[slot][al] = '\0';
        module_argc[slot] = al;
        pending_argc = 0;
    } else {
        uint32_t al = name_len < 127 ? name_len : 127;
        memcpy(module_argv[slot], mem + name_off, al);
        module_argv[slot][al] = '\0';
        module_argc[slot] = al;
    }

    /* Init fd table for new module */
    init_module_fds(module_table[slot].id);

    fprintf(stderr, "ok (id=%u)\n", module_table[slot].id);
    *ret_val = (int32_t)module_table[slot].id;
    return m3Err_none;
}

/* host_spawn(name_off, name_len) — load and start a module */
static const void *host_spawn(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t name_off = (uint32_t)*(_sp + 1);
    uint32_t name_len = (uint32_t)*(_sp + 2);
    int32_t *ret_val = (int32_t *)(_sp);
    _sp += 3;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (name_off + name_len > mem_size) {
        *ret_val = -1;
        return m3Err_none;
    }

    char name_buf[64];
    if (name_len >= 64) name_len = 63;
    memcpy(name_buf, mem + name_off, name_len);
    name_buf[name_len] = '\0';

    fprintf(stderr, "[spawn] %s... ", name_buf);

    wasm_registry_entry_t *entry = find_registry_entry(name_buf);
    if (!entry) {
        fprintf(stderr, "unknown\n");
        *ret_val = -1;
        return m3Err_none;
    }

    int slot = find_module_slot();
    if (slot < 0) {
        fprintf(stderr, "no slot\n");
        *ret_val = -2;
        return m3Err_none;
    }

    const char *err = load_module_internal(slot, entry->name, entry->wasm_bytes, entry->wasm_size);
    if (err) {
        fprintf(stderr, "error: %s\n", err);
        *ret_val = -3;
        return m3Err_none;
    }

    /* Save argv for the spawned module */
    if (pending_argc > 0) {
        uint32_t argv_len = pending_argc < 127 ? pending_argc : 127;
        memcpy(module_argv[slot], pending_argv, argv_len);
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
        pending_argc = 0;
    } else {
        uint32_t argv_len = name_len < 127 ? name_len : 127;
        memcpy(module_argv[slot], mem + name_off, argv_len);
        module_argv[slot][argv_len] = '\0';
        module_argc[slot] = argv_len;
    }

    /* Init fd table for new module */
    init_module_fds(module_table[slot].id);

    /* Cooperative scheduler: immediately run the spawned module */
    fprintf(stderr, "running... ");
    uint32_t saved_current = current_module_id;
    const char *run_err = run_module(module_table[slot].id);
    current_module_id = saved_current;
    if (run_err) fprintf(stderr, "run error: %s\n", run_err);

    fprintf(stderr, "ok (id=%u)\n", module_table[slot].id);
    *ret_val = (int32_t)module_table[slot].id;
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Run module                                                                 */
/* -------------------------------------------------------------------------- */

const char *run_module(uint32_t module_id)
{
    int slot_idx = -1;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].id == module_id && module_table[i].state == MOD_READY) {
            slot_idx = i;
            break;
        }
    }

    if (slot_idx < 0) return "module not found or not ready";

    wasm_module_slot_t *slot = &module_table[slot_idx];
    current_module_id = module_id;
    slot->state = MOD_RUNNING;

    M3Result result = m3_CallV(slot->entry, 0);
    slot->state = result ? MOD_EXITED : slot->state;

    current_module_id = 0;
    return result;
}

/* -------------------------------------------------------------------------- */
/* Process management stubs                                                   */
/* -------------------------------------------------------------------------- */

int find_ready_module(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (module_table[i].state == MOD_READY)
            return (int)module_table[i].id;
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
/* SIGINT handler                                                             */
/* -------------------------------------------------------------------------- */

static void sigint_handler(int sig)
{
    (void)sig;
    fprintf(stderr, "\n[runtime] SIGINT received, shutting down...\n");
    g_running = 0;
}

/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    printf("=== AI-ASM AArch64 Linux Runtime ===\n");
    printf("Wasm3 v%s | AArch64 | Linux proot mode\n\n", M3_VERSION);

    /* Parse arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--modules-dir") == 0 && i + 1 < argc) {
            strncpy(g_modules_dir, argv[++i], MODULES_DIR_MAX - 1);
        } else if (strcmp(argv[i], "--boot") == 0 && i + 1 < argc) {
            strncpy(g_boot_module, argv[++i], sizeof(g_boot_module) - 1);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: ./linux_runtime [options]\n");
            printf("  --modules-dir DIR   Directory containing WASM modules (default: modules)\n");
            printf("  --boot MODULE       Boot module name (default: init)\n");
            printf("  --help              Show this help\n");
            return 0;
        }
    }

    printf("Modules dir: %s\n", g_modules_dir);
    printf("Boot module: %s\n\n", g_boot_module);

    /* Signal handler */
    signal(SIGINT, sigint_handler);

    /* Initialize Wasm3 environment */
    g_shared_env = m3_NewEnvironment();
    if (!g_shared_env) {
        fprintf(stderr, "[runtime] ERROR: m3_NewEnvironment failed\n");
        return 1;
    }

    jit_cache_init();

    /* Scan modules directory */
    if (scan_modules_dir(g_modules_dir) <= 0) {
        fprintf(stderr, "[runtime] ERROR: no modules found in %s\n", g_modules_dir);
        m3_FreeEnvironment(g_shared_env);
        return 1;
    }

    /* Initialize VFS */
    vfs_init();

    /* Load boot module */
    wasm_registry_entry_t *boot_entry = find_registry_entry(g_boot_module);
    if (!boot_entry) {
        fprintf(stderr, "[runtime] ERROR: boot module '%s' not found\n", g_boot_module);
        m3_FreeEnvironment(g_shared_env);
        return 1;
    }

    int slot = find_module_slot();
    if (slot < 0) {
        fprintf(stderr, "[runtime] ERROR: no free module slot\n");
        m3_FreeEnvironment(g_shared_env);
        return 1;
    }

    const char *err = load_module_internal(slot, boot_entry->name,
                                            boot_entry->wasm_bytes, boot_entry->wasm_size);
    if (err) {
        fprintf(stderr, "[runtime] ERROR: failed to load boot module: %s\n", err);
        m3_FreeEnvironment(g_shared_env);
        return 1;
    }

    /* Register host functions for boot module */
    register_host_functions_for_module(module_table[slot].module);

    /* Compile boot module */
    err = (const char *)m3_CompileModule(module_table[slot].module);
    if (err) {
        fprintf(stderr, "[runtime] ERROR: failed to compile boot module: %s\n", err);
        m3_FreeEnvironment(g_shared_env);
        return 1;
    }

    module_table[slot].state = MOD_READY;
    module_count++;

    printf("\n=== System Boot ===\n");
    printf("Loaded boot module: %s (id=%u)\n\n", module_table[slot].name, module_table[slot].id);

    /* Run modules in a loop — execute READY modules one at a time */
    while (g_running) {
        int ready_id = find_ready_module();
        if (ready_id < 0) {
            /* No ready modules — check if all exited */
            int any_running = 0;
            for (int i = 0; i < MAX_MODULES; i++) {
                if (module_table[i].state == MOD_RUNNING ||
                    module_table[i].state == MOD_READY) {
                    any_running = 1;
                    break;
                }
            }
            if (!any_running) {
                printf("\n=== All modules exited ===\n");
                break;
            }
            sched_yield();
            continue;
        }

        const char *result = run_module((uint32_t)ready_id);
        if (result) {
            fprintf(stderr, "[runtime] ERROR running module %d: %s\n", ready_id, result);
        }

        /* Check if module exited */
        for (int i = 0; i < MAX_MODULES; i++) {
            if (module_table[i].id == (uint32_t)ready_id &&
                module_table[i].state == MOD_EXITED) {
                printf("[runtime] module %s exited with code %d\n",
                       module_table[i].name, module_table[i].exit_code);
            }
        }
    }

    /* Cleanup */
    printf("\n=== Shutdown ===\n");
    for (int i = 0; i < MAX_MODULES; i++) {
        if (g_registry[i].wasm_bytes) {
            free(g_registry[i].wasm_bytes);
        }
    }

    m3_FreeEnvironment(g_shared_env);
    printf("Runtime terminated.\n");
    return 0;
}
