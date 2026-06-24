/* conch_admin: Conch (Strombus gigas) sea snail aquaculture (v1.0)
 * Conch seeding, feeding, water management, harvest, market
 * Features: shell_len_mm, shell_wt_g, meat_pct, depth_cm, water_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sh_ln,sh_wt,meat_pct,dpth,wt_tmp,harv_wk,active;} cnc_t;
typedef struct{int n_seed,n_feed,n_water,n_harv,n_mkt,t_ln,t_wt,t_meat,t_dpth,t_tmp;} cnc_state_t;
static cnc_t cncsd[N],cncfd[N-2],cncwt[N-4],cnchv[N-6],cncmk[N-6]; static cnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnc_t*a,int*cnt,int*sum,int mx,int lc,int sl,int sw,int mp,int dp,int wt,int hw){if(*cnt>=mx)return -1;cnc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sh_ln=sl;x->sh_wt=sw;x->meat_pct=mp;x->dpth=dp;x->wt_tmp=wt;x->harv_wk=hw;x->active=1;*sum+=sl;(*cnt)++;ps("[CNC] Conch ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" sw=");pi(sw);ps(" mp=");pi(mp);ps(" dp=");pi(dp);ps(" wt=");pi(wt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cnc_init(void){if(init)return -1;st.n_seed=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_meat=0;st.t_dpth=0;st.t_tmp=0;for(int i=0;i<N;i++)cncsd[i].active=0;for(int i=0;i<N-2;i++)cncfd[i].active=0;for(int i=0;i<N-4;i++)cncwt[i].active=0;for(int i=0;i<N-6;i++)cnchv[i].active=0;for(int i=0;i<N-6;i++)cncmk[i].active=0;init=1;ps("[CNC] Conch initialized\n");return 0;}
int cnc_seeding(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(cncsd,&st.n_seed,&st.t_ln,N,lc,sl,sw,mp,dp,wt,hw);}
int cnc_feeding(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(cncfd,&st.n_feed,&st.t_wt,N-2,lc,sl,sw,mp,dp,wt,hw);}
int cnc_water_mgmt(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(cncwt,&st.n_water,&st.t_meat,N-4,lc,sl,sw,mp,dp,wt,hw);}
int cnc_harvest(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(cnchv,&st.n_harv,&st.t_dpth,N-6,lc,sl,sw,mp,dp,wt,hw);}
int cnc_market(int lc,int sl,int sw,int mp,int dp,int wt,int hw){return add(cncmk,&st.n_mkt,&st.t_tmp,N-6,lc,sl,sw,mp,dp,wt,hw);}
void cnc_report(void){ps("[CNC] Seed: ");pi(st.n_seed);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nWater: ");pi(st.n_water);ps(" Meat=");pi(st.t_meat);ps("\nHarv: ");pi(st.n_harv);ps(" Dpth=");pi(st.t_dpth);ps("\nMkt: ");pi(st.n_mkt);ps(" Tmp=");pi(st.t_tmp);ps("\n");}
void cnc_state(void){ps("[CNC] Seed=");pi(st.n_seed);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Conch Admin Demo ===\n\n");cnc_init();
/* 1=coastal 2=lagoon 3=tank 4=reef 5=market */
ps("Conch seeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sl=60+(i*8),sw=200+(i*30),mp=25+(i*3),dp=3+(i*2),wt=22+(i%8),hw=24+(i%8);cnc_seeding(lc,sl,sw,mp,dp,wt,hw);}
ps("\nConch feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sl=65+(i*7),sw=210+(i*25),mp=27+(i*3),dp=4+(i*2),wt=23+(i%7),hw=26+(i%6);cnc_feeding(lc,sl,sw,mp,dp,wt,hw);}
ps("\nConch water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sl=70+(i*6),sw=220+(i*20),mp=29+(i*2),dp=5+(i*2),wt=24+(i%6),hw=28+(i%5);cnc_water_mgmt(lc,sl,sw,mp,dp,wt,hw);}
ps("\nConch harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sl=55+(i*10),sw=180+(i*40),mp=23+(i*4),dp=2+(i*3),wt=20+(i%10),hw=22+(i%10);cnc_harvest(lc,sl,sw,mp,dp,wt,hw);}
ps("\nConch market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sl=75+(i*5),sw=230+(i*15),mp=31+(i*2),dp=6+(i*2),wt=25+(i%5),hw=30+(i%4);cnc_market(lc,sl,sw,mp,dp,wt,hw);}
ps("\n");cnc_report();cnc_state();ps("\n=== Demo Complete ===\n");return 0;}
