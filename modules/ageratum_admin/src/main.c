/* ageratum_admin: Ageratum (Floss Flower) soft blue annual and bedding plant management (v1.0)
 * Ageratum planning, planting, evaluation, propagation, market
 * Features: flower cluster density, bloom duration, plant height, fuzzy texture, pollinator visits, shade tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,cluster_dens,bloom_dk,plant_ht,fuzzy_tx,pollinator_v,shade_tol,active;} age_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_cluster,t_bloom,t_plant,t_fuzzy,t_pollinator;} age_state_t;
static age_t ageps[N],agees[N-2],agevs[N-4],agepr[N-6],agems[N-6]; static age_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(age_t*a,int*cnt,int*sum,int mx,int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){if(*cnt>=mx)return -1;age_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->cluster_dens=cd;x->bloom_dk=bd;x->plant_ht=ph;x->fuzzy_tx=ft;x->pollinator_v=pv;x->shade_tol=st_tol;x->active=1;*sum+=cd;(*cnt)++;ps("[AGE] Ageratum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cd=");pi(cd);ps(" bd=");pi(bd);ps(" ph=");pi(ph);ps(" ft=");pi(ft);ps(" pv=");pi(pv);ps("\n");return *cnt-1;}
int age_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_cluster=0;st.t_bloom=0;st.t_plant=0;st.t_fuzzy=0;st.t_pollinator=0;for(int i=0;i<N;i++)ageps[i].active=0;for(int i=0;i<N-2;i++)agees[i].active=0;for(int i=0;i<N-4;i++)agevs[i].active=0;for(int i=0;i<N-6;i++)agepr[i].active=0;for(int i=0;i<N-6;i++)agems[i].active=0;init=1;ps("[AGE] Ageratum (floss flower) initialized\n");return 0;}
/* 1=bedding 2=border 3=container 4=shade_garden 5=edging */
int age_planning(int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){return add(ageps,&st.n_plan,&st.t_cluster,N,lc,cd,bd,ph,ft,pv,st_tol);}
int age_execution(int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){return add(agees,&st.n_exec,&st.t_bloom,N-2,lc,cd,bd,ph,ft,pv,st_tol);}
int age_evaluation(int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){return add(agevs,&st.n_eval,&st.t_plant,N-4,lc,cd,bd,ph,ft,pv,st_tol);}
int age_propagation(int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){return add(agepr,&st.n_prop,&st.t_fuzzy,N-6,lc,cd,bd,ph,ft,pv,st_tol);}
int age_market(int lc,int cd,int bd,int ph,int ft,int pv,int st_tol){return add(agems,&st.n_mkt,&st.t_pollinator,N-6,lc,cd,bd,ph,ft,pv,st_tol);}
void age_report(void){ps("[AGE] Plan: ");pi(st.n_plan);ps(" cluster=");pi(st.t_cluster);ps("\nExec: ");pi(st.n_exec);ps(" bloom=");pi(st.t_bloom);ps("\nEval: ");pi(st.n_eval);ps(" plant=");pi(st.t_plant);ps("\nProp: ");pi(st.n_prop);ps(" fuzzy=");pi(st.t_fuzzy);ps("\nMkt: ");pi(st.n_mkt);ps(" poll=");pi(st.t_pollinator);ps("\n");}
void age_state(void){ps("[AGE] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ageratum (Floss Flower) Admin Demo ===\n\n");age_init();
ps("Ageratum planning (bedding layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;age_planning(lc,15+(i*4),25+(i*5),20+(i*3),(i%5)+1,10+(i*3),(i%4)+1);}
ps("\nAgeratum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;age_execution(lc,18+(i*3),28+(i*4),22+(i*3),(i%5)+1,12+(i*2),(i%4)+1);}
ps("\nAgeratum evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;age_evaluation(lc,20+(i*3),30+(i*3),25+(i*2),(i%4)+2,14+(i*2),(i%3)+2);}
ps("\nAgeratum propagation (cuttings)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;age_propagation(lc,12+(i*3),20+(i*4),18+(i*3),(i%4)+1,8+(i*3),(i%4)+1);}
ps("\nAgeratum bedding plant market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;age_market(lc,22+(i*2),32+(i*3),28+(i*2),(i%4)+2,16+(i*2),(i%3)+2);}
ps("\n");age_report();age_state();ps("\n=== Demo Complete ===\n");return 0;}
