/* playground_wasm: WASM playground environment (v46.0)
 *
 * Interactive WebAssembly development playground:
 * - Code editor (line-based editing, syntax highlighting markers)
 * - Live compilation preview (status tracking, error reporting)
 * - Module runner (sandboxed WASM execution)
 * - Output capture (stdout/stderr collection)
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

#define MAX_LINES   128
#define MAX_LLEN    96
#define MAX_MODS    16
#define MAX_OUT     512
#define MAX_SESS    8
#define MAX_HIST    32
#define MAX_TMPL    8
#define MAX_ERRS    16
#define MAX_NM      24

#define TOK_NONE 0
#define TOK_KW 1
#define TOK_STR 2
#define TOK_CMT 3
#define TOK_NUM 4
#define TOK_TYP 5
#define TOK_FN 6
#define TOK_OP 7
#define TOK_PP 8

#define C_IDLE 0
#define C_OK 2
#define C_ERR 3
#define C_WARN 4
#define R_IDLE 0
#define R_DONE 2
#define R_CRASH 3

typedef struct { int line,cs,ce,type; } hl_t;
typedef struct { char t[MAX_LLEN]; int len,dirty,bp; hl_t hl[4]; int hlc; } eline_t;
typedef struct { int line,col,sev; char msg[48]; } cerr_t;
typedef struct { int status,errs,warns,time; unsigned int wsz; cerr_t errors[MAX_ERRS]; } cres_t;
typedef struct { char d[MAX_OUT]; int ol,el,ooff,eoff,ec,time,status,mu,mp; } out_t;
typedef struct { int active; char name[MAX_NM]; unsigned int wsz,mp; int cs,rs,ec,runs,cr; } mod_t;
typedef struct { int active; char name[MAX_NM]; int lines,cr,mod,as; } sess_t;
typedef struct { int active; char a[40]; int ts,det; } hist_t;
typedef struct { char name[MAX_NM]; char desc[48]; int lines,cat; } tmpl_t;
typedef struct { eline_t lines[MAX_LINES]; int cnt,cl,cc,scr,mod,edits; char fn[MAX_NM]; } ed_t;

static ed_t editor;
static mod_t modules[MAX_MODS];
static sess_t sessions[MAX_SESS];
static hist_t history[MAX_HIST];
static tmpl_t templates[MAX_TMPL];
static out_t output;
static cres_t lcomp;
static int mc,sc,hc,tc,clk;

static int slen(const char *s){int l=0;while(s[l])l++;return l;}
static int scmp(const char *a,const char *b){while(*a&&*b){if(*a!=*b)return *a-*b;a++;b++;}return *a-*b;}
static void scpy(char *d,const char *s,int n){int i=0;while(i<n-1&&s[i]){d[i]=s[i];i++;}d[i]='\0';}
static void mset(void *d,int v,int n){char *p=(char*)d;for(int i=0;i<n;i++)p[i]=(char)v;}
static void ps(const char *s){host_print(s);}
static void pi(int v){
    char b[20];int p=0;
    if(v<0){b[p++]='-';v=-v;}
    if(v==0)b[p++]='0';
    else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}}
    host_print(b);
}

static void hadd(const char *a,int d){
    if(hc>=MAX_HIST){for(int i=0;i<MAX_HIST-1;i++)history[i]=history[i+1];hc=MAX_HIST-1;}
    int i=hc++;history[i].active=1;scpy(history[i].a,a,39);history[i].ts=++clk;history[i].det=d;
}

static void edinit(void){
    mset(&editor,0,sizeof(editor));editor.cnt=1;
    scpy(editor.fn,"untitled.c",MAX_NM-1);hadd("edinit",0);
}

static int edins(int at,const char *t){
    if(at<0||at>editor.cnt||editor.cnt>=MAX_LINES)return -1;
    for(int i=editor.cnt;i>at;i--)editor.lines[i]=editor.lines[i-1];
    scpy(editor.lines[at].t,t,MAX_LLEN-1);
    editor.lines[at].len=slen(editor.lines[at].t);
    editor.lines[at].dirty=1;editor.cnt++;editor.mod=1;editor.edits++;return 0;
}

static int eddel(int at){
    if(at<0||at>=editor.cnt||editor.cnt<=1)return -1;
    for(int i=at;i<editor.cnt-1;i++)editor.lines[i]=editor.lines[i+1];
    editor.cnt--;editor.mod=1;editor.edits++;return 0;
}

static int iskw(const char *w){
    const char *k[]={"if","else","for","while","return","int","void","char",
        "const","static","struct","switch","case","break"};
    for(int i=0;i<14;i++)if(scmp(w,k[i])==0)return 1;return 0;
}

static void edhl(int ln){
    if(ln<0||ln>=editor.cnt)return;
    eline_t *e=&editor.lines[ln];e->hlc=0;
    int i=0,len=e->len;
    while(i<len&&e->hlc<4){
        while(i<len&&(e->t[i]==' '||e->t[i]=='\t'))i++;
        if(i>=len)break;
        char c=e->t[i];
        if(c=='/'&&i+1<len&&e->t[i+1]=='/'){e->hl[e->hlc++]=(hl_t){ln,i,len,TOK_CMT};break;}
        if(c=='"'){int s=i;i++;while(i<len&&e->t[i]!='"'){if(e->t[i]=='\\')i++;i++;}
            if(i<len)i++;e->hl[e->hlc++]=(hl_t){ln,s,i,TOK_STR};continue;}
        if(c>='0'&&c<='9'){int s=i;while(i<len&&((e->t[i]>='0'&&e->t[i]<='9')||e->t[i]=='x'))i++;
            e->hl[e->hlc++]=(hl_t){ln,s,i,TOK_NUM};continue;}
        if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='_'){
            int s=i;char w[24];int wl=0;
            while(i<len&&((e->t[i]>='a'&&e->t[i]<='z')||(e->t[i]>='A'&&e->t[i]<='Z')||
                  (e->t[i]>='0'&&e->t[i]<='9')||e->t[i]=='_')&&wl<23)w[wl++]=e->t[i++];
            w[wl]='\0';int tok=TOK_NONE;
            if(scmp(w,"int")==0||scmp(w,"void")==0||scmp(w,"char")==0)tok=TOK_TYP;
            else if(iskw(w))tok=TOK_KW;
            else if(i<len&&e->t[i]=='(')tok=TOK_FN;
            if(tok!=TOK_NONE)e->hl[e->hlc++]=(hl_t){ln,s,i,tok};continue;
        }
        if(c=='#'){e->hl[e->hlc++]=(hl_t){ln,i,len,TOK_PP};break;}
        if(c=='+'||c=='-'||c=='*'||c=='/'||c=='='||c=='<'||c=='>'){
            e->hl[e->hlc++]=(hl_t){ln,i,i+1,TOK_OP};i++;continue;}
        i++;
    }
}

static void edhla(void){for(int i=0;i<editor.cnt;i++)edhl(i);}

static const char *tn(int t){
    switch(t){case TOK_KW:return"KW";case TOK_STR:return"STR";case TOK_CMT:return"CMT";
        case TOK_NUM:return"NUM";case TOK_TYP:return"TYP";case TOK_FN:return"FN";
        case TOK_OP:return"OP";case TOK_PP:return"PP";default:return"--";}
}

static int docompile(void){
    lcomp.status=C_IDLE;lcomp.errs=0;lcomp.warns=0;
    int hm=0,br=0;
    for(int i=0;i<editor.cnt;i++){
        const char *t=editor.lines[i].t;
        for(int j=0;t[j]&&t[j+1];j++){
            if(t[j]=='m'&&t[j+1]=='a'&&t[j+2]=='i'&&t[j+3]=='n')hm=1;
            if(t[j]=='_'&&t[j+1]=='s'&&t[j+2]=='t'&&t[j+3]=='a')hm=1;
        }
        for(int j=0;t[j];j++){if(t[j]=='{')br++;if(t[j]=='}')br--;}
        if(t[0]=='#'){int sl=slen(t);
            if(sl>10&&t[sl-1]!='>'&&t[sl-1]!='"'){
                int ei=lcomp.errs+lcomp.warns;
                if(ei<MAX_ERRS){lcomp.errors[ei]=(cerr_t){i,sl-1,C_ERR,"bad #include"};lcomp.errs++;}
            }
        }
    }
    if(!hm){int ei=lcomp.errs+lcomp.warns;
        if(ei<MAX_ERRS){lcomp.errors[ei]=(cerr_t){0,0,C_WARN,"no entry"};lcomp.warns++;}}
    if(br!=0){int ei=lcomp.errs+lcomp.warns;
        if(ei<MAX_ERRS){lcomp.errors[ei]=(cerr_t){editor.cnt-1,0,C_ERR,"unbalanced braces"};lcomp.errs++;}}
    lcomp.status=lcomp.errs>0?C_ERR:(lcomp.warns>0?C_WARN:C_OK);
    lcomp.wsz=256+(unsigned int)(editor.cnt*14);lcomp.time=editor.cnt*2+5;
    hadd("compile",lcomp.status);return lcomp.status;
}

static void pcomp(void){
    const char *s=lcomp.status==C_OK?"OK":lcomp.status==C_ERR?"ERR":"WARN";
    ps("  Status: ");ps(s);ps("\n  Errs: ");pi(lcomp.errs);ps("\n  Warns: ");pi(lcomp.warns);
    ps("\n  Time: ");pi(lcomp.time);ps("ms\n  WASM: ");pi((int)lcomp.wsz);ps("b\n");
    int tot=lcomp.errs+lcomp.warns;
    for(int i=0;i<tot&&i<MAX_ERRS;i++){
        ps("    [");pi(lcomp.errors[i].line);ps(":");pi(lcomp.errors[i].col);ps("] ");
        ps(lcomp.errors[i].sev==C_ERR?"err: ":"warn: ");ps(lcomp.errors[i].msg);ps("\n");
    }
}

static void oreset(void){mset(&output,0,sizeof(output));output.status=R_IDLE;}
static void ostdout(const char *s){
    int l=slen(s);if(output.ooff+l>=MAX_OUT)l=MAX_OUT-output.ooff-1;
    for(int i=0;i<l;i++)output.d[output.ooff++]=s[i];
    output.d[output.ooff]='\0';output.ol=output.ooff;
}
static void ostderr(const char *s){
    int l=slen(s);int b=MAX_OUT/2;
    if(output.eoff+l>=MAX_OUT-b)l=MAX_OUT-b-output.eoff-1;
    for(int i=0;i<l;i++)output.d[b+output.eoff++]=s[i];output.el=output.eoff;
}

static int mcreate(const char *name){
    if(mc>=MAX_MODS)return -1;int idx=-1;
    for(int i=0;i<MAX_MODS;i++)if(!modules[i].active){idx=i;break;}
    if(idx<0)return -1;
    modules[idx].active=1;scpy(modules[idx].name,name,MAX_NM-1);
    modules[idx].wsz=lcomp.wsz;modules[idx].mp=1;modules[idx].cs=lcomp.status;
    modules[idx].rs=R_IDLE;modules[idx].ec=0;modules[idx].runs=0;modules[idx].cr=++clk;
    mc++;hadd("mcreate",idx);return idx;
}

static int mrun(int idx){
    if(idx<0||idx>=MAX_MODS||!modules[idx].active)return -1;
    if(modules[idx].cs==C_ERR)return -2;
    oreset();output.status=R_IDLE+1;
    int ln=editor.cnt;
    if(ln<3){
        modules[idx].rs=R_CRASH;modules[idx].ec=139;
        output.ec=139;output.status=R_CRASH;ostderr("segfault at 0x0\n");
    } else {
        ostdout("Hello from WASM playground!\nRan ");
        char lb[12];int p=0,v=ln;
        if(v==0)lb[p++]='0';else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;lb[p]='\0';p--;
            while(v>0){lb[p--]='0'+(v%10);v/=10;}}
        ostdout(lb);ostdout(" lines\n");
        modules[idx].rs=R_DONE;modules[idx].ec=0;output.ec=0;output.status=R_DONE;
    }
    output.time=ln*3+10;output.mu=(int)modules[idx].wsz+ln*8;output.mp=output.mu+256;
    modules[idx].runs++;hadd("mrun",modules[idx].rs);return modules[idx].ec;
}

static void pout(void){
    ps("  Exit: ");pi(output.ec);ps("\n  Time: ");pi(output.time);ps("ms\n  Mem: ");
    pi(output.mu);ps("/");pi(output.mp);ps("b\n  stdout: ");
    if(output.ol>0){output.d[output.ol]='\0';ps(output.d);}else ps("(empty)");
    ps("\n  stderr: ");
    if(output.el>0){int b=MAX_OUT/2;output.d[b+output.el]='\0';ps(&output.d[b]);}
    else ps("(empty)");ps("\n");
}

static int ssave(const char *name){
    if(sc>=MAX_SESS)return -1;int idx=-1;
    for(int i=0;i<MAX_SESS;i++)if(!sessions[i].active){idx=i;break;}
    if(idx<0)return -1;
    sessions[idx].active=1;scpy(sessions[idx].name,name,MAX_NM-1);
    sessions[idx].lines=editor.cnt;sessions[idx].cr=++clk;sessions[idx].mod=clk;
    sessions[idx].as=0;sc++;editor.mod=0;hadd("ssave",idx);return idx;
}

static void sload(int idx){
    if(idx<0||idx>=MAX_SESS||!sessions[idx].active)return;
    edinit();int lc=sessions[idx].lines;if(lc>MAX_LINES)lc=MAX_LINES;
    for(int i=1;i<lc;i++)edins(i,"// restored");
    scpy(editor.fn,sessions[idx].name,MAX_NM-1);editor.mod=0;hadd("sload",idx);
}

static void slist(void){
    ps("  Sessions:\n");int sh=0;
    for(int i=0;i<MAX_SESS;i++)if(sessions[i].active){
        ps("    [");pi(i);ps("] ");ps(sessions[i].name);
        ps(" (");pi(sessions[i].lines);ps(" lines)\n");sh++;
    }
    if(!sh)ps("    (none)\n");
}

static void tinit(void){
    const char *nm[]={"hello_wasm","fibonacci","memory_ops","sort_array","canvas_draw"};
    const char *ds[]={"Minimal hello","Recursive fib","Mem ops","Bubble sort","Pixel draw"};
    int ct[]={0,1,2,1,3},ln[]={5,8,9,10,7};
    for(int i=0;i<5;i++){
        scpy(templates[tc].name,nm[i],MAX_NM-1);scpy(templates[tc].desc,ds[i],47);
        templates[tc].cat=ct[i];templates[tc].lines=ln[i];tc++;
    }
}

static int tapply(int idx){
    if(idx<0||idx>=tc)return -1;
    edinit();scpy(editor.fn,templates[idx].name,MAX_NM-1);
    int c=templates[idx].cat;
    if(c==0){edins(0,"/* hi */");edins(1,"void _start(void){");edins(2,"  host_print(\"hi\");");edins(3,"}");}
    else if(c==1){edins(0,"/* algo */");edins(1,"int fib(int n){");edins(2,"  if(n<=1)return n;");
        edins(3,"  return fib(n-1)+fib(n-2);");edins(4,"}");edins(5,"void _start(void){");
        edins(6,"  int r=fib(10);");edins(7,"}");}
    else if(c==2){edins(0,"/* mem */");edins(1,"void _start(void){");
        edins(2,"  unsigned int p=host_alloc(64,8);");edins(3,"  char *b=(char*)p;");
        edins(4,"  b[0]='H';b[1]='i';b[2]=0;");edins(5,"  host_print(b);");
        edins(6,"  b[3]='!';b[4]=0;");edins(7,"  host_print(b);");edins(8,"}");}
    else{edins(0,"/* gfx */");edins(1,"void px(int x,int y,int c){");
        edins(2,"  int *p=(int*)(0x8000+(y*320+x)*4);*p=c;}");
        edins(3,"void _start(void){");edins(4,"  for(int i=0;i<100;i++)px(i,i,0xFF0000);");
        edins(5,"}");}
    editor.mod=0;hadd("tapply",idx);return 0;
}

static void tlist(void){
    const char *cn[]={"basic","algo","sys","gfx"};
    ps("  Templates:\n");
    for(int i=0;i<tc;i++){ps("    [");pi(i);ps("] ");ps(templates[i].name);
        ps(" (");ps(cn[templates[i].cat]);ps(") ");ps(templates[i].desc);ps("\n");}
}

static void edisp(void){
    ps("  File: ");ps(editor.fn);ps(" (");pi(editor.cnt);ps(" lines");
    if(editor.mod)ps(", modified");ps(")\n  Cursor: ");pi(editor.cl);ps(":");pi(editor.cc);ps("\n");
    int e=editor.scr+8;if(e>editor.cnt)e=editor.cnt;
    for(int i=editor.scr;i<e;i++){
        ps(i==editor.cl?"> ":"  ");pi(i+1);ps("|");
        if(editor.lines[i].bp)ps("*");
        if(editor.lines[i].hlc>0){ps("[");ps(tn(editor.lines[i].hl[0].type));ps("]");}
        ps(" ");ps(editor.lines[i].t);ps("\n");
    }
}

static void hdisp(void){
    ps("  History:\n");int s=hc-8;if(s<0)s=0;
    for(int i=s;i<hc;i++){ps("    [");pi(history[i].ts);ps("] ");
        ps(history[i].a);ps(" (");pi(history[i].det);ps(")\n");}
}

void _start(void){
    unsigned int buf=host_alloc(512,16);host_get_argv(buf,512);
    int help=0,test=0;unsigned int pos=0;char *ap=(char*)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){
        char *a=&ap[pos];
        if(scmp(a,"-h")==0||scmp(a,"--help")==0)help=1;
        else if(scmp(a,"-t")==0||scmp(a,"--test")==0)test=1;
        while(pos<512&&ap[pos])pos++;pos++;
    }
    ps("PlaygroundWASM v46.0 - WASM Playground Environment\n");
    if(help){
        ps("Usage: playground_wasm [options]\n");
        ps("  -h, --help    Show help\n  -t, --test    Run test suite\n\n");
        ps("Features: editor, compile preview, module runner,\n");
        ps("  output capture, sessions, templates\n");return;
    }
    if(test){
        ps("=== PlaygroundWASM Test Suite ===\n\n");
        edinit();tinit();

        ps("[Test 1] Editor\n");
        edins(0,"/* test */");edins(1,"void _start(void){");edins(2,"  int x=42;");
        edins(3,"  host_print(\"hi\");");edins(4,"}");
        ed_cursor: editor.cl=2;editor.cc=6;editor.lines[2].bp=1;
        edisp();ps("  edits: ");pi(editor.edits);ps("\n\n");

        ps("[Test 2] Highlighting\n");edhla();
        for(int i=0;i<editor.cnt;i++)if(editor.lines[i].hlc>0){
            ps("  L");pi(i);ps(": ");pi(editor.lines[i].hlc);ps(" tok(");
            for(int j=0;j<editor.lines[i].hlc;j++){if(j)ps(",");ps(tn(editor.lines[i].hl[j].type));}
            ps(")\n");
        }
        ps("\n");

        ps("[Test 3] Compile OK\n");pi(docompile());ps("\n");pcomp();ps("\n");

        ps("[Test 4] Compile ERR\n");
        edins(5,"#include <bad");edins(6,"void unclosed(){");
        pi(docompile());ps("\n");pcomp();eddel(6);eddel(5);ps("\n");

        ps("[Test 5] Module Run\n");
        int m=mcreate("test_mod");ps("  mod=");pi(m);ps("\n");
        pi(mrun(m));ps("\n");pout();ps("\n");

        ps("[Test 6] Module Crash\n");
        eddel(4);eddel(3);m=mcreate("crash");pi(mrun(m));ps("\n");pout();ps("\n");

        ps("[Test 7] Sessions\n");
        int s1=ssave("snip1"),s2=ssave("fib");
        ps("  saved: ");pi(s1);ps(",");pi(s2);ps("\n");slist();
        sload(s1);ps("  loaded (");pi(editor.cnt);ps(" lines)\n\n");

        ps("[Test 8] Templates\n");tlist();
        ps("  Apply fib:\n");tapply(1);ps("  lines=");pi(editor.cnt);ps("\n");edisp();ps("\n");

        ps("[Test 9] Pipeline\n");
        tapply(0);edins(3,"  host_print(\"world\");");edhla();
        pi(docompile());ps("\n");m=mcreate("pipe");pi(mrun(m));ps("\n");pout();ps("\n");

        ps("[Test 10] History\n");hdisp();ps("  total=");pi(hc);ps("\n\n");
        ps("=== PlaygroundWASM Test Complete ===\n");return;
    }
    ps("Use -h for help, -t for test\n");
}
