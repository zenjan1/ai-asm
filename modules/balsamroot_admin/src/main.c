/* balsamroot_admin: Balsamroot (Lomatium oreganum) taproot wildflower management (v1.0)
 * Balsamroot planning, planting, evaluation, root harvest, market
 * Features: leaf length, flower stem, root depth, flower color, drought tolerance, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_ln,flower_st,root_dp,flower_color,drought_tl,bloom_wk,active;} bals_t;
typedef struct{int n_plan,n_exec,n_eval,n_root,n_mkt,t_leaf,t_stem,t_root_dp,t_color,t_drought;} bals_state_t;
static bals_t balsps[N],balses[N-2],balsvs[N-4],balsrh[N-6],balsms[N-6]; static bals_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bals_t*a,int*cnt,int*sum,int mx,int lc,int ll,int fst,int rd,int fc,int dt,int bw){if(*cnt>=mx)return -1;bals_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_ln=ll;x->flower_st=fst;x->root_dp=rd;x->flower_color=fc;x->drought_tl=dt;x->bloom_wk=bw;x->active=1;*sum+=ll;(*cnt)++;ps("[BALS] Balsamroot ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" fst=");pi(fst);ps(" rd=");pi(rd);ps(" fc=");pi(fc);ps(" dt=");pi(dt);ps("\n");return *cnt-1;}
int bals_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_root=0;st.n_mkt=0;st.t_leaf=0;st.t_stem=0;st.t_root_dp=0;st.t_color=0;st.t_drought=0;for(int i=0;i<N;i++)balsps[i].active=0;for(int i=0;i<N-2;i++)balses[i].active=0;for(int i=0;i<N-4;i++)balsvs[i].active=0;for(int i=0;i<N-6;i++)balsrh[i].active=0;for(int i=0;i<N-6;i++)balsms[i].active=0;init=1;ps("[BALS] Balsamroot initialized\n");return 0;}
/* 1=prairie 2=meadow 3=sagebrush 4=rocky_slope 5=wildflower_garden */
int bals_planning(int lc,int ll,int fst,int rd,int fc,int dt,int bw){return add(balsps,&st.n_plan,&st.t_leaf,N,lc,ll,fst,rd,fc,dt,bw);}
int bals_execution(int lc,int ll,int fst,int rd,int fc,int dt,int bw){return add(balses,&st.n_exec,&st.t_stem,N-2,lc,ll,fst,rd,fc,dt,bw);}
int bals_evaluation(int lc,int ll,int fst,int rd,int fc,int dt,int bw){return add(balsvs,&st.n_eval,&st.t_root_dp,N-4,lc,ll,fst,rd,fc,dt,bw);}
int bals_root_harvest(int lc,int ll,int fst,int rd,int fc,int dt,int bw){return add(balsrh,&st.n_root,&st.t_color,N-6,lc,ll,fst,rd,fc,dt,bw);}
int bals_market(int lc,int ll,int fst,int rd,int fc,int dt,int bw){return add(balsms,&st.n_mkt,&st.t_drought,N-6,lc,ll,fst,rd,fc,dt,bw);}
void bals_report(void){ps("[BALS] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" root=");pi(st.t_root_dp);ps("\nRoot: ");pi(st.n_root);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" drought=");pi(st.t_drought);ps("\n");}
void bals_state(void){ps("[BALS] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Root=");pi(st.n_root);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Balsamroot (Wild Parsnip) Admin Demo ===\n\n");bals_init();
ps("Balsamroot planning (meadow layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bals_planning(lc,15+(i*4),20+(i*5),30+(i*8),(i%5)+1,65+(i*4),16+(i%6));}
ps("\nBalsamroot execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bals_execution(lc,18+(i*3),22+(i*4),35+(i*7),(i%5)+1,68+(i*3),18+(i%5));}
ps("\nBalsamroot evaluation (root check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bals_evaluation(lc,20+(i*3),25+(i*3),40+(i*6),(i%4)+2,70+(i*3),20+(i%4));}
ps("\nBalsamroot root harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bals_root_harvest(lc,12+(i*4),18+(i*5),25+(i*8),(i%5)+1,60+(i*4),14+(i%5));}
ps("\nBalsamroot wildflower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bals_market(lc,22+(i*3),28+(i*3),45+(i*5),(i%4)+2,72+(i*3),22+(i%3));}
ps("\n");bals_report();bals_state();ps("\n=== Demo Complete ===\n");return 0;}
