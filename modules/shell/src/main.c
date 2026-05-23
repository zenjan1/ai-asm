/*
 * aiasm-aarch64/modules/shell/src/main.c
 * Wasm Shell module for AI-ASM AArch64 kernel v0.4
 *
 * Interactive command loop: reads characters via host_getc,
 * echoes input, handles backspace, executes commands on newline.
 */

/* -------------------------------------------------------------------------- */
/* WASM imports                                                               */
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

__attribute__((import_module("host"), import_name("getc")))
extern int wasm_host_getc(void);

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

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b) ? 1 : 0;
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

static void print_char(char c)
{
    unsigned int offset = alloc(1);
    ((char *)offset)[0] = c;
    wasm_host_print(offset, 1);
}

/* -------------------------------------------------------------------------- */
/* Logging                                                                    */
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
/* Memory helpers                                                             */
/* -------------------------------------------------------------------------- */

static void *shell_malloc(unsigned int n)
{
    return wasm_host_alloc(n, 16);
}

static void shell_free(void *p)
{
    if (p) wasm_host_free(p);
}

/* -------------------------------------------------------------------------- */
/* Input: blocking read one character from UART                               */
/* -------------------------------------------------------------------------- */

static char read_char(void)
{
    return (char)wasm_host_getc();
}

/* -------------------------------------------------------------------------- */
/* Command execution                                                          */
/* -------------------------------------------------------------------------- */

static void cmd_help(void)
{
    print_str("Commands:\n");
    print_str("  help    - show this help\n");
    print_str("  tick    - print current tick counter\n");
    print_str("  echo X  - echo text back\n");
    print_str("  alloc   - test kernel allocator\n");
    print_str("  log     - test kernel logging\n");
    print_str("  clear   - clear screen\n");
    print_str("  exit    - exit shell\n");
}

static void cmd_tick(void)
{
    print_str("tick: ");
    print_u64(wasm_host_get_tick());
    print_str("\n");
}

static void cmd_echo(const char *arg)
{
    if (*arg) {
        print_str(arg);
    }
    print_str("\n");
}

static void cmd_alloc(void)
{
    char *buf = (char *)shell_malloc(64);
    if (buf) {
        for (int i = 0; i < 32; i++)
            buf[i] = 'A' + (char)(i % 26);
        buf[32] = '\0';
        print_str("alloc 64 bytes: ");
        print_str(buf);
        print_str("\n");
        shell_free(buf);
        print_str("free ok\n");
    } else {
        print_str("alloc failed!\n");
    }
}

static void cmd_log(void)
{
    shell_log("INFO", "user requested log test");
    print_str("log sent\n");
}

static void cmd_clear(void)
{
    print_str("\033[2J\033[H");
}

/* -------------------------------------------------------------------------- */
/* Shell entry point                                                          */
/* -------------------------------------------------------------------------- */

#define MAX_LINE 128

__attribute__((export_name("_start")))
void shell_entry(void)
{
    print_str("\n== AI-ASM AArch64 v0.4 Wasm Shell ==\n");
    print_str("Type 'help' for commands.\n\n");

    print_str("tick: ");
    print_u64(wasm_host_get_tick());
    print_str("\n\n");

    shell_log("INFO", "shell started");

    /* Demonstrate alloc/free at startup */
    char *buf = (char *)shell_malloc(32);
    if (buf) {
        for (int i = 0; i < 16; i++)
            buf[i] = 'A' + (char)(i % 26);
        buf[16] = '\0';
        print_str("alloc test: ");
        print_str(buf);
        print_str("\n\n");
        shell_free(buf);
    }

    /* Main interactive loop */
    char line[MAX_LINE];
    unsigned int len = 0;

    for (;;) {
        print_str("wasm> ");
        len = 0;

        for (;;) {
            char c = read_char();

            if (c == '\r' || c == '\n') {
                /* End of line — execute */
                print_str("\n");
                line[len] = '\0';

                if (len > 0) {
                    if (my_strcmp(line, "help") == 0) {
                        cmd_help();
                    } else if (my_strcmp(line, "tick") == 0) {
                        cmd_tick();
                    } else if (my_strcmp(line, "alloc") == 0) {
                        cmd_alloc();
                    } else if (my_strcmp(line, "log") == 0) {
                        cmd_log();
                    } else if (my_strcmp(line, "clear") == 0) {
                        cmd_clear();
                    } else if (my_strcmp(line, "exit") == 0) {
                        shell_log("OP", "shell exiting");
                        wasm_host_exit(0);
                    } else if (line[0] == 'e' && line[1] == 'c' &&
                               line[2] == 'h' && line[3] == 'o' &&
                               line[4] == ' ') {
                        cmd_echo(line + 5);
                    } else {
                        print_str("unknown: ");
                        print_str(line);
                        print_str("\n");
                    }
                }
                /* Reset for next command */
                len = 0;
                break;
            } else if (c == '\b' || c == 0x7f) {
                /* Backspace */
                if (len > 0) {
                    len--;
                    print_str("\x1b[D\x1b[K");  /* cursor left, erase to end */
                }
            } else if (c >= 0x20 && c < 0x7f) {
                /* Printable character */
                if (len < MAX_LINE - 1) {
                    line[len] = c;
                    len++;
                    print_char(c);
                }
            }
            /* Ignore other control characters */
        }
    }
}
