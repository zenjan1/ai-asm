/*
 * aiasm-aarch64/modules/shell/src/main.c
 * Wasm Shell module for AI-ASM AArch64 kernel
 */

/* -------------------------------------------------------------------------- */
/* WASM imports — clang uses __attribute__((import_module, import_name))      */
/* -------------------------------------------------------------------------- */

__attribute__((import_module("host"), import_name("print")))
extern void wasm_host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long long wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("exit")))
extern void wasm_host_exit(int code);

/* -------------------------------------------------------------------------- */
/* WASM memory allocator (bump)                                                */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

/* -------------------------------------------------------------------------- */
/* String helpers                                                              */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (unsigned int)(p - s);
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int offset = alloc(len);
    char *dst = (char *)(offset);
    for (unsigned int i = 0; i < len; i++)
        dst[i] = s[i];
    wasm_host_print(offset, len);
}

static void print_u64(unsigned long long v)
{
    char buf[20];
    int i = 0;
    if (v == 0) {
        buf[i++] = '0';
    } else {
        while (v > 0) {
            buf[i++] = '0' + (char)(v % 10);
            v /= 10;
        }
    }
    unsigned int offset = alloc((unsigned int)i);
    char *dst = (char *)(offset);
    for (int j = 0; j < i; j++)
        dst[j] = buf[i - 1 - j];
    wasm_host_print(offset, (unsigned int)i);
}

/* -------------------------------------------------------------------------- */
/* Shell entry point                                                           */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void shell_entry(void)
{
    print_str("\n== AI-ASM AArch64 v0.3 Wasm Shell ==\n\n");

    print_str("tick: ");
    unsigned long long tick = wasm_host_get_tick();
    print_u64(tick);
    print_str("\n\n");

    print_str("wasm> (input pending v0.4)\n");

    wasm_host_exit(0);
}
