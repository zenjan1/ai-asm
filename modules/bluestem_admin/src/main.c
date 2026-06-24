/* bluestem_admin: Bluestem (Andropogon) prairie grass management (v1.0)
 * Bluestem planning, planting, evaluation, harvesting, market
 * Features: stem height, leaf width, seed head length, foliage color, root depth, growth week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_ht,leaf_wd,seed_ln,foliage_color,root_dp,growth_wk,active;} bst_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_stem,t_leaf,t_seed,t_foliage,t_root;} bst_state_t;
static bst_t bstps[N],bstes[N-2],bstvs[N-4],bshr[N-6],bstms[N-6]; static bst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bst_t*a,int*cnt,int*sum,int mx,int lc,int sh,int lw,int sl,int fc,int rd,int gw){if(*cnt>=mx)return -1;bst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_ht=sh;x->leaf_wd=lw;x->seed_ln=sl;x->foliage_color=fc;x->root_dp=rd;x->growth_wk=gw;x->active=1;*sum+=sh;(*cnt)++;ps("[BST] Bluestem ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" lw=");pi(lw);ps(" sl=");pi(sl);ps(" fc=");pi(fc);ps(" rd=");pi(rd);ps("\n");return *cnt-1;}
int bst_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_stem=0;st.t_leaf=0;st.t_seed=0;st.t_foliage=0;st.t_root=0;for(int i=0;i<N;i++)bstps[i].active=0;for(int i=0;i<N-2;i++)bstes[i].active=0;for(int i=0;i<N-4;i++)bstvs[i].active=0;for(int i=0;i<N-6;i++)bshr[i].active=0;for(int i=0;i<N-6;i++)bstms[i].active=0;init=1;ps("[BST] Bluestem initialized\n");return 0;}
/* 1=prairie 2=grassland 3=restoration 4=pasture 5=roadsides */
int bst_planning(int lc,int sh,int lw,int sl,int fc,int rd,int gw){return add(bstps,&st.n_plan,&st.t_stem,N,lc,sh,lw,sl,fc,rd,gw);}
int bst_execution(int lc,int sh,int lw,int sl,int fc,int rd,int gw){return add(bstes,&st.n_exec,&st.t_leaf,N-2,lc,sh,lw,sl,fc,rd,gw);}
int bst_evaluation(int lc,int sh,int lw,int sl,int fc,int rd,int gw){return add(bstvs,&st.n_eval,&st.t_seed,N-4,lc,sh,lw,sl,fc,rd,gw);}
int bst_harvesting(int lc,int sh,int lw,int sl,int fc,int rd,int gw){return add(bshr,&st.n_harv,&st.t_foliage,N-6,lc,sh,lw,sl,fc,rd,gw);}
int bst_market(int lc,int sh,int lw,int sl,int fc,int rd,int gw){return add(bstms,&st.n_mkt,&st.t_root,N-6,lc,sh,lw,sl,fc,rd,gw);}
void bst_report(void){ps("[BST] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" seed=");pi(st.t_seed);ps("\nHarv: ");pi(st.n_harv);ps(" foliage=");pi(st.t_foliage);ps("\nMkt: ");pi(st.n_mkt);ps(" root=");pi(st.t_root);ps("\n");}
void bst_state(void){ps("[BST] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluestem (Andropogon) Admin Demo ===\n\n");bst_init();
ps("Bluestem planning (variety layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bst_planning(lc,60+(i*10),2+(i*1),8+(i*3),(i%4)+1,30+(i*5),24+(i%6));}
ps("\nBluestem execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bst_execution(lc,65+(i*8),3+(i*1),10+(i*2),(i%4)+1,35+(i*4),26+(i%5));}
ps("\nBluestem evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bst_evaluation(lc,70+(i*7),4+(i*1),12+(i*2),(i%3)+2,40+(i*3),28+(i%4));}
ps("\nBluestem harvesting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bst_harvesting(lc,55+(i*9),2+(i*1),7+(i*3),(i%4)+1,25+(i*5),22+(i%5));}
ps("\nBluestem market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bst_market(lc,75+(i*6),5+(i*1),14+(i*2),(i%3)+3,45+(i*3),30+(i%3));}
ps("\n");bst_report();bst_state();ps("\n=== Demo Complete ===\n");return 0;}
