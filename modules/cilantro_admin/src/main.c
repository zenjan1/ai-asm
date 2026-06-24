/* cilantro_admin: Cilantro (Coriandrum sativum) herb cultivation (v1.0)
 * Cilantro planting, thinning, bolting, harvest, market
 * Features: plant_ht_cm, leaf_span_cm, stem_count, seed_yield_g, oil_pct, bolt_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,lf_sp,stm_ct,seed_yd,oil_pct,bolt_wk,active;} chnt_t;
typedef struct{int n_plant,n_thin,n_bolt,n_harv,n_mkt,t_ht,t_sp,t_stm,t_seed,t_oil;} chnt_state_t;
static chnt_t chntps[N],chnts[N-2],chntds[N-4],chnths[N-6],chntms[N-6]; static chnt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chnt_t*a,int*cnt,int*sum,int mx,int lc,int ph,int ls,int sc,int sy,int op,int bw){if(*cnt>=mx)return -1;chnt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->lf_sp=ls;x->stm_ct=sc;x->seed_yd=sy;x->oil_pct=op;x->bolt_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[CHNT] Cilantro ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" ls=");pi(ls);ps(" sc=");pi(sc);ps(" sy=");pi(sy);ps(" op=");pi(op);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int chnt_init(void){if(init)return -1;st.n_plant=0;st.n_thin=0;st.n_bolt=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_sp=0;st.t_stm=0;st.t_seed=0;st.t_oil=0;for(int i=0;i<N;i++)chntps[i].active=0;for(int i=0;i<N-2;i++)chnts[i].active=0;for(int i=0;i<N-4;i++)chntds[i].active=0;for(int i=0;i<N-6;i++)chnths[i].active=0;for(int i=0;i<N-6;i++)chntms[i].active=0;init=1;ps("[CHNT] Cilantro initialized\n");return 0;}
int chnt_planting(int lc,int ph,int ls,int sc,int sy,int op,int bw){return add(chntps,&st.n_plant,&st.t_ht,N,lc,ph,ls,sc,sy,op,bw);}
int chnt_thinning(int lc,int ph,int ls,int sc,int sy,int op,int bw){return add(chnts,&st.n_thin,&st.t_sp,N-2,lc,ph,ls,sc,sy,op,bw);}
int chnt_bolting(int lc,int ph,int ls,int sc,int sy,int op,int bw){return add(chntds,&st.n_bolt,&st.t_stm,N-4,lc,ph,ls,sc,sy,op,bw);}
int chnt_harvest(int lc,int ph,int ls,int sc,int sy,int op,int bw){return add(chnths,&st.n_harv,&st.t_seed,N-6,lc,ph,ls,sc,sy,op,bw);}
int chnt_market(int lc,int ph,int ls,int sc,int sy,int op,int bw){return add(chntms,&st.n_mkt,&st.t_oil,N-6,lc,ph,ls,sc,sy,op,bw);}
void chnt_report(void){ps("[CHNT] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nThin: ");pi(st.n_thin);ps(" Sp=");pi(st.t_sp);ps("\nBolt: ");pi(st.n_bolt);ps(" Stm=");pi(st.t_stm);ps("\nHarv: ");pi(st.n_harv);ps(" Seed=");pi(st.t_seed);ps("\nMkt: ");pi(st.n_mkt);ps(" Oil=");pi(st.t_oil);ps("\n");}
void chnt_state(void){ps("[CHNT] Plant=");pi(st.n_plant);ps(" Thin=");pi(st.n_thin);ps(" Bolt=");pi(st.n_bolt);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cilantro Admin Demo ===\n\n");chnt_init();
/* 1=garden 2=raised_bed 3=greenhouse 4=container 5=market */
ps("Cilantro planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=15+(i*4),ls=10+(i*3),sc=3+(i*2),sy=5+(i*3),op=1+(i%5),bw=10+(i%4);chnt_planting(lc,ph,ls,sc,sy,op,bw);}
ps("\nCilantro thinning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=18+(i*3),ls=12+(i*3),sc=4+(i*2),sy=6+(i*2),op=2+(i%4),bw=11+(i%3);chnt_thinning(lc,ph,ls,sc,sy,op,bw);}
ps("\nCilantro bolting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=22+(i*3),ls=14+(i*2),sc=5+(i*2),sy=7+(i*2),op=3+(i%3),bw=12+(i%3);chnt_bolting(lc,ph,ls,sc,sy,op,bw);}
ps("\nCilantro harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=12+(i*5),ls=8+(i*4),sc=2+(i*3),sy=4+(i*4),op=1+(i%6),bw=9+(i%5);chnt_harvest(lc,ph,ls,sc,sy,op,bw);}
ps("\nCilantro market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=25+(i*3),ls=16+(i*2),sc=6+(i*2),sy=8+(i*2),op=4+(i%3),bw=13+(i%2);chnt_market(lc,ph,ls,sc,sy,op,bw);}
ps("\n");chnt_report();chnt_state();ps("\n=== Demo Complete ===\n");return 0;}
