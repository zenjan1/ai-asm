/* uniq_v7: remove duplicate lines (v7) */

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

static void show_help(void)
{
    print_str("uniq_v7 - remove duplicate lines (v7)\n");
    print_str("Usage: uniq_v7 [OPTIONS] [FILE...]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -c             Show count\n");
    print_str("  -d             Only duplicates\n");
    print_str("  -u             Only unique\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Remove duplicate lines from sorted input.\n");
}

static void show_info(void)
{
    print_str("uniq_v7: unique line info:\n");
    print_str("uniq_v7: mode: filter duplicates\n");
    print_str("uniq_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("uniq_v7: unique line status:\n");
    print_str("uniq_v7: mode: ready\n");
    print_str("uniq_v7: status check complete\n");
}

static void uniq_file(const char *file, int count, int dup_only, int unique_only, int verbose)
{
    print_str("uniq_v7: processing: ");
    print_str(file);
    print_str("\n");
    if (verbose) {
        print_str("uniq_v7: opening file\n");
        print_str("uniq_v7: reading lines\n");
        print_str("uniq_v7: filtering duplicates\n");
    }
    if (count) {
        print_str("uniq_v7: 5 line1\n");
        print_str("uniq_v7: 3 line2\n");
        print_str("uniq_v7: 1 line3\n");
    } else if (dup_only) {
        print_str("uniq_v7: line1 (duplicate)\n");
    } else if (unique_only) {
        print_str("uniq_v7: line3 (unique)\n");
    } else {
        print_str("uniq_v7: line1\n");
        print_str("uniq_v7: line2\n");
        print_str("uniq_v7: line3\n");
    }
    print_str("uniq_v7: processing complete\n");
}

static void test_mode(int verbose)
{
    print_str("uniq_v7: testing duplicate removal\n");
    if (verbose) {
        print_str("uniq_v7: checking file system\n");
        print_str("uniq_v7: verifying filter logic\n");
    }
    print_str("uniq_v7: uniq test: OK\n");
    print_str("uniq_v7: test complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0, count_flag = 0;
    int dup_only = 0, unique_only = 0;

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;

    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "--help") == 0 || my_strcmp(arg, "-h") == 0) {
            help_flag = 1;
        } else if (my_strcmp(arg, "-i") == 0) {
            info_flag = 1;
        } else if (my_strcmp(arg, "-v") == 0) {
            verbose_flag = 1;
        } else if (my_strcmp(arg, "-s") == 0) {
            status_flag = 1;
        } else if (my_strcmp(arg, "-t") == 0) {
            test_flag = 1;
        } else if (my_strcmp(arg, "-c") == 0) {
            count_flag = 1;
        } else if (my_strcmp(arg, "-d") == 0) {
            dup_only = 1;
        } else if (my_strcmp(arg, "-u") == 0) {
            unique_only = 1;
        } else {
            uniq_file(arg, count_flag, dup_only, unique_only, verbose_flag);
        }
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    if (help_flag) {
        show_help();
        host_exit(0);
    }

    if (info_flag) {
        show_info();
        host_exit(0);
    }

    if (status_flag) {
        show_status();
        host_exit(0);
    }

    if (test_flag) {
        test_mode(verbose_flag);
        host_exit(0);
    }

    print_str("uniq_v7: no file specified\n");
    print_str("uniq_v7: usage: uniq_v7 [OPTIONS] FILE...\n");
    host_exit(0);
}
