/*
 * aiasm-aarch64/modules/shell/src/main.c
 * Wasm Shell module for AI-ASM AArch64 kernel v0.4
 *
 * New v0.4: uses host_alloc/host_free with alignment, host_log,
 * and demonstrates simple interactive input loop.
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

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("free")))
extern void wasm_host_free(void *ptr);

__attribute__((import_module("host"), import_name("log")))
extern void wasm_host_log(unsigned int level_off, unsigned int level_len,
                          unsigned int msg_off, unsigned int msg_len);

/* -------------------------------------------------------------------------- */
/* WASM memory allocator (bump, uses host_alloc)                              */
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
/* String helpers                                                             */
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
/* Host log wrapper: send level + message to kernel JSON log                  */
/* -------------------------------------------------------------------------- */

static void shell_log(const char *level, const char *msg)
{
    unsigned int ll = my_strlen(level);
    unsigned int ml = my_strlen(msg);
    unsigned int loff = alloc(ll);
    unsigned int moff = alloc(ml);
    char *ld = (char *)(loff);
    char *md = (char *)(moff);
    for (unsigned int i = 0; i < ll; i++) ld[i] = level[i];
    for (unsigned int i = 0; i < ml; i++) md[i] = msg[i];
    wasm_host_log(loff, ll, moff, ml);
}

/* -------------------------------------------------------------------------- */
/* Host alloc/free wrapper (use kernel aligned allocator)                     */
/* -------------------------------------------------------------------------- */

static void *shell_malloc(unsigned int n)
{
    unsigned int align = 16;
    void *p = wasm_host_alloc(n, align);
    return p;
}

static void shell_free(void *p)
{
    if (p)
        wasm_host_free(p);
}

/* -------------------------------------------------------------------------- */
/* Shell entry point                                                          */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void shell_entry(void)
{
    print_str("\n== AI-ASM AArch64 v0.4 Wasm Shell ==\n\n");

    print_str("tick: ");
    unsigned long long tick = wasm_host_get_tick();
    print_u64(tick);
    print_str("\n\n");

    /* Demonstrate host_log */
    shell_log("INFO", "shell started");

    /* Demonstrate host_alloc/host_free */
    char *buf = (char *)shell_malloc(32);
    if (buf) {
        for (int i = 0; i < 16; i++)
            buf[i] = 'A' + (char)(i % 26);
        buf[16] = '\0';
        print_str("alloc test: ");
        print_str(buf);
        print_str("\n");
        shell_free(buf);
    }

    print_str("\nwasm> (input pending v0.5)\n");

    shell_log("INFO", "shell exiting");
    wasm_host_exit(0);
}
