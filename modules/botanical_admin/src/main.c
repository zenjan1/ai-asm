/* botanical_admin: Botanical garden plant collection and exhibit management (v1.0)
 * Collection, research, education, conservation, market
 * Features: species count, area sqm, climate zone, rare species, visitor count, irrigation level
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spc_ct,area_sqm,clim_zn,rare_spc,visitor_ct,irrig_lv,active;} btnc_t;
typedef struct{int n_coll,n_res,n_edu,n_cons,n_mkt,t_spc,t_area,t_clim,t_rare,t_visit;} btnc_state_t;
static btnc_t btncps[N],btncrs[N-2],btnceds[N-4],btnccs[N-6],btncms[N-6]; static btnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(btnc_t*a,int*cnt,int*sum,int mx,int lc,int sc,int asz,int cz,int rs,int vc,int il){if(*cnt>=mx)return -1;btnc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spc_ct=sc;x->area_sqm=asz;x->clim_zn=cz;x->rare_spc=rs;x->visitor_ct=vc;x->irrig_lv=il;x->active=1;*sum+=sc;(*cnt)++;ps("[BTNC] Botanical ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" asz=");pi(asz);ps(" cz=");pi(cz);ps(" rs=");pi(rs);ps(" vc=");pi(vc);ps(" il=");pi(il);ps("\n");return *cnt-1;}
int btnc_init(void){if(init)return -1;st.n_coll=0;st.n_res=0;st.n_edu=0;st.n_cons=0;st.n_mkt=0;st.t_spc=0;st.t_area=0;st.t_clim=0;st.t_rare=0;st.t_visit=0;for(int i=0;i<N;i++)btncps[i].active=0;for(int i=0;i<N-2;i++)btncrs[i].active=0;for(int i=0;i<N-4;i++)btnceds[i].active=0;for(int i=0;i<N-6;i++)btnccs[i].active=0;for(int i=0;i<N-6;i++)btncms[i].active=0;init=1;ps("[BTNC] Botanical initialized\n");return 0;}
int btnc_collection(int lc,int sc,int asz,int cz,int rs,int vc,int il){return add(btncps,&st.n_coll,&st.t_spc,N,lc,sc,asz,cz,rs,vc,il);}
int btnc_research(int lc,int sc,int asz,int cz,int rs,int vc,int il){return add(btncrs,&st.n_res,&st.t_area,N-2,lc,sc,asz,cz,rs,vc,il);}
int btnc_education(int lc,int sc,int asz,int cz,int rs,int vc,int il){return add(btnceds,&st.n_edu,&st.t_clim,N-4,lc,sc,asz,cz,rs,vc,il);}
int btnc_conservation(int lc,int sc,int asz,int cz,int rs,int vc,int il){return add(btnccs,&st.n_cons,&st.t_rare,N-6,lc,sc,asz,cz,rs,vc,il);}
int btnc_market(int lc,int sc,int asz,int cz,int rs,int vc,int il){return add(btncms,&st.n_mkt,&st.t_visit,N-6,lc,sc,asz,cz,rs,vc,il);}
void btnc_report(void){ps("[BTNC] Coll/Spc=");pi(st.t_spc);ps("\nRes/Area=");pi(st.t_area);ps("\nEdu/Clim=");pi(st.t_clim);ps("\nCons/Rare=");pi(st.t_rare);ps("\nMkt/Visit=");pi(st.t_visit);ps("\n");}
void btnc_state(void){ps("[BTNC] Coll=");pi(st.n_coll);ps(" Res=");pi(st.n_res);ps(" Edu=");pi(st.n_edu);ps(" Cons=");pi(st.n_cons);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Botanical Admin Demo ===\n\n");btnc_init();
/* 1=tropical 2=temperate 3=arid 4=alpine 5=coastal */
ps("Botanical collection...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sc=50+(i*20),asz=200+(i*100),cz=(i%5)+1,rs=5+(i*3),vc=100+(i*50),il=3+(i%5);btnc_collection(lc,sc,asz,cz,rs,vc,il);}
ps("\nBotanical research...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sc=60+(i*15),asz=250+(i*80),cz=(i%4)+2,rs=8+(i*2),vc=50+(i*30),il=4+(i%4);btnc_research(lc,sc,asz,cz,rs,vc,il);}
ps("\nBotanical education...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sc=40+(i*25),asz=300+(i*60),cz=(i%3)+1,rs=10+(i*2),vc=200+(i*40),il=5+(i%3);btnc_education(lc,sc,asz,cz,rs,vc,il);}
ps("\nBotanical conservation...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sc=30+(i*10),asz=150+(i*120),cz=(i%5)+1,rs=15+(i*3),vc=80+(i*20),il=2+(i%5);btnc_conservation(lc,sc,asz,cz,rs,vc,il);}
ps("\nBotanical market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sc=70+(i*10),asz=400+(i*50),cz=(i%4)+2,rs=12+(i*2),vc=300+(i*30),il=6+(i%2);btnc_market(lc,sc,asz,cz,rs,vc,il);}
ps("\n");btnc_report();btnc_state();ps("\n=== Demo Complete ===\n");return 0;}
