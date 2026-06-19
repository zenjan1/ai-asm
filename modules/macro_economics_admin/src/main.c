/* macro_economics_admin: Macroeconomics administration system (v1.0)
 * National accounts, employment & prices, fiscal policy, monetary policy, international macro
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

#define MAX_NAT_ACC    16
#define MAX_EMP_PRI    14
#define MAX_FIS_POL    12
#define MAX_MON_POL    10
#define MAX_INT_MAC    10

typedef struct { int id; int type; int cat; int gdp; int n_inc; int n_out; int grth; int year; int active; } nat_acc_t;
typedef struct { int id; int type; int cat; int unemp; int infl; int cpi; int ppi; int year; int active; } emp_pri_t;
typedef struct { int id; int type; int cat; int g_exp; int tax; int fis_d; int debt; int year; int active; } fis_pol_t;
typedef struct { int id; int type; int cat; int m_sup; int intr; int cb_tool; int omo; int year; int active; } mon_pol_t;
typedef struct { int id; int type; int cat; int bop; int exch; int fx_r; int cap_a; int year; int active; } int_mac_t;

typedef struct {
    int n_nat; int n_emp; int n_fis; int n_mon; int n_int;
    int t_gdp; int t_unemp; int t_g_exp; int t_m_sup; int t_bop;
} mac_state_t;

static nat_acc_t nats[MAX_NAT_ACC]; static emp_pri_t emps[MAX_EMP_PRI];
static fis_pol_t fiss[MAX_FIS_POL]; static mon_pol_t mons[MAX_MON_POL];
static int_mac_t ints[MAX_INT_MAC]; static mac_state_t mac;
static int initialized = 0;

static void ps(const char* s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i=0; if(v<0){b[i++]='-';v=-v;} if(v==0){b[i++]='0';}
    else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}
    b[i]='\0'; host_print(b);
}

int mac_init(void) {
    if (initialized) return -1;
    mac.n_nat=0; mac.n_emp=0; mac.n_fis=0; mac.n_mon=0; mac.n_int=0;
    mac.t_gdp=0; mac.t_unemp=0; mac.t_g_exp=0; mac.t_m_sup=0; mac.t_bop=0;
    for(int i=0;i<MAX_NAT_ACC;i++) nats[i].active=0;
    for(int i=0;i<MAX_EMP_PRI;i++) emps[i].active=0;
    for(int i=0;i<MAX_FIS_POL;i++) fiss[i].active=0;
    for(int i=0;i<MAX_MON_POL;i++) mons[i].active=0;
    for(int i=0;i<MAX_INT_MAC;i++) ints[i].active=0;
    initialized=1; ps("[MAC] Macroeconomics initialized\n"); return 0;
}

int mac_nat(int t,int c,int g,int ni,int no,int gr,int y) {
    if(mac.n_nat>=MAX_NAT_ACC) return -1;
    nat_acc_t* n=&nats[mac.n_nat]; n->id=mac.n_nat; n->type=t; n->cat=c;
    n->gdp=g; n->n_inc=ni; n->n_out=no; n->grth=gr; n->year=y; n->active=1;
    mac.t_gdp+=g; mac.n_nat++;
    ps("[MAC] Nat "); pi(mac.n_nat-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" gdp="); pi(g); ps(" ni="); pi(ni); ps(" no="); pi(no); ps(" gr="); pi(gr); ps("\n"); return mac.n_nat-1;
}

int mac_emp(int t,int c,int u,int inf,int cpi,int ppi,int y) {
    if(mac.n_emp>=MAX_EMP_PRI) return -1;
    emp_pri_t* e=&emps[mac.n_emp]; e->id=mac.n_emp; e->type=t; e->cat=c;
    e->unemp=u; e->infl=inf; e->cpi=cpi; e->ppi=ppi; e->year=y; e->active=1;
    mac.t_unemp+=u; mac.n_emp++;
    ps("[MAC] Emp "); pi(mac.n_emp-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" u="); pi(u); ps(" inf="); pi(inf); ps(" cpi="); pi(cpi); ps(" ppi="); pi(ppi); ps("\n"); return mac.n_emp-1;
}

int mac_fis(int t,int c,int ge,int tx,int fd,int dt,int y) {
    if(mac.n_fis>=MAX_FIS_POL) return -1;
    fis_pol_t* f=&fiss[mac.n_fis]; f->id=mac.n_fis; f->type=t; f->cat=c;
    f->g_exp=ge; f->tax=tx; f->fis_d=fd; f->debt=dt; f->year=y; f->active=1;
    mac.t_g_exp+=ge; mac.n_fis++;
    ps("[MAC] Fis "); pi(mac.n_fis-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" ge="); pi(ge); ps(" tx="); pi(tx); ps(" fd="); pi(fd); ps(" dt="); pi(dt); ps("\n"); return mac.n_fis-1;
}

int mac_mon(int t,int c,int ms,int ir,int cbt,int om,int y) {
    if(mac.n_mon>=MAX_MON_POL) return -1;
    mon_pol_t* m=&mons[mac.n_mon]; m->id=mac.n_mon; m->type=t; m->cat=c;
    m->m_sup=ms; m->intr=ir; m->cb_tool=cbt; m->omo=om; m->year=y; m->active=1;
    mac.t_m_sup+=ms; mac.n_mon++;
    ps("[MAC] Mon "); pi(mac.n_mon-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" ms="); pi(ms); ps(" ir="); pi(ir); ps(" cbt="); pi(cbt); ps(" om="); pi(om); ps("\n"); return mac.n_mon-1;
}

int mac_int(int t,int c,int bp,int ex,int fr,int ca,int y) {
    if(mac.n_int>=MAX_INT_MAC) return -1;
    int_mac_t* i=&ints[mac.n_int]; i->id=mac.n_int; i->type=t; i->cat=c;
    i->bop=bp; i->exch=ex; i->fx_r=fr; i->cap_a=ca; i->year=y; i->active=1;
    mac.t_bop+=bp; mac.n_int++;
    ps("[MAC] Int "); pi(mac.n_int-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" bp="); pi(bp); ps(" ex="); pi(ex); ps(" fr="); pi(fr); ps(" ca="); pi(ca); ps("\n"); return mac.n_int-1;
}

void mac_nat_report(void) { ps("[MAC] National accounts: "); pi(mac.n_nat); ps(" categories, GDP="); pi(mac.t_gdp); ps("\n"); }
void mac_emp_report(void) { ps("[MAC] Employment & prices: "); pi(mac.n_emp); ps(" categories, unemployment="); pi(mac.t_unemp); ps("\n"); }
void mac_full_report(void) {
    ps("[MAC] Full report:\n  Fiscal: "); pi(mac.n_fis); ps(" gov_exp="); pi(mac.t_g_exp);
    ps("\n  Monetary: "); pi(mac.n_mon); ps(" money_sup="); pi(mac.t_m_sup);
    ps("\n  Int'l: "); pi(mac.n_int); ps(" balance="); pi(mac.t_bop); ps("\n");
}
void mac_print_state(void) {
    ps("[MAC] Na="); pi(mac.n_nat); ps(" Ep="); pi(mac.n_emp); ps(" Fi="); pi(mac.n_fis);
    ps(" Mo="); pi(mac.n_mon); ps(" In="); pi(mac.n_int); ps("\n");
}

int main(void) {
    ps("=== Macroeconomics Admin Demo ===\n\n"); mac_init();
    ps("National accounts...\n");
    for(int i=0;i<16;i++){int t=(i%5)+1,c=(i%4)+1; mac_nat(t,c,70+(i*16),55+(i*13),35+(i*9),20+(i*5),2020+(i%5));}
    ps("\nEmployment & prices...\n");
    for(int i=0;i<14;i++){int t=(i%4)+1,c=(i%5)+1; mac_emp(t,c,60+(i*14),45+(i*11),28+(i*7),18+(i*4),2021+(i%4));}
    ps("\nFiscal policy...\n");
    for(int i=0;i<12;i++){int t=(i%4)+1,c=(i%5)+1; mac_fis(t,c,52+(i*12),38+(i*9),22+(i*5),14+(i*3),2022+(i%3));}
    ps("\nMonetary policy...\n");
    for(int i=0;i<10;i++){int t=(i%4)+1,c=(i%5)+1; mac_mon(t,c,44+(i*10),32+(i*7),19+(i*4),13+(i*3),2023+(i%2));}
    ps("\nInternational macro...\n");
    for(int i=0;i<10;i++){int t=(i%4)+1,c=(i%5)+1; mac_int(t,c,38+(i*8),28+(i*6),16+(i*4),11+(i*2),2024);}
    ps("\n"); mac_nat_report(); mac_emp_report(); mac_full_report(); mac_print_state();
    ps("\n=== Demo Complete ===\n"); return 0;
}
