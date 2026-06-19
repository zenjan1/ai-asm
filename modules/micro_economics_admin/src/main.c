/* micro_economics_admin: Microeconomics administration system (v1.0)
 * Supply-demand theory, consumer theory, production theory, market structure, welfare economics
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

#define MAX_SUP_DEM    16
#define MAX_CON_THE    14
#define MAX_PRO_THE    12
#define MAX_MKT_STR    10
#define MAX_WEL_ECO    10

typedef struct { int id; int type; int cat; int dem; int sup; int pri; int ela; int year; int active; } sup_dem_t;
typedef struct { int id; int type; int cat; int util; int ind_c; int bud_c; int con_s; int year; int active; } con_the_t;
typedef struct { int id; int type; int cat; int pro_f; int mar_p; int scl_r; int cos_c; int year; int active; } pro_the_t;
typedef struct { int id; int type; int cat; int m_str; int pri_d; int m_eq; int bar_e; int year; int active; } mkt_str_t;
typedef struct { int id; int type; int cat; int par_o; int soc_w; int ext_e; int pub_g; int year; int active; } wel_eco_t;

typedef struct {
    int n_sup; int n_con; int n_pro; int n_mkt; int n_wel;
    int t_dem; int t_util; int t_pro_f; int t_m_str; int t_par_o;
} mic_state_t;

static sup_dem_t sups[MAX_SUP_DEM]; static con_the_t cons[MAX_CON_THE];
static pro_the_t pros[MAX_PRO_THE]; static mkt_str_t mkts[MAX_MKT_STR];
static wel_eco_t wels[MAX_WEL_ECO]; static mic_state_t mic;
static int initialized = 0;

static void ps(const char* s) { host_print(s); }
static void pi(int v) {
    char b[32]; int i=0; if(v<0){b[i++]='-';v=-v;} if(v==0){b[i++]='0';}
    else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}
    b[i]='\0'; host_print(b);
}

int mic_init(void) {
    if (initialized) return -1;
    mic.n_sup=0; mic.n_con=0; mic.n_pro=0; mic.n_mkt=0; mic.n_wel=0;
    mic.t_dem=0; mic.t_util=0; mic.t_pro_f=0; mic.t_m_str=0; mic.t_par_o=0;
    for(int i=0;i<MAX_SUP_DEM;i++) sups[i].active=0;
    for(int i=0;i<MAX_CON_THE;i++) cons[i].active=0;
    for(int i=0;i<MAX_PRO_THE;i++) pros[i].active=0;
    for(int i=0;i<MAX_MKT_STR;i++) mkts[i].active=0;
    for(int i=0;i<MAX_WEL_ECO;i++) wels[i].active=0;
    initialized=1; ps("[MIC] Microeconomics initialized\n"); return 0;
}

int mic_sup(int t,int c,int d,int s,int p,int e,int y) {
    if(mic.n_sup>=MAX_SUP_DEM) return -1;
    sup_dem_t* x=&sups[mic.n_sup]; x->id=mic.n_sup; x->type=t; x->cat=c;
    x->dem=d; x->sup=s; x->pri=p; x->ela=e; x->year=y; x->active=1;
    mic.t_dem+=d; mic.n_sup++;
    ps("[MIC] Sup "); pi(mic.n_sup-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" d="); pi(d); ps(" s="); pi(s); ps(" p="); pi(p); ps(" e="); pi(e); ps("\n"); return mic.n_sup-1;
}

int mic_con(int t,int c,int u,int ic,int bc,int cs,int y) {
    if(mic.n_con>=MAX_CON_THE) return -1;
    con_the_t* x=&cons[mic.n_con]; x->id=mic.n_con; x->type=t; x->cat=c;
    x->util=u; x->ind_c=ic; x->bud_c=bc; x->con_s=cs; x->year=y; x->active=1;
    mic.t_util+=u; mic.n_con++;
    ps("[MIC] Con "); pi(mic.n_con-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" u="); pi(u); ps(" ic="); pi(ic); ps(" bc="); pi(bc); ps(" cs="); pi(cs); ps("\n"); return mic.n_con-1;
}

int mic_pro(int t,int c,int pf,int mp,int sr,int cc,int y) {
    if(mic.n_pro>=MAX_PRO_THE) return -1;
    pro_the_t* x=&pros[mic.n_pro]; x->id=mic.n_pro; x->type=t; x->cat=c;
    x->pro_f=pf; x->mar_p=mp; x->scl_r=sr; x->cos_c=cc; x->year=y; x->active=1;
    mic.t_pro_f+=pf; mic.n_pro++;
    ps("[MIC] Pro "); pi(mic.n_pro-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" pf="); pi(pf); ps(" mp="); pi(mp); ps(" sr="); pi(sr); ps(" cc="); pi(cc); ps("\n"); return mic.n_pro-1;
}

int mic_mkt(int t,int c,int ms,int pd,int me,int be,int y) {
    if(mic.n_mkt>=MAX_MKT_STR) return -1;
    mkt_str_t* x=&mkts[mic.n_mkt]; x->id=mic.n_mkt; x->type=t; x->cat=c;
    x->m_str=ms; x->pri_d=pd; x->m_eq=me; x->bar_e=be; x->year=y; x->active=1;
    mic.t_m_str+=ms; mic.n_mkt++;
    ps("[MIC] Mkt "); pi(mic.n_mkt-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" ms="); pi(ms); ps(" pd="); pi(pd); ps(" me="); pi(me); ps(" be="); pi(be); ps("\n"); return mic.n_mkt-1;
}

int mic_wel(int t,int c,int po,int sw,int ex,int pg,int y) {
    if(mic.n_wel>=MAX_WEL_ECO) return -1;
    wel_eco_t* x=&wels[mic.n_wel]; x->id=mic.n_wel; x->type=t; x->cat=c;
    x->par_o=po; x->soc_w=sw; x->ext_e=ex; x->pub_g=pg; x->year=y; x->active=1;
    mic.t_par_o+=po; mic.n_wel++;
    ps("[MIC] Wel "); pi(mic.n_wel-1); ps(" t="); pi(t); ps(" c="); pi(c);
    ps(" po="); pi(po); ps(" sw="); pi(sw); ps(" ex="); pi(ex); ps(" pg="); pi(pg); ps("\n"); return mic.n_wel-1;
}

void mic_sup_report(void) { ps("[MIC] Supply-demand: "); pi(mic.n_sup); ps(" categories, demand="); pi(mic.t_dem); ps("\n"); }
void mic_con_report(void) { ps("[MIC] Consumer: "); pi(mic.n_con); ps(" categories, utility="); pi(mic.t_util); ps("\n"); }
void mic_full_report(void) {
    ps("[MIC] Full report:\n  Production: "); pi(mic.n_pro); ps(" prod_f="); pi(mic.t_pro_f);
    ps("\n  Market: "); pi(mic.n_mkt); ps(" mkt_str="); pi(mic.t_m_str);
    ps("\n  Welfare: "); pi(mic.n_wel); ps(" par_opt="); pi(mic.t_par_o); ps("\n");
}
void mic_print_state(void) {
    ps("[MIC] Su="); pi(mic.n_sup); ps(" Co="); pi(mic.n_con); ps(" Pr="); pi(mic.n_pro);
    ps(" Mk="); pi(mic.n_mkt); ps(" We="); pi(mic.n_wel); ps("\n");
}

int main(void) {
    ps("=== Microeconomics Admin Demo ===\n\n"); mic_init();
    ps("Supply-demand...\n");
    for(int i=0;i<16;i++){int t=(i%5)+1,c=(i%4)+1; mic_sup(t,c,72+(i*16),58+(i*13),38+(i*9),22+(i*5),2020+(i%5));}
    ps("\nConsumer theory...\n");
    for(int i=0;i<14;i++){int t=(i%4)+1,c=(i%5)+1; mic_con(t,c,62+(i*14),48+(i*11),30+(i*7),19+(i*4),2021+(i%4));}
    ps("\nProduction theory...\n");
    for(int i=0;i<12;i++){int t=(i%4)+1,c=(i%5)+1; mic_pro(t,c,54+(i*12),40+(i*9),24+(i*5),15+(i*3),2022+(i%3));}
    ps("\nMarket structure...\n");
    for(int i=0;i<10;i++){int t=(i%4)+1,c=(i%5)+1; mic_mkt(t,c,46+(i*10),34+(i*7),20+(i*4),13+(i*3),2023+(i%2));}
    ps("\nWelfare economics...\n");
    for(int i=0;i<10;i++){int t=(i%4)+1,c=(i%5)+1; mic_wel(t,c,40+(i*8),30+(i*6),17+(i*4),11+(i*2),2024);}
    ps("\n"); mic_sup_report(); mic_con_report(); mic_full_report(); mic_print_state();
    ps("\n=== Demo Complete ===\n"); return 0;
}
