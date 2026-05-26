/*
 * aiasm-aarch64/modules/uptime/src/main.c
 * uptime WASM module for AI-ASM AArch64 v57.0
 *
 * Usage: uptime
 * Displays system uptime based on host_get_tick().
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned int wasm_host_get_tick(void);

__attribute__((import_module("host"), import_name("alloc")))
extern void *wasm_host_alloc(unsigned int size, unsigned int align);

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void print_str(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    unsigned int off = alloc(len);
    for (unsigned int i = 0; i < len; i++)
        ((char *)off)[i] = s[i];
    host_print(off, len);
}

static void print_uint(unsigned int v)
{
    char buf[12];
    int i = 0;
    if (v == 0) buf[i++] = '0';
    else while (v > 0) { buf[i++] = (char)('0' + (v % 10)); v /= 10; }
    unsigned int off = alloc((unsigned int)i);
    char *dst = (char *)off;
    for (int j = 0; j < i; j++) dst[j] = buf[i - 1 - j];
    host_print(off, (unsigned int)i);
}

/* CNTVCT_EL0 runs at 62.5MHz */
#define TICK_FREQ 62500000

__attribute__((export_name("_start")))
void _start(void)
{
    unsigned int ticks = wasm_host_get_tick();
    unsigned int seconds = ticks / TICK_FREQ;
    unsigned int minutes = seconds / 60;
    unsigned int hours = minutes / 60;
    unsigned int days = hours / 24;

    print_str("up ");
    if (days > 0) {
        print_uint(days);
        print_str(" day(s), ");
    }
    hours = hours % 24;
    minutes = minutes % 60;

    print_uint(hours);
    print_str(":");
    if (minutes < 10) print_str("0");
    print_uint(minutes);
    print_str(", 1 user, load average: 0.00, 0.01, 0.05\n");

    host_exit(0);
}
