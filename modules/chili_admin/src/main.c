/* chili_admin: Chili (Capsicum annuum) hot pepper crop management (v1.0)
 * Chili planting, pruning, pollination, harvest, market
 * Features: plant_ht_cm, fruit_len_cm, heat_scoville, wall_thick_mm, mature_days, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,frt_ln,heat_scl,wall_th,mat_dys,harv_wk,active;} chli_t;
typedef struct{int n_plant,n_prune,n_poll,n_harv,n_mkt,t_ht,t_ln,t_heat,t_wall,t_mat;} chli_state_t;
static chli_t chlips[N],chlis[N-2],chlids[N-4],chlihs[N-6],chlims[N-6]; static chli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chli_t*a,int*cnt,int*sum,int mx,int lc,int ph,int fl,int hs,int wt,int md,int hw){if(*cnt>=mx)return -1;chli_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->frt_ln=fl;x->heat_scl=hs;x->wall_th=wt;x->mat_dys=md;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CHLI] Chili ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" fl=");pi(fl);ps(" hs=");pi(hs);ps(" wt=");pi(wt);ps(" md=");pi(md);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int chli_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_ln=0;st.t_heat=0;st.t_wall=0;st.t_mat=0;for(int i=0;i<N;i++)chlips[i].active=0;for(int i=0;i<N-2;i++)chlis[i].active=0;for(int i=0;i<N-4;i++)chlids[i].active=0;for(int i=0;i<N-6;i++)chlihs[i].active=0;for(int i=0;i<N-6;i++)chlims[i].active=0;init=1;ps("[CHLI] Chili initialized\n");return 0;}
int chli_planting(int lc,int ph,int fl,int hs,int wt,int md,int hw){return add(chlips,&st.n_plant,&st.t_ht,N,lc,ph,fl,hs,wt,md,hw);}
int chli_pruning(int lc,int ph,int fl,int hs,int wt,int md,int hw){return add(chlis,&st.n_prune,&st.t_ln,N-2,lc,ph,fl,hs,wt,md,hw);}
int chli_pollination(int lc,int ph,int fl,int hs,int wt,int md,int hw){return add(chlids,&st.n_poll,&st.t_heat,N-4,lc,ph,fl,hs,wt,md,hw);}
int chli_harvest(int lc,int ph,int fl,int hs,int wt,int md,int hw){return add(chlihs,&st.n_harv,&st.t_wall,N-6,lc,ph,fl,hs,wt,md,hw);}
int chli_market(int lc,int ph,int fl,int hs,int wt,int md,int hw){return add(chlims,&st.n_mkt,&st.t_mat,N-6,lc,ph,fl,hs,wt,md,hw);}
void chli_report(void){ps("[CHLI] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Ln=");pi(st.t_ln);ps("\nPoll: ");pi(st.n_poll);ps(" Heat=");pi(st.t_heat);ps("\nHarv: ");pi(st.n_harv);ps(" Wall=");pi(st.t_wall);ps("\nMkt: ");pi(st.n_mkt);ps(" Mat=");pi(st.t_mat);ps("\n");}
void chli_state(void){ps("[CHLI] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chili Admin Demo ===\n\n");chli_init();
/* 1=field 2=greenhouse 3=raised_bed 4=container 5=organic */
ps("Chili planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=30+(i*5),fl=5+(i*2),hs=1000+(i*500),wt=3+(i%4),md=60+(i*5),hw=10+(i%6);chli_planting(lc,ph,fl,hs,wt,md,hw);}
ps("\nChili pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=35+(i*4),fl=6+(i*2),hs=2000+(i*400),wt=4+(i%3),md=65+(i*4),hw=11+(i%5);chli_pruning(lc,ph,fl,hs,wt,md,hw);}
ps("\nChili pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=40+(i*3),fl=7+(i*2),hs=3000+(i*300),wt=5+(i%3),md=70+(i*3),hw=12+(i%4);chli_pollination(lc,ph,fl,hs,wt,md,hw);}
ps("\nChili harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=25+(i*6),fl=4+(i*3),hs=5000+(i*1000),wt=2+(i%5),md=55+(i*6),hw=9+(i%7);chli_harvest(lc,ph,fl,hs,wt,md,hw);}
ps("\nChili market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=45+(i*3),fl=8+(i*2),hs=8000+(i*500),wt=6+(i%2),md=80+(i*3),hw=14+(i%3);chli_market(lc,ph,fl,hs,wt,md,hw);}
ps("\n");chli_report();chli_state();ps("\n=== Demo Complete ===\n");return 0;}
