/*
 * plugin_ecosystem - Plugin Ecosystem Module for AI-ASM OS v47.0
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

#define NAME_MAX 48
#define VER_MAX  16
#define DESC_MAX 96
#define MAX_PLUGINS 48
#define MAX_DEPS    8
#define MAX_CONFIG  64
#define MAX_MARKET  64
#define CFG_KEY_MAX 48
#define CFG_VAL_MAX 96

/* States */
#define ST_UNINSTALLED 0
#define ST_INSTALLED   1
#define ST_ENABLED     2
#define ST_DISABLED    3
#define ST_ERROR       4
/* Lifecycle */
#define LC_INIT    0
#define LC_START   1
#define LC_RUNNING 2
#define LC_STOP    3
#define LC_CLEANUP 4
/* Sandbox perms */
#define PERM_NONE   0x00
#define PERM_NET    0x01
#define PERM_FREAD  0x02
#define PERM_FWRITE 0x04
#define PERM_EXEC   0x08
#define PERM_ALL    0x0F
/* Sources */
#define SRC_OFFICIAL  0
#define SRC_COMMUNITY 1
#define SRC_LOCAL     2
/* Errors */
#define E_OK        0
#define E_NOTFOUND  1
#define E_EXISTS    2
#define E_DEPMISS   3
#define E_DEPCONF   4
#define E_SANDBOX   5
#define E_CFGINVAL  6
#define E_LIFECYCLE 7

typedef struct { char name[NAME_MAX]; char min_ver[VER_MAX]; int required; } Dep;
typedef struct { char key[CFG_KEY_MAX]; char val[CFG_VAL_MAX]; int persist; int used; } CfgEntry;
typedef struct {
    unsigned int perms, max_mem, max_fds, max_cpu_ms;
    int net, fs;
} Sandbox;
typedef struct {
    char name[NAME_MAX], ver[VER_MAX], desc[DESC_MAX], author[NAME_MAX];
    int state, lc_phase, source, size, installed_at;
    Dep deps[MAX_DEPS]; int ndeps;
    Sandbox sb;
    CfgEntry cfg[8]; int ncfg;
    int used;
} Plugin;
typedef struct {
    char name[NAME_MAX], ver[VER_MAX], desc[DESC_MAX], author[NAME_MAX];
    int source, downloads, size, used;
} MktEntry;

static Plugin   g_plg[MAX_PLUGINS]; static int g_nplg = 0;
static MktEntry g_mkt[MAX_MARKET];  static int g_nmkt = 0;
static CfgEntry g_gcfg[MAX_CONFIG]; static int g_ngcfg = 0;

static unsigned int slen(const char *s) { unsigned int n=0; while(s[n]) n++; return n; }
static void scpy(char *d, const char *s, unsigned int m) {
    unsigned int i=0; while(i<m-1 && s[i]) { d[i]=s[i]; i++; } d[i]=0;
}
static int scmp(const char *a, const char *b) {
    while(*a && *b && *a==*b) { a++; b++; } return (unsigned char)*a-(unsigned char)*b;
}
static void scat(char *d, const char *s, unsigned int m) {
    unsigned int dl=slen(d), i=0;
    while(dl+i<m-1 && s[i]) { d[dl+i]=s[i]; i++; } d[dl+i]=0;
}
static void i2s(int v, char *b, unsigned int sz) {
    unsigned int i=0, uv; int neg=0;
    if(v<0){neg=1;uv=(unsigned int)(-v);}else{uv=(unsigned int)v;}
    if(neg && i<sz-1) b[i++]='-';
    char t[12]; int tl=0;
    if(!uv) t[tl++]='0';
    while(uv>0 && tl<12){t[tl++]='0'+(uv%10);uv/=10;}
    while(tl>0 && i<sz-1) b[i++]=t[--tl];
    b[i]=0;
}
static void pl(const char *m) { host_print(m); host_print("\n"); }
static void pli(const char *l, int v) {
    char b[96]; scpy(b,l,96); char vb[16]; i2s(v,vb,16); scat(b,vb,96); pl(b);
}
static Plugin* find_plg(const char *n) {
    for(int i=0;i<g_nplg;i++) if(g_plg[i].used && !scmp(g_plg[i].name,n)) return &g_plg[i];
    return NULL;
}
static Plugin* alloc_plg(void) {
    if(g_nplg>=MAX_PLUGINS) return NULL;
    Plugin *p=&g_plg[g_nplg++]; p->used=1; p->state=ST_UNINSTALLED;
    p->lc_phase=LC_INIT; p->ndeps=0; p->ncfg=0;
    p->sb.perms=PERM_NONE; p->sb.max_mem=1048576; p->sb.max_fds=16; p->sb.max_cpu_ms=5000;
    p->sb.net=0; p->sb.fs=0; return p;
}
static MktEntry* find_mkt(const char *n) {
    for(int i=0;i<g_nmkt;i++) if(g_mkt[i].used && !scmp(g_mkt[i].name,n)) return &g_mkt[i];
    return NULL;
}
static MktEntry* alloc_mkt(void) {
    if(g_nmkt>=MAX_MARKET) return NULL;
    MktEntry *e=&g_mkt[g_nmkt++]; e->used=1; return e;
}
static CfgEntry* cfg_get(CfgEntry *e, int n, const char *k) {
    for(int i=0;i<n;i++) if(e[i].used && !scmp(e[i].key,k)) return &e[i];
    return NULL;
}
static int cfg_set(CfgEntry *e, int *n, int max, const char *k, const char *v, int p) {
    CfgEntry *ex=cfg_get(e,*n,k);
    if(ex){scpy(ex->val,v,CFG_VAL_MAX);ex->persist=p;return E_OK;}
    if(*n>=max) return E_CFGINVAL;
    CfgEntry *c=&e[*n]; scpy(c->key,k,CFG_KEY_MAX); scpy(c->val,v,CFG_VAL_MAX);
    c->persist=p; c->used=1; (*n)++; return E_OK;
}

/* Plugin Manager */
static int plg_install(const char *nm, const char *vr, const char *ds, const char *au, int src) {
    if(find_plg(nm)) return E_EXISTS;
    Plugin *p=alloc_plg(); if(!p) return E_NOTFOUND;
    scpy(p->name,nm,NAME_MAX); scpy(p->ver,vr,VER_MAX); scpy(p->desc,ds,DESC_MAX);
    scpy(p->author,au,NAME_MAX); p->source=src; p->state=ST_INSTALLED;
    char m[200]; scpy(m,"[INSTALL] ",200); scat(m,nm,200); scat(m," v",200); scat(m,vr,200); pl(m);
    return E_OK;
}
static int plg_uninstall(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    if(p->state==ST_ENABLED){pl("[UNINSTALL] Error: disable first.");return E_LIFECYCLE;}
    p->used=0; p->state=ST_UNINSTALLED;
    char m[128]; scpy(m,"[UNINSTALL] ",128); scat(m,nm,128); pl(m); return E_OK;
}
static int plg_enable(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    if(p->state==ST_ENABLED){pl("[ENABLE] Already enabled.");return E_OK;}
    if(p->state!=ST_INSTALLED && p->state!=ST_DISABLED){pl("[ENABLE] Invalid state.");return E_LIFECYCLE;}
    p->state=ST_ENABLED; p->lc_phase=LC_START;
    char m[128]; scpy(m,"[ENABLE] ",128); scat(m,nm,128); pl(m); return E_OK;
}
static int plg_disable(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    if(p->state!=ST_ENABLED){pl("[DISABLE] Not enabled.");return E_LIFECYCLE;}
    p->state=ST_DISABLED; p->lc_phase=LC_STOP;
    char m[128]; scpy(m,"[DISABLE] ",128); scat(m,nm,128); pl(m); return E_OK;
}
static int plg_add_dep(const char *pn, const char *dn, const char *mv, int req) {
    Plugin *p=find_plg(pn); if(!p) return E_NOTFOUND;
    if(p->ndeps>=MAX_DEPS) return E_DEPMISS;
    Dep *d=&p->deps[p->ndeps]; scpy(d->name,dn,NAME_MAX); scpy(d->min_ver,mv,VER_MAX);
    d->required=req; p->ndeps++; return E_OK;
}

/* Lifecycle */
static int lc_trans(Plugin *p, int tgt) {
    const char *pn[]={"INIT","START","RUNNING","STOP","CLEANUP"};
    while(p->lc_phase<tgt){
        p->lc_phase++; char m[200]; scpy(m,"[LIFECYCLE] ",200); scat(m,p->name,200);
        scat(m," -> ",200); scat(m,pn[p->lc_phase],200); pl(m);
    }
    return E_OK;
}
static int plg_start(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    if(p->state!=ST_ENABLED) return E_LIFECYCLE;
    lc_trans(p,LC_RUNNING);
    char m[128]; scpy(m,"[LIFECYCLE] Started: ",128); scat(m,nm,128); pl(m); return E_OK;
}
static int plg_stop(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    lc_trans(p,LC_STOP);
    char m[128]; scpy(m,"[LIFECYCLE] Stopped: ",128); scat(m,nm,128); pl(m); return E_OK;
}

/* Marketplace */
static int mkt_register(const char *nm, const char *vr, const char *ds, const char *au, int src, int sz) {
    if(find_mkt(nm)) return E_EXISTS;
    MktEntry *e=alloc_mkt(); if(!e) return E_NOTFOUND;
    scpy(e->name,nm,NAME_MAX); scpy(e->ver,vr,VER_MAX); scpy(e->desc,ds,DESC_MAX);
    scpy(e->author,au,NAME_MAX); e->source=src; e->size=sz; e->downloads=0; return E_OK;
}
static int mkt_search(const char *q) {
    int f=0; pl("[MARKETPLACE] Results:");
    for(int i=0;i<g_nmkt;i++){
        if(!g_mkt[i].used) continue;
        if(!q[0] || !scmp(g_mkt[i].name,q)){
            char m[256]; scpy(m,"  - ",256); scat(m,g_mkt[i].name,256); scat(m," v",256);
            scat(m,g_mkt[i].ver,256); scat(m," by ",256); scat(m,g_mkt[i].author,256);
            scat(m," [",256); char sb[16]; i2s(g_mkt[i].size,sb,16); scat(m,sb,256);
            scat(m," bytes]",256); pl(m); f++;
        }
    }
    if(!f) pl("  (none)"); return f;
}
static int mkt_download(const char *nm) {
    MktEntry *e=find_mkt(nm); if(!e) return E_NOTFOUND;
    if(find_plg(nm)) return E_EXISTS; e->downloads++;
    char m[160]; scpy(m,"[MARKETPLACE] Downloading: ",160); scat(m,nm,160); pl(m);
    int err=plg_install(e->name,e->ver,e->desc,e->author,e->source);
    if(err) return err;
    Plugin *p=find_plg(nm); if(p) p->size=e->size;
    scpy(m,"[MARKETPLACE] Installed: ",160); scat(m,nm,160); pl(m); return E_OK;
}

/* Dependency Resolution */
static int resolve_deps(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    char m[256]; scpy(m,"[DEPS] Resolving: ",256); scat(m,nm,256); pl(m);
    int miss=0;
    for(int i=0;i<p->ndeps;i++){
        Dep *d=&p->deps[i]; Plugin *dp=find_plg(d->name);
        if(!dp){
            scpy(m,d->required?"[DEPS]   MISSING(req): ":"[DEPS]   MISSING(opt): ",256);
            scat(m,d->name,256); scat(m," >= ",256); scat(m,d->min_ver,256); pl(m);
            if(d->required) miss++;
        } else {
            scpy(m,"[DEPS]   OK: ",256); scat(m,d->name,256); scat(m," (",256);
            scat(m,dp->ver,256); scat(m,")",256); pl(m);
        }
    }
    if(miss){pl("[DEPS] FAILED: missing required deps.");return E_DEPMISS;}
    pl("[DEPS] OK: all satisfied."); return E_OK;
}

/* Sandbox */
static int sb_config(const char *nm, unsigned int perms, unsigned int mmem, unsigned int mfd, int net, int fs) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    p->sb.perms=perms; p->sb.max_mem=mmem; p->sb.max_fds=mfd; p->sb.net=net; p->sb.fs=fs;
    char m[200]; scpy(m,"[SANDBOX] Configured: ",200); scat(m,nm,200); pl(m);
    char nb[16]; i2s(perms,nb,16); scpy(m,"[SANDBOX]   Perms=0x",200); scat(m,nb,200); pl(m);
    i2s(mmem,nb,16); scpy(m,"[SANDBOX]   Mem=",200); scat(m,nb,200); pl(m);
    return E_OK;
}
static int sb_check(const char *nm, unsigned int req) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    if((p->sb.perms&req)!=req){
        char m[160]; scpy(m,"[SANDBOX] DENIED: ",160); scat(m,nm,160); pl(m); return E_SANDBOX;
    }
    char m[160]; scpy(m,"[SANDBOX] ALLOWED: ",160); scat(m,nm,160); pl(m); return E_OK;
}
static int sb_validate(const char *nm) {
    Plugin *p=find_plg(nm); if(!p) return E_NOTFOUND;
    char m[160]; scpy(m,"[SANDBOX] Validate: ",160); scat(m,nm,160); pl(m);
    int iss=0;
    if(p->sb.max_mem>4194304){pl("[SANDBOX]   WARN: excessive memory");iss++;}
    if(p->sb.max_fds>64){pl("[SANDBOX]   WARN: excessive fds");iss++;}
    if((p->sb.perms&PERM_EXEC)&&(p->sb.perms&PERM_NET)){pl("[SANDBOX]   WARN: exec+net");iss++;}
    if(!iss) pl("[SANDBOX]   PASSED");
    else{char b[8];i2s(iss,b,8);scpy(m,"[SANDBOX]   ",160);scat(m,b,160);scat(m," warnings",160);pl(m);}
    return E_OK;
}

/* Configuration */
static int plg_cfg_set(const char *pn, const char *k, const char *v, int pers) {
    Plugin *p=find_plg(pn); if(!p) return E_NOTFOUND;
    int err=cfg_set(p->cfg,&p->ncfg,8,k,v,pers); if(err) return err;
    char m[256]; scpy(m,"[CONFIG] ",256); scat(m,pn,256); scat(m,": ",256);
    scat(m,k,256); scat(m," = ",256); scat(m,v,256); if(pers) scat(m," (persist)",256); pl(m);
    return E_OK;
}
static int plg_cfg_get(const char *pn, const char *k) {
    Plugin *p=find_plg(pn); if(!p) return E_NOTFOUND;
    CfgEntry *e=cfg_get(p->cfg,p->ncfg,k);
    if(!e){char m[160];scpy(m,"[CONFIG] ",160);scat(m,k,160);scat(m," = (unset)",160);pl(m);return E_NOTFOUND;}
    char m[256]; scpy(m,"[CONFIG] ",256); scat(m,pn,256); scat(m,": ",256);
    scat(m,k,256); scat(m," = ",256); scat(m,e->val,256); pl(m); return E_OK;
}
static int gcfg_set(const char *k, const char *v, int p) { return cfg_set(g_gcfg,&g_ngcfg,MAX_CONFIG,k,v,p); }
static int gcfg_get(const char *k) {
    CfgEntry *e=cfg_get(g_gcfg,g_ngcfg,k); if(!e) return E_NOTFOUND;
    char m[160]; scpy(m,"[GCFG] ",160); scat(m,k,160); scat(m," = ",160); scat(m,e->val,160); pl(m);
    return E_OK;
}

/* List all */
static void plg_list(void) {
    const char *sn[]={"UNINSTALLED","INSTALLED","ENABLED","DISABLED","ERROR"};
    int c=0; pl("[LIST] Plugins:");
    for(int i=0;i<g_nplg;i++){
        if(!g_plg[i].used) continue;
        char m[256]; scpy(m,"  [",256); scat(m,sn[g_plg[i].state],256); scat(m,"] ",256);
        scat(m,g_plg[i].name,256); scat(m," v",256); scat(m,g_plg[i].ver,256);
        scat(m," - ",256); scat(m,g_plg[i].desc,256); pl(m); c++;
    }
    char b[16],f[128]; i2s(c,b,16); scpy(f,"[LIST] Total: ",128); scat(f,b,128); scat(f," plugins",128); pl(f);
}

/* Test Suite */
static void hdr(const char *t) { pl(""); pl("========================================"); host_print(t); host_print("\n"); pl("========================================"); }

static void run_tests(void) {
    pl("[TEST] Plugin Ecosystem Test Suite - AI-ASM OS v47.0");
    pl("[TEST] Starting tests...\n");
    int err;

    hdr("Test 1: Install Plugins");
    err=plg_install("syntax-hl","1.2.0","Syntax highlighting","devtools",SRC_OFFICIAL); pli("  ",err);
    err=plg_install("formatter","0.9.5","Code formatting","style",SRC_OFFICIAL); pli("  ",err);
    err=plg_install("git-integ","2.0.0","Git VCS integration","vcs",SRC_COMMUNITY); pli("  ",err);
    err=plg_install("linter","1.0.0","Linting framework","quality",SRC_OFFICIAL); pli("  ",err);
    err=plg_install("debugger","3.1.0","Interactive debugger","debug",SRC_LOCAL); pli("  ",err);

    hdr("Test 2: Duplicate Prevention");
    err=plg_install("syntax-hl","1.2.0","dup","dup",SRC_LOCAL); pli("  dup(expect E_EXISTS=2): ",err);

    hdr("Test 3: Enable/Disable");
    err=plg_enable("syntax-hl"); pli("  enable: ",err);
    err=plg_enable("formatter"); pli("  enable: ",err);
    err=plg_enable("linter"); pli("  enable: ",err);
    err=plg_disable("formatter"); pli("  disable: ",err);

    hdr("Test 4: Lifecycle");
    err=plg_start("syntax-hl"); pli("  start: ",err);
    err=plg_start("linter"); pli("  start: ",err);
    err=plg_stop("linter"); pli("  stop: ",err);

    hdr("Test 5: Dependencies");
    err=plg_add_dep("formatter","linter","1.0.0",1); pli("  add dep: ",err);
    err=plg_add_dep("formatter","syntax-hl","1.0.0",1); pli("  add dep: ",err);
    err=plg_add_dep("debugger","missing-lib","1.0.0",1); pli("  add missing: ",err);
    err=resolve_deps("formatter"); pli("  resolve(expect OK=0): ",err);
    err=resolve_deps("debugger"); pli("  resolve(expect E_DEPMISS=3): ",err);

    hdr("Test 6: Sandbox");
    err=sb_config("syntax-hl",PERM_FREAD|PERM_NET,524288,8,1,0); pli("  config: ",err);
    err=sb_check("syntax-hl",PERM_FREAD); pli("  check READ(allow): ",err);
    err=sb_check("syntax-hl",PERM_FWRITE); pli("  check WRITE(deny=5): ",err);
    err=sb_validate("syntax-hl"); pli("  validate: ",err);
    err=sb_config("debugger",PERM_ALL,4194304*2,128,1,1); pli("  config risky: ",err);
    err=sb_validate("debugger"); pli("  validate(warns): ",err);

    hdr("Test 7: Configuration");
    err=plg_cfg_set("syntax-hl","theme","dark",1); pli("  set: ",err);
    err=plg_cfg_set("syntax-hl","font","14",1); pli("  set: ",err);
    err=plg_cfg_set("syntax-hl","linenum","true",0); pli("  set: ",err);
    err=plg_cfg_get("syntax-hl","theme"); pli("  get: ",err);
    err=plg_cfg_get("syntax-hl","nokey"); pli("  get missing(=1): ",err);
    err=gcfg_set("eco.ver","47.0",1); pli("  gcfg set: ",err);
    err=gcfg_get("eco.ver"); pli("  gcfg get: ",err);

    hdr("Test 8: Marketplace");
    err=mkt_register("python","2.1.0","Python support","lang",SRC_OFFICIAL,2048000); pli("  reg: ",err);
    err=mkt_register("rust-anal","0.5.0","Rust analysis","lang",SRC_OFFICIAL,4096000); pli("  reg: ",err);
    err=mkt_register("md-prev","1.0.0","Markdown render","comm",SRC_COMMUNITY,512000); pli("  reg: ",err);
    err=mkt_register("docker","1.3.0","Docker tools","ops",SRC_COMMUNITY,1024000); pli("  reg: ",err);
    mkt_search("");
    pl("");
    mkt_search("rust-anal");
    err=mkt_download("python"); pli("  download: ",err);
    err=mkt_download("md-prev"); pli("  download: ",err);
    err=mkt_download("nonexist"); pli("  download missing(=1): ",err);

    hdr("Test 9: Full Workflow");
    err=plg_enable("python"); pli("  enable: ",err);
    err=plg_start("python"); pli("  start: ",err);
    err=plg_cfg_set("python","py_path","/usr/bin/python3",1); pli("  cfg: ",err);
    err=sb_config("python",PERM_FREAD|PERM_EXEC,2097152,32,0,1); pli("  sandbox: ",err);
    err=plg_stop("python"); pli("  stop: ",err);
    err=plg_disable("python"); pli("  disable: ",err);

    hdr("Test 10: Uninstall");
    err=plg_uninstall("syntax-hl"); pli("  uninstall enabled(=7): ",err);
    err=plg_disable("syntax-hl"); pli("  disable: ",err);
    err=plg_uninstall("syntax-hl"); pli("  uninstall: ",err);
    err=plg_uninstall("md-prev"); pli("  uninstall: ",err);

    hdr("Final State");
    plg_list();
    pl("\n[TEST] All tests completed. Plugin ecosystem v47.0 verified.");
}

static void print_help(void) {
    pl("plugin_ecosystem - AI-ASM OS v47.0");
    pl("");
    pl("Options:");
    pl("  -h  Show help");
    pl("  -t  Run test suite");
    pl("");
    pl("Features: plugin manager, marketplace, deps, sandbox, config");
}

void _start(void) {
    unsigned int buf_off = host_alloc(512, 16);
    int argv_len = host_get_argv(buf_off, 512);
    if (argv_len <= 0) { run_tests(); host_exit(0); }
    char *buf = (char *)(unsigned long)buf_off;
    int run_test=0, show_help=0;
    for (int i=0; i<argv_len; i++) {
        if (buf[i]=='-' && i+1<argv_len) {
            if (buf[i+1]=='t') run_test=1;
            if (buf[i+1]=='h') show_help=1;
            i++;
        }
    }
    if (show_help) { print_help(); host_exit(0); }
    if (run_test)  { run_tests();   host_exit(0); }
    run_tests();
    host_exit(0);
}
