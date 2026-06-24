/* aucuba_admin: Aucuba (Japanese Laurel) shade evergreen shrub management (v1.0)
 * Aucuba planning, planting, evaluation, pruning, market
 * Features: leaf length, leaf width, berry color, plant height, shade tolerance, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_len,leaf_wd,berry_color,plant_ht,shade_tol,growth_rt,active;} aucu_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_leaf,t_width,t_berry,t_ht,t_shade;} aucu_state_t;
static aucu_t aucups[N],aucues[N-2],aucuvs[N-4],aucupr[N-6],aucums[N-6]; static aucu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aucu_t*a,int*cnt,int*sum,int mx,int lc,int ll,int lw,int bc,int ph,int st_val,int gr){if(*cnt>=mx)return -1;aucu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_len=ll;x->leaf_wd=lw;x->berry_color=bc;x->plant_ht=ph;x->shade_tol=st_val;x->growth_rt=gr;x->active=1;*sum+=ll;(*cnt)++;ps("[AUCU] Aucuba ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" lw=");pi(lw);ps(" bc=");pi(bc);ps(" ph=");pi(ph);ps(" st=");pi(st_val);ps("\n");return *cnt-1;}
int aucu_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_leaf=0;st.t_width=0;st.t_berry=0;st.t_ht=0;st.t_shade=0;for(int i=0;i<N;i++)aucups[i].active=0;for(int i=0;i<N-2;i++)aucues[i].active=0;for(int i=0;i<N-4;i++)aucuvs[i].active=0;for(int i=0;i<N-6;i++)aucupr[i].active=0;for(int i=0;i<N-6;i++)aucums[i].active=0;init=1;ps("[AUCU] Aucuba initialized\n");return 0;}
/* 1=shade_border 2=woodland 3=container 4=hedge 5=deep_shade */
int aucu_planning(int lc,int ll,int lw,int bc,int ph,int st_val,int gr){return add(aucups,&st.n_plan,&st.t_leaf,N,lc,ll,lw,bc,ph,st_val,gr);}
int aucu_execution(int lc,int ll,int lw,int bc,int ph,int st_val,int gr){return add(aucues,&st.n_exec,&st.t_width,N-2,lc,ll,lw,bc,ph,st_val,gr);}
int aucu_evaluation(int lc,int ll,int lw,int bc,int ph,int st_val,int gr){return add(aucuvs,&st.n_eval,&st.t_berry,N-4,lc,ll,lw,bc,ph,st_val,gr);}
int aucu_pruning(int lc,int ll,int lw,int bc,int ph,int st_val,int gr){return add(aucupr,&st.n_prune,&st.t_ht,N-6,lc,ll,lw,bc,ph,st_val,gr);}
int aucu_market(int lc,int ll,int lw,int bc,int ph,int st_val,int gr){return add(aucums,&st.n_mkt,&st.t_shade,N-6,lc,ll,lw,bc,ph,st_val,gr);}
void aucu_report(void){ps("[AUCU] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" width=");pi(st.t_width);ps("\nEval: ");pi(st.n_eval);ps(" berry=");pi(st.t_berry);ps("\nPrune: ");pi(st.n_prune);ps(" ht=");pi(st.t_ht);ps("\nMkt: ");pi(st.n_mkt);ps(" shade=");pi(st.t_shade);ps("\n");}
void aucu_state(void){ps("[AUCU] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aucuba (Japanese Laurel) Admin Demo ===\n\n");aucu_init();
ps("Aucuba planning (shade layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aucu_planning(lc,12+(i*3),5+(i%4),(i%5)+1,60+(i*10),70+(i*4),3+(i%5));}
ps("\nAucuba execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aucu_execution(lc,14+(i*3),6+(i%3),(i%5)+1,65+(i*9),72+(i*3),4+(i%4));}
ps("\nAucuba evaluation (berry check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aucu_evaluation(lc,16+(i*2),7+(i%3),(i%4)+2,70+(i*8),75+(i*3),4+(i%3));}
ps("\nAucuba pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aucu_pruning(lc,10+(i*3),4+(i%4),(i%5)+1,55+(i*10),68+(i*4),3+(i%4));}
ps("\nAucuba evergreen shrub market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aucu_market(lc,18+(i*2),8+(i%3),(i%4)+2,75+(i*7),78+(i*3),5+(i%3));}
ps("\n");aucu_report();aucu_state();ps("\n=== Demo Complete ===\n");return 0;}
