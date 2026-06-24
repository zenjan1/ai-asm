/* brush_admin: Brush and vegetation clearing for land management (v1.0)
 * Brush clearing, mulching, herbicide, restoration, market
 * Features: density_pct, height_cm, species_type, coverage_acres, moisture_pct, regrowth_rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,dens_pct,ht_cm,spc_tp,cov_ac,moist_pct,rgrow_rt,active;} brush_t;
typedef struct{int n_clear,n_mulch,n_herb,n_restore,n_mkt,t_dens,t_ht,t_spc,t_cov,t_moist;} brush_state_t;
static brush_t brushps[N],brushms[N-2],brushhs[N-4],brushrs[N-6],brushmks[N-6]; static brush_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brush_t*a,int*cnt,int*sum,int mx,int lc,int dp,int hc,int st2,int ca,int mp,int rr){if(*cnt>=mx)return -1;brush_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->dens_pct=dp;x->ht_cm=hc;x->spc_tp=st2;x->cov_ac=ca;x->moist_pct=mp;x->rgrow_rt=rr;x->active=1;*sum+=dp;(*cnt)++;ps("[BRUSH] Brush ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" dp=");pi(dp);ps(" hc=");pi(hc);ps(" st=");pi(st2);ps(" ca=");pi(ca);ps(" mp=");pi(mp);ps(" rr=");pi(rr);ps("\n");return *cnt-1;}
int brush_init(void){if(init)return -1;st.n_clear=0;st.n_mulch=0;st.n_herb=0;st.n_restore=0;st.n_mkt=0;st.t_dens=0;st.t_ht=0;st.t_spc=0;st.t_cov=0;st.t_moist=0;for(int i=0;i<N;i++)brushps[i].active=0;for(int i=0;i<N-2;i++)brushms[i].active=0;for(int i=0;i<N-4;i++)brushhs[i].active=0;for(int i=0;i<N-6;i++)brushrs[i].active=0;for(int i=0;i<N-6;i++)brushmks[i].active=0;init=1;ps("[BRUSH] Brush initialized\n");return 0;}
int brush_clearing(int lc,int dp,int hc,int st2,int ca,int mp,int rr){return add(brushps,&st.n_clear,&st.t_dens,N,lc,dp,hc,st2,ca,mp,rr);}
int brush_mulching(int lc,int dp,int hc,int st2,int ca,int mp,int rr){return add(brushms,&st.n_mulch,&st.t_ht,N-2,lc,dp,hc,st2,ca,mp,rr);}
int brush_herbicide(int lc,int dp,int hc,int st2,int ca,int mp,int rr){return add(brushhs,&st.n_herb,&st.t_spc,N-4,lc,dp,hc,st2,ca,mp,rr);}
int brush_restoration(int lc,int dp,int hc,int st2,int ca,int mp,int rr){return add(brushrs,&st.n_restore,&st.t_cov,N-6,lc,dp,hc,st2,ca,mp,rr);}
int brush_market(int lc,int dp,int hc,int st2,int ca,int mp,int rr){return add(brushmks,&st.n_mkt,&st.t_moist,N-6,lc,dp,hc,st2,ca,mp,rr);}
void brush_report(void){ps("[BRUSH] Clear: ");pi(st.n_clear);ps(" Dens=");pi(st.t_dens);ps("\nMulch: ");pi(st.n_mulch);ps(" Ht=");pi(st.t_ht);ps("\nHerb: ");pi(st.n_herb);ps(" Spc=");pi(st.t_spc);ps("\nRestore: ");pi(st.n_restore);ps(" Cov=");pi(st.t_cov);ps("\nMkt: ");pi(st.n_mkt);ps(" Moist=");pi(st.t_moist);ps("\n");}
void brush_state(void){ps("[BRUSH] Clear=");pi(st.n_clear);ps(" Mulch=");pi(st.n_mulch);ps(" Herb=");pi(st.n_herb);ps(" Restore=");pi(st.n_restore);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Brush Admin Demo ===\n\n");brush_init();
/* 1=pasture 2=roadside 3=forest_edge 4=fenceline 5=riparian */
ps("Brush clearing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,dp=40+(i*3),hc=80+(i*20),st2=(i%5)+1,ca=5+(i*2),mp=15+(i*3),rr=3+(i%5);brush_clearing(lc,dp,hc,st2,ca,mp,rr);}
ps("\nBrush mulching...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,dp=50+(i*2),hc=100+(i*15),st2=(i%4)+2,ca=8+(i*2),mp=20+(i*2),rr=4+(i%4);brush_mulching(lc,dp,hc,st2,ca,mp,rr);}
ps("\nBrush herbicide...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,dp=60+(i*2),hc=120+(i*10),st2=(i%3)+1,ca=10+(i*2),mp=25+(i*2),rr=5+(i%3);brush_herbicide(lc,dp,hc,st2,ca,mp,rr);}
ps("\nBrush restoration...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,dp=30+(i*4),hc=60+(i*25),st2=(i%5)+1,ca=3+(i*3),mp=10+(i*4),rr=2+(i%5);brush_restoration(lc,dp,hc,st2,ca,mp,rr);}
ps("\nBrush market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,dp=70+(i*2),hc=140+(i*8),st2=(i%4)+2,ca=12+(i*2),mp=30+(i*2),rr=6+(i%2);brush_market(lc,dp,hc,st2,ca,mp,rr);}
ps("\n");brush_report();brush_state();ps("\n=== Demo Complete ===\n");return 0;}
