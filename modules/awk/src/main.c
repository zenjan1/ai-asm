/*
 * aiasm-aarch64/modules/awk/src/main.c
 * awk WASM module for AI-ASM AArch64 v34.0
 *
 * Usage: awk [-F sep] 'PATTERN { action }' [file ...]
 * Supports: -F, BEGIN, END, $1..$NF, NR, NF, print, arithmetic, comparisons
 */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("getc")))
extern int host_getc(void);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("fs_open")))
extern int host_fs_open(int fd_off, unsigned int path_off);

__attribute__((import_module("host"), import_name("fs_read")))
extern int host_fs_read(int fd, unsigned int buf_off, unsigned int max_len);

__attribute__((import_module("host"), import_name("fs_close")))
extern void host_fs_close(int fd);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

#define MAX_LINE    1024
#define MAX_FIELDS  32
#define FIELD_LEN   256
#define MAX_ACTIONS 8
#define ACTION_LEN  128
#define MAX_PROGRAM 512

/* -------------------------------------------------------------------------- */
/* Memory (bump allocator)                                                    */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static char *my_strchr(const char *s, char c)
{
    while (*s) { if (*s == c) return (char *)s; s++; }
    return (char *)0;
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || a[i] != b[i])
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

static void my_strncpy(char *dst, const char *src, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        dst[i] = src[i];
        if (!src[i]) break;
    }
}

static void my_itoa(int n, char *buf)
{
    char tmp[16];
    int i = 0;
    if (n < 0) { *buf++ = '-'; n = -n; }
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (n > 0) { tmp[i++] = (char)('0' + (n % 10)); n /= 10; }
    int j = 0;
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

static int is_fs_char(char c, const char *fs)
{
    while (*fs) { if (c == *fs) return 1; fs++; }
    return 0;
}

static int is_digit(char c) { return c >= '0' && c <= '9'; }

/* -------------------------------------------------------------------------- */
/* Output helpers                                                             */
/* -------------------------------------------------------------------------- */

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    if (len == 0) return;
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

static void print_char(char c)
{
    unsigned int off = alloc(1);
    char *p = (char *)off;
    p[0] = c;
    host_print(off, 1);
}

/* -------------------------------------------------------------------------- */
/* Number parsing / arithmetic                                                */
/* -------------------------------------------------------------------------- */

static int parse_int(const char *s)
{
    int n = 0;
    int neg = 0;
    if (*s == '-') { neg = 1; s++; }
    while (is_digit(*s)) { n = n * 10 + (*s - '0'); s++; }
    return neg ? -n : n;
}

/* Simple expression evaluator: handles $N, integers, +, -, spaces */
static int eval_expr(const char *expr, char fields[][FIELD_LEN], int nf);

static int eval_term(const char **pp, char fields[][FIELD_LEN], int nf)
{
    const char *p = *pp;
    while (*p == ' ' || *p == '\t') p++;

    int val = 0;
    if (*p == '$') {
        p++;
        if (is_digit(*p)) {
            int idx = 0;
            while (is_digit(*p)) { idx = idx * 10 + (*p - '0'); p++; }
            if (idx >= 1 && idx <= nf)
                val = parse_int(fields[idx - 1]);
        }
    } else if (is_digit(*p) || *p == '-') {
        char tmp[16];
        int i = 0;
        if (*p == '-') { tmp[i++] = *p; p++; }
        while (is_digit(*p) && i < 15) { tmp[i++] = *p; p++; }
        tmp[i] = '\0';
        val = parse_int(tmp);
    }
    *pp = p;
    return val;
}

static int eval_expr(const char *expr, char fields[][FIELD_LEN], int nf)
{
    const char *p = expr;
    int left = eval_term(&p, fields, nf);

    while (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
        char op = *p;
        p++;
        int right = eval_term(&p, fields, nf);
        if (op == '+') left = left + right;
        else if (op == '-') left = left - right;
        else if (op == '*') left = left * right;
        else if (op == '/' && right != 0) left = left / right;
    }
    return left;
}

/* -------------------------------------------------------------------------- */
/* Field splitting                                                            */
/* -------------------------------------------------------------------------- */

static int split_fields(const char *line, char fields[][FIELD_LEN], const char *fs)
{
    int count = 0;
    const char *p = line;

    /* Skip leading whitespace for default separator */
    if (fs[0] == ' ' && fs[1] == '\0') {
        while (*p == ' ' || *p == '\t') p++;
    }

    while (*p && count < MAX_FIELDS) {
        /* Skip separators */
        if (fs[0] == ' ' && fs[1] == '\0') {
            /* whitespace: skip run of spaces/tabs */
            while (*p == ' ' || *p == '\t') p++;
            if (!*p) break;
        } else {
            while (*p && is_fs_char(*p, fs)) p++;
            if (!*p) break;
        }

        /* Copy field */
        int len = 0;
        if (fs[0] == ' ' && fs[1] == '\0') {
            while (*p && *p != ' ' && *p != '\t' && *p != '\n' && *p != '\r' && len < FIELD_LEN - 1)
                fields[count][len++] = *p++;
        } else {
            while (*p && !is_fs_char(*p, fs) && *p != '\n' && *p != '\r' && len < FIELD_LEN - 1)
                fields[count][len++] = *p++;
        }
        fields[count][len] = '\0';
        count++;
    }
    return count;
}

/* -------------------------------------------------------------------------- */
/* Action execution                                                           */
/* -------------------------------------------------------------------------- */

/* Action types */
#define ACT_PRINT_EXPR  1
#define ACT_PRINT_NR    2
#define ACT_PRINT_NF    3
#define ACT_PRINT_STR   4
#define ACT_PRINT_LINE  5
#define ACT_PRINT_FIELD 6
#define ACT_PRINT_EXPR_STR 7

typedef struct {
    int type;
    char expr[ACTION_LEN];   /* for arithmetic */
    char str[ACTION_LEN];    /* for string literal */
    int field_idx;           /* for $N */
} action_t;

static int parse_actions(const char *body, action_t *actions, int max_actions)
{
    int count = 0;
    const char *p = body;

    while (*p && count < max_actions) {
        /* Skip whitespace */
        while (*p == ' ' || *p == '\t' || *p == '\n') p++;
        if (!*p || *p == '}') break;

        if (my_strncmp(p, "print", 5) == 0) {
            p += 5;
            while (*p == ' ' || *p == '\t') p++;

            if (*p == '$' && is_digit(p[1])) {
                p++;
                int idx = 0;
                while (is_digit(*p)) { idx = idx * 10 + (*p - '0'); p++; }
                if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
                    /* arithmetic: backtrack to $ */
                    p -= (idx >= 10 ? 2 : 1);
                    if (idx >= 10) p--;
                    /* Collect full expression */
                    int elen = 0;
                    const char *start = p;
                    while (*p && *p != ',' && *p != ';' && *p != '}' && *p != '\n' && elen < ACTION_LEN - 1)
                        p++, elen++;
                    my_strncpy(actions[count].expr, start, elen);
                    actions[count].expr[elen] = '\0';
                    actions[count].type = ACT_PRINT_EXPR;
                } else if (*p == ' ' || *p == '\t') {
                    /* print $N followed by string concatenation */
                    actions[count].field_idx = idx;
                    actions[count].type = ACT_PRINT_EXPR_STR;
                    p++;
                    while (*p == ' ' || *p == '\t') p++;
                    if (*p == '"') {
                        p++;
                        int slen = 0;
                        while (*p && *p != '"' && slen < ACTION_LEN - 1)
                            actions[count].str[slen++] = *p++;
                        actions[count].str[slen] = '\0';
                        if (*p == '"') p++;
                    }
                } else {
                    actions[count].field_idx = idx;
                    actions[count].type = ACT_PRINT_FIELD;
                }
            } else if (*p == '"') {
                /* print "string literal" */
                p++;
                int slen = 0;
                while (*p && *p != '"' && slen < ACTION_LEN - 1)
                    actions[count].str[slen++] = *p++;
                actions[count].str[slen] = '\0';
                if (*p == '"') p++;
                actions[count].type = ACT_PRINT_STR;
            } else if (my_strncmp(p, "NR", 2) == 0) {
                p += 2;
                actions[count].type = ACT_PRINT_NR;
            } else if (my_strncmp(p, "NF", 2) == 0) {
                p += 2;
                actions[count].type = ACT_PRINT_NF;
            } else if (my_strncmp(p, "$0", 2) == 0) {
                p += 2;
                actions[count].type = ACT_PRINT_LINE;
            } else {
                /* General expression */
                int elen = 0;
                const char *start = p;
                while (*p && *p != ',' && *p != ';' && *p != '}' && *p != '\n' && elen < ACTION_LEN - 1)
                    p++, elen++;
                my_strncpy(actions[count].expr, start, elen);
                actions[count].expr[elen] = '\0';
                actions[count].type = ACT_PRINT_EXPR;
            }
            count++;
        } else {
            /* Skip unknown token */
            while (*p && *p != ',' && *p != ';' && *p != '}') p++;
        }
    }
    return count;
}

static void execute_actions(action_t *actions, int nactions,
                            char fields[][FIELD_LEN], int nf, int nr,
                            const char *line)
{
    char num_buf[16];

    for (int i = 0; i < nactions; i++) {
        if (i > 0) print_char(' ');

        switch (actions[i].type) {
            case ACT_PRINT_EXPR: {
                int val = eval_expr(actions[i].expr, fields, nf);
                my_itoa(val, num_buf);
                print_str(num_buf);
                break;
            }
            case ACT_PRINT_NR: {
                my_itoa(nr, num_buf);
                print_str(num_buf);
                break;
            }
            case ACT_PRINT_NF: {
                my_itoa(nf, num_buf);
                print_str(num_buf);
                break;
            }
            case ACT_PRINT_STR: {
                print_str(actions[i].str);
                break;
            }
            case ACT_PRINT_LINE: {
                /* Print line without trailing newline */
                unsigned int len = my_strlen(line);
                while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
                    len--;
                unsigned int off = alloc(len);
                copy_to_mem(off, line, len);
                host_print(off, len);
                break;
            }
            case ACT_PRINT_FIELD: {
                int idx = actions[i].field_idx;
                if (idx >= 1 && idx <= nf)
                    print_str(fields[idx - 1]);
                break;
            }
            case ACT_PRINT_EXPR_STR: {
                int idx = actions[i].field_idx;
                if (idx >= 1 && idx <= nf)
                    print_str(fields[idx - 1]);
                print_str(actions[i].str);
                break;
            }
        }
    }
    print_char('\n');
}

/* -------------------------------------------------------------------------- */
/* Pattern matching                                                           */
/* -------------------------------------------------------------------------- */

/* Check if a line matches the pattern (simple: /regex/ or comparison) */
static int match_pattern(const char *pattern, const char *line,
                         char fields[][FIELD_LEN], int nf, int nr)
{
    if (!pattern || !*pattern) return 1;  /* no pattern = match all */

    /* BEGIN and END are handled separately */
    if (my_strncmp(pattern, "BEGIN", 5) == 0 || my_strncmp(pattern, "END", 3) == 0)
        return 0;

    /* /regex/ - simple substring match */
    if (*pattern == '/') {
        const char *re = pattern + 1;
        unsigned int re_len = 0;
        while (re[re_len] && re[re_len] != '/') re_len++;

        if (re_len == 0) return 1;

        /* Simple substring search */
        unsigned int line_len = my_strlen(line);
        for (unsigned int i = 0; i + re_len <= line_len; i++) {
            if (my_strncmp(&line[i], re, re_len) == 0)
                return 1;
        }
        return 0;
    }

    /* NR comparison: NR==N, NR>N, NR<N */
    if (my_strncmp(pattern, "NR", 2) == 0) {
        const char *p = pattern + 2;
        char op = *p;
        p++;
        int cmp_val = 0;
        while (is_digit(*p)) { cmp_val = cmp_val * 10 + (*p - '0'); p++; }

        if (op == '=') return nr == cmp_val;
        if (op == '>') return nr > cmp_val;
        if (op == '<') return nr < cmp_val;
        return 1;
    }

    /* $N comparison: $1==val, $2>val */
    if (*pattern == '$') {
        const char *dp = pattern + 1;
        int field_idx = 0;
        while (is_digit(*dp)) { field_idx = field_idx * 10 + (*dp - '0'); dp++; }

        char op = *dp;
        dp++;
        int cmp_val = 0;
        int neg = 0;
        if (*dp == '-') { neg = 1; dp++; }
        while (is_digit(*dp)) { cmp_val = cmp_val * 10 + (*dp - '0'); dp++; }
        if (neg) cmp_val = -cmp_val;

        int field_val = 0;
        if (field_idx >= 1 && field_idx <= nf)
            field_val = parse_int(fields[field_idx - 1]);

        if (op == '=') return field_val == cmp_val;
        if (op == '>') return field_val > cmp_val;
        if (op == '<') return field_val < cmp_val;
        return 1;
    }

    return 1;  /* default: match all */
}

/* -------------------------------------------------------------------------- */
/* Program parsing                                                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    char pattern[MAX_PROGRAM];
    action_t actions[MAX_ACTIONS];
    int nactions;
} rule_t;

#define MAX_RULES 8

/* Parse awk program: 'pattern { action } pattern { action } ...' */
static int parse_program(const char *prog, rule_t *rules, int max_rules)
{
    int count = 0;
    const char *p = prog;

    while (*p && count < max_rules) {
        /* Skip whitespace */
        while (*p == ' ' || *p == '\t' || *p == '\n') p++;
        if (!*p) break;

        /* Parse pattern (everything until '{') */
        int plen = 0;
        while (*p && *p != '{' && plen < MAX_PROGRAM - 1)
            rules[count].pattern[plen++] = *p++;
        rules[count].pattern[plen] = '\0';

        /* Find action body between '{' and '}' */
        if (*p == '{') {
            p++;
            char body[MAX_PROGRAM];
            int blen = 0;
            while (*p && *p != '}' && blen < MAX_PROGRAM - 1)
                body[blen++] = *p++;
            body[blen] = '\0';
            if (*p == '}') p++;

            rules[count].nactions = parse_actions(body, rules[count].actions, MAX_ACTIONS);
        } else {
            /* No action block, default to print */
            rules[count].actions[0].type = ACT_PRINT_LINE;
            rules[count].nactions = 1;
        }
        count++;
    }
    return count;
}

/* -------------------------------------------------------------------------- */
/* Input reading                                                              */
/* -------------------------------------------------------------------------- */

static int read_line(char *buf, int max_len)
{
    int pos = 0;
    int c;
    while (pos < max_len - 1) {
        c = host_getc();
        if (c < 0) break;  /* EOF */
        buf[pos++] = (char)c;
        if (c == '\n') break;
    }
    buf[pos] = '\0';
    return pos;
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get command line arguments */
    unsigned int argv_off = alloc(4096);
    int argv_len = host_get_argv(argv_off, 4095);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;

    /* Parse arguments: skip "awk", handle -F sep, then pattern, then optional files */
    const char *fs = " ";
    const char *program = (const char *)0;
    const char *filename = (const char *)0;

    int i = 0;
    /* Skip argv[0] ("awk") */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;

    /* Parse -F separator */
    if (i + 1 < argv_len && argv[i] == '-' && argv[i + 1] == 'F') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        /* Read separator */
        char sep_buf[8];
        int slen = 0;
        while (i < argv_len && argv[i] != ' ' && slen < 7)
            sep_buf[slen++] = argv[i++];
        sep_buf[slen] = '\0';
        fs = sep_buf;
        while (i < argv_len && argv[i] == ' ') i++;
    } else if (i + 3 < argv_len && my_strncmp(&argv[i], "-F", 2) == 0 && argv[i+2] == ' ') {
        i += 3;
        while (i < argv_len && argv[i] == ' ') i++;
        char sep_buf[8];
        int slen = 0;
        while (i < argv_len && argv[i] != ' ' && slen < 7)
            sep_buf[slen++] = argv[i++];
        sep_buf[slen] = '\0';
        fs = sep_buf;
        while (i < argv_len && argv[i] == ' ') i++;
    }

    /* Read program (everything until next space after -F or from start) */
    const char *prog_start = &argv[i];
    int prog_len = 0;
    while (i < argv_len && argv[i] != ' ') {
        prog_len++;
        i++;
    }

    /* Build program string (handle quoted patterns with spaces) */
    char prog_buf[MAX_PROGRAM];
    int pidx = 0;

    /* Copy first part */
    for (int j = 0; j < prog_len && pidx < MAX_PROGRAM - 1; j++)
        prog_buf[pidx++] = prog_start[j];

    /* Check if there's more (space-separated continuation, typically for patterns with spaces) */
    while (i < argv_len) {
        while (i < argv_len && argv[i] == ' ') i++;
        if (i >= argv_len || argv[i] == '.') break;  /* end of args or filename */
        if (pidx < MAX_PROGRAM - 1) prog_buf[pidx++] = ' ';
        while (i < argv_len && argv[i] != ' ' && pidx < MAX_PROGRAM - 1)
            prog_buf[pidx++] = argv[i++];
    }
    prog_buf[pidx] = '\0';

    /* Check for filename argument */
    /* Look for a token that's not part of the program */
    i = 0;
    /* Re-skip */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    /* Skip -F and sep */
    if (i + 1 < argv_len && argv[i] == '-' && argv[i + 1] == 'F') {
        i += 2;
        while (i < argv_len && argv[i] == ' ') i++;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
    } else if (i + 3 < argv_len && my_strncmp(&argv[i], "-F", 2) == 0 && argv[i+2] == ' ') {
        i += 3;
        while (i < argv_len && argv[i] == ' ') i++;
        while (i < argv_len && argv[i] != ' ') i++;
        while (i < argv_len && argv[i] == ' ') i++;
    }
    /* Skip program */
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    /* This is the filename */
    if (i < argv_len)
        filename = &argv[i];

    /* Parse the program */
    rule_t rules[MAX_RULES];
    int nrules = parse_program(prog_buf, rules, MAX_RULES);

    /* Separate BEGIN, main, END rules */
    int has_begin = 0, has_end = 0;
    int end_rule_idx = -1;
    for (int r = 0; r < nrules; r++) {
        if (my_strncmp(rules[r].pattern, "BEGIN", 5) == 0)
            has_begin = 1, rules[r].nactions = rules[r].nactions;
        if (my_strncmp(rules[r].pattern, "END", 3) == 0)
            has_end = 1, end_rule_idx = r;
    }

    /* Execute BEGIN blocks */
    char fields[MAX_FIELDS][FIELD_LEN];
    if (has_begin) {
        for (int r = 0; r < nrules; r++) {
            if (my_strncmp(rules[r].pattern, "BEGIN", 5) == 0) {
                split_fields("", fields, fs);
                execute_actions(rules[r].actions, rules[r].nactions, fields, 0, 0, "");
            }
        }
    }

    /* Process input lines */
    int nr = 0;
    char line[MAX_LINE];

    if (filename) {
        /* Read from file via fs_open/fs_read */
        unsigned int path_off = alloc(my_strlen(filename) + 1);
        copy_to_mem(path_off, filename, my_strlen(filename) + 1);
        int fd = host_fs_open(0, path_off);
        if (fd >= 0) {
            unsigned int buf_off = alloc(MAX_LINE);
            int bytes;
            char line_buf[MAX_LINE];
            int pos = 0;

            while ((bytes = host_fs_read(fd, buf_off, MAX_LINE - 1)) > 0) {
                const char *data = (const char *)buf_off;
                for (int j = 0; j < bytes; j++) {
                    if (pos < MAX_LINE - 1) {
                        line_buf[pos++] = data[j];
                    }
                    if (data[j] == '\n') {
                        line_buf[pos] = '\0';
                        nr++;
                        int nf = split_fields(line_buf, fields, fs);
                        /* Check non-END rules */
                        for (int r = 0; r < nrules; r++) {
                            if (my_strncmp(rules[r].pattern, "END", 3) == 0) continue;
                            if (my_strncmp(rules[r].pattern, "BEGIN", 5) == 0) continue;
                            if (match_pattern(rules[r].pattern, line_buf, fields, nf, nr)) {
                                execute_actions(rules[r].actions, rules[r].nactions, fields, nf, nr, line_buf);
                            }
                        }
                        pos = 0;
                    }
                }
            }
            /* Handle last line without newline */
            if (pos > 0) {
                line_buf[pos] = '\0';
                nr++;
                int nf = split_fields(line_buf, fields, fs);
                for (int r = 0; r < nrules; r++) {
                    if (my_strncmp(rules[r].pattern, "END", 3) == 0) continue;
                    if (my_strncmp(rules[r].pattern, "BEGIN", 5) == 0) continue;
                    if (match_pattern(rules[r].pattern, line_buf, fields, nf, nr)) {
                        execute_actions(rules[r].actions, rules[r].nactions, fields, nf, nr, line_buf);
                    }
                }
            }
            host_fs_close(fd);
        }
    } else {
        /* Read from stdin (pipe) via getc */
        while (read_line(line, MAX_LINE) > 0) {
            nr++;
            int nf = split_fields(line, fields, fs);
            /* Check non-BEGIN, non-END rules */
            for (int r = 0; r < nrules; r++) {
                if (my_strncmp(rules[r].pattern, "END", 3) == 0) continue;
                if (my_strncmp(rules[r].pattern, "BEGIN", 5) == 0) continue;
                if (match_pattern(rules[r].pattern, line, fields, nf, nr)) {
                    execute_actions(rules[r].actions, rules[r].nactions, fields, nf, nr, line);
                }
            }
        }
    }

    /* Execute END blocks */
    if (has_end) {
        for (int r = 0; r < nrules; r++) {
            if (my_strncmp(rules[r].pattern, "END", 3) == 0) {
                split_fields("", fields, fs);
                execute_actions(rules[r].actions, rules[r].nactions, fields, 0, nr, "");
            }
        }
    }

    host_exit(0);
}
