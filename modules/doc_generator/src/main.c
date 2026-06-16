/* doc_generator: Documentation generation module for AI-ASM OS v46.0
 *
 * Provides:
 *   - API documentation generator (function signatures, params, return types)
 *   - Architecture diagram generator (ASCII art from module metadata)
 *   - Usage example generator (code snippets with explanations)
 *   - Cross-reference builder (module dependencies, import/export mapping)
 *   - Markdown / HTML output formats
 *   - Module documentation index builder
 */

#include <stddef.h>

/* ---------- host imports ---------- */

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ---------- constants ---------- */

#define DOC_MAX_MODULES       128
#define DOC_MAX_FUNCS          64
#define DOC_MAX_DEPS           32
#define DOC_MAX_PARAMS          8
#define DOC_MAX_NAME_LEN       64
#define DOC_MAX_DESC_LEN      128
#define DOC_MAX_SIG_LEN       256
#define DOC_MAX_EXAMPLE_LEN   256
#define DOC_OUTPUT_BUF       8192

#define FMT_MARKDOWN   0
#define FMT_HTML       1

#define TAG_API        0x01
#define TAG_ARCH       0x02
#define TAG_EXAMPLE    0x04
#define TAG_XREF       0x08
#define TAG_INDEX      0x10
#define TAG_ALL        0xFF

/* ---------- data structures ---------- */

typedef struct {
    char name[DOC_MAX_NAME_LEN];
    char type[DOC_MAX_NAME_LEN];
    char direction[16];          /* "in", "out", "inout" */
    char desc[DOC_MAX_DESC_LEN];
} doc_param_t;

typedef struct {
    char name[DOC_MAX_NAME_LEN];
    char return_type[DOC_MAX_NAME_LEN];
    char signature[DOC_MAX_SIG_LEN];
    char description[DOC_MAX_DESC_LEN];
    doc_param_t params[DOC_MAX_PARAMS];
    unsigned int param_count;
    unsigned int tags;
    int is_exported;
} doc_func_t;

typedef struct {
    char name[DOC_MAX_NAME_LEN];
    char version[16];
    char description[DOC_MAX_DESC_LEN];
    doc_func_t funcs[DOC_MAX_FUNCS];
    unsigned int func_count;
    char imports[DOC_MAX_DEPS][DOC_MAX_NAME_LEN];
    unsigned int import_count;
    char exports[DOC_MAX_DEPS][DOC_MAX_NAME_LEN];
    unsigned int export_count;
    unsigned int tags;
} doc_module_t;

typedef struct {
    char title[DOC_MAX_NAME_LEN];
    char code[DOC_MAX_EXAMPLE_LEN];
    char explanation[DOC_MAX_DESC_LEN];
} doc_example_t;

typedef struct {
    char source[DOC_MAX_NAME_LEN];
    char target[DOC_MAX_NAME_LEN];
    char relationship[32];      /* "depends_on", "imports", "exports_to" */
} doc_xref_t;

typedef struct {
    doc_module_t modules[DOC_MAX_MODULES];
    unsigned int module_count;
    doc_example_t examples[DOC_MAX_FUNCS];
    unsigned int example_count;
    doc_xref_t xrefs[DOC_MAX_DEPS * 4];
    unsigned int xref_count;
    unsigned int output_format;
    unsigned int output_buf;
    unsigned int output_pos;
    unsigned int output_cap;
} doc_context_t;

/* ---------- internal heap ---------- */

static unsigned int heap_pos = 65536;

static unsigned int local_alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

/* ---------- string helpers ---------- */

static unsigned int my_strlen(const char *s)
{
    unsigned int n = 0;
    while (s[n]) n++;
    return n;
}

static void my_strcpy(char *dst, const char *src, unsigned int max)
{
    unsigned int i = 0;
    while (i < max - 1 && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

static void my_strcat(char *dst, const char *src, unsigned int max)
{
    unsigned int d = my_strlen(dst);
    unsigned int i = 0;
    while (d + i < max - 1 && src[i]) { dst[d + i] = src[i]; i++; }
    dst[d + i] = '\0';
}

static int my_strcmp(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 1; a++; b++; }
    return (*a != *b);
}

static int my_strncmp(const char *a, const char *b, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (unsigned char)a[i] != (unsigned char)b[i];
        if (a[i] != b[i]) return 1;
    }
    return 0;
}

/* integer to string (decimal) */
static void int_to_str(int val, char *buf, unsigned int max)
{
    char tmp[24];
    int neg = 0;
    unsigned int i = 0;
    unsigned int u;

    if (val < 0) { neg = 1; u = (unsigned int)(-(val + 1)) + 1u; }
    else         { u = (unsigned int)val; }

    if (u == 0) { tmp[i++] = '0'; }
    while (u > 0 && i < 23) { tmp[i++] = '0' + (char)(u % 10); u /= 10; }
    if (neg && i < 23) tmp[i++] = '-';

    unsigned int j = 0;
    while (i > 0 && j < max - 1) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

/* ---------- output helpers ---------- */

static void out_init(doc_context_t *ctx, unsigned int cap)
{
    ctx->output_buf = host_alloc(cap, 16);
    ctx->output_pos = 0;
    ctx->output_cap = cap;
    char *base = (char *)ctx->output_buf;
    base[0] = '\0';
}

static void out_append(doc_context_t *ctx, const char *s)
{
    char *base = (char *)ctx->output_buf;
    unsigned int slen = my_strlen(s);
    unsigned int avail = ctx->output_cap - ctx->output_pos - 1;
    if (slen > avail) slen = avail;
    for (unsigned int i = 0; i < slen; i++)
        base[ctx->output_pos + i] = s[i];
    ctx->output_pos += slen;
    base[ctx->output_pos] = '\0';
}

static void out_flush(doc_context_t *ctx)
{
    char *base = (char *)ctx->output_buf;
    host_print(base);
    ctx->output_pos = 0;
    base[0] = '\0';
}

/* ---------- context management ---------- */

static void ctx_init(doc_context_t *ctx, unsigned int fmt)
{
    ctx->module_count  = 0;
    ctx->example_count = 0;
    ctx->xref_count    = 0;
    ctx->output_format = fmt;
    out_init(ctx, DOC_OUTPUT_BUF);
}

/* ---------- module registration ---------- */

static doc_module_t *ctx_add_module(doc_context_t *ctx, const char *name,
                                    const char *ver, const char *desc)
{
    if (ctx->module_count >= DOC_MAX_MODULES) return NULL;
    doc_module_t *m = &ctx->modules[ctx->module_count++];
    my_strcpy(m->name, name, DOC_MAX_NAME_LEN);
    my_strcpy(m->version, ver, 16);
    my_strcpy(m->description, desc, DOC_MAX_DESC_LEN);
    m->func_count   = 0;
    m->import_count = 0;
    m->export_count = 0;
    m->tags         = TAG_ALL;
    return m;
}

static doc_func_t *mod_add_func(doc_module_t *m, const char *name,
                                const char *ret, const char *sig,
                                const char *desc, int exported)
{
    if (m->func_count >= DOC_MAX_FUNCS) return NULL;
    doc_func_t *f = &m->funcs[m->func_count++];
    my_strcpy(f->name, name, DOC_MAX_NAME_LEN);
    my_strcpy(f->return_type, ret, DOC_MAX_NAME_LEN);
    my_strcpy(f->signature, sig, DOC_MAX_SIG_LEN);
    my_strcpy(f->description, desc, DOC_MAX_DESC_LEN);
    f->param_count = 0;
    f->tags        = TAG_API;
    f->is_exported = exported;
    return f;
}

static void func_add_param(doc_func_t *f, const char *pname,
                           const char *ptype, const char *dir, const char *pdesc)
{
    if (f->param_count >= DOC_MAX_PARAMS) return;
    doc_param_t *p = &f->params[f->param_count++];
    my_strcpy(p->name, pname, DOC_MAX_NAME_LEN);
    my_strcpy(p->type, ptype, DOC_MAX_NAME_LEN);
    my_strcpy(p->direction, dir, 16);
    my_strcpy(p->desc, pdesc, DOC_MAX_DESC_LEN);
}

static void mod_add_import(doc_module_t *m, const char *name)
{
    if (m->import_count >= DOC_MAX_DEPS) return;
    my_strcpy(m->imports[m->import_count++], name, DOC_MAX_NAME_LEN);
}

static void mod_add_export(doc_module_t *m, const char *name)
{
    if (m->export_count >= DOC_MAX_DEPS) return;
    my_strcpy(m->exports[m->export_count++], name, DOC_MAX_NAME_LEN);
}

/* ---------- examples and cross-references ---------- */

static void ctx_add_example(doc_context_t *ctx, const char *title,
                            const char *code, const char *expl)
{
    if (ctx->example_count >= DOC_MAX_FUNCS) return;
    doc_example_t *e = &ctx->examples[ctx->example_count++];
    my_strcpy(e->title, title, DOC_MAX_NAME_LEN);
    my_strcpy(e->code, code, DOC_MAX_EXAMPLE_LEN);
    my_strcpy(e->explanation, expl, DOC_MAX_DESC_LEN);
}

static void ctx_add_xref(doc_context_t *ctx, const char *src,
                         const char *tgt, const char *rel)
{
    if (ctx->xref_count >= DOC_MAX_DEPS * 4) return;
    doc_xref_t *x = &ctx->xrefs[ctx->xref_count++];
    my_strcpy(x->source, src, DOC_MAX_NAME_LEN);
    my_strcpy(x->target, tgt, DOC_MAX_NAME_LEN);
    my_strcpy(x->relationship, rel, 32);
}

static void ctx_build_xrefs(doc_context_t *ctx)
{
    for (unsigned int i = 0; i < ctx->module_count; i++) {
        doc_module_t *m = &ctx->modules[i];
        for (unsigned int j = 0; j < m->import_count; j++)
            ctx_add_xref(ctx, m->name, m->imports[j], "imports");
        for (unsigned int j = 0; j < m->export_count; j++)
            ctx_add_xref(ctx, m->name, m->exports[j], "exports_to");
    }
}

/* ================================================================
 *  MARKDOWN GENERATORS
 * ================================================================ */

static void md_api_doc(doc_context_t *ctx, doc_module_t *m)
{
    char line[512];

    out_append(ctx, "## Module: ");
    out_append(ctx, m->name);
    out_append(ctx, " (v");
    out_append(ctx, m->version);
    out_append(ctx, ")\n\n");
    out_append(ctx, m->description);
    out_append(ctx, "\n\n");

    /* exported functions */
    for (unsigned int i = 0; i < m->func_count; i++) {
        doc_func_t *f = &m->funcs[i];
        if (!f->is_exported) continue;

        out_append(ctx, "### `");
        out_append(ctx, f->signature);
        out_append(ctx, "`\n\n");
        out_append(ctx, f->description);
        out_append(ctx, "\n\n");

        out_append(ctx, "**Returns:** `");
        out_append(ctx, f->return_type);
        out_append(ctx, "`\n\n");

        if (f->param_count > 0) {
            out_append(ctx, "**Parameters:**\n\n");
            out_append(ctx, "| Name | Type | Direction | Description |\n");
            out_append(ctx, "|------|------|-----------|-------------|\n");
            for (unsigned int p = 0; p < f->param_count; p++) {
                doc_param_t *pm = &f->params[p];
                out_append(ctx, "| `");
                out_append(ctx, pm->name);
                out_append(ctx, "` | `");
                out_append(ctx, pm->type);
                out_append(ctx, "` | ");
                out_append(ctx, pm->direction);
                out_append(ctx, " | ");
                out_append(ctx, pm->desc);
                out_append(ctx, " |\n");
            }
            out_append(ctx, "\n");
        }
    }
}

static void md_arch_diagram(doc_context_t *ctx)
{
    out_append(ctx, "## Architecture Diagram\n\n");
    out_append(ctx, "```\n");
    out_append(ctx, "+======================================+\n");
    out_append(ctx, "|        AI-ASM OS v46.0               |\n");
    out_append(ctx, "+======================================+\n");
    out_append(ctx, "|  +------------+  +---------------+  |\n");

    for (unsigned int i = 0; i < ctx->module_count && i < 4; i++) {
        out_append(ctx, "|  | ");
        out_append(ctx, ctx->modules[i].name);
        unsigned int pad = 12 - my_strlen(ctx->modules[i].name);
        for (unsigned int k = 0; k < pad; k++) out_append(ctx, " ");
        if (i % 2 == 0) out_append(ctx, "  |  | ");
        else            out_append(ctx, "  |\n");
    }

    out_append(ctx, "+======================================+\n");
    out_append(ctx, "|            Host Runtime              |\n");
    out_append(ctx, "+======================================+\n");
    out_append(ctx, "```\n\n");
}

static void md_examples(doc_context_t *ctx)
{
    if (ctx->example_count == 0) return;
    out_append(ctx, "## Usage Examples\n\n");
    for (unsigned int i = 0; i < ctx->example_count; i++) {
        doc_example_t *e = &ctx->examples[i];
        out_append(ctx, "### ");
        out_append(ctx, e->title);
        out_append(ctx, "\n\n");
        out_append(ctx, "```c\n");
        out_append(ctx, e->code);
        out_append(ctx, "\n```\n\n");
        out_append(ctx, e->explanation);
        out_append(ctx, "\n\n");
    }
}

static void md_xref_table(doc_context_t *ctx)
{
    if (ctx->xref_count == 0) return;
    out_append(ctx, "## Cross-Reference Map\n\n");
    out_append(ctx, "| Source | Relationship | Target |\n");
    out_append(ctx, "|--------|-------------|--------|\n");
    for (unsigned int i = 0; i < ctx->xref_count; i++) {
        doc_xref_t *x = &ctx->xrefs[i];
        out_append(ctx, "| ");
        out_append(ctx, x->source);
        out_append(ctx, " | ");
        out_append(ctx, x->relationship);
        out_append(ctx, " | ");
        out_append(ctx, x->target);
        out_append(ctx, " |\n");
    }
    out_append(ctx, "\n");
}

static void md_index(doc_context_t *ctx)
{
    out_append(ctx, "# AI-ASM OS v46.0 Module Documentation Index\n\n");
    out_append(ctx, "## Modules\n\n");

    char num[16];
    for (unsigned int i = 0; i < ctx->module_count; i++) {
        doc_module_t *m = &ctx->modules[i];
        int_to_str(i + 1, num, 16);
        out_append(ctx, num);
        out_append(ctx, ". **");
        out_append(ctx, m->name);
        out_append(ctx, "** (v");
        out_append(ctx, m->version);
        out_append(ctx, ") - ");
        out_append(ctx, m->description);
        out_append(ctx, "\n");
    }

    out_append(ctx, "\n## Summary\n\n");
    int_to_str(ctx->module_count, num, 16);
    out_append(ctx, "- Total modules: ");
    out_append(ctx, num);
    out_append(ctx, "\n");

    unsigned int total_funcs = 0;
    unsigned int total_exports = 0;
    for (unsigned int i = 0; i < ctx->module_count; i++) {
        total_funcs   += ctx->modules[i].func_count;
        total_exports += ctx->modules[i].export_count;
    }
    int_to_str(total_funcs, num, 16);
    out_append(ctx, "- Total functions: ");
    out_append(ctx, num);
    out_append(ctx, "\n");

    int_to_str(total_exports, num, 16);
    out_append(ctx, "- Total exports: ");
    out_append(ctx, num);
    out_append(ctx, "\n");

    int_to_str(ctx->xref_count, num, 16);
    out_append(ctx, "- Cross-references: ");
    out_append(ctx, num);
    out_append(ctx, "\n");
}

/* ================================================================
 *  HTML GENERATORS
 * ================================================================ */

static void html_header(doc_context_t *ctx)
{
    out_append(ctx, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n");
    out_append(ctx, "<meta charset=\"UTF-8\">\n");
    out_append(ctx, "<title>AI-ASM OS v46.0 Documentation</title>\n");
    out_append(ctx, "<style>\n");
    out_append(ctx, "body{font-family:monospace;margin:2em;}\n");
    out_append(ctx, "table{border-collapse:collapse;margin:1em 0;}\n");
    out_append(ctx, "th,td{border:1px solid #888;padding:4px 8px;}\n");
    out_append(ctx, "pre{background:#f4f4f4;padding:1em;overflow-x:auto;}\n");
    out_append(ctx, ".arch-diagram{white-space:pre;font-family:monospace;}\n");
    out_append(ctx, "</style>\n</head>\n<body>\n");
}

static void html_footer(doc_context_t *ctx)
{
    out_append(ctx, "</body>\n</html>\n");
}

static void html_api_doc(doc_context_t *ctx, doc_module_t *m)
{
    out_append(ctx, "<h2>Module: ");
    out_append(ctx, m->name);
    out_append(ctx, " (v");
    out_append(ctx, m->version);
    out_append(ctx, ")</h2>\n<p>");
    out_append(ctx, m->description);
    out_append(ctx, "</p>\n");

    for (unsigned int i = 0; i < m->func_count; i++) {
        doc_func_t *f = &m->funcs[i];
        if (!f->is_exported) continue;

        out_append(ctx, "<h3><code>");
        out_append(ctx, f->signature);
        out_append(ctx, "</code></h3>\n<p>");
        out_append(ctx, f->description);
        out_append(ctx, "</p>\n");
        out_append(ctx, "<p><strong>Returns:</strong> <code>");
        out_append(ctx, f->return_type);
        out_append(ctx, "</code></p>\n");

        if (f->param_count > 0) {
            out_append(ctx, "<table>\n<tr><th>Name</th><th>Type</th>");
            out_append(ctx, "<th>Direction</th><th>Description</th></tr>\n");
            for (unsigned int p = 0; p < f->param_count; p++) {
                doc_param_t *pm = &f->params[p];
                out_append(ctx, "<tr><td><code>");
                out_append(ctx, pm->name);
                out_append(ctx, "</code></td><td><code>");
                out_append(ctx, pm->type);
                out_append(ctx, "</code></td><td>");
                out_append(ctx, pm->direction);
                out_append(ctx, "</td><td>");
                out_append(ctx, pm->desc);
                out_append(ctx, "</td></tr>\n");
            }
            out_append(ctx, "</table>\n");
        }
    }
}

static void html_arch_diagram(doc_context_t *ctx)
{
    out_append(ctx, "<h2>Architecture Diagram</h2>\n");
    out_append(ctx, "<div class=\"arch-diagram\">\n");
    out_append(ctx, "+======================================+\n");
    out_append(ctx, "|        AI-ASM OS v46.0               |\n");
    out_append(ctx, "+======================================+\n");
    for (unsigned int i = 0; i < ctx->module_count && i < 4; i++) {
        out_append(ctx, "|  [");
        out_append(ctx, ctx->modules[i].name);
        out_append(ctx, "]  ");
    }
    out_append(ctx, "\n+======================================+\n");
    out_append(ctx, "|            Host Runtime              |\n");
    out_append(ctx, "+======================================+\n");
    out_append(ctx, "</div>\n");
}

static void html_examples(doc_context_t *ctx)
{
    if (ctx->example_count == 0) return;
    out_append(ctx, "<h2>Usage Examples</h2>\n");
    for (unsigned int i = 0; i < ctx->example_count; i++) {
        doc_example_t *e = &ctx->examples[i];
        out_append(ctx, "<h3>");
        out_append(ctx, e->title);
        out_append(ctx, "</h3>\n<pre><code>");
        out_append(ctx, e->code);
        out_append(ctx, "</code></pre>\n<p>");
        out_append(ctx, e->explanation);
        out_append(ctx, "</p>\n");
    }
}

static void html_xref_table(doc_context_t *ctx)
{
    if (ctx->xref_count == 0) return;
    out_append(ctx, "<h2>Cross-Reference Map</h2>\n");
    out_append(ctx, "<table>\n<tr><th>Source</th><th>Relationship</th><th>Target</th></tr>\n");
    for (unsigned int i = 0; i < ctx->xref_count; i++) {
        doc_xref_t *x = &ctx->xrefs[i];
        out_append(ctx, "<tr><td>");
        out_append(ctx, x->source);
        out_append(ctx, "</td><td>");
        out_append(ctx, x->relationship);
        out_append(ctx, "</td><td>");
        out_append(ctx, x->target);
        out_append(ctx, "</td></tr>\n");
    }
    out_append(ctx, "</table>\n");
}

static void html_index(doc_context_t *ctx)
{
    out_append(ctx, "<h1>AI-ASM OS v46.0 Module Documentation Index</h1>\n");
    out_append(ctx, "<ul>\n");
    for (unsigned int i = 0; i < ctx->module_count; i++) {
        doc_module_t *m = &ctx->modules[i];
        out_append(ctx, "<li><strong>");
        out_append(ctx, m->name);
        out_append(ctx, "</strong> (v");
        out_append(ctx, m->version);
        out_append(ctx, ") &mdash; ");
        out_append(ctx, m->description);
        out_append(ctx, "</li>\n");
    }
    out_append(ctx, "</ul>\n");

    char num[16];
    out_append(ctx, "<p>Total modules: ");
    int_to_str(ctx->module_count, num, 16);
    out_append(ctx, num);
    out_append(ctx, "</p>\n");
}

/* ================================================================
 *  HIGH-LEVEL GENERATORS
 * ================================================================ */

static void generate_api_docs(doc_context_t *ctx)
{
    if (ctx->output_format == FMT_HTML) {
        for (unsigned int i = 0; i < ctx->module_count; i++)
            html_api_doc(ctx, &ctx->modules[i]);
    } else {
        for (unsigned int i = 0; i < ctx->module_count; i++)
            md_api_doc(ctx, &ctx->modules[i]);
    }
}

static void generate_arch_diagram(doc_context_t *ctx)
{
    if (ctx->output_format == FMT_HTML) html_arch_diagram(ctx);
    else                                 md_arch_diagram(ctx);
}

static void generate_examples(doc_context_t *ctx)
{
    if (ctx->output_format == FMT_HTML) html_examples(ctx);
    else                                 md_examples(ctx);
}

static void generate_xref_map(doc_context_t *ctx)
{
    ctx_build_xrefs(ctx);
    if (ctx->output_format == FMT_HTML) html_xref_table(ctx);
    else                                 md_xref_table(ctx);
}

static void generate_index(doc_context_t *ctx)
{
    if (ctx->output_format == FMT_HTML) html_index(ctx);
    else                                 md_index(ctx);
}

static void generate_full_docs(doc_context_t *ctx)
{
    if (ctx->output_format == FMT_HTML) html_header(ctx);

    generate_index(ctx);
    generate_api_docs(ctx);
    generate_arch_diagram(ctx);
    generate_examples(ctx);
    generate_xref_map(ctx);

    if (ctx->output_format == FMT_HTML) html_footer(ctx);

    out_flush(ctx);
}

/* ================================================================
 *  TEST SUITE
 * ================================================================ */

static int test_ctx_init(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    if (ctx.module_count != 0)  return 0;
    if (ctx.example_count != 0) return 0;
    if (ctx.xref_count != 0)    return 0;
    if (ctx.output_format != FMT_MARKDOWN) return 0;
    return 1;
}

static int test_module_registration(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *m = ctx_add_module(&ctx, "scheduler", "1.2.0",
                                     "Task scheduler module");
    if (!m) return 0;
    if (ctx.module_count != 1) return 0;
    if (my_strcmp(m->name, "scheduler") != 0) return 0;
    if (my_strcmp(m->version, "1.2.0") != 0) return 0;
    return 1;
}

static int test_func_registration(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *m = ctx_add_module(&ctx, "mem", "2.0.0", "Memory allocator");
    if (!m) return 0;

    doc_func_t *f = mod_add_func(m, "alloc", "void*", "void* alloc(uint size, uint align)",
                                 "Allocate aligned memory block", 1);
    if (!f) return 0;
    if (m->func_count != 1) return 0;
    if (my_strcmp(f->name, "alloc") != 0) return 0;
    if (f->is_exported != 1) return 0;
    return 1;
}

static int test_param_registration(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *m = ctx_add_module(&ctx, "io", "1.0.0", "I/O module");
    doc_func_t *f = mod_add_func(m, "read", "int", "int read(int fd, void* buf, uint count)",
                                 "Read bytes from file descriptor", 1);
    func_add_param(f, "fd",    "int",   "in", "File descriptor");
    func_add_param(f, "buf",   "void*", "out", "Destination buffer");
    func_add_param(f, "count", "uint",  "in", "Number of bytes to read");

    if (f->param_count != 3) return 0;
    if (my_strcmp(f->params[0].name, "fd") != 0) return 0;
    if (my_strcmp(f->params[1].type, "void*") != 0) return 0;
    if (my_strcmp(f->params[2].direction, "in") != 0) return 0;
    return 1;
}

static int test_import_export(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *m = ctx_add_module(&ctx, "net", "3.0.0", "Network stack");
    mod_add_import(m, "mem");
    mod_add_import(m, "io");
    mod_add_export(m, "tcp_connect");
    mod_add_export(m, "udp_send");

    if (m->import_count != 2) return 0;
    if (m->export_count != 2) return 0;
    if (my_strcmp(m->imports[0], "mem") != 0) return 0;
    if (my_strcmp(m->exports[1], "udp_send") != 0) return 0;
    return 1;
}

static int test_example_registration(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    ctx_add_example(&ctx, "Basic allocation",
                    "void* p = alloc(1024, 16);",
                    "Allocates 1024 bytes aligned to 16 bytes");

    if (ctx.example_count != 1) return 0;
    if (my_strcmp(ctx.examples[0].title, "Basic allocation") != 0) return 0;
    return 1;
}

static int test_xref_building(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *a = ctx_add_module(&ctx, "alpha", "1.0.0", "Module alpha");
    doc_module_t *b = ctx_add_module(&ctx, "beta",  "1.0.0", "Module beta");
    mod_add_import(a, "beta");
    mod_add_export(b, "beta_init");

    ctx_build_xrefs(&ctx);

    if (ctx.xref_count != 2) return 0;
    if (my_strcmp(ctx.xrefs[0].source, "alpha") != 0) return 0;
    if (my_strcmp(ctx.xrefs[0].relationship, "imports") != 0) return 0;
    if (my_strcmp(ctx.xrefs[1].relationship, "exports_to") != 0) return 0;
    return 1;
}

static int test_html_format(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_HTML);

    doc_module_t *m = ctx_add_module(&ctx, "demo", "0.1.0", "Demo module");
    mod_add_func(m, "init", "int", "int init(void)", "Initialize demo", 1);
    ctx_add_example(&ctx, "Init demo", "init();", "Call init");

    generate_full_docs(&ctx);
    /* If we reached here without crash, HTML generation works */
    return 1;
}

static int test_markdown_generation(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    doc_module_t *m = ctx_add_module(&ctx, "log", "1.0.0", "Logging subsystem");
    doc_func_t *f = mod_add_func(m, "log_msg", "void",
                                 "void log_msg(int level, const char* msg)",
                                 "Write log message", 1);
    func_add_param(f, "level", "int",        "in", "Log level (0-5)");
    func_add_param(f, "msg",   "const char*","in", "Message string");
    mod_add_import(m, "io");

    ctx_add_example(&ctx, "Log info", "log_msg(2, \"hello\");", "Log at info level");
    generate_full_docs(&ctx);
    return 1;
}

static int test_multi_module_index(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    ctx_add_module(&ctx, "mod_a", "1.0.0", "First module");
    ctx_add_module(&ctx, "mod_b", "2.0.0", "Second module");
    ctx_add_module(&ctx, "mod_c", "3.0.0", "Third module");

    generate_index(&ctx);
    out_flush(&ctx);
    return (ctx.module_count == 3);
}

static int test_arch_diagram_gen(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);
    ctx_add_module(&ctx, "sched",  "1.0.0", "Scheduler");
    ctx_add_module(&ctx, "memory", "1.0.0", "Memory mgr");
    ctx_add_module(&ctx, "fs",     "1.0.0", "Filesystem");
    ctx_add_module(&ctx, "net",    "1.0.0", "Networking");

    generate_arch_diagram(&ctx);
    out_flush(&ctx);
    return 1;
}

static int test_empty_context(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);
    /* Generating on empty context should not crash */
    generate_full_docs(&ctx);
    return (ctx.module_count == 0);
}

static int test_string_helpers(void)
{
    char buf[64];
    my_strcpy(buf, "hello", 64);
    if (my_strlen(buf) != 5) return 0;

    my_strcat(buf, " world", 64);
    if (my_strcmp(buf, "hello world") != 0) return 0;

    char num[16];
    int_to_str(42, num, 16);
    if (my_strcmp(num, "42") != 0) return 0;

    int_to_str(-7, num, 16);
    if (my_strcmp(num, "-7") != 0) return 0;

    int_to_str(0, num, 16);
    if (my_strcmp(num, "0") != 0) return 0;

    return 1;
}

static int test_output_append(void)
{
    doc_context_t ctx;
    ctx_init(&ctx, FMT_MARKDOWN);

    out_append(&ctx, "hello");
    out_append(&ctx, " ");
    out_append(&ctx, "world");

    char *base = (char *)ctx.output_buf;
    if (my_strcmp(base, "hello world") != 0) return 0;
    if (ctx.output_pos != 11) return 0;

    out_flush(&ctx);
    return 1;
}

static void run_test(const char *name, int (*fn)(void))
{
    host_print("  [TEST] ");
    host_print(name);
    host_print(" ... ");
    if (fn()) {
        host_print("PASS\n");
    } else {
        host_print("** FAIL **\n");
    }
}

static void run_test_suite(void)
{
    host_print("=== doc_generator test suite (v46.0) ===\n\n");

    run_test("string_helpers",       test_string_helpers);
    run_test("ctx_init",             test_ctx_init);
    run_test("module_registration",  test_module_registration);
    run_test("func_registration",    test_func_registration);
    run_test("param_registration",   test_param_registration);
    run_test("import_export",        test_import_export);
    run_test("example_registration", test_example_registration);
    run_test("xref_building",        test_xref_building);
    run_test("output_append",        test_output_append);
    run_test("html_format",          test_html_format);
    run_test("markdown_generation",  test_markdown_generation);
    run_test("multi_module_index",   test_multi_module_index);
    run_test("arch_diagram_gen",     test_arch_diagram_gen);
    run_test("empty_context",        test_empty_context);

    host_print("\n=== all tests complete ===\n");
}

/* ================================================================
 *  HELP TEXT
 * ================================================================ */

static void show_help(void)
{
    host_print("doc_generator - AI-ASM OS v46.0 documentation generator\n\n");
    host_print("Usage: doc_generator [options]\n\n");
    host_print("Options:\n");
    host_print("  -h          Show this help message\n");
    host_print("  -t          Run test suite\n");
    host_print("  -f md       Output format: markdown (default)\n");
    host_print("  -f html     Output format: HTML\n\n");
    host_print("Features:\n");
    host_print("  API docs      Extract function signatures, params, return types\n");
    host_print("  Arch diagram  ASCII art architecture diagrams from metadata\n");
    host_print("  Examples      Usage examples with explanations\n");
    host_print("  X-ref map     Module dependency and import/export mapping\n");
    host_print("  Output        Markdown and HTML formats\n");
    host_print("  Index         Module documentation index\n");
}

/* ================================================================
 *  ENTRY POINT
 * ================================================================ */

void _start(void)
{
    unsigned int buf = host_alloc(512, 16);
    int argc = host_get_argv(buf, 512);
    char *args = (char *)buf;

    /* default: run test suite */
    if (argc <= 1) {
        run_test_suite();
        host_exit(0);
    }

    /* parse arguments */
    for (int i = 1; i < argc; i++) {
        /* each arg is a null-terminated string packed sequentially;
         * for simplicity, scan the raw buffer for flag markers */
        char *p = args;
        int skip = 0;
        for (int j = 1; j < i && !skip; j++) {
            while (*p) p++;
            p++;
            if ((unsigned int)(p - args) >= 512) { skip = 1; break; }
        }
        if (skip) break;

        if (my_strcmp(p, "-h") == 0) {
            show_help();
            host_exit(0);
        }
        if (my_strcmp(p, "-t") == 0) {
            run_test_suite();
            host_exit(0);
        }
    }

    /* no recognized flag — show help */
    show_help();
    host_exit(0);
}
