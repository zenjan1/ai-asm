/*
 * aiasm-aarch64/kernel/wasm_host.c
 * Wasm3 host integration v0.4: AArch64 native, complete host function registry,
 * error logging, external module loading, aligned alloc/free.
 *
 * All host functions are batch-registered via a static table at load time.
 * Errors are routed through kernel_log() for JSON-formatted UART output.
 */

#include "wasm3.h"
#include "m3_env.h"
#include "m3_compile.h"

/* -------------------------------------------------------------------------- */
/* UART helpers                                                               */
/* -------------------------------------------------------------------------- */

#define UART_BASE_ADDR 0x09000000UL
#define UART_DR   (*(volatile unsigned int *)(UART_BASE_ADDR + 0x000))
#define UART_FR   (*(volatile unsigned int *)(UART_BASE_ADDR + 0x018))
#define UART_FR_TXFF (1 << 5)

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
/* Extern symbols from wasm_embed.asm                                          */
/* -------------------------------------------------------------------------- */

extern const uint8_t wasm_module_start[];
extern const uint32_t wasm_module_size;

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
/* External ASM symbols                                                       */
/* -------------------------------------------------------------------------- */

extern unsigned char __image_end[];
extern void mem_free_page(void *);
extern void serial_puts(const char *);
extern void serial_putc(char);

/* -------------------------------------------------------------------------- */
/* Host function implementations                                              */
/* -------------------------------------------------------------------------- */

/* host_print(offset, len) — print WASM linear memory to UART */
static const void *host_print(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t offset = (uint32_t)*_sp++;
    uint32_t len    = (uint32_t)*_sp++;

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

/* host_exit(code) — print exit code and halt */
static const void *host_exit(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t code = (int32_t)(int64_t)*_sp++;

    uart_puts_raw("\n[WASM exit ");
    if (code == 0) {
        uart_puts_raw("ok");
    } else {
        uart_puts_raw("code=");
        if (code < 0) { uart_putc_raw('-'); code = -code; }
        char buf[12]; int i = 0;
        do { buf[i++] = (char)('0' + (code % 10)); code /= 10; } while (code > 0);
        while (i > 0) uart_putc_raw(buf[--i]);
    }
    uart_puts_raw("]\n");

    while (1)
        __asm__ volatile("wfi");
}

/* host_alloc(size, align) — allocate from kernel heap with alignment */
static const void *host_alloc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint32_t size  = (uint32_t)*_sp++;
    uint32_t align = (uint32_t)*_sp++;

    /* Forward to C wrapper of mem_alloc_aligned */
    extern void *mem_alloc_aligned(unsigned int size, unsigned int align);
    void *ptr = mem_alloc_aligned(size, align ? align : 16);

    if (!ptr) {
        LOG_ERROR("wasm_alloc", "kernel heap OOM");
        uint32_t *ret = (uint32_t *)_sp++;
        *ret = 0;
        return m3Err_none;
    }

    uint32_t *ret = (uint32_t *)_sp++;
    *ret = (uint32_t)(uintptr_t)ptr;
    return m3Err_none;
}

/* host_free(ptr) — free heap allocation */
static const void *host_free(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint64_t addr = *_sp++;

    extern void mem_free(void *);
    mem_free((void *)(uintptr_t)addr);
    return m3Err_none;
}

/* host_alloc_page() — allocate one 4KB page from kernel page allocator */
static const void *host_alloc_page(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    extern void *kernel_alloc_page(void);
    void *ptr = kernel_alloc_page();
    uint32_t *ret = (uint32_t *)_sp++;
    *ret = (uint32_t)(uintptr_t)ptr;
    return m3Err_none;
}

/* host_free_page(addr) — free a 4KB page */
static const void *host_free_page(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    uint64_t addr = *_sp++;
    extern void mem_free_page(void *);
    mem_free_page((void *)(uintptr_t)addr);
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

    /* Save null terminators, write them, log, restore */
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

/* host_getc() — blocking read one character from UART, return i32 char code */
static const void *host_getc(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;

    /* Poll UART RX until character available, no wfi (no interrupts configured) */
    while (UART_FR & (1 << 4))  /* FR_RXFE: RX FIFO empty */
        __asm__ volatile("wfi");

    uint8_t ch = (uint8_t)(UART_DR & 0xff);
    int32_t *ret = (int32_t *)_sp++;
    *ret = (int32_t)ch;
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

/* All host functions registered in a single batch at module load time */
static const host_reg_t host_registry[] = {
    { "host", "print",       "v(ii)",  &host_print       },
    { "host", "get_tick",    "I()",    &host_get_tick    },
    { "host", "exit",        "v(i)",   &host_exit        },
    { "host", "alloc",       "i(ii)",  &host_alloc       },
    { "host", "free",        "v(i)",   &host_free        },
    { "host", "alloc_page",  "i()",    &host_alloc_page  },
    { "host", "free_page",   "v(i)",   &host_free_page   },
    { "host", "log",         "v(iiii)",&host_log         },
    { "host", "getc",        "i()",    &host_getc        },
};

#define HOST_REG_COUNT (sizeof(host_registry) / sizeof(host_registry[0]))

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static IM3Environment g_env = NULL;
static IM3Runtime     g_runtime = NULL;
static IM3Module      g_module = NULL;

/* External WASM module loading: fixed buffer for external .wasm */
#define EXT_WASM_BUF_SIZE 0x00100000    /* 1MB external module buffer */
static uint8_t g_ext_wasm_buf[EXT_WASM_BUF_SIZE];
static uint32_t g_ext_wasm_size = 0;

/* -------------------------------------------------------------------------- */
/* wasm_host_init: set up wasm3 environment and runtime                        */
/* -------------------------------------------------------------------------- */

const char *wasm_host_init(void)
{
    g_env = m3_NewEnvironment();
    if (!g_env) {
        LOG_ERROR("wasm_init", "m3_NewEnvironment failed");
        return "m3_NewEnvironment failed";
    }

    g_runtime = m3_NewRuntime(g_env, 8192, NULL);
    if (!g_runtime) {
        LOG_ERROR("wasm_init", "m3_NewRuntime failed");
        return "m3_NewRuntime failed";
    }

    LOG_INFO("wasm_init", "runtime ready");
    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* register_host_functions: batch register all host functions                  */
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
/* wasm_host_load: parse and load WASM module (embedded or external)           */
/* -------------------------------------------------------------------------- */

const char *wasm_host_load(const uint8_t *wasm_bytes, uint32_t wasm_size)
{
    M3Result result;

    /* Validate WASM magic */
    if (wasm_size < 4 || wasm_bytes[0] != 0x00 || wasm_bytes[1] != 0x61 ||
        wasm_bytes[2] != 0x73 || wasm_bytes[3] != 0x6d) {
        LOG_ERROR("wasm_load", "invalid magic");
        return "invalid WASM magic";
    }

    result = m3_ParseModule(g_env, &g_module, wasm_bytes, wasm_size);
    if (result) {
        LOG_ERROR("wasm_parse", result);
        return result;
    }

    result = m3_LoadModule(g_runtime, g_module);
    if (result) {
        LOG_ERROR("wasm_load", result);
        return result;
    }

    /* Batch register all host functions */
    result = register_host_functions(g_module);
    if (result)
        return result;

    LOG_INFO("wasm_load", "module loaded");
    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* wasm_host_load_external: load external WASM from memory buffer              */
/* Caller must copy .wasm bytes into g_ext_wasm_buf first                      */
/* -------------------------------------------------------------------------- */

const char *wasm_host_load_external(const uint8_t *buf, uint32_t size)
{
    if (size > EXT_WASM_BUF_SIZE) {
        LOG_ERROR("wasm_external", "module too large");
        return "external module too large";
    }

    /* Copy to our buffer */
    extern void *memcpy(void *, const void *, unsigned long);
    memcpy(g_ext_wasm_buf, buf, size);
    g_ext_wasm_size = size;

    return wasm_host_load(g_ext_wasm_buf, g_ext_wasm_size);
}

/* -------------------------------------------------------------------------- */
/* wasm_host_run: find and call "_start" in the module                         */
/* -------------------------------------------------------------------------- */

const char *wasm_host_run(void)
{
    IM3Function func = NULL;
    M3Result result;

    IM3Module mod = g_runtime->modules;
    if (!mod) {
        return "no modules loaded";
    }

    /* Find first non-import function (entry point like _start) */
    for (uint32_t i = 0; i < mod->numFunctions; i++) {
        IM3Function f = &mod->functions[i];
        if (f->import.moduleUtf8 || f->import.fieldUtf8)
            continue;
        func = f;
        break;
    }

    if (!func) {
        return "no entry point found";
    }

    /* Compile the function before calling */
    result = CompileFunction(func);
    if (result) {
        return result;
    }

    result = m3_Call(func, 0, NULL);
    if (result) {
        return result;
    }

    LOG_INFO("wasm_run", "completed");
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* wasm_host_get_ext_wasm_buf: return pointer to external WASM buffer          */
/* -------------------------------------------------------------------------- */

void *wasm_host_get_ext_wasm_buf(void)
{
    return (void *)g_ext_wasm_buf;
}

uint32_t wasm_host_get_ext_wasm_size(void)
{
    return g_ext_wasm_size;
}
