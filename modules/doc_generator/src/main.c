/* doc_generator: Documentation generation module for AI-ASM OS v46.0 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_MODS 64
#define MAX_FUNCS 32
#define MAX_DEPS 16
#define MAX_PARAMS 8
#define MAX_NAME 48
#define MAX_DESC 96
#define MAX_SIG 128
#define MAX_EXAMPLE 192
#define OUT_BUF 4096
#define FMT_MD 0
#define FMT_HTML 1

typedef struct { char name[MAX_NAME]; char type[MAX_NAME]; char dir[12]; char desc[MAX_DESC]; } doc_param_t;
typedef struct {
    char name[MAX_NAME]; char ret[MAX_NAME]; char sig[MAX_SIG]; char desc[MAX_DESC];
    doc_param_t params[MAX_PARAMS]; unsigned int pcnt; int exported;
} doc_func_t;
typedef struct {
    char name[MAX_NAME]; char ver[12]; char desc[MAX_DESC];
    doc_func_t funcs[MAX_FUNCS]; unsigned int fcnt;
    char imports[MAX_DEPS][MAX_NAME]; unsigned int icnt;
    char exports[MAX_DEPS][MAX_NAME]; unsigned int ecnt;
} doc_module_t;
typedef struct { char title[MAX_NAME]; char code[MAX_EXAMPLE]; char expl[MAX_DESC]; } doc_example_t;
typedef struct { char src[MAX_NAME]; char tgt[MAX_NAME]; char rel[24]; } doc_xref_t;
typedef struct {
    doc_module_t mods[MAX_MODS]; unsigned int mcnt;
    doc_example_t examples[MAX_FUNCS]; unsigned int ecnt;
    doc_xref_t xrefs[MAX_DEPS * 4]; unsigned int xcnt;
    unsigned int fmt; unsigned int obuf; unsigned int opos; unsigned int ocap;
} doc_ctx_t;

static unsigned int heap = 65536;
static unsigned int lalloc(unsigned int n) { unsigned int p = heap; heap += n; heap = (heap+15)&~15u; return p; }

static unsigned int slen(const char *s) { unsigned int n=0; while(s[n]) n++; return n; }
static void scpy(char *d, const char *s, unsigned int mx) {
    unsigned int i=0; while(i<mx-1 && s[i]){d[i]=s[i]; i++;} d[i]='\0';
}
static void scat(char *d, const char *s, unsigned int mx) {
    unsigned int dl=slen(d), i=0; while(dl+i<mx-1 && s[i]){d[dl+i]=s[i]; i++;} d[dl+i]='\0';
}
static int seq(const char *a, const char *b) {
    while(*a && *b){if(*a!=*b) return 1; a++; b++;} return (*a!=*b);
}
static void i2s(int v, char *b, unsigned int mx) {
    char t[20]; unsigned int i=0, u; int neg=0;
    if(v<0){neg=1; u=(unsigned int)(-(v+1))+1u;} else u=(unsigned int)v;
    if(u==0){t[i++]='0';} while(u>0 && i<19){t[i++]='0'+(char)(u%10); u/=10;}
    if(neg && i<19) t[i++]='-';
    unsigned int j=0; while(i>0 && j<mx-1) b[j++]=t[--i]; b[j]='\0';
}

static void oinit(doc_ctx_t *c, unsigned int cap) {
    c->obuf = host_alloc(cap, 16); c->opos = 0; c->ocap = cap;
    ((char*)c->obuf)[0] = '\0';
}
static void oapp(doc_ctx_t *c, const char *s) {
    char *b = (char*)c->obuf; unsigned int sl = slen(s);
    unsigned int av = c->ocap - c->opos - 1; if(sl>av) sl=av;
    for(unsigned int i=0; i<sl; i++) b[c->opos+i] = s[i];
    c->opos += sl; b[c->opos] = '\0';
}
static void oflush(doc_ctx_t *c) { host_print((char*)c->obuf); c->opos=0; ((char*)c->obuf)[0]='\0'; }

static void cinit(doc_ctx_t *c, unsigned int fmt) {
    c->mcnt=0; c->ecnt=0; c->xcnt=0; c->fmt=fmt; oinit(c, OUT_BUF);
}
static doc_module_t *cadd_mod(doc_ctx_t *c, const char *n, const char *v, const char *d) {
    if(c->mcnt>=MAX_MODS) return NULL;
    doc_module_t *m = &c->mods[c->mcnt++];
    scpy(m->name,n,MAX_NAME); scpy(m->ver,v,12); scpy(m->desc,d,MAX_DESC);
    m->fcnt=0; m->icnt=0; m->ecnt=0; return m;
}
static doc_func_t *madd_func(doc_module_t *m, const char *n, const char *r, const char *s, const char *d, int ex) {
    if(m->fcnt>=MAX_FUNCS) return NULL;
    doc_func_t *f = &m->funcs[m->fcnt++];
    scpy(f->name,n,MAX_NAME); scpy(f->ret,r,MAX_NAME); scpy(f->sig,s,MAX_SIG);
    scpy(f->desc,d,MAX_DESC); f->pcnt=0; f->exported=ex; return f;
}
static void fadd_param(doc_func_t *f, const char *n, const char *t, const char *d, const char *desc) {
    if(f->pcnt>=MAX_PARAMS) return;
    doc_param_t *p = &f->params[f->pcnt++];
    scpy(p->name,n,MAX_NAME); scpy(p->type,t,MAX_NAME); scpy(p->dir,d,12); scpy(p->desc,desc,MAX_DESC);
}
static void madd_imp(doc_module_t *m, const char *n) { if(m->icnt<MAX_DEPS) scpy(m->imports[m->icnt++],n,MAX_NAME); }
static void madd_exp(doc_module_t *m, const char *n) { if(m->ecnt<MAX_DEPS) scpy(m->exports[m->ecnt++],n,MAX_NAME); }
static void cadd_ex(doc_ctx_t *c, const char *t, const char *code, const char *e) {
    if(c->ecnt>=MAX_FUNCS) return;
    scpy(c->examples[c->ecnt].title,t,MAX_NAME);
    scpy(c->examples[c->ecnt].code,code,MAX_EXAMPLE);
    scpy(c->examples[c->ecnt++].expl,e,MAX_DESC);
}
static void cadd_xr(doc_ctx_t *c, const char *s, const char *t, const char *r) {
    if(c->xcnt>=MAX_DEPS*4) return;
    scpy(c->xrefs[c->xcnt].src,s,MAX_NAME); scpy(c->xrefs[c->xcnt].tgt,t,MAX_NAME);
    scpy(c->xrefs[c->xcnt++].rel,r,24);
}
static void build_xrefs(doc_ctx_t *c) {
    for(unsigned int i=0; i<c->mcnt; i++) {
        doc_module_t *m = &c->mods[i];
        for(unsigned int j=0; j<m->icnt; j++) cadd_xr(c, m->name, m->imports[j], "imports");
        for(unsigned int j=0; j<m->ecnt; j++) cadd_xr(c, m->name, m->exports[j], "exports_to");
    }
}

/* ---- Markdown generators ---- */
static void md_api(doc_ctx_t *c, doc_module_t *m) {
    oapp(c,"## Module: "); oapp(c,m->name); oapp(c," (v"); oapp(c,m->ver); oapp(c,")\n\n");
    oapp(c,m->desc); oapp(c,"\n\n");
    for(unsigned int i=0; i<m->fcnt; i++) {
        doc_func_t *f = &m->funcs[i]; if(!f->exported) continue;
        oapp(c,"### `"); oapp(c,f->sig); oapp(c,"`\n\n"); oapp(c,f->desc); oapp(c,"\n\n");
        oapp(c,"**Returns:** `"); oapp(c,f->ret); oapp(c,"`\n\n");
        if(f->pcnt>0) {
            oapp(c,"**Parameters:**\n\n| Name | Type | Dir | Description |\n|---|---|---|---|\n");
            for(unsigned int p=0; p<f->pcnt; p++) {
                doc_param_t *pm = &f->params[p];
                oapp(c,"| `"); oapp(c,pm->name); oapp(c,"` | `"); oapp(c,pm->type);
                oapp(c,"` | "); oapp(c,pm->dir); oapp(c," | "); oapp(c,pm->desc); oapp(c," |\n");
            }
            oapp(c,"\n");
        }
    }
}
static void md_arch(doc_ctx_t *c) {
    oapp(c,"## Architecture\n\n```\n+============================+\n|    AI-ASM OS v46.0         |\n+============================+\n");
    for(unsigned int i=0; i<c->mcnt && i<4; i++) {
        oapp(c,"| ["); oapp(c,c->mods[i].name); oapp(c,"]  ");
    }
    oapp(c,"\n+============================+\n|       Host Runtime         |\n+============================+\n```\n\n");
}
static void md_exs(doc_ctx_t *c) {
    if(!c->ecnt) return;
    oapp(c,"## Examples\n\n");
    for(unsigned int i=0; i<c->ecnt; i++) {
        oapp(c,"### "); oapp(c,c->examples[i].title); oapp(c,"\n```c\n");
        oapp(c,c->examples[i].code); oapp(c,"\n```\n"); oapp(c,c->examples[i].expl); oapp(c,"\n\n");
    }
}
static void md_xref(doc_ctx_t *c) {
    if(!c->xcnt) return;
    oapp(c,"## Cross-Reference\n\n| Source | Relation | Target |\n|---|---|---|\n");
    for(unsigned int i=0; i<c->xcnt; i++) {
        oapp(c,"| "); oapp(c,c->xrefs[i].src); oapp(c," | "); oapp(c,c->xrefs[i].rel);
        oapp(c," | "); oapp(c,c->xrefs[i].tgt); oapp(c," |\n");
    }
    oapp(c,"\n");
}
static void md_idx(doc_ctx_t *c) {
    oapp(c,"# AI-ASM OS v46.0 Module Index\n\n");
    char n[12];
    for(unsigned int i=0; i<c->mcnt; i++) {
        i2s(i+1,n,12); oapp(c,n); oapp(c,". **"); oapp(c,c->mods[i].name);
        oapp(c,"** (v"); oapp(c,c->mods[i].ver); oapp(c,") - "); oapp(c,c->mods[i].desc); oapp(c,"\n");
    }
    unsigned int tf=0, te=0;
    for(unsigned int i=0; i<c->mcnt; i++) { tf+=c->mods[i].fcnt; te+=c->mods[i].ecnt; }
    oapp(c,"\n## Summary\n\n- Modules: "); i2s(c->mcnt,n,12); oapp(c,n);
    oapp(c,"\n- Functions: "); i2s(tf,n,12); oapp(c,n);
    oapp(c,"\n- Exports: "); i2s(te,n,12); oapp(c,n);
    oapp(c,"\n- X-refs: "); i2s(c->xcnt,n,12); oapp(c,n); oapp(c,"\n");
}

/* ---- HTML generators ---- */
static void html_hdr(doc_ctx_t *c) {
    oapp(c,"<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\">\n<title>AI-ASM v46.0 Docs</title>\n");
    oapp(c,"<style>body{font-family:monospace;margin:2em}table{border-collapse:collapse;margin:1em 0}");
    oapp(c,"th,td{border:1px solid #888;padding:4px 8px}pre{background:#f4f4f4;padding:1em}</style>\n");
    oapp(c,"</head><body>\n");
}
static void html_ftr(doc_ctx_t *c) { oapp(c,"</body></html>\n"); }
static void html_api(doc_ctx_t *c, doc_module_t *m) {
    oapp(c,"<h2>"); oapp(c,m->name); oapp(c," v"); oapp(c,m->ver); oapp(c,"</h2><p>"); oapp(c,m->desc); oapp(c,"</p>\n");
    for(unsigned int i=0; i<m->fcnt; i++) {
        doc_func_t *f = &m->funcs[i]; if(!f->exported) continue;
        oapp(c,"<h3><code>"); oapp(c,f->sig); oapp(c,"</code></h3><p>"); oapp(c,f->desc);
        oapp(c,"</p><p><b>Returns:</b> <code>"); oapp(c,f->ret); oapp(c,"</code></p>\n");
        if(f->pcnt>0) {
            oapp(c,"<table><tr><th>Name</th><th>Type</th><th>Dir</th><th>Desc</th></tr>\n");
            for(unsigned int p=0; p<f->pcnt; p++) {
                doc_param_t *pm = &f->params[p];
                oapp(c,"<tr><td><code>"); oapp(c,pm->name); oapp(c,"</code></td><td><code>");
                oapp(c,pm->type); oapp(c,"</code></td><td>"); oapp(c,pm->dir);
                oapp(c,"</td><td>"); oapp(c,pm->desc); oapp(c,"</td></tr>\n");
            }
            oapp(c,"</table>\n");
        }
    }
}
static void html_arch(doc_ctx_t *c) {
    oapp(c,"<h2>Architecture</h2><pre>\n+============================+\n|    AI-ASM OS v46.0         |\n+============================+\n");
    for(unsigned int i=0; i<c->mcnt && i<4; i++) { oapp(c,"["); oapp(c,c->mods[i].name); oapp(c,"] "); }
    oapp(c,"\n+============================+\n|       Host Runtime         |\n+============================+\n</pre>\n");
}
static void html_exs(doc_ctx_t *c) {
    if(!c->ecnt) return;
    oapp(c,"<h2>Examples</h2>\n");
    for(unsigned int i=0; i<c->ecnt; i++) {
        oapp(c,"<h3>"); oapp(c,c->examples[i].title); oapp(c,"</h3><pre><code>");
        oapp(c,c->examples[i].code); oapp(c,"</code></pre><p>"); oapp(c,c->examples[i].expl); oapp(c,"</p>\n");
    }
}
static void html_xref(doc_ctx_t *c) {
    if(!c->xcnt) return;
    oapp(c,"<h2>Cross-Reference</h2><table><tr><th>Source</th><th>Relation</th><th>Target</th></tr>\n");
    for(unsigned int i=0; i<c->xcnt; i++) {
        oapp(c,"<tr><td>"); oapp(c,c->xrefs[i].src); oapp(c,"</td><td>"); oapp(c,c->xrefs[i].rel);
        oapp(c,"</td><td>"); oapp(c,c->xrefs[i].tgt); oapp(c,"</td></tr>\n");
    }
    oapp(c,"</table>\n");
}
static void html_idx(doc_ctx_t *c) {
    oapp(c,"<h1>AI-ASM OS v46.0 Index</h1><ul>\n");
    for(unsigned int i=0; i<c->mcnt; i++) {
        oapp(c,"<li><b>"); oapp(c,c->mods[i].name); oapp(c,"</b> v"); oapp(c,c->mods[i].ver);
        oapp(c," &mdash; "); oapp(c,c->mods[i].desc); oapp(c,"</li>\n");
    }
    char n[12]; oapp(c,"</ul><p>Modules: "); i2s(c->mcnt,n,12); oapp(c,n); oapp(c,"</p>\n");
}

/* ---- High-level dispatch ---- */
static void gen_api(doc_ctx_t *c) {
    if(c->fmt==FMT_HTML) { for(unsigned int i=0;i<c->mcnt;i++) html_api(c,&c->mods[i]); }
    else { for(unsigned int i=0;i<c->mcnt;i++) md_api(c,&c->mods[i]); }
}
static void gen_arch(doc_ctx_t *c) { if(c->fmt==FMT_HTML) html_arch(c); else md_arch(c); }
static void gen_exs(doc_ctx_t *c)  { if(c->fmt==FMT_HTML) html_exs(c);  else md_exs(c); }
static void gen_xref(doc_ctx_t *c) { build_xrefs(c); if(c->fmt==FMT_HTML) html_xref(c); else md_xref(c); }
static void gen_idx(doc_ctx_t *c)  { if(c->fmt==FMT_HTML) html_idx(c);  else md_idx(c); }
static void gen_full(doc_ctx_t *c) {
    if(c->fmt==FMT_HTML) html_hdr(c);
    gen_idx(c); gen_api(c); gen_arch(c); gen_exs(c); gen_xref(c);
    if(c->fmt==FMT_HTML) html_ftr(c);
    oflush(c);
}

/* ---- Test suite ---- */
static int t_strings(void) {
    char b[48]; scpy(b,"hello",48); if(slen(b)!=5) return 0;
    scat(b," world",48); if(seq(b,"hello world")) return 0;
    char n[12]; i2s(42,n,12); if(seq(n,"42")) return 0;
    i2s(-7,n,12); if(seq(n,"-7")) return 0;
    i2s(0,n,12); if(seq(n,"0")) return 0;
    return 1;
}
static int t_ctx(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    return (c.mcnt==0 && c.ecnt==0 && c.xcnt==0 && c.fmt==FMT_MD);
}
static int t_mod(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *m = cadd_mod(&c,"sched","1.2.0","Task scheduler");
    return (m && c.mcnt==1 && !seq(m->name,"sched") && !seq(m->ver,"1.2.0"));
}
static int t_func(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *m = cadd_mod(&c,"mem","2.0","Memory");
    doc_func_t *f = madd_func(m,"alloc","void*","void* alloc(uint,uint)","Alloc mem",1);
    return (f && m->fcnt==1 && !seq(f->name,"alloc") && f->exported==1);
}
static int t_param(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *m = cadd_mod(&c,"io","1.0","I/O");
    doc_func_t *f = madd_func(m,"read","int","int read(int fd,void* buf,uint n)","Read bytes",1);
    fadd_param(f,"fd","int","in","File desc");
    fadd_param(f,"buf","void*","out","Buffer");
    fadd_param(f,"n","uint","in","Byte count");
    return (f->pcnt==3 && !seq(f->params[0].name,"fd") && !seq(f->params[1].type,"void*"));
}
static int t_impexp(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *m = cadd_mod(&c,"net","3.0","Network");
    madd_imp(m,"mem"); madd_imp(m,"io"); madd_exp(m,"tcp_connect"); madd_exp(m,"udp_send");
    return (m->icnt==2 && m->ecnt==2 && !seq(m->imports[0],"mem") && !seq(m->exports[1],"udp_send"));
}
static int t_ex(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    cadd_ex(&c,"Alloc","void* p=alloc(1024,16);","Allocate 1K aligned");
    return (c.ecnt==1 && !seq(c.examples[0].title,"Alloc"));
}
static int t_xref(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *a = cadd_mod(&c,"alpha","1.0","A");
    doc_module_t *b = cadd_mod(&c,"beta","1.0","B");
    madd_imp(a,"beta"); madd_exp(b,"beta_init");
    build_xrefs(&c);
    return (c.xcnt==2 && !seq(c.xrefs[0].src,"alpha") && !seq(c.xrefs[0].rel,"imports") && !seq(c.xrefs[1].rel,"exports_to"));
}
static int t_out(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    oapp(&c,"hello"); oapp(&c," "); oapp(&c,"world");
    char *b=(char*)c.obuf;
    int ok = (!seq(b,"hello world") && c.opos==11);
    oflush(&c); return ok;
}
static int t_html(void) {
    doc_ctx_t c; cinit(&c,FMT_HTML);
    doc_module_t *m = cadd_mod(&c,"demo","0.1","Demo");
    madd_func(m,"init","int","int init(void)","Init",1);
    cadd_ex(&c,"Init","init();","Call init");
    gen_full(&c); return 1;
}
static int t_md(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    doc_module_t *m = cadd_mod(&c,"log","1.0","Logging");
    doc_func_t *f = madd_func(m,"log_msg","void","void log_msg(int,const char*)","Log msg",1);
    fadd_param(f,"level","int","in","Level"); fadd_param(f,"msg","const char*","in","Message");
    madd_imp(m,"io"); cadd_ex(&c,"Log","log_msg(2,\"hi\");","Info log");
    gen_full(&c); return 1;
}
static int t_idx(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    cadd_mod(&c,"mod_a","1.0","First"); cadd_mod(&c,"mod_b","2.0","Second"); cadd_mod(&c,"mod_c","3.0","Third");
    gen_idx(&c); oflush(&c); return (c.mcnt==3);
}
static int t_arch(void) {
    doc_ctx_t c; cinit(&c,FMT_MD);
    cadd_mod(&c,"sched","1.0","S"); cadd_mod(&c,"mem","1.0","M");
    cadd_mod(&c,"fs","1.0","F"); cadd_mod(&c,"net","1.0","N");
    gen_arch(&c); oflush(&c); return 1;
}
static int t_full_integration(void) {
    doc_ctx_t c; cinit(&c, FMT_MD);
    doc_module_t *sched = cadd_mod(&c, "scheduler", "2.1.0", "Cooperative task scheduler");
    doc_func_t *f1 = madd_func(sched, "sched_start", "int",
                               "int sched_start(uint tick_ms)",
                               "Start the scheduler with given tick rate", 1);
    fadd_param(f1, "tick_ms", "uint", "in", "Tick interval in milliseconds");
    doc_func_t *f2 = madd_func(sched, "sched_yield", "void", "void sched_yield(void)",
                               "Yield current time slice", 1);
    doc_func_t *f3 = madd_func(sched, "sched_internal", "void", "void sched_internal(void)",
                               "Internal tick handler (not exported)", 0);
    madd_imp(sched, "mem"); madd_imp(sched, "timer");
    madd_exp(sched, "sched_start"); madd_exp(sched, "sched_yield");

    doc_module_t *timer = cadd_mod(&c, "timer", "1.0.0", "Hardware timer abstraction");
    doc_func_t *f4 = madd_func(timer, "timer_init", "int", "int timer_init(uint freq)",
                               "Initialize hardware timer", 1);
    fadd_param(f4, "freq", "uint", "in", "Timer frequency in Hz");
    madd_imp(timer, "arch");
    madd_exp(timer, "timer_init");

    cadd_ex(&c, "Start scheduler",
            "timer_init(1000);\nsched_start(10);",
            "Initialize timer at 1kHz then start scheduler with 10ms ticks");
    cadd_ex(&c, "Yield task", "sched_yield();", "Voluntarily yield the current time slice");

    gen_full(&c);
    return (c.mcnt == 2 && c.ecnt == 2 && c.xcnt > 0);
}

static int t_boundary(void) {
    doc_ctx_t c; cinit(&c, FMT_MD);
    for (unsigned int i = 0; i < 10; i++) {
        char nbuf[MAX_NAME];
        nbuf[0]='m'; nbuf[1]='_'; nbuf[2]='0'+(char)i; nbuf[3]='\0';
        doc_module_t *m = cadd_mod(&c, nbuf, "1.0", "Test mod");
        if (!m) return 0;
        madd_func(m, "fn", "void", "void fn(void)", "Test func", 1);
        madd_imp(m, "core");
        madd_exp(m, "fn");
    }
    build_xrefs(&c);
    return (c.mcnt == 10 && c.xcnt == 20);
}

static int t_empty(void) {
    doc_ctx_t c; cinit(&c, FMT_MD);
    gen_full(&c); return (c.mcnt==0);
}

static void run_t(const char *name, int (*fn)(void)) {
    host_print("  [TEST] "); host_print(name); host_print(" ... ");
    if(fn()) host_print("PASS\n"); else host_print("** FAIL **\n");
}
static void run_suite(void) {
    host_print("=== doc_generator test suite (v46.0) ===\n\n");
    run_t("string_helpers",    t_strings);
    run_t("ctx_init",          t_ctx);
    run_t("module_reg",        t_mod);
    run_t("func_reg",          t_func);
    run_t("param_reg",         t_param);
    run_t("import_export",     t_impexp);
    run_t("example_reg",       t_ex);
    run_t("xref_build",        t_xref);
    run_t("output_append",     t_out);
    run_t("html_format",       t_html);
    run_t("markdown_gen",      t_md);
    run_t("multi_module_idx",  t_idx);
    run_t("arch_diagram",      t_arch);
    run_t("full_integration",  t_full_integration);
    run_t("boundary_10mods",   t_boundary);
    run_t("empty_context",     t_empty);
    host_print("\n=== all tests complete ===\n");
}

static void show_help(void) {
    host_print("doc_generator - AI-ASM OS v46.0\n\n");
    host_print("Usage: doc_generator [options]\n\n");
    host_print("Options:\n  -h          Show help\n  -t          Run tests\n");
    host_print("  -f md       Markdown output (default)\n  -f html     HTML output\n\n");
    host_print("Features:\n  API docs, arch diagrams, examples,\n");
    host_print("  cross-ref maps, MD/HTML output, module index\n");
}

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    int argc = host_get_argv(buf, 512);
    char *args = (char *)buf;
    if(argc <= 1) { run_suite(); host_exit(0); }
    for(int i=1; i<argc; i++) {
        char *p = args; int skip=0;
        for(int j=1; j<i && !skip; j++) { while(*p) p++; p++; if((unsigned int)(p-args)>=512) skip=1; }
        if(skip) break;
        if(seq(p,"-h")==0) { show_help(); host_exit(0); }
        if(seq(p,"-t")==0) { run_suite(); host_exit(0); }
    }
    show_help(); host_exit(0);
}
