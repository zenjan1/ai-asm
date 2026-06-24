/* clam_admin: Clam (Meretrix lusoria) shellfish aquaculture (v1.0)
 * Clam seeding, feeding, water management, harvest, market
 * Features: shell_len_mm, shell_wt_g, meat_pct, depth_cm, water_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sh_ln,sh_wt,meat_pct,dpth,wt_tmp,harv_wk,active;} clm_t;
typedef struct{int n_seed,n_feed,n_water,n_harv,n_mkt,t_ln,t_wt,t_meat,t_dpth,t_tmp;} clm_state_t;
static clm_t clmsd[N],clmfd[N-2],clmwt[N-4],clmhs[N-6],clmms[N-6]; static clm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clm_t*a,int*cnt,int*sum,int mx,int lc,int sl,int sw,int mp,int dp,int wt,int hw){if(*cnt>=mx)return -1;clm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sh_ln=sl;x->sh_wt=sw;x->meat_pct=mp;x->dpth=dp;x->wt_tmp=wt;x->harv_wk=hw;x->active=1;*sum+=sl;(*cnt)++;ps("[CLM] Clam ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" sw=");pi(sw);ps(" mp=");pi(mp);ps(" dp=");pi(dp);ps(" wt=");pi(wt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int clm_init(void){if(init)return -1;st.n_seed=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_meat=0;st.t_dpth=0;st.t_tmp=0;for(int i=0;i<N;i++)clmsd[i].active=0;for(int i=0;i<N-2;i++)clmfd[i].active=0;for(int i=0;i<N-4;i++)clmwt[i].active=0;for(int i=0;i<N-6;i++)clmhs[i].active=0;for(int i=0;i<N-6;i++)clmms[i].active=0;init=1;ps("[CLM] Clam initialized\n");return 0;}
int clm_seeding(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(clmsd,&st.n_seed,&st.t_ln,N,lc,sl,sw,mp,dp,wt,hw);}
int clm_feeding(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(clmfd,&st.n_feed,&st.t_wt,N-2,lc,sl,sw,mp,dp,wt,hw);}
int clm_water_mgmt(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(clmwt,&st.n_water,&st.t_meat,N-4,lc,sl,sw,mp,dp,wt,hw);}
int clm_harvest(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(clmhs,&st.n_harv,&st.t_dpth,N-6,lc,sl,sw,mp,dp,wt,hw);}
int clm_market(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(clmms,&st.n_mkt,&st.t_tmp,N-6,lc,sl,sw,mp,dp,wt,hw);}
void clm_report(void){ps("[CLM] Seed: ");pi(st.n_seed);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nWater: ");pi(st.n_water);ps(" Meat=");pi(st.t_meat);ps("\nHarv: ");pi(st.n_harv);ps(" Dpth=");pi(st.t_dpth);ps("\nMkt: ");pi(st.n_mkt);ps(" Tmp=");pi(st.t_tmp);ps("\n");}
void clm_state(void){ps("[CLM] Seed=");pi(st.n_seed);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Clam Admin Demo ===\n\n");clm_init();
/* 1=coastal 2=estuary 3=lagoon 4=tank 5=market */
ps("Clam seeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sl=20+(i*5),sw=10+(i*3),mp=30+(i*4),dp=5+(i*2),wt=15+(i%10),hw=20+(i%8);clm_seeding(lc,sl,sw,mp,dp,wt,hw);}
ps("\nClam feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sl=25+(i*4),sw=12+(i*3),mp=32+(i*3),dp=6+(i*2),wt=16+(i%8),hw=22+(i%6);clm_feeding(lc,sl,sw,mp,dp,wt,hw);}
ps("\nClam water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sl=30+(i*3),sw=14+(i*3),mp=34+(i*3),dp=7+(i*2),wt=17+(i%7),hw=24+(i%5);clm_water_mgmt(lc,sl,sw,mp,dp,wt,hw);}
ps("\nClam harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sl=18+(i*6),sw=8+(i*4),mp=28+(i*5),dp=4+(i*3),wt=14+(i%12),hw=18+(i%10);clm_harvest(lc,sl,sw,mp,dp,wt,hw);}
ps("\nClam market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sl=35+(i*3),sw=16+(i*2),mp=36+(i*2),dp=8+(i*2),wt=18+(i%6),hw=26+(i%4);clm_market(lc,sl,sw,mp,dp,wt,hw);}
ps("\n");clm_report();clm_state();ps("\n=== Demo Complete ===\n");return 0;}
