/* fuzzing_engine: Multi-target fuzzing with crash detection (v1.0) */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_TARGETS    16
#define MAX_CRASHES    64
#define MAX_COVERAGE   128
#define MAX_NAME       32
#define MAX_FIELDS     16
#define MAX_PATH       12

#define TARGET_WASM    0
#define TARGET_NET     1
#define TARGET_FS      2

#define MUT_BITFLIP    0
#define MUT_BYTEFLIP   1
#define MUT_ARITH      2
#define MUT_INTEREST   3
#define MUT_STRUCT     4

#define CRASH_NONE     0
#define CRASH_SEGFAULT 1
#define CRASH_STACK    2
#define CRASH_OOM      3
#define CRASH_TRAP     4
#define CRASH_TIMEOUT  5
#define CRASH_ASSERT   6

typedef struct { int id, addr, hits, taken, not_taken; } cov_site_t;
typedef struct { int rid, ttype, sig, pc, depth, cov, repro;
    char name[MAX_NAME], desc[MAX_NAME]; } crash_t;
typedef struct { int mid; char name[MAX_NAME]; int funcs, pages, blen;
    unsigned char code[128]; } wasm_t;
typedef struct { int proto; char name[MAX_NAME]; int fc, psz;
    unsigned char pkt[128]; int off[MAX_FIELDS], len[MAX_FIELDS]; } net_t;
typedef struct { char name[MAX_NAME]; int depth, bsz, inodes;
    char seg[MAX_PATH][MAX_NAME]; } fs_t;
typedef struct { int execs, crashes, unique, cov, t0; } stats_t;

static cov_site_t covs[MAX_COVERAGE]; static int cov_n = 0;
static crash_t crashes[MAX_CRASHES];  static int crash_n = 0, next_rid = 1;
static wasm_t wasms[MAX_TARGETS];     static int wasm_n = 0;
static net_t nets[MAX_TARGETS];       static int net_n = 0;
static fs_t fss[MAX_TARGETS];         static int fs_n = 0;
static stats_t st; static int clk = 0;

static unsigned int rng = 0xDEADBEEF;
static unsigned int rng_next(void) {
    unsigned int x = rng; x ^= x<<13; x ^= x>>17; x ^= x<<5; rng = x; return x;
}
static int rng_range(int a, int b) { return a>=b?a:a+(int)(rng_next()%(unsigned)(b-a)); }

static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while(*a&&*b) { if(*a!=*b) return *a-*b; a++; b++; } return *a-*b;
}
static void my_strncpy(char *d, const char *s, int n) {
    int i=0; while(i<n-1&&s[i]) { d[i]=s[i]; i++; } d[i]='\0';
}
static void ps(const char *s) { host_print(s); }
static void pi(int v) {
    char b[32]; int p=0;
    if(v<0){b[p++]='-';v=-v;} if(v==0) b[p++]='0';
    else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;
        while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b);
}
static void ph(int v) {
    char b[16]; const char *h="0123456789abcdef"; int p=0;
    if(!v) b[p++]='0';
    else{unsigned u=v;char t[12];int tp=0;while(u>0){t[tp++]=h[u&0xf];u>>=4;}
        b[p++]='0';b[p++]='x';for(int i=tp-1;i>=0;i--)b[p++]=t[i];}
    b[p]='\0'; host_print(b);
}

/* Coverage */
int fuzz_cov_register(int addr) {
    if(cov_n>=MAX_COVERAGE) return -1;
    covs[cov_n].id=cov_n; covs[cov_n].addr=addr; covs[cov_n].hits=0;
    covs[cov_n].taken=0; covs[cov_n].not_taken=0; return cov_n++;
}
int fuzz_cov_hit(int id, int taken) {
    if(id<0||id>=cov_n) return -1;
    covs[id].hits++; if(taken) covs[id].taken++; else covs[id].not_taken++;
    st.cov++; return covs[id].hits;
}

/* WASM fuzzing */
int fuzz_wasm_reg(const char *name, int funcs, int pages) {
    if(wasm_n>=MAX_TARGETS) return -1;
    wasms[wasm_n].mid=wasm_n; my_strncpy(wasms[wasm_n].name,name,MAX_NAME-1);
    wasms[wasm_n].funcs=funcs; wasms[wasm_n].pages=pages; wasms[wasm_n].blen=0;
    return wasm_n++;
}
int fuzz_wasm_load(int id, const unsigned char *code, int len) {
    if(id<0||id>=wasm_n) return -1;
    int c=len>128?128:len;
    for(int i=0;i<c;i++) wasms[id].code[i]=code[i];
    wasms[id].blen=c; return c;
}
int fuzz_wasm_mutate(int id, int strat) {
    if(id<0||id>=wasm_n||wasms[id].blen==0) return -1;
    int o=rng_range(0,wasms[id].blen), orig=wasms[id].code[o], m=orig;
    if(strat==MUT_BITFLIP) m=orig^(1<<rng_range(0,8));
    else if(strat==MUT_BYTEFLIP) m=orig^0xFF;
    else if(strat==MUT_ARITH) m=(orig+rng_range(-4,5))&0xFF;
    else if(strat==MUT_INTEREST){static const int v[]={0,1,0x7F,0x80,0xFF,0x0B};m=v[rng_range(0,6)];}
    else if(strat==MUT_STRUCT) m=(rng_range(0,6)<<5)|(orig&0x1F);
    else m=(int)rng_next()&0xFF;
    wasms[id].code[o]=(unsigned char)m; return o;
}

/* Network protocol fuzzing */
int fuzz_net_reg(const char *name, int proto, int sz) {
    if(net_n>=MAX_TARGETS) return -1;
    nets[net_n].proto=proto; my_strncpy(nets[net_n].name,name,MAX_NAME-1);
    nets[net_n].fc=0; nets[net_n].psz=sz>128?128:sz;
    for(int i=0;i<nets[net_n].psz;i++) nets[net_n].pkt[i]=(unsigned char)(i&0xFF);
    return net_n++;
}
int fuzz_net_field(int id, int off, int len) {
    if(id<0||id>=net_n||nets[id].fc>=MAX_FIELDS) return -1;
    int i=nets[id].fc; nets[id].off[i]=off; nets[id].len[i]=len; nets[id].fc++; return i;
}
int fuzz_net_mutate(int id, int strat) {
    if(id<0||id>=net_n||nets[id].psz==0) return -1;
    int o;
    if(strat==MUT_STRUCT&&nets[id].fc>0){
        int fi=rng_range(0,nets[id].fc);
        o=nets[id].off[fi]+rng_range(0,nets[id].len[fi]);
        if(o>=nets[id].psz) o=nets[id].psz-1;
    } else o=rng_range(0,nets[id].psz);
    int orig=nets[id].pkt[o], m;
    if(strat==MUT_BYTEFLIP) m=orig^0xFF;
    else if(strat==MUT_INTEREST){static const unsigned char v[]={0,1,0x7F,0x80,0xFF,0x0A};m=v[rng_range(0,6)];}
    else if(strat==MUT_STRUCT){static const unsigned char t[]={0x47,0x45,0x54,0x20,0x48,0x54,0x54,0x50};m=t[rng_range(0,8)];}
    else m=(int)rng_next()&0xFF;
    nets[id].pkt[o]=(unsigned char)m; return o;
}

/* File system fuzzing */
int fuzz_fs_reg(const char *name, int bsz) {
    if(fs_n>=MAX_TARGETS) return -1;
    my_strncpy(fss[fs_n].name,name,MAX_NAME-1); fss[fs_n].depth=0;
    fss[fs_n].bsz=bsz>0?bsz:4096; fss[fs_n].inodes=0; return fs_n++;
}
int fuzz_fs_seg(int id, const char *s) {
    if(id<0||id>=fs_n||fss[id].depth>=MAX_PATH) return -1;
    my_strncpy(fss[id].seg[fss[id].depth],s,MAX_NAME-1); return fss[id].depth++;
}
int fuzz_fs_mutpath(int id, int strat) {
    if(id<0||id>=fs_n||fss[id].depth==0) return -1;
    int sg=rng_range(0,fss[id].depth), len=my_strlen(fss[id].seg[sg]);
    if(strat==MUT_INTEREST){
        static const char *t[]={"..","../","/dev/null","\x00","CON","PRN"};
        my_strncpy(fss[id].seg[sg],t[rng_range(0,6)],MAX_NAME-1);
    } else if(strat==MUT_STRUCT){
        if(fss[id].depth<MAX_PATH){
            int src=rng_range(0,fss[id].depth);
            my_strncpy(fss[id].seg[fss[id].depth],fss[id].seg[src],MAX_NAME-1);
            fss[id].depth++;
        }
    } else if(strat==MUT_BYTEFLIP&&len>0) fss[id].seg[sg][rng_range(0,len)]^=0xFF;
    else if(len<MAX_NAME-2){
        int p=rng_range(0,len+1);
        for(int i=len;i>p;i--) fss[id].seg[sg][i]=fss[id].seg[sg][i-1];
        fss[id].seg[sg][p]=(char)(rng_next()&0xFF); fss[id].seg[sg][len+1]='\0';
    }
    return sg;
}
int fuzz_fs_mutinode(int id, int strat) {
    if(id<0||id>=fs_n) return -1;
    if(strat==MUT_ARITH){fss[id].bsz+=rng_range(-512,513);if(fss[id].bsz<=0)fss[id].bsz=512;}
    else if(strat==MUT_INTEREST){static const int s[]={0,1,512,4096,65536};fss[id].bsz=s[rng_range(0,5)];}
    else fss[id].bsz=rng_range(1,1048576);
    return ++fss[id].inodes;
}

/* Crash detection */
int fuzz_detect(int ttype, int result, int time_us, int depth, int covh) {
    int sig=CRASH_NONE;
    if(result==-1) sig=CRASH_SEGFAULT; else if(result==-2) sig=CRASH_STACK;
    else if(result==-3) sig=CRASH_OOM; else if(result==-4) sig=CRASH_TRAP;
    else if(result==-5) sig=CRASH_TIMEOUT; else if(result==-6) sig=CRASH_ASSERT;
    if(sig==CRASH_NONE||crash_n>=MAX_CRASHES) return sig==CRASH_NONE?0:-1;
    int i=crash_n;
    crashes[i].rid=next_rid++; crashes[i].ttype=ttype; crashes[i].sig=sig;
    crashes[i].pc=rng_range(0,0xFFFF); crashes[i].depth=depth; crashes[i].cov=covh;
    crashes[i].repro=rng_range(0,2);
    if(ttype==TARGET_WASM){my_strncpy(crashes[i].name,"wasm_module",MAX_NAME-1);
        my_strncpy(crashes[i].desc,"WASM execution trap",MAX_NAME-1);}
    else if(ttype==TARGET_NET){my_strncpy(crashes[i].name,"net_protocol",MAX_NAME-1);
        my_strncpy(crashes[i].desc,"Protocol parser crash",MAX_NAME-1);}
    else{my_strncpy(crashes[i].name,"filesystem",MAX_NAME-1);
        my_strncpy(crashes[i].desc,"FS metadata corruption",MAX_NAME-1);}
    crash_n++; st.crashes++; st.unique++; return sig;
}
static const char *signame(int s){
    if(s==CRASH_SEGFAULT)return"SIGSEGV";if(s==CRASH_STACK)return"STACK_OVERFLOW";
    if(s==CRASH_OOM)return"OUT_OF_MEMORY";if(s==CRASH_TRAP)return"WASM_TRAP";
    if(s==CRASH_TIMEOUT)return"TIMEOUT";if(s==CRASH_ASSERT)return"ASSERT_FAIL";
    return"UNKNOWN";
}
int fuzz_print_crash(int rid) {
    for(int i=0;i<crash_n;i++) if(crashes[i].rid==rid){
        ps("  Crash #");pi(crashes[i].rid);ps("\n  ----\n");
        ps("  Target: ");ps(crashes[i].name);ps("\n");
        ps("  Signal: ");ps(signame(crashes[i].sig));ps("\n");
        ps("  Desc:   ");ps(crashes[i].desc);ps("\n");
        ps("  PC: ");ph(crashes[i].pc);ps("  depth=");pi(crashes[i].depth);
        ps("  cov=");pi(crashes[i].cov);ps("  repro=");
        ps(crashes[i].repro?"YES":"NO");ps("\n\n"); return 0;
    } return -1;
}

int fuzz_print_stats(void){
    ps("  Engine Stats\n  ============================================================\n");
    ps("  Execs: ");pi(st.execs);ps("  Cov: ");pi(st.cov);ps(" hits\n");
    ps("  Crashes: ");pi(st.crashes);ps("  Unique: ");pi(st.unique);ps("\n");
    ps("  WASM: ");pi(wasm_n);ps("  Net: ");pi(net_n);ps("  FS: ");pi(fs_n);ps("\n\n");
    return 0;
}

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help=0, test=0; unsigned int pos=0; char *ap=(char*)buf;
    while(pos<512&&ap[pos])pos++;pos++;
    while(pos<512&&ap[pos]){char*a=&ap[pos];
        if(my_strcmp(a,"-h")==0||my_strcmp(a,"--help")==0)help=1;
        else if(my_strcmp(a,"-t")==0||my_strcmp(a,"--test")==0)test=1;
        while(pos<512&&ap[pos])pos++;pos++;}

    ps("FuzzingEngine v1.0 - Multi-Target Fuzzing & Crash Detection\n");
    if(help){ps("Usage: fuzzing_engine [-h] [-t]\n  -h  Help  -t  Test suite\n");return;}
    if(test){
        st.execs=0;st.crashes=0;st.unique=0;st.cov=0;st.t0=++clk; rng=0xCAFEBABE;
        ps("=== FuzzingEngine Test Suite ===\n\n");

        ps("[Test 1] Coverage Tracking\n");
        int s0=fuzz_cov_register(0x0000),s1=fuzz_cov_register(0x1000);
        int s2=fuzz_cov_register(0x2000),s3=fuzz_cov_register(0x3000);
        ps("  Registered ");pi(cov_n);ps(" sites\n");
        fuzz_cov_hit(s0,1);fuzz_cov_hit(s0,0);fuzz_cov_hit(s1,1);fuzz_cov_hit(s1,1);
        fuzz_cov_hit(s2,0);fuzz_cov_hit(s3,1);fuzz_cov_hit(s3,1);fuzz_cov_hit(s3,0);
        ps("  s0 hits=");pi(covs[s0].hits);ps(" s3 hits=");pi(covs[s3].hits);
        ps(" (most exercised)\n\n");

        ps("[Test 2] WASM Module Fuzzing\n");
        int w0=fuzz_wasm_reg("parser.wasm",12,4),w1=fuzz_wasm_reg("compute.wasm",8,2);
        ps("  Targets: ");ps(wasms[w0].name);ps(", ");ps(wasms[w1].name);ps("\n");
        unsigned char ws[]={0x00,0x61,0x73,0x6D,0x01,0x00,0x00,0x00,
            0x01,0x04,0x01,0x60,0x00,0x00,0x03,0x02,0x01,0x00,0x0A,0x04};
        fuzz_wasm_load(w0,ws,20); ps("  Loaded 20-byte seed\n");
        int m1=fuzz_wasm_mutate(w0,MUT_BITFLIP),m2=fuzz_wasm_mutate(w0,MUT_BYTEFLIP);
        int m3=fuzz_wasm_mutate(w0,MUT_INTEREST),m4=fuzz_wasm_mutate(w0,MUT_STRUCT);
        ps("  Mutations at: ");pi(m1);ps(", ");pi(m2);ps(", ");pi(m3);ps(", ");pi(m4);ps("\n\n");

        ps("[Test 3] Network Protocol Fuzzing\n");
        int n0=fuzz_net_reg("http_server",1,32),n1=fuzz_net_reg("dns_resolver",2,16);
        ps("  Targets: ");ps(nets[n0].name);ps(", ");ps(nets[n1].name);ps("\n");
        fuzz_net_field(n0,0,4);fuzz_net_field(n0,4,16);fuzz_net_field(n0,20,8);
        ps("  HTTP fields: 3\n");
        int p0=fuzz_net_mutate(n0,MUT_STRUCT),p1=fuzz_net_mutate(n0,MUT_BYTEFLIP);
        int p2=fuzz_net_mutate(n0,MUT_INTEREST),p3=fuzz_net_mutate(n1,MUT_INTEREST);
        ps("  Packet mut at: ");pi(p0);ps(", ");pi(p1);ps(", ");pi(p2);ps(", ");pi(p3);ps("\n\n");

        ps("[Test 4] File System Fuzzing\n");
        int f0=fuzz_fs_reg("ext4_root",4096),f1=fuzz_fs_reg("fat32_usb",512);
        ps("  Targets: ");ps(fss[f0].name);ps(", ");ps(fss[f1].name);ps("\n");
        fuzz_fs_seg(f0,"home");fuzz_fs_seg(f0,"user");fuzz_fs_seg(f0,"data");fuzz_fs_seg(f0,"file.txt");
        ps("  Path depth: ");pi(fss[f0].depth);ps("\n");
        int pm0=fuzz_fs_mutpath(f0,MUT_INTEREST),pm1=fuzz_fs_mutpath(f0,MUT_STRUCT);
        int pm2=fuzz_fs_mutpath(f0,MUT_BYTEFLIP);
        ps("  Path mut segs: ");pi(pm0);ps(", ");pi(pm1);ps(", ");pi(pm2);ps("\n");
        ps("  New depth: ");pi(fss[f0].depth);ps("\n");
        int in0=fuzz_fs_mutinode(f0,MUT_ARITH),in1=fuzz_fs_mutinode(f0,MUT_INTEREST);
        ps("  Inode muts: ");pi(in0);ps(", ");pi(in1);ps(" bsz=");pi(fss[f0].bsz);ps("\n\n");

        ps("[Test 5] Crash Detection\n");
        int c1=fuzz_detect(TARGET_WASM,-4,250,3,15);
        ps("  WASM: ");ps(signame(c1));ps("\n");
        int c2=fuzz_detect(TARGET_NET,-1,5000,7,42);
        ps("  NET:  ");ps(signame(c2));ps("\n");
        int c3=fuzz_detect(TARGET_FS,-3,120000,12,88);
        ps("  FS:   ");ps(signame(c3));ps("\n");
        int c4=fuzz_detect(TARGET_WASM,-2,100,1,5);
        ps("  WASM: ");ps(signame(c4));ps("\n");
        int c5=fuzz_detect(TARGET_WASM,0,50,1,10);
        ps("  Normal: ");pi(c5);ps(" (no crash)\n\n");

        ps("[Test 6] Crash Reports\n");
        fuzz_print_crash(1);fuzz_print_crash(3);

        ps("[Test 7] Engine Statistics\n");
        st.execs=15234; fuzz_print_stats();
        ps("=== FuzzingEngine Test Complete ===\n");
        return;
    }
    ps("Use -h for help, -t for test\n");
}
