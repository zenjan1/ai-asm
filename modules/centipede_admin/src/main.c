/* centipede_admin: Centipede grass (Eremochloa ophiuroides) lawn management (v1.0)
 * Centipede establishment, mowing, fertilizing, thatch_removal, market
 * Features: coverage_sqm, blade_height_cm, density_pct, thatch_mm, ph_level, mow_interval_days
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,cov_sqm,blade_ht,dens_pct,thatch_mm,ph_lv,mow_int,active;} cent_t;
typedef struct{int n_est,n_mow,n_fert,n_thatch,n_mkt,t_cov,t_blade,t_dens,t_thatch,t_ph;} cent_state_t;
static cent_t centes[N],centms[N-2],centfs[N-4],centts[N-6],centks[N-6]; static cent_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cent_t*a,int*cnt,int*sum,int mx,int lc,int cv,int bh,int dp,int tm,int pl,int mi){if(*cnt>=mx)return -1;cent_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->cov_sqm=cv;x->blade_ht=bh;x->dens_pct=dp;x->thatch_mm=tm;x->ph_lv=pl;x->mow_int=mi;x->active=1;*sum+=cv;(*cnt)++;ps("[CENT] Centipede ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cv=");pi(cv);ps(" bh=");pi(bh);ps(" dp=");pi(dp);ps(" tm=");pi(tm);ps(" pl=");pi(pl);ps(" mi=");pi(mi);ps("\n");return *cnt-1;}
int cent_init(void){if(init)return -1;st.n_est=0;st.n_mow=0;st.n_fert=0;st.n_thatch=0;st.n_mkt=0;st.t_cov=0;st.t_blade=0;st.t_dens=0;st.t_thatch=0;st.t_ph=0;for(int i=0;i<N;i++)centes[i].active=0;for(int i=0;i<N-2;i++)centms[i].active=0;for(int i=0;i<N-4;i++)centfs[i].active=0;for(int i=0;i<N-6;i++)centts[i].active=0;for(int i=0;i<N-6;i++)centks[i].active=0;init=1;ps("[CENT] Centipede initialized\n");return 0;}
int cent_establishment(int lc,int cv,int bh,int dp,int tm,int pl,int mi){return add(centes,&st.n_est,&st.t_cov,N,lc,cv,bh,dp,tm,pl,mi);}
int cent_mowing(int lc,int cv,int bh,int dp,int tm,int pl,int mi){return add(centms,&st.n_mow,&st.t_blade,N-2,lc,cv,bh,dp,tm,pl,mi);}
int cent_fertilizing(int lc,int cv,int bh,int dp,int tm,int pl,int mi){return add(centfs,&st.n_fert,&st.t_dens,N-4,lc,cv,bh,dp,tm,pl,mi);}
int cent_thatch_removal(int lc,int cv,int bh,int dp,int tm,int pl,int mi){return add(centts,&st.n_thatch,&st.t_thatch,N-6,lc,cv,bh,dp,tm,pl,mi);}
int cent_market(int lc,int cv,int bh,int dp,int tm,int pl,int mi){return add(centks,&st.n_mkt,&st.t_ph,N-6,lc,cv,bh,dp,tm,pl,mi);}
void cent_report(void){ps("[CENT] Est: ");pi(st.n_est);ps(" Cov=");pi(st.t_cov);ps("\nMow: ");pi(st.n_mow);ps(" Blade=");pi(st.t_blade);ps("\nFert: ");pi(st.n_fert);ps(" Dens=");pi(st.t_dens);ps("\nThatch: ");pi(st.n_thatch);ps(" Thatch=");pi(st.t_thatch);ps("\nMkt: ");pi(st.n_mkt);ps(" pH=");pi(st.t_ph);ps("\n");}
void cent_state(void){ps("[CENT] Est=");pi(st.n_est);ps(" Mow=");pi(st.n_mow);ps(" Fert=");pi(st.n_fert);ps(" Thatch=");pi(st.n_thatch);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Centipede Admin Demo ===\n\n");cent_init();
/* 1=lawn 2=park 3=roadside 4=sports_field 5=low_maintenance */
ps("Centipede establishment...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,cv=5+(i*3),bh=3+(i%2),dp=30+(i*4),tm=2+(i%3),pl=5+(i%2),mi=10+(i%3);cent_establishment(lc,cv,bh,dp,tm,pl,mi);}
ps("\nCentipede mowing...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,cv=8+(i*2),bh=4+(i%2),dp=40+(i*3),tm=3+(i%2),pl=6+(i%2),mi=7+(i%2);cent_mowing(lc,cv,bh,dp,tm,pl,mi);}
ps("\nCentipede fertilizing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,cv=10+(i*2),bh=5+(i%2),dp=50+(i*3),tm=4+(i%2),pl=6+(i%1),mi=5+(i%2);cent_fertilizing(lc,cv,bh,dp,tm,pl,mi);}
ps("\nCentipede thatch removal...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,cv=3+(i*4),bh=2+(i%3),dp=25+(i*5),tm=1+(i%4),pl=5+(i%2),mi=12+(i%3);cent_thatch_removal(lc,cv,bh,dp,tm,pl,mi);}
ps("\nCentipede market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,cv=12+(i*2),bh=6+(i%2),dp=60+(i*3),tm=5+(i%2),pl=7+(i%1),mi=4+(i%2);cent_market(lc,cv,bh,dp,tm,pl,mi);}
ps("\n");cent_report();cent_state();ps("\n=== Demo Complete ===\n");return 0;}
