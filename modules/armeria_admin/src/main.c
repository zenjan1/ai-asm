/* armeria_admin: Armeria (Thrift / Sea Pink) coastal wildflower management (v1.0)
 * Armeria planning, planting, evaluation, division, market
 * Features: flower ball diameter, stem height, petal color, leaf length, bloom week, salt tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,ball_dia,stem_ht,petal_color,leaf_len,bloom_wk,salt_tol,active;} arm_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_ball,t_stem,t_petal,t_leaf,t_salt;} arm_state_t;
static arm_t armps[N],armes[N-2],armvs[N-4],armdv[N-6],armms[N-6]; static arm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arm_t*a,int*cnt,int*sum,int mx,int lc,int bd,int sh,int pc,int ll,int bw,int slt){if(*cnt>=mx)return -1;arm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->ball_dia=bd;x->stem_ht=sh;x->petal_color=pc;x->leaf_len=ll;x->bloom_wk=bw;x->salt_tol=slt;x->active=1;*sum+=bd;(*cnt)++;ps("[ARM] Armeria ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bd=");pi(bd);ps(" sh=");pi(sh);ps(" pc=");pi(pc);ps(" ll=");pi(ll);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int arm_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_ball=0;st.t_stem=0;st.t_petal=0;st.t_leaf=0;st.t_salt=0;for(int i=0;i<N;i++)armps[i].active=0;for(int i=0;i<N-2;i++)armes[i].active=0;for(int i=0;i<N-4;i++)armvs[i].active=0;for(int i=0;i<N-6;i++)armdv[i].active=0;for(int i=0;i<N-6;i++)armms[i].active=0;init=1;ps("[ARM] Armeria initialized\n");return 0;}
/* 1=coastal 2=rockery 3=border 4=container 5=wildflower */
int arm_planning(int lc,int bd,int sh,int pc,int ll,int bw,int slt){return add(armps,&st.n_plan,&st.t_ball,N,lc,bd,sh,pc,ll,bw,slt);}
int arm_execution(int lc,int bd,int sh,int pc,int ll,int bw,int slt){return add(armes,&st.n_exec,&st.t_stem,N-2,lc,bd,sh,pc,ll,bw,slt);}
int arm_evaluation(int lc,int bd,int sh,int pc,int ll,int bw,int slt){return add(armvs,&st.n_eval,&st.t_petal,N-4,lc,bd,sh,pc,ll,bw,slt);}
int arm_division(int lc,int bd,int sh,int pc,int ll,int bw,int slt){return add(armdv,&st.n_div,&st.t_leaf,N-6,lc,bd,sh,pc,ll,bw,slt);}
int arm_market(int lc,int bd,int sh,int pc,int ll,int bw,int slt){return add(armms,&st.n_mkt,&st.t_salt,N-6,lc,bd,sh,pc,ll,bw,slt);}
void arm_report(void){ps("[ARM] Plan: ");pi(st.n_plan);ps(" ball=");pi(st.t_ball);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" petal=");pi(st.t_petal);ps("\nDiv: ");pi(st.n_div);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" salt=");pi(st.t_salt);ps("\n");}
void arm_state(void){ps("[ARM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Armeria (Thrift / Sea Pink) Admin Demo ===\n\n");arm_init();
ps("Armeria planning (coastal layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;arm_planning(lc,8+(i*2),15+(i*3),(i%6)+1,10+(i*2),18+(i%6),6+(i%4));}
ps("\nArmeria execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;arm_execution(lc,9+(i*2),18+(i*3),(i%6)+1,12+(i*2),20+(i%5),7+(i%3));}
ps("\nArmeria evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;arm_evaluation(lc,10+(i*2),20+(i*2),(i%5)+2,14+(i*2),22+(i%4),8+(i%3));}
ps("\nArmeria division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;arm_division(lc,7+(i*2),12+(i*3),(i%4)+1,8+(i*2),16+(i%5),5+(i%4));}
ps("\nArmeria coastal flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;arm_market(lc,11+(i*2),22+(i*2),(i%5)+2,16+(i*2),24+(i%3),9+(i%3));}
ps("\n");arm_report();arm_state();ps("\n=== Demo Complete ===\n");return 0;}
