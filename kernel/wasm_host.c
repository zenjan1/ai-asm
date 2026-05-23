/*
 * aiasm-aarch64/kernel/wasm_host.c
 * Wasm3 host integration: initializes wasm3 runtime, loads embedded module,
 * links host functions, and executes the Wasm shell.
 */

#include "wasm3.h"

/* UART PL011 direct access */
#define UART_BASE 0x09000000UL
#define UART_DR   (*(volatile unsigned int *)(UART_BASE + 0x000))
#define UART_FR   (*(volatile unsigned int *)(UART_BASE + 0x018))
#define UART_FR_TXFF (1 << 5)

static void uart_putc_raw(char c)
{
    while (UART_FR & UART_FR_TXFF)
        ;
    UART_DR = (unsigned int)c;
}

static void uart_puts_raw(const char *s)
{
    while (*s)
        uart_putc_raw(*s++);
}

/* Extern symbols from assembly kernel */
extern unsigned char __image_end[];

/* Page allocator: allocate one 4KB page, return pointer */
static unsigned int alloc_page_counter = 0;
void *kernel_alloc_page(void)
{
    unsigned char *base = __image_end;
    unsigned char *ptr = base + (alloc_page_counter * 4096);
    alloc_page_counter++;
    return (void *)ptr;
}

/* -------------------------------------------------------------------------- */
/* Host function: host_print(offset, len) — print WASM memory to UART         */
/* -------------------------------------------------------------------------- */

static const void *host_print(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint32_t offset = (uint32_t)*_sp++;
    uint32_t len    = (uint32_t)*_sp++;

    uint8_t *mem = (uint8_t *)_mem;
    uint32_t mem_size = m3_GetMemorySize(runtime);

    if (offset + len > mem_size)
        return m3Err_trapOutOfBoundsMemoryAccess;

    uint8_t *p = mem + offset;
    for (uint32_t i = 0; i < len; i++)
        uart_putc_raw((char)p[i]);

    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Host function: host_get_tick() — return ARM CNTVCT_EL0                     */
/* -------------------------------------------------------------------------- */

static const void *host_get_tick(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint64_t *ret = (uint64_t *)_sp++;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(*ret));
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Host function: host_exit(code) — halt the system                          */
/* -------------------------------------------------------------------------- */

static const void *host_exit(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    (void)runtime; (void)_ctx; (void)_mem;
    int32_t code = (int32_t)(int64_t)*_sp++;
    uart_puts_raw("\n[WASM exit code: ");
    if (code < 0) { uart_putc_raw('-'); code = -code; }
    char buf[12]; int i = 0;
    do { buf[i++] = (char)('0' + (code % 10)); code /= 10; } while (code > 0);
    while (i > 0) uart_putc_raw(buf[--i]);
    uart_puts_raw("]\n");

    while (1)
        __asm__ volatile("wfi");
}

/* -------------------------------------------------------------------------- */
/* Host function: host_alloc_page() — allocate a 4KB page from kernel         */
/* -------------------------------------------------------------------------- */

static const void *host_alloc_page(IM3Runtime runtime, IM3ImportContext _ctx, uint64_t * _sp, void * _mem)
{
    uint64_t *ret = (uint64_t *)_sp++;
    *ret = (uint64_t)(uintptr_t)kernel_alloc_page();
    return m3Err_none;
}

/* -------------------------------------------------------------------------- */
/* Globals                                                                    */
/* -------------------------------------------------------------------------- */

static IM3Environment g_env = NULL;
static IM3Runtime     g_runtime = NULL;
static IM3Module      g_module = NULL;

/* -------------------------------------------------------------------------- */
/* wasm_host_init: set up environment and runtime                              */
/* -------------------------------------------------------------------------- */

const char *wasm_host_init(void)
{
    g_env = m3_NewEnvironment();
    if (!g_env)
        return "m3_NewEnvironment failed";

    g_runtime = m3_NewRuntime(g_env, 8192, NULL);
    if (!g_runtime)
        return "m3_NewRuntime failed";

    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* wasm_host_load: parse and load embedded WASM module                         */
/* -------------------------------------------------------------------------- */

const char *wasm_host_load(const uint8_t *wasm_bytes, uint32_t wasm_size)
{
    M3Result result;

    result = m3_ParseModule(g_env, &g_module, wasm_bytes, wasm_size);
    if (result)
        return result;

    result = m3_LoadModule(g_runtime, g_module);
    if (result)
        return result;

    /* Link host functions */
    result = m3_LinkRawFunction(g_module, "host", "print",    "v(ii)", &host_print);
    if (result && result != m3Err_functionLookupFailed) return result;

    result = m3_LinkRawFunction(g_module, "host", "get_tick", "I()", &host_get_tick);
    if (result && result != m3Err_functionLookupFailed) return result;

    result = m3_LinkRawFunction(g_module, "host", "exit",     "v(i)", &host_exit);
    if (result && result != m3Err_functionLookupFailed) return result;

    result = m3_LinkRawFunction(g_module, "host", "alloc_page", "i()", &host_alloc_page);
    if (result && result != m3Err_functionLookupFailed) return result;

    return NULL;  /* success */
}

/* -------------------------------------------------------------------------- */
/* wasm_host_run: find and call "_start" in the module                         */
/* -------------------------------------------------------------------------- */

const char *wasm_host_run(void)
{
    IM3Function func;
    M3Result result;

    result = m3_FindFunction(&func, g_runtime, "_start");
    if (result) {
        result = m3_FindFunction(&func, g_runtime, "main");
        if (result)
            return "no _start or main found in WASM module";
    }

    result = m3_Call(func, 0, NULL);
    if (result)
        return result;

    return NULL;
}
