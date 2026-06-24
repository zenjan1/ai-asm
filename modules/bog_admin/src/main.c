/* bog_admin: Bog wetland ecosystem management and conservation (v1.0)
 * Bog hydrology, vegetation, conservation, monitoring, market
 * Features: water pH, depth, sphagnum coverage, flora count, fauna count, carbon stored
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,wtr_ph,dpth_cm,sphgm_pct,flora_ct,fauna_ct,crbn_st,active;} bog_t;
typedef struct{int n_hydro,n_veg,n_cons,n_monitor,n_mkt,t_ph,t_depth,t_sphgm,t_flora,t_fauna;} bog_state_t;
static bog_t bogps[N],bogvs[N-2],bogcs[N-4],bogmns[N-6],bogms[N-6]; static bog_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bog_t*a,int*cnt,int*sum,int mx,int lc,int wp,int dc,int sp,int fc,int fa,int cs){if(*cnt>=mx)return -1;bog_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->wtr_ph=wp;x->dpth_cm=dc;x->sphgm_pct=sp;x->flora_ct=fc;x->fauna_ct=fa;x->crbn_st=cs;x->active=1;*sum+=wp;(*cnt)++;ps("[BOG] Bog ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" wp=");pi(wp);ps(" dc=");pi(dc);ps(" sp=");pi(sp);ps(" fc=");pi(fc);ps(" fa=");pi(fa);ps(" cs=");pi(cs);ps("\n");return *cnt-1;}
int bog_init(void){if(init)return -1;st.n_hydro=0;st.n_veg=0;st.n_cons=0;st.n_monitor=0;st.n_mkt=0;st.t_ph=0;st.t_depth=0;st.t_sphgm=0;st.t_flora=0;st.t_fauna=0;for(int i=0;i<N;i++)bogps[i].active=0;for(int i=0;i<N-2;i++)bogvs[i].active=0;for(int i=0;i<N-4;i++)bogcs[i].active=0;for(int i=0;i<N-6;i++)bogmns[i].active=0;for(int i=0;i<N-6;i++)bogms[i].active=0;init=1;ps("[BOG] Bog initialized\n");return 0;}
int bog_hydrology(int lc,int wp,int dc,int sp,int fc,int fa,int cs){return add(bogps,&st.n_hydro,&st.t_ph,N,lc,wp,dc,sp,fc,fa,cs);}
int bog_vegetation(int lc,int wp,int dc,int sp,int fc,int fa,int cs){return add(bogvs,&st.n_veg,&st.t_depth,N-2,lc,wp,dc,sp,fc,fa,cs);}
int bog_conservation(int lc,int wp,int dc,int sp,int fc,int fa,int cs){return add(bogcs,&st.n_cons,&st.t_sphgm,N-4,lc,wp,dc,sp,fc,fa,cs);}
int bog_monitoring(int lc,int wp,int dc,int sp,int fc,int fa,int cs){return add(bogmns,&st.n_monitor,&st.t_flora,N-6,lc,wp,dc,sp,fc,fa,cs);}
int bog_market(int lc,int wp,int dc,int sp,int fc,int fa,int cs){return add(bogms,&st.n_mkt,&st.t_fauna,N-6,lc,wp,dc,sp,fc,fa,cs);}
void bog_report(void){ps("[BOG] Hydro/pH=");pi(st.t_ph);ps("\nVeg/Depth=");pi(st.t_depth);ps("\nCons/Sphgm=");pi(st.t_sphgm);ps("\nMonitor/Flora=");pi(st.t_flora);ps("\nMkt/Fauna=");pi(st.t_fauna);ps("\n");}
void bog_state(void){ps("[BOG] Hydro=");pi(st.n_hydro);ps(" Veg=");pi(st.n_veg);ps(" Cons=");pi(st.n_cons);ps(" Monitor=");pi(st.n_monitor);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bog Admin Demo ===\n\n");bog_init();
/* 1=peat_bog 2=marsh 3=swamp 4=fen 5=mire */
ps("Bog hydrology...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,wp=3+(i%5),dc=30+(i*10),sp=20+(i*5),fc=10+(i*3),fa=5+(i*2),cs=100+(i*50);bog_hydrology(lc,wp,dc,sp,fc,fa,cs);}
ps("\nBog vegetation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,wp=4+(i%4),dc=40+(i*8),sp=25+(i*4),fc=15+(i*2),fa=8+(i*2),cs=150+(i*40);bog_vegetation(lc,wp,dc,sp,fc,fa,cs);}
ps("\nBog conservation...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,wp=5+(i%3),dc=50+(i*6),sp=30+(i*3),fc=20+(i*2),fa=10+(i*2),cs=200+(i*30);bog_conservation(lc,wp,dc,sp,fc,fa,cs);}
ps("\nBog monitoring...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,wp=3+(i%4),dc=20+(i*12),sp=15+(i*6),fc=8+(i*3),fa=4+(i*2),cs=80+(i*40);bog_monitoring(lc,wp,dc,sp,fc,fa,cs);}
ps("\nBog market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,wp=6+(i%2),dc=60+(i*5),sp=35+(i*3),fc=25+(i*2),fa=12+(i*2),cs=250+(i*20);bog_market(lc,wp,dc,sp,fc,fa,cs);}
ps("\n");bog_report();bog_state();ps("\n=== Demo Complete ===\n");return 0;}
