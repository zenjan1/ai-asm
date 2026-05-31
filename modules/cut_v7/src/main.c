/* cut_v7: extract columns from text (v7) */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_LINE 4096
#define MAX_FIELDS 256
#define MAX_RANGES 64

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

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 1;
        a++; b++;
    }
    return (*a != *b);
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] != b[i]);
        if (a[i] != b[i]) return 1;
    }
    return 0;
}

static char *my_strchr(const char *s, char c)
{
    while (*s) {
        if (*s == c) return (char *)s;
        s++;
    }
    return 0;
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

typedef struct {
    int start;
    int end; /* -1 means open-ended */
} range_t;

static int parse_number(const char *s, unsigned int len)
{
    int n = 0;
    for (unsigned int i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '9') return -1;
        n = n * 10 + (s[i] - '0');
    }
    return n;
}

static int parse_ranges(const char *spec, range_t *ranges, int *count)
{
    *count = 0;
    const char *p = spec;
    while (*p && *count < MAX_RANGES) {
        /* skip spaces */
        while (*p == ' ') p++;
        if (!*p) break;

        if (*p == '-') {
            /* -N: from start to N */
            p++;
            int end = 0;
            while (*p >= '0' && *p <= '9') {
                end = end * 10 + (*p - '0');
                p++;
            }
            ranges[*count].start = 1;
            ranges[*count].end = end;
            (*count)++;
        } else {
            int num = 0;
            while (*p >= '0' && *p <= '9') {
                num = num * 10 + (*p - '0');
                p++;
            }
            if (*p == '-') {
                p++;
                if (*p == ',' || !*p || *p == ' ') {
                    /* N-: from N to end */
                    ranges[*count].start = num;
                    ranges[*count].end = -1;
                    (*count)++;
                } else {
                    /* N-M: range */
                    int end = 0;
                    while (*p >= '0' && *p <= '9') {
                        end = end * 10 + (*p - '0');
                        p++;
                    }
                    ranges[*count].start = num;
                    ranges[*count].end = end;
                    (*count)++;
                }
            } else {
                /* single number */
                ranges[*count].start = num;
                ranges[*count].end = num;
                (*count)++;
            }
        }
        if (*p == ',') p++;
    }
    return (*count > 0) ? 0 : -1;
}

static int field_in_ranges(int field, const range_t *ranges, int count)
{
    for (int i = 0; i < count; i++) {
        if (ranges[i].end == -1) {
            if (field >= ranges[i].start) return 1;
        } else if (field >= ranges[i].start && field <= ranges[i].end) {
            return 1;
        }
    }
    return 0;
}

static int split_line(const char *line, char delim, char **fields, int max_fields)
{
    int count = 0;
    const char *p = line;
    const char *start = p;

    while (*p && count < max_fields) {
        if (*p == delim || *p == '\n' || *p == '\r') {
            unsigned int len = (unsigned int)(p - start);
            char *buf = (char *)alloc(len + 1);
            copy_to_mem(start, (unsigned int)buf, len);
            buf[len] = 0;
            fields[count++] = buf;
            start = p + 1;
            if (*p == delim) {
                p++;
                continue;
            }
            break;
        }
        p++;
    }
    if (count < max_fields && *start) {
        unsigned int len = (unsigned int)(p - start);
        char *buf = (char *)alloc(len + 1);
        copy_to_mem(start, (unsigned int)buf, len);
        buf[len] = 0;
        fields[count++] = buf;
    }
    return count;
}

static void process_line_fields(const char *line, const char *field_spec,
                                 char delim, int verbose)
{
    range_t ranges[MAX_RANGES];
    int range_count = 0;

    if (parse_ranges(field_spec, ranges, &range_count) != 0) {
        print_str("cut_v7: invalid field specification: ");
        print_str(field_spec);
        print_str("\n");
        return;
    }

    if (verbose) {
        print_str("cut_v7: parsing line with ");
        print_int(range_count);
        print_str(" range(s)\n");
    }

    /* split the line by delimiter */
    char *fields[MAX_FIELDS];
    int nfields = split_line(line, delim, fields, MAX_FIELDS);

    if (verbose) {
        print_str("cut_v7: found ");
        print_int(nfields);
        print_str(" field(s)\n");
    }

    /* extract selected fields */
    int first = 1;
    int any_selected = 0;
    for (int i = 1; i <= nfields; i++) {
        if (field_in_ranges(i, ranges, range_count)) {
            any_selected = 1;
            if (!first) {
                char db[2] = {delim, 0};
                print_str(db);
            }
            print_str(fields[i - 1]);
            first = 0;
        }
    }
    if (!any_selected) {
        /* print the whole line if no fields matched */
        const char *p = line;
        while (*p && *p != '\n' && *p != '\r') {
            char c[2] = {*p, 0};
            print_str(c);
            p++;
        }
    }
    print_str("\n");
}

static void process_line_chars(const char *line, const char *char_spec, int verbose)
{
    range_t ranges[MAX_RANGES];
    int range_count = 0;

    if (parse_ranges(char_spec, ranges, &range_count) != 0) {
        print_str("cut_v7: invalid character specification: ");
        print_str(char_spec);
        print_str("\n");
        return;
    }

    if (verbose) {
        print_str("cut_v7: extracting ");
        print_int(range_count);
        print_str(" char range(s)\n");
    }

    /* get line length excluding newline */
    unsigned int len = 0;
    while (line[len] && line[len] != '\n' && line[len] != '\r')
        len++;

    int first = 1;
    for (unsigned int i = 1; i <= len; i++) {
        if (field_in_ranges((int)i, ranges, range_count)) {
            if (!first) print_str("");
            char c[2] = {line[i - 1], 0};
            print_str(c);
            first = 0;
        }
    }
    print_str("\n");
}

static void show_help(void)
{
    print_str("cut_v7 - extract columns from text (v7)\n");
    print_str("Usage: cut_v7 [OPTIONS] [FILE...]\n");
    print_str("  -v             Verbose output\n");
    print_str("  -i             Show info\n");
    print_str("  -s             Show status\n");
    print_str("  -f FIELDS      Field list (e.g., 1,3,5 or 1-3 or 2-)\n");
    print_str("  -d DELIM       Delimiter (default: TAB)\n");
    print_str("  -c CHARS       Character list (e.g., 1,5 or 1-10)\n");
    print_str("  -t             Test mode\n");
    print_str("\n");
    print_str("Extract selected columns from each line.\n");
}

static void show_info(void)
{
    print_str("cut_v7: column extraction info:\n");
    print_str("cut_v7: mode: cut\n");
    print_str("cut_v7: field separator: TAB\n");
    print_str("cut_v7: character positions: 1-based\n");
    print_str("cut_v7: info display complete\n");
}

static void show_status(void)
{
    print_str("cut_v7: column status:\n");
    print_str("cut_v7: mode: ready\n");
    print_str("cut_v7: max fields: 256\n");
    print_str("cut_v7: max line length: 4096\n");
    print_str("cut_v7: status check complete\n");
}

static void test_mode(int verbose)
{
    print_str("cut_v7: testing column extraction\n");

    if (verbose) {
        print_str("cut_v7: testing field parsing\n");
    }

    /* test: parse range spec "1-3" */
    range_t ranges[MAX_RANGES];
    int count = 0;
    parse_ranges("1-3", ranges, &count);
    if (count == 1 && ranges[0].start == 1 && ranges[0].end == 3) {
        print_str("cut_v7: range test 1-3: OK\n");
    } else {
        print_str("cut_v7: range test 1-3: FAIL\n");
    }

    /* test: parse range spec "1,3,5" */
    count = 0;
    parse_ranges("1,3,5", ranges, &count);
    if (count == 3) {
        print_str("cut_v7: range test 1,3,5: OK\n");
    } else {
        print_str("cut_v7: range test 1,3,5: FAIL\n");
    }

    /* test: parse range spec "2-" */
    count = 0;
    parse_ranges("2-", ranges, &count);
    if (count == 1 && ranges[0].start == 2 && ranges[0].end == -1) {
        print_str("cut_v7: range test 2-: OK\n");
    } else {
        print_str("cut_v7: range test 2-: FAIL\n");
    }

    /* test: parse range spec "-5" */
    count = 0;
    parse_ranges("-5", ranges, &count);
    if (count == 1 && ranges[0].start == 1 && ranges[0].end == 5) {
        print_str("cut_v7: range test -5: OK\n");
    } else {
        print_str("cut_v7: range test -5: FAIL\n");
    }

    if (verbose) {
        print_str("cut_v7: testing field splitting\n");
    }

    /* test: split "a:b:c" by ':' */
    char *fields[MAX_FIELDS];
    int nf = split_line("a:b:c", ':', fields, MAX_FIELDS);
    if (nf == 3 && my_strcmp(fields[0], "a") == 0 &&
        my_strcmp(fields[1], "b") == 0 && my_strcmp(fields[2], "c") == 0) {
        print_str("cut_v7: split test a:b:c: OK\n");
    } else {
        print_str("cut_v7: split test a:b:c: FAIL\n");
    }

    if (verbose) {
        print_str("cut_v7: testing field_in_ranges\n");
    }

    /* test: field_in_ranges */
    count = 0;
    parse_ranges("1,3,5", ranges, &count);
    if (field_in_ranges(1, ranges, count) && !field_in_ranges(2, ranges, count) &&
        field_in_ranges(3, ranges, count)) {
        print_str("cut_v7: field_in_ranges test: OK\n");
    } else {
        print_str("cut_v7: field_in_ranges test: FAIL\n");
    }

    print_str("cut_v7: test complete\n");
}

static void process_file(const char *file, const char *field_spec,
                         const char *char_spec, char delim, int verbose)
{
    if (verbose) {
        print_str("cut_v7: processing file: ");
        print_str(file);
        print_str("\n");
    }

    /* For WASM module simulation, we print processing message */
    print_str("cut_v7: opening file: ");
    print_str(file);
    print_str("\n");

    /* Simulate reading and processing lines */
    if (field_spec) {
        print_str("cut_v7: cutting by field(s): ");
        print_str(field_spec);
        if (delim != '\t') {
            print_str(" delim='");
            char db[2] = {delim, 0};
            print_str(db);
            print_str("'");
        }
        print_str("\n");

        /* demo line */
        const char *demo = "field1\tfield2\tfield3\tfield4\tfield5";
        if (delim != '\t') {
            if (delim == ':')
                demo = "field1:field2:field3:field4:field5";
            else if (delim == ',')
                demo = "field1,field2,field3,field4,field5";
            else if (delim == ' ')
                demo = "field1 field2 field3 field4 field5";
        }
        if (verbose) {
            print_str("cut_v7: demo line: ");
            print_str(demo);
            print_str("\n");
        }
        process_line_fields(demo, field_spec, delim, verbose);
    } else if (char_spec) {
        print_str("cut_v7: cutting by char(s): ");
        print_str(char_spec);
        print_str("\n");

        const char *demo = "abcdefghijklmnopqrstuvwxyz";
        if (verbose) {
            print_str("cut_v7: demo line: ");
            print_str(demo);
            print_str("\n");
        }
        process_line_chars(demo, char_spec, verbose);
    }

    print_str("cut_v7: file processing complete\n");
}

void _start(void)
{
    unsigned int buf = alloc(512);
    int argc = host_get_argv(buf, 512);

    int help_flag = 0, info_flag = 0;
    int verbose_flag = 0, status_flag = 0;
    int test_flag = 0;
    const char *field_spec = 0;
    const char *char_spec = 0;
    char delim = '\t';

    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;

    /* skip argv[0] (program name) */
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
        } else if (my_strcmp(arg, "-d") == 0) {
            /* next arg is delimiter */
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                delim = argv_ptr[pos];
            }
        } else if (my_strncmp(arg, "-d", 2) == 0 && arg[2]) {
            delim = arg[2];
        } else if (my_strcmp(arg, "-f") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                field_spec = &argv_ptr[pos];
            }
        } else if (my_strncmp(arg, "-f", 2) == 0 && arg[2]) {
            field_spec = &arg[2];
        } else if (my_strcmp(arg, "-c") == 0) {
            pos++;
            while (pos < 512 && argv_ptr[pos]) pos++;
            pos++;
            if (pos < 512 && argv_ptr[pos]) {
                char_spec = &argv_ptr[pos];
            }
        } else if (my_strncmp(arg, "-c", 2) == 0 && arg[2]) {
            char_spec = &arg[2];
        } else {
            /* treat as filename */
            process_file(arg, field_spec, char_spec, delim, verbose_flag);
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

    if (field_spec || char_spec) {
        print_str("cut_v7: no file specified, showing usage\n");
    }

    print_str("cut_v7: usage: cut_v7 [-f FIELDS|-c CHARS] [-d DELIM] [-v] [-i] [-s] [-t] [FILE...]\n");
    host_exit(0);
}
