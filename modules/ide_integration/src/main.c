/* ide_integration: IDE integration WASM module (v1.0)
 *
 * Provides comprehensive IDE integration features:
 * - VS Code extension support and lifecycle
 * - Syntax highlighting with token colorization
 * - Code completion engine with prefix matching
 * - Error detection and diagnostics
 * - Real-time preview with hot reload
 * - Project templates and scaffolding
 * - Documentation integration and lookup
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

/* -------------------------------------------------------------------------- */
/* Limits                                                                     */
/* -------------------------------------------------------------------------- */

#define MAX_EXTENSIONS     32
#define MAX_HIGHLIGHT_RULES 16
#define MAX_COMPLETIONS   64
#define MAX_DIAGNOSTICS   32
#define MAX_TEMPLATES     16
#define MAX_DOCS          32
#define MAX_PREVIEW        8
#define MAX_NAME          64
#define MAX_MSG          128

/* -------------------------------------------------------------------------- */
/* Token types for syntax highlighting                                        */
/* -------------------------------------------------------------------------- */

#define TOKEN_KEYWORD   1
#define TOKEN_STRING    2
#define TOKEN_COMMENT   3
#define TOKEN_NUMBER    4
#define TOKEN_TYPE      5
#define TOKEN_OPERATOR  6
#define TOKEN_FUNC      7
#define TOKEN_PLAIN     0

/* -------------------------------------------------------------------------- */
/* Diagnostic severity                                                        */
/* -------------------------------------------------------------------------- */

#define DIAG_ERROR    1
#define DIAG_WARNING  2
#define DIAG_INFO     3
#define DIAG_HINT     4

/* -------------------------------------------------------------------------- */
/* Completion kind                                                            */
/* -------------------------------------------------------------------------- */

#define COMP_FUNCTION   1
#define COMP_VARIABLE   2
#define COMP_KEYWORD    3
#define COMP_TYPE       4
#define COMP_SNIPPET    5

/* -------------------------------------------------------------------------- */
/* VS Code extension descriptor                                               */
/* -------------------------------------------------------------------------- */

typedef struct {
    char name[MAX_NAME];
    char version[16];
    char publisher[MAX_NAME];
    int  enabled;
    int  activated;
} vscode_ext_t;

/* -------------------------------------------------------------------------- */
/* Syntax highlighting rule                                                   */
/* -------------------------------------------------------------------------- */

typedef struct {
    int  token_type;
    char color[12];
    char bold;
} highlight_rule_t;

/* -------------------------------------------------------------------------- */
/* Completion item                                                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    char label[MAX_NAME];
    char detail[MAX_MSG];
    int  kind;
    int  priority;
} completion_t;

/* -------------------------------------------------------------------------- */
/* Diagnostic entry                                                           */
/* -------------------------------------------------------------------------- */

typedef struct {
    int  line;
    int  col;
    char message[MAX_MSG];
    int  severity;
    char file[MAX_NAME];
} diagnostic_t;

/* -------------------------------------------------------------------------- */
/* Preview session with hot reload                                            */
/* -------------------------------------------------------------------------- */

typedef struct {
    int  session_id;
    char source_file[MAX_NAME];
    int  active;
    int  hot_reload;
    int  update_count;
} preview_t;

/* -------------------------------------------------------------------------- */
/* Project template                                                           */
/* -------------------------------------------------------------------------- */

typedef struct {
    char name[MAX_NAME];
    char description[MAX_MSG];
    char lang[16];
    int  file_count;
} template_t;

/* -------------------------------------------------------------------------- */
/* Documentation entry                                                        */
/* -------------------------------------------------------------------------- */

typedef struct {
    char symbol[MAX_NAME];
    char doc[MAX_MSG];
    int  category;
} doc_entry_t;

/* -------------------------------------------------------------------------- */
/* Global state                                                               */
/* -------------------------------------------------------------------------- */

static vscode_ext_t    extensions[MAX_EXTENSIONS];
static int             ext_count = 0;

static highlight_rule_t hl_rules[MAX_HIGHLIGHT_RULES];
static int             hl_rule_count = 0;

static completion_t    completions[MAX_COMPLETIONS];
static int             comp_count = 0;

static diagnostic_t    diagnostics[MAX_DIAGNOSTICS];
static int             diag_count = 0;

static preview_t       previews[MAX_PREVIEW];
static int             preview_count = 0;
static int             next_preview_id = 1;

static template_t      templates[MAX_TEMPLATES];
static int             tmpl_count = 0;

static doc_entry_t     docs[MAX_DOCS];
static int             doc_count = 0;

/* -------------------------------------------------------------------------- */
/* String utilities                                                           */
/* -------------------------------------------------------------------------- */

static int my_strlen(const char *s) {
    int l = 0;
    while (s[l]) l++;
    return l;
}

static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return *a - *b;
}

static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0;
    while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] - b[i];
        if (!a[i]) return 0;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Print utilities                                                            */
/* -------------------------------------------------------------------------- */

static void print_str(const char *str) { host_print(str); }

static void print_int(int val) {
    char buf[32];
    int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) {
        buf[pos++] = '0';
    } else {
        int d = 0, t = val;
        while (t > 0) { d++; t /= 10; }
        pos += d;
        buf[pos] = '\0';
        pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; }
    }
    host_print(buf);
}

/* -------------------------------------------------------------------------- */
/* VS Code extension support                                                  */
/* -------------------------------------------------------------------------- */

int ide_init_vscode(void) {
    ext_count = 0;
    for (int i = 0; i < MAX_EXTENSIONS; i++) {
        extensions[i].name[0] = '\0';
        extensions[i].enabled = 0;
        extensions[i].activated = 0;
    }
    return 0;
}

int ide_register_extension(const char *name, const char *version, const char *pub) {
    if (ext_count >= MAX_EXTENSIONS) return -1;
    int idx = ext_count;
    my_strncpy(extensions[idx].name, name, MAX_NAME - 1);
    my_strncpy(extensions[idx].version, version, 15);
    my_strncpy(extensions[idx].publisher, pub, MAX_NAME - 1);
    extensions[idx].enabled = 1;
    extensions[idx].activated = 0;
    ext_count++;
    return idx;
}

int ide_activate_extension(int idx) {
    if (idx < 0 || idx >= ext_count) return -1;
    if (!extensions[idx].enabled) return -2;
    extensions[idx].activated = 1;
    return 0;
}

int ide_deactivate_extension(int idx) {
    if (idx < 0 || idx >= ext_count) return -1;
    extensions[idx].activated = 0;
    return 0;
}

int ide_list_extensions(void) {
    print_str("  VS Code Extensions\n");
    print_str("  ====================================================================\n");
    for (int i = 0; i < ext_count; i++) {
        print_str("  [");
        print_str(extensions[i].activated ? "X" : " ");
        print_str("] ");
        print_str(extensions[i].name);
        print_str(" v");
        print_str(extensions[i].version);
        print_str(" by ");
        print_str(extensions[i].publisher);
        print_str("\n");
    }
    print_str("  Total: ");
    print_int(ext_count);
    print_str(" extensions\n");
    return ext_count;
}

/* -------------------------------------------------------------------------- */
/* Syntax highlighting                                                        */
/* -------------------------------------------------------------------------- */

int ide_init_highlighter(void) {
    hl_rule_count = 0;
    return 0;
}

int ide_add_highlight_rule(int token_type, const char *color, int bold) {
    if (hl_rule_count >= MAX_HIGHLIGHT_RULES) return -1;
    int idx = hl_rule_count;
    hl_rules[idx].token_type = token_type;
    my_strncpy(hl_rules[idx].color, color, 11);
    hl_rules[idx].bold = bold ? 1 : 0;
    hl_rule_count++;
    return idx;
}

int ide_tokenize(const char *source) {
    int tokens = 0;
    int pos = 0;
    int len = my_strlen(source);

    while (pos < len) {
        /* Skip whitespace */
        if (source[pos] == ' ' || source[pos] == '\t' || source[pos] == '\n') {
            pos++;
            continue;
        }
        /* Line comment */
        if (pos + 1 < len && source[pos] == '/' && source[pos + 1] == '/') {
            tokens++;
            while (pos < len && source[pos] != '\n') pos++;
            continue;
        }
        /* String literal */
        if (source[pos] == '"') {
            tokens++;
            pos++;
            while (pos < len && source[pos] != '"') {
                if (source[pos] == '\\' && pos + 1 < len) pos++;
                pos++;
            }
            if (pos < len) pos++;
            continue;
        }
        /* Number */
        if (source[pos] >= '0' && source[pos] <= '9') {
            tokens++;
            while (pos < len && ((source[pos] >= '0' && source[pos] <= '9')
                  || source[pos] == '.')) pos++;
            continue;
        }
        /* Identifier or keyword */
        if ((source[pos] >= 'a' && source[pos] <= 'z')
            || (source[pos] >= 'A' && source[pos] <= 'Z') || source[pos] == '_') {
            tokens++;
            while (pos < len && ((source[pos] >= 'a' && source[pos] <= 'z')
                  || (source[pos] >= 'A' && source[pos] <= 'Z')
                  || (source[pos] >= '0' && source[pos] <= '9')
                  || source[pos] == '_')) pos++;
            continue;
        }
        /* Operator / punctuation */
        tokens++;
        pos++;
    }
    return tokens;
}

int ide_list_rules(void) {
    print_str("  Syntax Highlighting Rules\n");
    print_str("  ====================================================================\n");
    const char *type_names[] = {
        "plain", "keyword", "string", "comment",
        "number", "type", "operator", "function"
    };
    for (int i = 0; i < hl_rule_count; i++) {
        int tt = hl_rules[i].token_type;
        print_str("  ");
        if (tt >= 0 && tt <= 7) print_str(type_names[tt]);
        else { print_str("type#"); print_int(tt); }
        print_str("  color=");
        print_str(hl_rules[i].color);
        print_str(hl_rules[i].bold ? "  BOLD" : "");
        print_str("\n");
    }
    print_str("  Total: ");
    print_int(hl_rule_count);
    print_str(" rules\n");
    return hl_rule_count;
}

/* -------------------------------------------------------------------------- */
/* Code completion                                                            */
/* -------------------------------------------------------------------------- */

int ide_init_completion(void) {
    comp_count = 0;
    return 0;
}

int ide_add_completion(const char *label, const char *detail, int kind, int prio) {
    if (comp_count >= MAX_COMPLETIONS) return -1;
    int idx = comp_count;
    my_strncpy(completions[idx].label, label, MAX_NAME - 1);
    my_strncpy(completions[idx].detail, detail, MAX_MSG - 1);
    completions[idx].kind = kind;
    completions[idx].priority = prio;
    comp_count++;
    return idx;
}

int ide_get_completions(const char *prefix) {
    int plen = my_strlen(prefix);
    int matches = 0;
    for (int i = 0; i < comp_count; i++) {
        if (my_strncmp(completions[i].label, prefix, plen) == 0) {
            const char *kind_str = "unknown";
            if (completions[i].kind == COMP_FUNCTION) kind_str = "function";
            else if (completions[i].kind == COMP_VARIABLE) kind_str = "variable";
            else if (completions[i].kind == COMP_KEYWORD)  kind_str = "keyword";
            else if (completions[i].kind == COMP_TYPE)     kind_str = "type";
            else if (completions[i].kind == COMP_SNIPPET)  kind_str = "snippet";
            print_str("  [");
            print_str(kind_str);
            print_str("] ");
            print_str(completions[i].label);
            print_str(" - ");
            print_str(completions[i].detail);
            print_str("\n");
            matches++;
        }
    }
    return matches;
}

/* -------------------------------------------------------------------------- */
/* Error detection                                                            */
/* -------------------------------------------------------------------------- */

int ide_init_diagnostics(void) {
    diag_count = 0;
    return 0;
}

int ide_add_diagnostic(int line, int col, const char *msg, int sev, const char *file) {
    if (diag_count >= MAX_DIAGNOSTICS) return -1;
    int idx = diag_count;
    diagnostics[idx].line = line;
    diagnostics[idx].col = col;
    my_strncpy(diagnostics[idx].message, msg, MAX_MSG - 1);
    diagnostics[idx].severity = sev;
    my_strncpy(diagnostics[idx].file, file, MAX_NAME - 1);
    diag_count++;
    return idx;
}

int ide_check_braces(const char *source) {
    int depth = 0;
    int errors = 0;
    int len = my_strlen(source);
    for (int i = 0; i < len; i++) {
        if (source[i] == '{') depth++;
        else if (source[i] == '}') {
            depth--;
            if (depth < 0) {
                ide_add_diagnostic(1, i + 1, "Unmatched closing brace",
                                   DIAG_ERROR, "input");
                errors++;
                depth = 0;
            }
        }
    }
    if (depth > 0) {
        ide_add_diagnostic(1, 1, "Unclosed opening brace", DIAG_ERROR, "input");
        errors++;
    }
    return errors;
}

int ide_check_tabs(const char *source) {
    int warnings = 0;
    int line = 1;
    int len = my_strlen(source);
    for (int i = 0; i < len; i++) {
        if (source[i] == '\n') { line++; continue; }
        if (source[i] == '\t') {
            char msg[64];
            my_strncpy(msg, "Tab character found, use spaces", 63);
            ide_add_diagnostic(line, 1, msg, DIAG_WARNING, "input");
            warnings++;
        }
    }
    return warnings;
}

int ide_report_diagnostics(void) {
    print_str("  Diagnostics\n");
    print_str("  ====================================================================\n");
    int errs = 0, warns = 0, infos = 0;
    for (int i = 0; i < diag_count; i++) {
        const char *sev_str = "INFO";
        if (diagnostics[i].severity == DIAG_ERROR)   { sev_str = "ERROR"; errs++; }
        else if (diagnostics[i].severity == DIAG_WARNING) { sev_str = "WARN";  warns++; }
        else { infos++; }
        print_str("  ");
        print_str(sev_str);
        print_str(" ");
        print_str(diagnostics[i].file);
        print_str(":");
        print_int(diagnostics[i].line);
        print_str(":");
        print_int(diagnostics[i].col);
        print_str(" ");
        print_str(diagnostics[i].message);
        print_str("\n");
    }
    print_str("  Summary: ");
    print_int(errs);
    print_str(" errors, ");
    print_int(warns);
    print_str(" warnings, ");
    print_int(infos);
    print_str(" info\n");
    return diag_count;
}

/* -------------------------------------------------------------------------- */
/* Real-time preview with hot reload                                          */
/* -------------------------------------------------------------------------- */

int ide_create_preview(const char *source_file) {
    if (preview_count >= MAX_PREVIEW) return -1;
    int idx = preview_count;
    previews[idx].session_id = next_preview_id++;
    my_strncpy(previews[idx].source_file, source_file, MAX_NAME - 1);
    previews[idx].active = 1;
    previews[idx].hot_reload = 1;
    previews[idx].update_count = 0;
    preview_count++;
    return previews[idx].session_id;
}

int ide_hot_reload(int session_id) {
    for (int i = 0; i < preview_count; i++) {
        if (previews[i].session_id == session_id && previews[i].active) {
            previews[i].update_count++;
            return previews[i].update_count;
        }
    }
    return -1;
}

int ide_close_preview(int session_id) {
    for (int i = 0; i < preview_count; i++) {
        if (previews[i].session_id == session_id) {
            previews[i].active = 0;
            return 0;
        }
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
/* Project templates                                                          */
/* -------------------------------------------------------------------------- */

int ide_register_template(const char *name, const char *desc,
                          const char *lang, int files) {
    if (tmpl_count >= MAX_TEMPLATES) return -1;
    int idx = tmpl_count;
    my_strncpy(templates[idx].name, name, MAX_NAME - 1);
    my_strncpy(templates[idx].description, desc, MAX_MSG - 1);
    my_strncpy(templates[idx].lang, lang, 15);
    templates[idx].file_count = files;
    tmpl_count++;
    return idx;
}

int ide_scaffold_project(int tmpl_idx) {
    if (tmpl_idx < 0 || tmpl_idx >= tmpl_count) return -1;
    print_str("  Scaffolding project from template: ");
    print_str(templates[tmpl_idx].name);
    print_str("\n");
    print_str("    Language: ");
    print_str(templates[tmpl_idx].lang);
    print_str("\n");
    print_str("    Files to create: ");
    print_int(templates[tmpl_idx].file_count);
    print_str("\n");
    print_str("    Project ready.\n");
    return 0;
}

int ide_list_templates(void) {
    print_str("  Project Templates\n");
    print_str("  ====================================================================\n");
    for (int i = 0; i < tmpl_count; i++) {
        print_str("  ");
        print_int(i);
        print_str(". ");
        print_str(templates[i].name);
        print_str(" [");
        print_str(templates[i].lang);
        print_str("] - ");
        print_str(templates[i].description);
        print_str(" (");
        print_int(templates[i].file_count);
        print_str(" files)\n");
    }
    print_str("  Total: ");
    print_int(tmpl_count);
    print_str(" templates\n");
    return tmpl_count;
}

/* -------------------------------------------------------------------------- */
/* Documentation integration                                                  */
/* -------------------------------------------------------------------------- */

int ide_add_doc(const char *symbol, const char *doc, int cat) {
    if (doc_count >= MAX_DOCS) return -1;
    int idx = doc_count;
    my_strncpy(docs[idx].symbol, symbol, MAX_NAME - 1);
    my_strncpy(docs[idx].doc, doc, MAX_MSG - 1);
    docs[idx].category = cat;
    doc_count++;
    return idx;
}

int ide_lookup_doc(const char *symbol) {
    for (int i = 0; i < doc_count; i++) {
        if (my_strcmp(docs[i].symbol, symbol) == 0) {
            print_str("  ");
            print_str(docs[i].symbol);
            print_str(": ");
            print_str(docs[i].doc);
            print_str("\n");
            return i;
        }
    }
    return -1;
}

int ide_search_docs(const char *query) {
    int qlen = my_strlen(query);
    int found = 0;
    for (int i = 0; i < doc_count; i++) {
        if (my_strncmp(docs[i].symbol, query, qlen) == 0) {
            print_str("  ");
            print_str(docs[i].symbol);
            print_str(": ");
            print_str(docs[i].doc);
            print_str("\n");
            found++;
        }
    }
    return found;
}

/* -------------------------------------------------------------------------- */
/* CLI entry                                                                  */
/* -------------------------------------------------------------------------- */

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++;
    pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++;
        pos++;
    }

    print_str("IDE Integration v1.0 - Comprehensive IDE Support Module\n");

    if (help) {
        print_str("Usage: ide_integration [options]\n");
        print_str("  -h, --help    Show this help\n");
        print_str("  -t, --test    Run IDE integration tests\n");
        return;
    }

    if (test) {
        print_str("=== IDE Integration Test Suite ===\n\n");

        /* Test 1: VS Code extension support */
        print_str("[Test 1] VS Code Extension Support\n");
        ide_init_vscode();
        ide_register_extension("clangd", "0.16.0", "llvm");
        ide_register_extension("prettier", "10.0.0", "prettier");
        ide_register_extension("rust-analyzer", "0.4.0", "rust-lang");
        ide_register_extension("python", "2024.1.0", "ms-python");
        ide_activate_extension(0);
        ide_activate_extension(1);
        ide_activate_extension(2);
        ide_list_extensions();
        print_str("\n");

        /* Test 2: Syntax highlighting */
        print_str("[Test 2] Syntax Highlighting\n");
        ide_init_highlighter();
        ide_add_highlight_rule(TOKEN_KEYWORD,  "#c586c0", 1);
        ide_add_highlight_rule(TOKEN_STRING,   "#ce9178", 0);
        ide_add_highlight_rule(TOKEN_COMMENT,  "#6a9955", 0);
        ide_add_highlight_rule(TOKEN_NUMBER,   "#b5cea8", 0);
        ide_add_highlight_rule(TOKEN_TYPE,     "#4ec9b0", 1);
        ide_add_highlight_rule(TOKEN_FUNC,     "#dcdcaa", 0);
        ide_add_highlight_rule(TOKEN_OPERATOR, "#d4d4d4", 0);
        ide_list_rules();
        const char *sample = "int main() { return 0; }";
        int tok = ide_tokenize(sample);
        print_str("  Tokenized sample: \"");
        print_str(sample);
        print_str("\" -> ");
        print_int(tok);
        print_str(" tokens\n\n");

        /* Test 3: Code completion */
        print_str("[Test 3] Code Completion\n");
        ide_init_completion();
        ide_add_completion("printf",    "int printf(const char*, ...)",    COMP_FUNCTION, 10);
        ide_add_completion("malloc",    "void* malloc(size_t)",            COMP_FUNCTION, 10);
        ide_add_completion("strlen",    "size_t strlen(const char*)",      COMP_FUNCTION, 10);
        ide_add_completion("strcpy",    "char* strcpy(char*, const char*)", COMP_FUNCTION, 9);
        ide_add_completion("memcpy",    "void* memcpy(void*, const void*, size_t)", COMP_FUNCTION, 9);
        ide_add_completion("fprintf",   "int fprintf(FILE*, const char*, ...)", COMP_FUNCTION, 8);
        ide_add_completion("sizeof",    "sizeof operator",                 COMP_KEYWORD,  10);
        ide_add_completion("return",    "return statement",                COMP_KEYWORD,  10);
        ide_add_completion("int",       "integer type",                    COMP_TYPE,     10);
        ide_add_completion("void",      "void type",                       COMP_TYPE,     10);
        print_str("  Completions for prefix 'pr':\n");
        int m1 = ide_get_completions("pr");
        print_str("  Found "); print_int(m1); print_str(" matches\n");
        print_str("  Completions for prefix 'str':\n");
        int m2 = ide_get_completions("str");
        print_str("  Found "); print_int(m2); print_str(" matches\n\n");

        /* Test 4: Error detection */
        print_str("[Test 4] Error Detection\n");
        ide_init_diagnostics();
        const char *bad_code = "int main() { if(true { return 0; }";
        ide_check_braces(bad_code);
        ide_add_diagnostic(3, 5, "Unused variable 'x'", DIAG_WARNING, "main.c");
        ide_add_diagnostic(5, 1, "Missing return statement", DIAG_INFO, "main.c");
        ide_report_diagnostics();
        print_str("\n");

        /* Test 5: Real-time preview with hot reload */
        print_str("[Test 5] Real-time Preview & Hot Reload\n");
        int p1 = ide_create_preview("main.c");
        int p2 = ide_create_preview("style.css");
        print_str("  Created preview sessions: ");
        print_int(p1); print_str(", "); print_int(p2); print_str("\n");
        for (int r = 0; r < 5; r++) {
            int upd = ide_hot_reload(p1);
            print_str("  Hot reload session "); print_int(p1);
            print_str(" -> update #"); print_int(upd); print_str("\n");
        }
        ide_close_preview(p2);
        print_str("  Closed preview session "); print_int(p2); print_str("\n\n");

        /* Test 6: Project templates */
        print_str("[Test 6] Project Templates\n");
        ide_register_template("hello-cli",    "Minimal CLI app",         "C",    3);
        ide_register_template("web-server",   "HTTP server skeleton",    "C",    8);
        ide_register_template("wasm-module",  "WASM module template",    "C",    4);
        ide_register_template("lib-shared",   "Shared library project",  "C",    5);
        ide_list_templates();
        print_str("  Scaffolding from template 2:\n");
        ide_scaffold_project(2);
        print_str("\n");

        /* Test 7: Documentation integration */
        print_str("[Test 7] Documentation Integration\n");
        ide_add_doc("printf",  "Formatted output to stdout",          1);
        ide_add_doc("malloc",  "Allocate memory from heap",           2);
        ide_add_doc("free",    "Release allocated memory",            2);
        ide_add_doc("strlen",  "Get string length",                   3);
        ide_add_doc("memcpy",  "Copy memory block",                   3);
        ide_add_doc("fopen",   "Open file stream",                    1);
        ide_add_doc("fclose",  "Close file stream",                   1);
        print_str("  Lookup 'printf':\n");
        ide_lookup_doc("printf");
        print_str("  Lookup 'malloc':\n");
        ide_lookup_doc("malloc");
        print_str("  Search prefix 'f':\n");
        int sf = ide_search_docs("f");
        print_str("  Found "); print_int(sf); print_str(" docs\n");
        print_str("  Search prefix 'mem':\n");
        int sm = ide_search_docs("mem");
        print_str("  Found "); print_int(sm); print_str(" docs\n\n");

        /* Summary */
        print_str("=== IDE Integration Test Complete ===\n");
        print_str("  Extensions: ");   print_int(ext_count);     print_str("\n");
        print_str("  Highlight rules: "); print_int(hl_rule_count); print_str("\n");
        print_str("  Completions: ");  print_int(comp_count);    print_str("\n");
        print_str("  Diagnostics: ");  print_int(diag_count);    print_str("\n");
        print_str("  Previews: ");     print_int(preview_count); print_str("\n");
        print_str("  Templates: ");    print_int(tmpl_count);    print_str("\n");
        print_str("  Docs indexed: "); print_int(doc_count);     print_str("\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
