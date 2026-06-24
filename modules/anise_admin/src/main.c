/* anise_admin: Anise (Aniseed) spice herb and aromatic seed crop management (v1.0)
 * Anise planning, planting, evaluation, seed harvest, market
 * Features: plant height, seed yield, oil content, bloom cluster, leaf area, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plant_ht,seed_yield,oil_ct,bloom_cl,leaf_ar,harv_wk,active;} ans_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_plant,t_seed_y,t_oil,t_bloom,t_leaf;} ans_state_t;
static ans_t ansps[N],anses[N-2],ansvs[N-4],anssd[N-6],ansms[N-6]; static ans_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ans_t*a,int*cnt,int*sum,int mx,int lc,int ph,int sy,int oc,int bc,int la,int hw){if(*cnt>=mx)return -1;ans_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plant_ht=ph;x->seed_yield=sy;x->oil_ct=oc;x->bloom_cl=bc;x->leaf_ar=la;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[ANS] Anise ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" sy=");pi(sy);ps(" oc=");pi(oc);ps(" bc=");pi(bc);ps(" la=");pi(la);ps("\n");return *cnt-1;}
int ans_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_plant=0;st.t_seed_y=0;st.t_oil=0;st.t_bloom=0;st.t_leaf=0;for(int i=0;i<N;i++)ansps[i].active=0;for(int i=0;i<N-2;i++)anses[i].active=0;for(int i=0;i<N-4;i++)ansvs[i].active=0;for(int i=0;i<N-6;i++)anssd[i].active=0;for(int i=0;i<N-6;i++)ansms[i].active=0;init=1;ps("[ANS] Anise initialized\n");return 0;}
/* 1=herb_garden 2=spice_farm 3=container 4=border 5=medicinal */
int ans_planning(int lc,int ph,int sy,int oc,int bc,int la,int hw){return add(ansps,&st.n_plan,&st.t_plant,N,lc,ph,sy,oc,bc,la,hw);}
int ans_execution(int lc,int ph,int sy,int oc,int bc,int la,int hw){return add(anses,&st.n_exec,&st.t_seed_y,N-2,lc,ph,sy,oc,bc,la,hw);}
int ans_evaluation(int lc,int ph,int sy,int oc,int bc,int la,int hw){return add(ansvs,&st.n_eval,&st.t_oil,N-4,lc,ph,sy,oc,bc,la,hw);}
int ans_seed_harvest(int lc,int ph,int sy,int oc,int bc,int la,int hw){return add(anssd,&st.n_seed,&st.t_bloom,N-6,lc,ph,sy,oc,bc,la,hw);}
int ans_market(int lc,int ph,int sy,int oc,int bc,int la,int hw){return add(ansms,&st.n_mkt,&st.t_leaf,N-6,lc,ph,sy,oc,bc,la,hw);}
void ans_report(void){ps("[ANS] Plan: ");pi(st.n_plan);ps(" plant=");pi(st.t_plant);ps("\nExec: ");pi(st.n_exec);ps(" seed=");pi(st.t_seed_y);ps("\nEval: ");pi(st.n_eval);ps(" oil=");pi(st.t_oil);ps("\nSeed: ");pi(st.n_seed);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void ans_state(void){ps("[ANS] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anise (Aniseed) Admin Demo ===\n\n");ans_init();
ps("Anise planning (herb layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ans_planning(lc,40+(i*8),15+(i*6),8+(i*3),5+(i%4),20+(i*5),28+(i%6));}
ps("\nAnise execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ans_execution(lc,45+(i*6),18+(i*5),10+(i*2),6+(i%3),25+(i*4),30+(i%5));}
ps("\nAnise evaluation (oil check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ans_evaluation(lc,50+(i*5),20+(i*4),12+(i*2),7+(i%3),30+(i*3),32+(i%4));}
ps("\nAnise seed harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ans_seed_harvest(lc,35+(i*6),12+(i*5),6+(i*3),4+(i%4),18+(i*4),26+(i%5));}
ps("\nAnise spice market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ans_market(lc,55+(i*4),22+(i*3),14+(i*2),8+(i%3),35+(i*3),34+(i%3));}
ps("\n");ans_report();ans_state();ps("\n=== Demo Complete ===\n");return 0;}
