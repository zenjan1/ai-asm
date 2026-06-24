/* araucaria_admin: Araucaria (Monkey Puzzle Tree) ornamental conifer management (v1.0)
 * Araucaria planning, planting, evaluation, cone harvest, market
 * Features: trunk diameter, branch tiers, leaf scale, tree height, cone count, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,trunk_dia,branch_tiers,leaf_sc,tree_ht,cone_ct,growth_rt,active;} ara_t;
typedef struct{int n_plan,n_exec,n_eval,n_cone,n_mkt,t_trunk,t_branch,t_leaf,t_tree,t_cone_c;} ara_state_t;
static ara_t araps[N],araes[N-2],aravs[N-4],aracn[N-6],arams[N-6]; static ara_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ara_t*a,int*cnt,int*sum,int mx,int lc,int td,int bt,int ls,int th,int cc,int gr){if(*cnt>=mx)return -1;ara_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->trunk_dia=td;x->branch_tiers=bt;x->leaf_sc=ls;x->tree_ht=th;x->cone_ct=cc;x->growth_rt=gr;x->active=1;*sum+=td;(*cnt)++;ps("[ARA] Araucaria ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" td=");pi(td);ps(" bt=");pi(bt);ps(" ls=");pi(ls);ps(" th=");pi(th);ps(" cc=");pi(cc);ps("\n");return *cnt-1;}
int ara_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cone=0;st.n_mkt=0;st.t_trunk=0;st.t_branch=0;st.t_leaf=0;st.t_tree=0;st.t_cone_c=0;for(int i=0;i<N;i++)araps[i].active=0;for(int i=0;i<N-2;i++)araes[i].active=0;for(int i=0;i<N-4;i++)aravs[i].active=0;for(int i=0;i<N-6;i++)aracn[i].active=0;for(int i=0;i<N-6;i++)arams[i].active=0;init=1;ps("[ARA] Araucaria initialized\n");return 0;}
/* 1=ornamental 2=park 3=botanical 4=container 5=estate */
int ara_planning(int lc,int td,int bt,int ls,int th,int cc,int gr){return add(araps,&st.n_plan,&st.t_trunk,N,lc,td,bt,ls,th,cc,gr);}
int ara_execution(int lc,int td,int bt,int ls,int th,int cc,int gr){return add(araes,&st.n_exec,&st.t_branch,N-2,lc,td,bt,ls,th,cc,gr);}
int ara_evaluation(int lc,int td,int bt,int ls,int th,int cc,int gr){return add(aravs,&st.n_eval,&st.t_leaf,N-4,lc,td,bt,ls,th,cc,gr);}
int ara_cone_harvest(int lc,int td,int bt,int ls,int th,int cc,int gr){return add(aracn,&st.n_cone,&st.t_tree,N-6,lc,td,bt,ls,th,cc,gr);}
int ara_market(int lc,int td,int bt,int ls,int th,int cc,int gr){return add(arams,&st.n_mkt,&st.t_cone_c,N-6,lc,td,bt,ls,th,cc,gr);}
void ara_report(void){ps("[ARA] Plan: ");pi(st.n_plan);ps(" trunk=");pi(st.t_trunk);ps("\nExec: ");pi(st.n_exec);ps(" branch=");pi(st.t_branch);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nCone: ");pi(st.n_cone);ps(" tree=");pi(st.t_tree);ps("\nMkt: ");pi(st.n_mkt);ps(" cone=");pi(st.t_cone_c);ps("\n");}
void ara_state(void){ps("[ARA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cone=");pi(st.n_cone);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Araucaria (Monkey Puzzle Tree) Admin Demo ===\n\n");ara_init();
ps("Araucaria planning (conifer layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ara_planning(lc,15+(i*5),4+(i%4),3+(i%3),100+(i*20),2+(i%5),5+(i*2));}
ps("\nAraucaria execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ara_execution(lc,18+(i*4),5+(i%3),4+(i%3),120+(i*15),3+(i%4),6+(i*2));}
ps("\nAraucaria evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ara_evaluation(lc,20+(i*3),6+(i%3),5+(i%2),140+(i*12),4+(i%3),7+(i*2));}
ps("\nAraucaria cone harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ara_cone_harvest(lc,12+(i*4),3+(i%3),2+(i%3),80+(i*15),1+(i%4),4+(i*2));}
ps("\nAraucaria ornamental market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ara_market(lc,25+(i*3),7+(i%2),6+(i%2),160+(i*10),5+(i%3),8+(i*2));}
ps("\n");ara_report();ara_state();ps("\n=== Demo Complete ===\n");return 0;}
