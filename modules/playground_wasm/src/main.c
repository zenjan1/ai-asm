/* playground_wasm: WASM playground environment (v46.0)
 *
 * Provides an interactive WebAssembly development playground:
 * - Code editor simulation (line-based editing, syntax highlighting markers)
 * - Live compilation preview (compile status tracking, error reporting)
 * - Module runner (execute WASM modules in sandbox)
 * - Output capture (stdout/stderr collection, result display)
 * - Session management (save/load code snippets, history)
 * - Template library (pre-built starter templates)
 */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ===== Limits ===== */
#define MAX_LINES           256
#define MAX_LINE_LEN        128
#define MAX_MODULES         32
#define MAX_OUTPUT          1024
#define MAX_SESSIONS        16
#define MAX_HISTORY         64
#define MAX_TEMPLATES       16
#define MAX_ERRORS          64
#define MAX_MODULE_NAME     32
#define MAX_SNIPPET_NAME    32
#define MAX_TEMPLATE_NAME   32

/* ===== Syntax Highlight Token Types ===== */
#define TOK_NONE        0
#define TOK_KEYWORD     1
#define TOK_STRING      2
#define TOK_COMMENT     3
#define TOK_NUMBER      4
#define TOK_TYPE        5
#define TOK_FUNCTION    6
#define TOK_OPERATOR    7
#define TOK_PREPROCESSOR 8

/* ===== Compile Status ===== */
#define COMPILE_IDLE        0
#define COMPILE_PENDING     1
#define COMPILE_SUCCESS     2
#define COMPILE_ERROR       3
#define COMPILE_WARNING     4

/* ===== Run Status ===== */
#define RUN_IDLE        0
#define RUN_RUNNING     1
#define RUN_DONE        2
#define RUN_CRASHED     3
#define RUN_TIMEOUT     4

/* ===== Highlight marker struct ===== */
typedef struct {
    int line;
    int col_start;
    int col_end;
    int token_type;
} highlight_t;

/* ===== Editor Line ===== */
typedef struct {
    char        text[MAX_LINE_LEN];
    int         length;
    int         dirty;          /* modified since last compile */
    int         breakpoint;     /* breakpoint set */
    int         folded;         /* line folded in UI */
    highlight_t highlights[8];
    int         highlight_count;
} editor_line_t;

/* ===== Compile Error ===== */
typedef struct {
    int     line;
    int     col;
    int     severity;       /* COMPILE_ERROR or COMPILE_WARNING */
    char    message[128];
} compile_error_t;

/* ===== Compile Result ===== */
typedef struct {
    int             status;
    int             error_count;
    int             warning_count;
    int             compile_time_ms;
    unsigned int    wasm_size;
    compile_error_t errors[MAX_ERRORS];
} compile_result_t;

/* ===== Output Buffer ===== */
typedef struct {
    char    data[MAX_OUTPUT];
    int     stdout_len;
    int     stderr_len;
    int     stdout_offset;
    int     stderr_offset;
    int     exit_code;
    int     run_time_ms;
    int     run_status;
    int     memory_used;
    int     memory_peak;
} output_buf_t;

/* ===== WASM Module ===== */
typedef struct {
    int             active;
    char            name[MAX_MODULE_NAME];
    unsigned int    wasm_ptr;
    unsigned int    wasm_size;
    unsigned int    mem_pages;
    int             last_compile_status;
    int             last_run_status;
    int             last_exit_code;
    int             run_count;
    int             created_at;
} wasm_module_t;

/* ===== Session (saved snippet) ===== */
typedef struct {
    int     active;
    char    name[MAX_SNIPPET_NAME];
    int     line_count;
    int     created_at;
    int     modified_at;
    int     auto_save;
} session_t;

/* ===== History Entry ===== */
typedef struct {
    int     active;
    char    action[64];
    int     timestamp;
    int     details;
} history_entry_t;

/* ===== Template ===== */
typedef struct {
    char    name[MAX_TEMPLATE_NAME];
    char    description[128];
    int     line_count;
    int     category;       /* 0=basic, 1=algo, 2=system, 3=graphics */
} template_t;

/* ===== Code Editor ===== */
typedef struct {
    editor_line_t   lines[MAX_LINES];
    int             line_count;
    int             cursor_line;
    int             cursor_col;
    int             scroll_top;
    int             scroll_left;
    int             modified;
    int             total_edits;
    char            filename[MAX_MODULE_NAME];
} code_editor_t;

/* ===== Global State ===== */
static code_editor_t   editor;
static wasm_module_t   modules[MAX_MODULES];
static session_t       sessions[MAX_SESSIONS];
static history_entry_t history[MAX_HISTORY];
static template_t      templates[MAX_TEMPLATES];
static output_buf_t    output;
static compile_result_t last_compile;

static int module_count   = 0;
static int session_count  = 0;
static int history_count  = 0;
static int template_count = 0;
static int clock          = 0;

/* ===== String Utilities ===== */
static int my_strlen(const char *s) {
    int l = 0; while (s[l]) l++; return l;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; }
    return *a - *b;
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static void my_memset(void *dst, int val, int n) {
    char *d = (char *)dst;
    for (int i = 0; i < n; i++) d[i] = (char)val;
}

/* ===== Print Helpers ===== */
static void print_str(const char *s) { host_print(s); }

static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) { buf[pos++] = '0'; }
    else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

static void print_hex(unsigned int val) {
    char buf[12];
    buf[0] = '0'; buf[1] = 'x';
    const char hex[] = "0123456789abcdef";
    for (int i = 9; i >= 0; i--) { buf[2 + i] = hex[val & 0xf]; val >>= 4; }
    buf[12 - 2] = '\0';
    host_print(buf);
}

/* ===== History ===== */

static void history_add(const char *action, int details) {
    if (history_count >= MAX_HISTORY) {
        /* Shift everything down by one */
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history[i] = history[i + 1];
        }
        history_count = MAX_HISTORY - 1;
    }
    int idx = history_count++;
    history[idx].active = 1;
    my_strncpy(history[idx].action, action, 63);
    history[idx].timestamp = ++clock;
    history[idx].details = details;
}

/* ===== Editor Operations ===== */

static void editor_init(void) {
    my_memset(&editor, 0, sizeof(editor));
    editor.line_count = 1;
    editor.lines[0].text[0] = '\0';
    editor.lines[0].length = 0;
    my_strncpy(editor.filename, "untitled.c", MAX_MODULE_NAME - 1);
    history_add("editor_init", 0);
}

static int editor_insert_line(int at, const char *text) {
    if (at < 0 || at > editor.line_count || editor.line_count >= MAX_LINES) return -1;
    /* Shift lines down */
    for (int i = editor.line_count; i > at; i--) {
        editor.lines[i] = editor.lines[i - 1];
    }
    my_strncpy(editor.lines[at].text, text, MAX_LINE_LEN - 1);
    editor.lines[at].length = my_strlen(editor.lines[at].text);
    editor.lines[at].dirty = 1;
    editor.lines[at].breakpoint = 0;
    editor.lines[at].folded = 0;
    editor.lines[at].highlight_count = 0;
    editor.line_count++;
    editor.modified = 1;
    editor.total_edits++;
    return 0;
}

static int editor_delete_line(int at) {
    if (at < 0 || at >= editor.line_count || editor.line_count <= 1) return -1;
    for (int i = at; i < editor.line_count - 1; i++) {
        editor.lines[i] = editor.lines[i + 1];
    }
    editor.line_count--;
    editor.modified = 1;
    editor.total_edits++;
    return 0;
}

static int editor_replace_line(int at, const char *text) {
    if (at < 0 || at >= editor.line_count) return -1;
    my_strncpy(editor.lines[at].text, text, MAX_LINE_LEN - 1);
    editor.lines[at].length = my_strlen(editor.lines[at].text);
    editor.lines[at].dirty = 1;
    editor.modified = 1;
    editor.total_edits++;
    return 0;
}

static int editor_set_breakpoint(int line, int enable) {
    if (line < 0 || line >= editor.line_count) return -1;
    editor.lines[line].breakpoint = enable;
    return 0;
}

static int editor_move_cursor(int line, int col) {
    if (line < 0 || line >= editor.line_count) return -1;
    if (col < 0) col = 0;
    if (col > editor.lines[line].length) col = editor.lines[line].length;
    editor.cursor_line = line;
    editor.cursor_col = col;
    return 0;
}

/* ===== Syntax Highlighting Simulation ===== */

static int is_keyword(const char *word) {
    const char *keywords[] = {
        "if", "else", "for", "while", "return", "int", "void",
        "char", "const", "static", "struct", "typedef", "enum",
        "switch", "case", "break", "continue", "do"
    };
    for (int i = 0; i < 18; i++) {
        if (my_strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

static int is_type(const char *word) {
    const char *types[] = {
        "int", "char", "void", "float", "double", "long",
        "short", "unsigned", "signed"
    };
    for (int i = 0; i < 9; i++) {
        if (my_strcmp(word, types[i]) == 0) return 1;
    }
    return 0;
}

static void editor_highlight_line(int line) {
    if (line < 0 || line >= editor.line_count) return;
    editor_line_t *ln = &editor.lines[line];
    ln->highlight_count = 0;

    int i = 0;
    int len = ln->length;
    while (i < len && ln->highlight_count < 8) {
        /* Skip whitespace */
        while (i < len && (ln->text[i] == ' ' || ln->text[i] == '\t')) i++;
        if (i >= len) break;

        char c = ln->text[i];

        /* Comment: // */
        if (c == '/' && i + 1 < len && ln->text[i + 1] == '/') {
            highlight_t *h = &ln->highlights[ln->highlight_count++];
            h->line = line; h->col_start = i; h->col_end = len;
            h->token_type = TOK_COMMENT;
            break;
        }

        /* String literal */
        if (c == '"') {
            int start = i;
            i++;
            while (i < len && ln->text[i] != '"') {
                if (ln->text[i] == '\\' && i + 1 < len) i++;
                i++;
            }
            if (i < len) i++;
            highlight_t *h = &ln->highlights[ln->highlight_count++];
            h->line = line; h->col_start = start; h->col_end = i;
            h->token_type = TOK_STRING;
            continue;
        }

        /* Number */
        if (c >= '0' && c <= '9') {
            int start = i;
            while (i < len && ((ln->text[i] >= '0' && ln->text[i] <= '9') ||
                   ln->text[i] == 'x' || ln->text[i] == 'X' ||
                   (ln->text[i] >= 'a' && ln->text[i] <= 'f'))) i++;
            highlight_t *h = &ln->highlights[ln->highlight_count++];
            h->line = line; h->col_start = start; h->col_end = i;
            h->token_type = TOK_NUMBER;
            continue;
        }

        /* Identifier / keyword */
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            int start = i;
            char word[64];
            int wl = 0;
            while (i < len && ((ln->text[i] >= 'a' && ln->text[i] <= 'z') ||
                   (ln->text[i] >= 'A' && ln->text[i] <= 'Z') ||
                   (ln->text[i] >= '0' && ln->text[i] <= '9') ||
                   ln->text[i] == '_') && wl < 63) {
                word[wl++] = ln->text[i++];
            }
            word[wl] = '\0';
            int tok = TOK_NONE;
            if (is_type(word)) tok = TOK_TYPE;
            else if (is_keyword(word)) tok = TOK_KEYWORD;
            else if (i < len && ln->text[i] == '(') tok = TOK_FUNCTION;
            if (tok != TOK_NONE && ln->highlight_count < 8) {
                highlight_t *h = &ln->highlights[ln->highlight_count++];
                h->line = line; h->col_start = start; h->col_end = i;
                h->token_type = tok;
            }
            continue;
        }

        /* Preprocessor */
        if (c == '#') {
            highlight_t *h = &ln->highlights[ln->highlight_count++];
            h->line = line; h->col_start = i; h->col_end = len;
            h->token_type = TOK_PREPROCESSOR;
            break;
        }

        /* Operators */
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
            c == '<' || c == '>' || c == '!' || c == '&' || c == '|') {
            highlight_t *h = &ln->highlights[ln->highlight_count++];
            h->line = line; h->col_start = i; h->col_end = i + 1;
            h->token_type = TOK_OPERATOR;
            i++;
            continue;
        }

        i++;
    }
}

static void editor_highlight_all(void) {
    for (int i = 0; i < editor.line_count; i++) {
        editor_highlight_line(i);
    }
}

static const char *token_name(int tok) {
    switch (tok) {
        case TOK_KEYWORD:      return "KEYWORD";
        case TOK_STRING:       return "STRING";
        case TOK_COMMENT:      return "COMMENT";
        case TOK_NUMBER:       return "NUMBER";
        case TOK_TYPE:         return "TYPE";
        case TOK_FUNCTION:     return "FUNC";
        case TOK_OPERATOR:     return "OP";
        case TOK_PREPROCESSOR: return "PREPROC";
        default:               return "NONE";
    }
}

/* ===== Compilation Preview ===== */

static void compile_reset(void) {
    last_compile.status = COMPILE_IDLE;
    last_compile.error_count = 0;
    last_compile.warning_count = 0;
    last_compile.compile_time_ms = 0;
    last_compile.wasm_size = 0;
}

static int simulate_compile(void) {
    last_compile.status = COMPILE_PENDING;
    last_compile.error_count = 0;
    last_compile.warning_count = 0;

    int has_main = 0;
    int has_include_err = 0;
    int brace_balance = 0;

    for (int i = 0; i < editor.line_count; i++) {
        const char *t = editor.lines[i].text;

        /* Check for _start or main */
        for (int j = 0; t[j] && t[j + 1]; j++) {
            if ((t[j] == 'm' && t[j+1] == 'a' && t[j+2] == 'i' && t[j+3] == 'n') ||
                (t[j] == '_' && t[j+1] == 's' && t[j+2] == 't' && t[j+3] == 'a')) {
                has_main = 1;
            }
        }

        /* Brace balance */
        for (int j = 0; t[j]; j++) {
            if (t[j] == '{') brace_balance++;
            if (t[j] == '}') brace_balance--;
        }

        /* Check for obviously bad include */
        if (t[0] == '#') {
            int sl = my_strlen(t);
            if (sl > 10 && t[sl - 1] != '>' && t[sl - 1] != '"') {
                has_include_err = 1;
                int eidx = last_compile.error_count;
                if (eidx < MAX_ERRORS) {
                    last_compile.errors[eidx].line = i;
                    last_compile.errors[eidx].col = sl - 1;
                    last_compile.errors[eidx].severity = COMPILE_ERROR;
                    my_strncpy(last_compile.errors[eidx].message,
                               "malformed #include directive", 127);
                    last_compile.error_count++;
                }
            }
        }
    }

    if (!has_main) {
        int eidx = last_compile.error_count;
        if (eidx < MAX_ERRORS) {
            last_compile.errors[eidx].line = 0;
            last_compile.errors[eidx].col = 0;
            last_compile.errors[eidx].severity = COMPILE_WARNING;
            my_strncpy(last_compile.errors[eidx].message,
                       "no _start or main entry point found", 127);
            last_compile.warning_count++;
        }
    }

    if (brace_balance != 0) {
        int eidx = last_compile.error_count;
        if (eidx < MAX_ERRORS) {
            last_compile.errors[eidx].line = editor.line_count - 1;
            last_compile.errors[eidx].col = 0;
            last_compile.errors[eidx].severity = COMPILE_ERROR;
            my_strncpy(last_compile.errors[eidx].message,
                       "unbalanced braces", 127);
            last_compile.error_count++;
        }
    }

    if (has_include_err) {
        last_compile.status = COMPILE_ERROR;
    } else if (last_compile.error_count > 0) {
        last_compile.status = COMPILE_ERROR;
    } else if (last_compile.warning_count > 0) {
        last_compile.status = COMPILE_WARNING;
        last_compile.wasm_size = 256 + (unsigned int)(editor.line_count * 12);
    } else {
        last_compile.status = COMPILE_SUCCESS;
        last_compile.wasm_size = 256 + (unsigned int)(editor.line_count * 16);
    }

    last_compile.compile_time_ms = editor.line_count * 2 + 5;
    history_add("compile", last_compile.status);
    return last_compile.status;
}

static void print_compile_result(void) {
    const char *status_str = "UNKNOWN";
    if (last_compile.status == COMPILE_IDLE)    status_str = "IDLE";
    if (last_compile.status == COMPILE_PENDING) status_str = "PENDING";
    if (last_compile.status == COMPILE_SUCCESS) status_str = "SUCCESS";
    if (last_compile.status == COMPILE_ERROR)   status_str = "ERROR";
    if (last_compile.status == COMPILE_WARNING) status_str = "WARNING";

    print_str("  Status: ");    print_str(status_str); print_str("\n");
    print_str("  Errors: ");    print_int(last_compile.error_count); print_str("\n");
    print_str("  Warnings: ");  print_int(last_compile.warning_count); print_str("\n");
    print_str("  Time (ms): "); print_int(last_compile.compile_time_ms); print_str("\n");
    print_str("  WASM size: "); print_int((int)last_compile.wasm_size); print_str(" bytes\n");

    for (int i = 0; i < last_compile.error_count + last_compile.warning_count && i < MAX_ERRORS; i++) {
        print_str("    [line "); print_int(last_compile.errors[i].line);
        print_str(":"); print_int(last_compile.errors[i].col);
        print_str("] ");
        print_str(last_compile.errors[i].severity == COMPILE_ERROR ? "error" : "warn");
        print_str(": "); print_str(last_compile.errors[i].message);
        print_str("\n");
    }
}

/* ===== Output Capture ===== */

static void output_reset(void) {
    my_memset(&output, 0, sizeof(output));
    output.run_status = RUN_IDLE;
}

static void output_write_stdout(const char *s) {
    int len = my_strlen(s);
    int space = MAX_OUTPUT - output.stdout_offset - 1;
    if (len > space) len = space;
    for (int i = 0; i < len; i++) {
        output.data[output.stdout_offset++] = s[i];
    }
    output.data[output.stdout_offset] = '\0';
    output.stdout_len = output.stdout_offset;
}

static void output_write_stderr(const char *s) {
    int len = my_strlen(s);
    int space = MAX_OUTPUT - output.stderr_offset - 1;
    if (len > space) len = space;
    int base = MAX_OUTPUT / 2;
    for (int i = 0; i < len; i++) {
        output.data[base + output.stderr_offset++] = s[i];
    }
    output.stderr_len = output.stderr_offset;
}

/* ===== Module Runner ===== */

static int module_create(const char *name) {
    if (module_count >= MAX_MODULES) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_MODULES; i++) {
        if (!modules[i].active) { idx = i; break; }
    }
    if (idx < 0) return -1;

    modules[idx].active = 1;
    my_strncpy(modules[idx].name, name, MAX_MODULE_NAME - 1);
    modules[idx].wasm_ptr = 0;
    modules[idx].wasm_size = last_compile.wasm_size;
    modules[idx].mem_pages = 1;
    modules[idx].last_compile_status = last_compile.status;
    modules[idx].last_run_status = RUN_IDLE;
    modules[idx].last_exit_code = 0;
    modules[idx].run_count = 0;
    modules[idx].created_at = ++clock;
    module_count++;

    char msg[80];
    my_strncpy(msg, "module_create:", 79);
    history_add(msg, idx);
    return idx;
}

static int module_run(int idx) {
    if (idx < 0 || idx >= MAX_MODULES || !modules[idx].active) return -1;
    if (modules[idx].last_compile_status == COMPILE_ERROR) return -2;

    modules[idx].last_run_status = RUN_RUNNING;
    output_reset();
    output.run_status = RUN_RUNNING;

    /* Simulate execution based on line count */
    int lines = editor.line_count;
    if (lines < 3) {
        /* Too short, simulate crash */
        modules[idx].last_run_status = RUN_CRASHED;
        modules[idx].last_exit_code = 139;
        output.exit_code = 139;
        output.run_status = RUN_CRASHED;
        output_write_stderr("segfault: null pointer dereference at 0x0\n");
    } else {
        /* Simulate successful run */
        output_write_stdout("Hello from WASM playground!\n");
        output_write_stdout("Executed ");
        char lc[16];
        int p = 0;
        int v = lines;
        if (v == 0) lc[p++] = '0';
        else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; lc[p] = '\0'; p--;
            while (v > 0) { lc[p--] = '0' + (v % 10); v /= 10; } }
        output_write_stdout(lc);
        output_write_stdout(" lines of code\n");
        modules[idx].last_run_status = RUN_DONE;
        modules[idx].last_exit_code = 0;
        output.exit_code = 0;
        output.run_status = RUN_DONE;
    }

    output.run_time_ms = lines * 3 + 10;
    output.memory_used = (int)modules[idx].wasm_size + lines * 8;
    output.memory_peak = output.memory_used + 256;
    modules[idx].run_count++;
    history_add("module_run", modules[idx].last_run_status);
    return modules[idx].last_exit_code;
}

static void print_output(void) {
    print_str("  Exit code: "); print_int(output.exit_code); print_str("\n");
    print_str("  Run time: "); print_int(output.run_time_ms); print_str(" ms\n");
    print_str("  Memory used: "); print_int(output.memory_used); print_str(" bytes\n");
    print_str("  Memory peak: "); print_int(output.memory_peak); print_str(" bytes\n");
    print_str("  --- stdout ---\n");
    if (output.stdout_len > 0) {
        /* Null terminate for printing */
        output.data[output.stdout_len] = '\0';
        print_str("  "); print_str(output.data);
    } else {
        print_str("  (empty)\n");
    }
    print_str("  --- stderr ---\n");
    if (output.stderr_len > 0) {
        int base = MAX_OUTPUT / 2;
        output.data[base + output.stderr_len] = '\0';
        print_str("  "); print_str(&output.data[base]);
    } else {
        print_str("  (empty)\n");
    }
}

/* ===== Session Management ===== */

static int session_save(const char *name) {
    if (session_count >= MAX_SESSIONS) return -1;
    int idx = -1;
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (!sessions[i].active) { idx = i; break; }
    }
    if (idx < 0) return -1;

    sessions[idx].active = 1;
    my_strncpy(sessions[idx].name, name, MAX_SNIPPET_NAME - 1);
    sessions[idx].line_count = editor.line_count;
    sessions[idx].created_at = ++clock;
    sessions[idx].modified_at = clock;
    sessions[idx].auto_save = 0;
    session_count++;

    editor.modified = 0;
    history_add("session_save", idx);
    return idx;
}

static int session_load(int idx) {
    if (idx < 0 || idx >= MAX_SESSIONS || !sessions[idx].active) return -1;

    /* Simulate load by restoring line count */
    editor_init();
    int lc = sessions[idx].line_count;
    if (lc > MAX_LINES) lc = MAX_LINES;
    for (int i = 1; i < lc; i++) {
        editor_insert_line(i, "// restored line");
    }
    my_strncpy(editor.filename, sessions[idx].name, MAX_MODULE_NAME - 1);
    editor.modified = 0;
    history_add("session_load", idx);
    return 0;
}

static void session_list(void) {
    print_str("  Saved Sessions:\n");
    int shown = 0;
    for (int i = 0; i < MAX_SESSIONS; i++) {
        if (sessions[i].active) {
            print_str("    ["); print_int(i); print_str("] ");
            print_str(sessions[i].name);
            print_str(" ("); print_int(sessions[i].line_count); print_str(" lines");
            if (sessions[i].auto_save) print_str(", auto");
            print_str(")\n");
            shown++;
        }
    }
    if (shown == 0) print_str("    (none)\n");
}

/* ===== Template Library ===== */

static void template_init(void) {
    /* Basic template */
    my_strncpy(templates[template_count].name, "hello_wasm", MAX_TEMPLATE_NAME - 1);
    my_strncpy(templates[template_count].description,
               "Minimal WASM hello world", 127);
    templates[template_count].line_count = 5;
    templates[template_count].category = 0;
    template_count++;

    my_strncpy(templates[template_count].name, "fibonacci", MAX_TEMPLATE_NAME - 1);
    my_strncpy(templates[template_count].description,
               "Recursive fibonacci computation", 127);
    templates[template_count].line_count = 12;
    templates[template_count].category = 1;
    template_count++;

    my_strncpy(templates[template_count].name, "memory_ops", MAX_TEMPLATE_NAME - 1);
    my_strncpy(templates[template_count].description,
               "Linear memory read/write operations", 127);
    templates[template_count].line_count = 15;
    templates[template_count].category = 2;
    template_count++;

    my_strncpy(templates[template_count].name, "sort_array", MAX_TEMPLATE_NAME - 1);
    my_strncpy(templates[template_count].description,
               "Bubble sort implementation", 127);
    templates[template_count].line_count = 20;
    templates[template_count].category = 1;
    template_count++;

    my_strncpy(templates[template_count].name, "canvas_draw", MAX_TEMPLATE_NAME - 1);
    my_strncpy(templates[template_count].description,
               "Pixel drawing to framebuffer", 127);
    templates[template_count].line_count = 18;
    templates[template_count].category = 3;
    template_count++;
}

static int template_apply(int idx) {
    if (idx < 0 || idx >= template_count) return -1;

    editor_init();
    my_strncpy(editor.filename, templates[idx].name, MAX_MODULE_NAME - 1);

    /* Insert template content based on category */
    if (templates[idx].category == 0) {
        editor_insert_line(0, "/* hello_wasm */");
        editor_insert_line(1, "void _start(void) {");
        editor_insert_line(2, "    host_print(\"hello\");");
        editor_insert_line(3, "}");
    } else if (templates[idx].category == 1) {
        editor_insert_line(0, "/* algorithm template */");
        editor_insert_line(1, "int compute(int n) {");
        editor_insert_line(2, "    if (n <= 1) return n;");
        editor_insert_line(3, "    return compute(n - 1) + compute(n - 2);");
        editor_insert_line(4, "}");
        editor_insert_line(5, "void _start(void) {");
        editor_insert_line(6, "    int result = compute(10);");
        editor_insert_line(7, "}");
    } else if (templates[idx].category == 2) {
        editor_insert_line(0, "/* memory operations */");
        editor_insert_line(1, "void _start(void) {");
        editor_insert_line(2, "    unsigned int ptr = host_alloc(64, 8);");
        editor_insert_line(3, "    char *buf = (char *)ptr;");
        editor_insert_line(4, "    buf[0] = 'H';");
        editor_insert_line(5, "    buf[1] = 'i';");
        editor_insert_line(6, "    buf[2] = '\\0';");
        editor_insert_line(7, "    host_print(buf);");
        editor_insert_line(8, "}");
    } else {
        editor_insert_line(0, "/* graphics template */");
        editor_insert_line(1, "void draw_pixel(int x, int y, int color) {");
        editor_insert_line(2, "    unsigned int fb = 0x8000;");
        editor_insert_line(3, "    int *pixel = (int *)(fb + (y * 320 + x) * 4);");
        editor_insert_line(4, "    *pixel = color;");
        editor_insert_line(5, "}");
        editor_insert_line(6, "void _start(void) {");
        editor_insert_line(7, "    for (int i = 0; i < 100; i++) {");
        editor_insert_line(8, "        draw_pixel(i, i, 0xFF0000);");
        editor_insert_line(9, "    }");
        editor_insert_line(10, "}");
    }

    editor.modified = 0;
    history_add("template_apply", idx);
    return 0;
}

static void template_list(void) {
    print_str("  Template Library:\n");
    for (int i = 0; i < template_count; i++) {
        const char *cat = "basic";
        if (templates[i].category == 1) cat = "algo";
        else if (templates[i].category == 2) cat = "system";
        else if (templates[i].category == 3) cat = "graphics";

        print_str("    ["); print_int(i); print_str("] ");
        print_str(templates[i].name);
        print_str(" ("); print_str(cat); print_str(") - ");
        print_str(templates[i].description);
        print_str("\n");
    }
}

/* ===== Editor Display ===== */

static void editor_display(void) {
    print_str("  File: "); print_str(editor.filename);
    print_str(" ("); print_int(editor.line_count); print_str(" lines");
    if (editor.modified) print_str(", modified");
    print_str(")\n");
    print_str("  Cursor: "); print_int(editor.cursor_line);
    print_str(":"); print_int(editor.cursor_col);
    print_str("\n");

    int start = editor.scroll_top;
    int end = start + 10;
    if (end > editor.line_count) end = editor.line_count;

    for (int i = start; i < end; i++) {
        print_str("  ");
        if (i == editor.cursor_line) print_str("> ");
        else print_str("  ");

        /* Line number */
        char num[8];
        int p = 0;
        int v = i + 1;
        if (v == 0) num[p++] = '0';
        else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; num[p] = '\0'; p--;
            while (v > 0) { num[p--] = '0' + (v % 10); v /= 10; } }
        print_str(num); print_str(" | ");

        /* Breakpoint marker */
        if (editor.lines[i].breakpoint) print_str("* ");

        /* Highlight markers */
        if (editor.lines[i].highlight_count > 0) {
            print_str("[");
            print_str(token_name(editor.lines[i].highlights[0].token_type));
            print_str("] ");
        }

        print_str(editor.lines[i].text);
        print_str("\n");
    }
}

/* ===== History Display ===== */

static void history_display(void) {
    print_str("  Action History:\n");
    int start = history_count - 10;
    if (start < 0) start = 0;
    for (int i = start; i < history_count; i++) {
        print_str("    ["); print_int(history[i].timestamp); print_str("] ");
        print_str(history[i].action);
        print_str(" (details="); print_int(history[i].details); print_str(")\n");
    }
}

/* ===== CLI Entry ===== */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);

    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("PlaygroundWASM v46.0 - WASM Playground Environment\n");

    if (help) {
        print_str("Usage: playground_wasm [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run feature test suite\n");
        print_str("\nFeatures:\n");
        print_str("  Code editor with syntax highlighting\n");
        print_str("  Live compilation preview\n");
        print_str("  Module runner (sandboxed execution)\n");
        print_str("  Output capture (stdout/stderr)\n");
        print_str("  Session save/load\n");
        print_str("  Template library\n");
        return;
    }

    if (test) {
        print_str("=== PlaygroundWASM Test Suite ===\n\n");

        /* Init */
        editor_init();
        template_init();
        compile_reset();
        output_reset();

        /* Test 1: Editor operations */
        print_str("[Test 1] Editor Operations\n");
        editor_insert_line(0, "/* test program */");
        editor_insert_line(1, "void _start(void) {");
        editor_insert_line(2, "    int x = 42;");
        editor_insert_line(3, "    host_print(\"hello\");");
        editor_insert_line(4, "}");
        editor_move_cursor(2, 8);
        editor_set_breakpoint(2, 1);
        editor_display();
        print_str("  Total edits: "); print_int(editor.total_edits); print_str("\n\n");

        /* Test 2: Syntax highlighting */
        print_str("[Test 2] Syntax Highlighting\n");
        editor_highlight_all();
        for (int i = 0; i < editor.line_count; i++) {
            if (editor.lines[i].highlight_count > 0) {
                print_str("  Line "); print_int(i); print_str(": ");
                print_int(editor.lines[i].highlight_count);
                print_str(" tokens (");
                for (int j = 0; j < editor.lines[i].highlight_count; j++) {
                    if (j > 0) print_str(", ");
                    print_str(token_name(editor.lines[i].highlights[j].token_type));
                }
                print_str(")\n");
            }
        }
        print_str("\n");

        /* Test 3: Compilation - success case */
        print_str("[Test 3] Compile (success)\n");
        int status = simulate_compile();
        print_str("  Result: "); print_int(status); print_str("\n");
        print_compile_result();
        print_str("\n");

        /* Test 4: Compilation - error cases */
        print_str("[Test 4] Compile (errors)\n");
        editor_insert_line(5, "#include <bad");
        editor_insert_line(6, "void unclosed() {");
        status = simulate_compile();
        print_str("  Result: "); print_int(status); print_str("\n");
        print_compile_result();
        editor_delete_line(6);
        editor_delete_line(5);
        print_str("\n");

        /* Test 5: Module creation and run */
        print_str("[Test 5] Module Run\n");
        int midx = module_create("test_module");
        print_str("  Created module: "); print_int(midx); print_str("\n");
        int rc = module_run(midx);
        print_str("  Run exit code: "); print_int(rc); print_str("\n");
        print_output();
        print_str("\n");

        /* Test 6: Module crash simulation */
        print_str("[Test 6] Module Crash\n");
        editor_delete_line(4);
        editor_delete_line(3);
        midx = module_create("crash_test");
        rc = module_run(midx);
        print_str("  Crash exit code: "); print_int(rc); print_str("\n");
        print_output();
        print_str("\n");

        /* Test 7: Session management */
        print_str("[Test 7] Sessions\n");
        int s1 = session_save("my_snippet");
        int s2 = session_save("fib_draft");
        print_str("  Saved sessions: "); print_int(s1); print_str(", ");
        print_int(s2); print_str("\n");
        session_list();
        session_load(s1);
        print_str("  Loaded session "); print_int(s1);
        print_str(" ("); print_int(editor.line_count); print_str(" lines)\n\n");

        /* Test 8: Templates */
        print_str("[Test 8] Templates\n");
        template_list();
        print_str("\n  Applying 'fibonacci' template...\n");
        template_apply(1);
        print_str("  Applied. Lines: "); print_int(editor.line_count); print_str("\n");
        editor_display();
        print_str("\n");

        /* Test 9: Full pipeline */
        print_str("[Test 9] Full Pipeline (edit -> compile -> run)\n");
        template_apply(0);
        editor_insert_line(3, "    host_print(\"world\");");
        editor_highlight_all();
        status = simulate_compile();
        print_str("  Compile: "); print_int(status); print_str("\n");
        midx = module_create("pipeline_test");
        rc = module_run(midx);
        print_str("  Run: exit="); print_int(rc); print_str("\n");
        print_output();
        print_str("\n");

        /* Test 10: History */
        print_str("[Test 10] History\n");
        history_display();
        print_str("  Total actions: "); print_int(history_count); print_str("\n\n");

        print_str("=== PlaygroundWASM Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
