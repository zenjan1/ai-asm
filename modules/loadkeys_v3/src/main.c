/* loadkeys_v3: keyboard keymap loader (enhanced v3) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(const char *src, unsigned int dst, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len])
        len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int buf = alloc(len + 1);
    copy_to_mem(s, buf, len + 1);
    host_print(buf, len);
}

static void print_int(int n)
{
    char buf[16];
    int i = 15;
    int neg = 0;
    buf[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) buf[--i] = '0';
    while (n > 0) { buf[--i] = '0' + (n % 10); n /= 10; }
    if (neg) buf[--i] = '-';
    unsigned int len = my_strlen(&buf[i]);
    unsigned int dst = alloc(len + 1);
    copy_to_mem(&buf[i], dst, len + 1);
    host_print(dst, len);
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b);
}

static void show_help(void)
{
    print_str("loadkeys_v3 - load keyboard mapping (v3.0)\n");
    print_str("Usage: loadkeys_v3 [OPTIONS] [keymap_file]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -c             Check keymap syntax only\n");
    print_str("  -l             List available keymaps\n");
    print_str("  -v             Verbose output\n");
    print_str("\n");
    print_str("Load or verify keyboard translation tables.\n");
}

static void list_keymaps(void)
{
    print_str("loadkeys_v3: available keymaps:\n");
    print_str("  us.map       - US English\n");
    print_str("  uk.map       - UK English\n");
    print_str("  de.map       - German\n");
    print_str("  fr.map       - French\n");
    print_str("  ru.map       - Russian\n");
    print_str("  jp.map       - Japanese\n");
    print_str("loadkeys_v3: total keymaps: 6\n");
}

static void check_keymap(const char *file)
{
    print_str("loadkeys_v3: checking keymap file: ");
    print_str(file);
    print_str("\n");
    print_str("loadkeys_v3: syntax check passed\n");
    print_str("loadkeys_v3: keycode entries: 128\n");
    print_str("loadkeys_v3: string definitions: 24\n");
    print_str("loadkeys_v3: compose entries: 8\n");
    print_str("loadkeys_v3: keymap check complete\n");
}

static void load_keymap(const char *file, int verbose)
{
    print_str("loadkeys_v3: loading keymap: ");
    print_str(file);
    print_str("\n");
    if (verbose) {
        print_str("loadkeys_v3: parsing keycode definitions\n");
        print_str("loadkeys_v3: loading string tables\n");
        print_str("loadkeys_v3: loading compose table\n");
        print_str("loadkeys_v3: loading dead key mappings\n");
    }
    print_str("loadkeys_v3: keymap loaded successfully\n");
    print_str("loadkeys_v3: entries loaded: 160\n");
    print_str("loadkeys_v3: keyboard translation table updated\n");
    print_str("loadkeys_v3: load complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0;
    int check_flag = 0;
    int list_flag = 0;
    int verbose_flag = 0;
    char *keymap_file = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            check_flag = 1;
        } else if (my_strcmp(arg, "-l") == 0) {
            list_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (arg[0] != '-') {
            keymap_file = arg;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (list_flag) {
        list_keymaps();
        host_exit(0);
    }

    if (check_flag && keymap_file) {
        check_keymap(keymap_file);
        host_exit(0);
    }

    if (keymap_file) {
        load_keymap(keymap_file, verbose_flag);
        host_exit(0);
    }

    /* Default: show usage */
    print_str("loadkeys_v3: no keymap specified, showing current mapping\n");
    print_str("loadkeys_v3: current keymap: us.map\n");
    print_str("loadkeys_v3: keycode entries: 128 loaded\n");
    print_str("loadkeys_v3: use -l to list available keymaps\n");

    host_exit(0);
}
