/* dumpkeys_v3: keyboard mapping table dump (enhanced v3) */

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

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b);
}

static void dump_full_keymap(void)
{
    print_str("keymaps 0-127\n");
    print_str("\n");
    print_str("altcodes   0: 223 156 223 157 223 158 223 159 223 160 223 161 223 162 223 163\n");
    print_str("altcodes   1: 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207\n");
    print_str("altcodes   2: 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222\n");
    print_str("\n");
    print_str("keycode   1 = Escape\n");
    print_str("keycode   2 = one              exclam\n");
    print_str("keycode   3 = two              at\n");
    print_str("keycode   4 = three            numbersign\n");
    print_str("keycode   5 = four             dollar\n");
    print_str("keycode   6 = five             percent\n");
    print_str("keycode   7 = six              asciicircum\n");
    print_str("keycode   8 = seven            ampersand\n");
    print_str("keycode   9 = eight            asterisk\n");
    print_str("keycode  10 = nine             parenleft\n");
    print_str("keycode  11 = zero             parenright\n");
    print_str("keycode  12 = minus            underscore\n");
    print_str("keycode  13 = equal            plus\n");
    print_str("keycode  14 = BackSpace\n");
    print_str("keycode  15 = Tab\n");
    print_str("keycode  16 = q                Q\n");
    print_str("keycode  17 = w                W\n");
    print_str("keycode  18 = e                E\n");
    print_str("keycode  19 = r                R\n");
    print_str("keycode  20 = t                T\n");
    print_str("keycode  21 = y                Y\n");
    print_str("keycode  22 = u                U\n");
    print_str("keycode  23 = i                I\n");
    print_str("keycode  24 = o                O\n");
    print_str("keycode  25 = p                P\n");
    print_str("keycode  26 = bracketleft      braceleft\n");
    print_str("keycode  27 = bracketright     braceright\n");
    print_str("keycode  28 = Return\n");
    print_str("keycode  29 = Control\n");
    print_str("keycode  30 = a                A\n");
    print_str("keycode  31 = s                S\n");
    print_str("keycode  32 = d                D\n");
    print_str("keycode  33 = f                F\n");
    print_str("keycode  34 = g                G\n");
    print_str("keycode  35 = h                H\n");
    print_str("keycode  36 = j                J\n");
    print_str("keycode  37 = k                K\n");
    print_str("keycode  38 = l                L\n");
    print_str("keycode  39 = semicolon        colon\n");
    print_str("keycode  40 = apostrophe       quotedbl\n");
    print_str("keycode  41 = grave            asciitilde\n");
    print_str("keycode  42 = Shift            Shift\n");
    print_str("keycode  43 = backslash        bar\n");
    print_str("keycode  44 = z                Z\n");
    print_str("keycode  45 = x                X\n");
    print_str("keycode  46 = c                C\n");
    print_str("keycode  47 = v                V\n");
    print_str("keycode  48 = b                B\n");
    print_str("keycode  49 = n                N\n");
    print_str("keycode  50 = m                M\n");
    print_str("keycode  51 = comma            less\n");
    print_str("keycode  52 = period           greater\n");
    print_str("keycode  53 = slash            question\n");
    print_str("keycode  54 = Shift            Shift\n");
    print_str("keycode  55 = KP_Multiply\n");
    print_str("keycode  56 = Alt              Alt\n");
    print_str("keycode  57 = space\n");
    print_str("keycode  58 = Caps_Lock\n");
    print_str("keycode  59 = F1\n");
    print_str("keycode  60 = F2\n");
    print_str("keycode  61 = F3\n");
    print_str("keycode  62 = F4\n");
    print_str("\n");
    print_str("dumpkeys v3.0: full keyboard mapping table dumped successfully\n");
}

static void dump_strings_v3(void)
{
    print_str("string F1 = \"\\033[[A\"\n");
    print_str("string F2 = \"\\033[[B\"\n");
    print_str("string F3 = \"\\033[[C\"\n");
    print_str("string F4 = \"\\033[[D\"\n");
    print_str("string F5 = \"\\033[[E\"\n");
    print_str("string F6 = \"\\033[17~\"\n");
    print_str("string F7 = \"\\033[18~\"\n");
    print_str("string F8 = \"\\033[19~\"\n");
    print_str("string F9 = \"\\033[20~\"\n");
    print_str("string F10 = \"\\033[21~\"\n");
    print_str("\n");
    print_str("dumpkeys v3.0: function key string table dumped (10 keys)\n");
}

static void dump_compose_v3(void)
{
    print_str("compose \"'\" \"e\" to 233\n");
    print_str("compose \"'\" \"E\" to 201\n");
    print_str("compose \"'\" \"a\" to 225\n");
    print_str("compose \"'\" \"A\" to 193\n");
    print_str("compose \"'\" \"i\" to 237\n");
    print_str("compose \"'\" \"I\" to 205\n");
    print_str("compose \"'\" \"o\" to 243\n");
    print_str("compose \"'\" \"O\" to 211\n");
    print_str("compose \"'\" \"u\" to 250\n");
    print_str("compose \"'\" \"U\" to 218\n");
    print_str("\n");
    print_str("dumpkeys v3.0: compose table dumped (10 entries)\n");
}

static void print_help(void)
{
    print_str("dumpkeys_v3 - dump keyboard mapping table (v3.0)\n");
    print_str("Usage: dumpkeys_v3 [OPTIONS]\n");
    print_str("  -h, --help     Show this help\n");
    print_str("  -s             Dump string table\n");
    print_str("  -c             Dump compose table\n");
    print_str("  -f             Dump full keymap with altcodes\n");
    print_str("\n");
    print_str("Dump simulated keyboard mapping for virtual terminal (v3 enhanced).\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int dump_strings_flag = 0;
    int dump_compose_flag = 0;
    int dump_full_flag = 0;

    /* Parse arguments: skip argv[0] (module name) */
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* Skip argv[0] */
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            print_help();
            host_exit(0);
        }
        if (my_strcmp(arg, "-s") == 0) {
            dump_strings_flag = 1;
        }
        if (my_strcmp(arg, "-c") == 0) {
            dump_compose_flag = 1;
        }
        if (my_strcmp(arg, "-f") == 0) {
            dump_full_flag = 1;
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    dump_full_keymap();

    if (dump_strings_flag)
        dump_strings_v3();

    if (dump_compose_flag)
        dump_compose_v3();

    host_exit(0);
}
