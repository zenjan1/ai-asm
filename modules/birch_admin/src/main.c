/* birch_admin: Birch (Silver Birch) ornamental tree management (v1.0)
 * Birch planning, planting, evaluation, pruning, market
 * Features: tree height, trunk diameter, leaf width, bark color, catkin length, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trunk_dia,leaf_wd,bark_color,catkin_ln,growth_rt,active;} birc_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_ht,t_trunk,t_leaf,t_bark,t_catkin;} birc_state_t;
static birc_t bircps[N],birces[N-2],bircvs[N-4],bircpr[N-6],bircms[N-6]; static birc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(birc_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int lw,int bc,int cl,int gr){if(*cnt>=mx)return -1;birc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trunk_dia=td;x->leaf_wd=lw;x->bark_color=bc;x->catkin_ln=cl;x->growth_rt=gr;x->active=1;*sum+=th;(*cnt)++;ps("[BIRC] Birch ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" lw=");pi(lw);ps(" bc=");pi(bc);ps(" cl=");pi(cl);ps("\n");return *cnt-1;}
int birc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_ht=0;st.t_trunk=0;st.t_leaf=0;st.t_bark=0;st.t_catkin=0;for(int i=0;i<N;i++)bircps[i].active=0;for(int i=0;i<N-2;i++)birces[i].active=0;for(int i=0;i<N-4;i++)bircvs[i].active=0;for(int i=0;i<N-6;i++)bircpr[i].active=0;for(int i=0;i<N-6;i++)bircms[i].active=0;init=1;ps("[BIRC] Birch initialized\n");return 0;}
/* 1=park 2=avenue 3=woodland 4=specimen 5=countryside */
int birc_planning(int lc,int th,int td,int lw,int bc,int cl,int gr){return add(bircps,&st.n_plan,&st.t_ht,N,lc,th,td,lw,bc,cl,gr);}
int birc_execution(int lc,int th,int td,int lw,int bc,int cl,int gr){return add(birces,&st.n_exec,&st.t_trunk,N-2,lc,th,td,lw,bc,cl,gr);}
int birc_evaluation(int lc,int th,int td,int lw,int bc,int cl,int gr){return add(bircvs,&st.n_eval,&st.t_leaf,N-4,lc,th,td,lw,bc,cl,gr);}
int birc_pruning(int lc,int th,int td,int lw,int bc,int cl,int gr){return add(bircpr,&st.n_prune,&st.t_bark,N-6,lc,th,td,lw,bc,cl,gr);}
int birc_market(int lc,int th,int td,int lw,int bc,int cl,int gr){return add(bircms,&st.n_mkt,&st.t_catkin,N-6,lc,th,td,lw,bc,cl,gr);}
void birc_report(void){ps("[BIRC] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" trunk=");pi(st.t_trunk);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nPrune: ");pi(st.n_prune);ps(" bark=");pi(st.t_bark);ps("\nMkt: ");pi(st.n_mkt);ps(" catkin=");pi(st.t_catkin);ps("\n");}
void birc_state(void){ps("[BIRC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Birch (Silver Birch) Admin Demo ===\n\n");birc_init();
ps("Birch planning (park layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;birc_planning(lc,500+(i*80),10+(i*5),5+(i*2),(i%4)+1,8+(i*3),15+(i*4));}
ps("\nBirch execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;birc_execution(lc,550+(i*70),12+(i*4),6+(i*2),(i%4)+1,9+(i*2),17+(i*3));}
ps("\nBirch evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;birc_evaluation(lc,600+(i*60),14+(i*3),7+(i*2),(i%3)+2,10+(i*2),19+(i*3));}
ps("\nBirch pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;birc_pruning(lc,450+(i*75),8+(i*5),4+(i*2),(i%4)+1,7+(i*3),13+(i*4));}
ps("\nBirch ornamental tree market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;birc_market(lc,650+(i*50),16+(i*3),8+(i*2),(i%3)+3,12+(i*2),21+(i%3));}
ps("\n");birc_report();birc_state();ps("\n=== Demo Complete ===\n");return 0;}
