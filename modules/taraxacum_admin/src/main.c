/* taraxacum_admin: Taraxacum (Dandelion) medicinal herb and edible green (v1.0)
 * Taraxacum planning, execution, evaluation, root harvest, medicinal market
 * Features: root harvest, leaf rosette count, seed dispersal, lawn management, medicinal grade
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,loc,root_dp,leaf_ct,seed_hd,harvest_kg,med_gr,season,active;} tar_t;
typedef struct{int n_plan,n_exec,n_eval,n_root,n_mkt,t_dp,t_leaf,t_seed,t_harv,t_med;} tar_state_t;
static tar_t tarps[N],tares[N-2],tarvs[N-4],tarrs[N-6],tarms[N-6]; static tar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tar_t*a,int*cnt,int*sum,int mx,int lc,int rd,int lc2,int sh,int hv,int mg,int sn){if(*cnt>=mx)return -1;tar_t*x=&a[*cnt];x->id=*cnt;x->loc=lc;x->root_dp=rd;x->leaf_ct=lc2;x->seed_hd=sh;x->harvest_kg=hv;x->med_gr=mg;x->season=sn;x->active=1;*sum+=rd;(*cnt)++;ps("[TAR] Taraxacum ");pi(*cnt-1);ps(" loc=");pi(lc);ps(" dp=");pi(rd);ps(" lf=");pi(lc2);ps(" sd=");pi(sh);ps(" hv=");pi(hv);ps("\n");return *cnt-1;}
int tar_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_root=0;st.n_mkt=0;st.t_dp=0;st.t_leaf=0;st.t_seed=0;st.t_harv=0;st.t_med=0;for(int i=0;i<N;i++)tarps[i].active=0;for(int i=0;i<N-2;i++)tares[i].active=0;for(int i=0;i<N-4;i++)tarvs[i].active=0;for(int i=0;i<N-6;i++)tarrs[i].active=0;for(int i=0;i<N-6;i++)tarms[i].active=0;init=1;ps("[TAR] Taraxacum (dandelion) initialized\n");return 0;}
/* 1=lawn 2=meadow 3=garden 4=wild 5=cultivated */
int tar_planning(int lc,int rd,int lf,int sh,int hv,int mg,int sn){return add(tarps,&st.n_plan,&st.t_dp,N,lc,rd,lf,sh,hv,mg,sn);}
int tar_execution(int lc,int rd,int lf,int sh,int hv,int mg,int sn){return add(tares,&st.n_exec,&st.t_leaf,N-2,lc,rd,lf,sh,hv,mg,sn);}
int tar_evaluation(int lc,int rd,int lf,int sh,int hv,int mg,int sn){return add(tarvs,&st.n_eval,&st.t_seed,N-4,lc,rd,lf,sh,hv,mg,sn);}
int tar_root(int lc,int rd,int lf,int sh,int hv,int mg,int sn){return add(tarrs,&st.n_root,&st.t_harv,N-6,lc,rd,lf,sh,hv,mg,sn);}
int tar_market(int lc,int rd,int lf,int sh,int hv,int mg,int sn){return add(tarms,&st.n_mkt,&st.t_med,N-6,lc,rd,lf,sh,hv,mg,sn);}
void tar_report(void){ps("[TAR] Plan: ");pi(st.n_plan);ps(" dp=");pi(st.t_dp);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" seed=");pi(st.t_seed);ps("\nRoot: ");pi(st.n_root);ps(" harv=");pi(st.t_harv);ps("\nMkt: ");pi(st.n_mkt);ps(" med=");pi(st.t_med);ps("\n");}
void tar_state(void){ps("[TAR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Root=");pi(st.n_root);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Taraxacum (Dandelion) Admin Demo ===\n\n");tar_init();
ps("Taraxacum planning (site survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;tar_planning(lc,15+(i*5),8+(i*3),4+(i%4),2+(i*2),(i%3)+1,(i%4)+1);}
ps("\nTaraxacum execution (leaf harvest)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;tar_execution(lc,18+(i*4),10+(i*2),5+(i%3),3+(i*2),(i%3)+1,(i%3)+1);}
ps("\nTaraxacum evaluation (quality check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;tar_evaluation(lc,20+(i*3),12+(i*2),6+(i%3),4+(i*1),(i%2)+2,(i%3)+2);}
ps("\nTaraxacum root harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;tar_root(lc,25+(i*4),6+(i*3),3+(i%3),1+(i%2),1,(i%4)+1);}
ps("\nTaraxacum medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;tar_market(lc,22+(i*3),14+(i*2),7+(i%2),5+(i*1),(i%3)+1,(i%3)+1);}
ps("\n");tar_report();tar_state();ps("\n=== Demo Complete ===\n");return 0;}
