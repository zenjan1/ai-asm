/* bluewood_admin: Bluewood (Cordia) tree and shrub management (v1.0)
 * Bluewood planning, execution, evaluation, pruning, market
 * Features: tree height, trunk diameter, leaf width, bark color, flower color, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trunk_dia,leaf_wd,bark_color,flower_color,growth_rt,active;} blwd_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_tree,t_trunk,t_leaf,t_color,t_growth;} blwd_state_t;
static blwd_t blwdps[N],blwdes[N-2],blwdvs[N-4],blwdpr[N-6],blwdms[N-6]; static blwd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blwd_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int lw,int bc,int fc,int gr){if(*cnt>=mx)return -1;blwd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trunk_dia=td;x->leaf_wd=lw;x->bark_color=bc;x->flower_color=fc;x->growth_rt=gr;x->active=1;*sum+=th;(*cnt)++;ps("[BLWD] Bluewood ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" lw=");pi(lw);ps(" bc=");pi(bc);ps(" fc=");pi(fc);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int blwd_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_tree=0;st.t_trunk=0;st.t_leaf=0;st.t_color=0;st.t_growth=0;for(int i=0;i<N;i++)blwdps[i].active=0;for(int i=0;i<N-2;i++)blwdes[i].active=0;for(int i=0;i<N-4;i++)blwdvs[i].active=0;for(int i=0;i<N-6;i++)blwdpr[i].active=0;for(int i=0;i<N-6;i++)blwdms[i].active=0;init=1;ps("[BLWD] Bluewood initialized\n");return 0;}
int blwd_planning(int lc,int th,int td,int lw,int bc,int fc,int gr){return add(blwdps,&st.n_plan,&st.t_tree,N,lc,th,td,lw,bc,fc,gr);}
int blwd_execution(int lc,int th,int td,int lw,int bc,int fc,int gr){return add(blwdes,&st.n_exec,&st.t_trunk,N-2,lc,th,td,lw,bc,fc,gr);}
int blwd_evaluation(int lc,int th,int td,int lw,int bc,int fc,int gr){return add(blwdvs,&st.n_eval,&st.t_leaf,N-4,lc,th,td,lw,bc,fc,gr);}
int blwd_pruning(int lc,int th,int td,int lw,int bc,int fc,int gr){return add(blwdpr,&st.n_prune,&st.t_color,N-6,lc,th,td,lw,bc,fc,gr);}
int blwd_market(int lc,int th,int td,int lw,int bc,int fc,int gr){return add(blwdms,&st.n_mkt,&st.t_growth,N-6,lc,th,td,lw,bc,fc,gr);}
void blwd_report(void){ps("[BLWD] Plan: ");pi(st.n_plan);ps(" Tree=");pi(st.t_tree);ps("\nExec: ");pi(st.n_exec);ps(" Trunk=");pi(st.t_trunk);ps("\nEval: ");pi(st.n_eval);ps(" Leaf=");pi(st.t_leaf);ps("\nPrune: ");pi(st.n_prune);ps(" Color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" Growth=");pi(st.t_growth);ps("\n");}
void blwd_state(void){ps("[BLWD] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluewood Admin Demo ===\n\n");blwd_init();
/* 1=garden 2=border 3=container 4=park 5=hedgerow */
ps("Bluewood planning...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=5+(i*3),td=10+(i*2),lw=4+(i*1),bc=(i%4)+1,fc=(i%3)+1,gr=2+(i%5);blwd_planning(lc,th,td,lw,bc,fc,gr);}
ps("\nBluewood execution...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=6+(i*2),td=12+(i*2),lw=5+(i*1),bc=(i%4)+1,fc=(i%3)+2,gr=3+(i%4);blwd_execution(lc,th,td,lw,bc,fc,gr);}
ps("\nBluewood evaluation...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=7+(i*2),td=14+(i*2),lw=6+(i*1),bc=(i%4)+2,fc=(i%3)+1,gr=4+(i%3);blwd_evaluation(lc,th,td,lw,bc,fc,gr);}
ps("\nBluewood pruning...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=4+(i*3),td=8+(i*2),lw=3+(i*1),bc=(i%4)+1,fc=(i%3)+3,gr=1+(i%5);blwd_pruning(lc,th,td,lw,bc,fc,gr);}
ps("\nBluewood market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=8+(i*2),td=16+(i*2),lw=7+(i*1),bc=(i%3)+2,fc=(i%3)+1,gr=5+(i%3);blwd_market(lc,th,td,lw,bc,fc,gr);}
ps("\n");blwd_report();blwd_state();ps("\n=== Demo Complete ===\n");return 0;}
