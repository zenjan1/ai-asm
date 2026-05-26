/* dircolors: color configuration output (simulated) */
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

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

__attribute__((export_name("_start")))
void _start(void)
{
    const char *ls_colors = "LS_COLORS='di=01;34:ln=01;36:so=01;35:pi=33:ex=01;32:bd=46;34:cd=43;34:su=41;37:sg=46;37:tw=42;37:ow=01;34:*.tar=01;31:*.gz=01;31:*.bz2=01;31:*.xz=01;31:*.zip=01;31:*.jpg=01;35:*.png=01;35:*.mp3=01;36:*.wav=01;36:';\nexport LS_COLORS\n";
    unsigned int len = my_strlen(ls_colors);
    unsigned int off = alloc(len);
    copy_to_mem(off, ls_colors, len);
    host_print(off, len);
    host_exit(0);
}
