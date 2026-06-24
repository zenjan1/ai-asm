/* arabis_admin: Arabis (Rock Cress) alpine groundcover management (v1.0)
 * Arabis planning, planting, evaluation, division, market
 * Features: flower cluster count, petal count, leaf rosette size, stem height, bloom color, spread rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,cluster_ct,petal_ct,leaf_ros,stem_ht,bloom_color,spread_rt,active;} arb_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_cluster,t_petal,t_leaf,t_stem,t_spread;} arb_state_t;
static arb_t arbps[N],arbes[N-2],arbvs[N-4],arbdv[N-6],arbms[N-6]; static arb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arb_t*a,int*cnt,int*sum,int mx,int lc,int cc,int pc,int lr,int sh,int bc,int sr){if(*cnt>=mx)return -1;arb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->cluster_ct=cc;x->petal_ct=pc;x->leaf_ros=lr;x->stem_ht=sh;x->bloom_color=bc;x->spread_rt=sr;x->active=1;*sum+=cc;(*cnt)++;ps("[ARB] Arabis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cc=");pi(cc);ps(" pc=");pi(pc);ps(" lr=");pi(lr);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" sr=");pi(sr);ps("\n");return *cnt-1;}
int arb_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_cluster=0;st.t_petal=0;st.t_leaf=0;st.t_stem=0;st.t_spread=0;for(int i=0;i<N;i++)arbps[i].active=0;for(int i=0;i<N-2;i++)arbes[i].active=0;for(int i=0;i<N-4;i++)arbvs[i].active=0;for(int i=0;i<N-6;i++)arbdv[i].active=0;for(int i=0;i<N-6;i++)arbms[i].active=0;init=1;ps("[ARB] Arabis initialized\n");return 0;}
/* 1=rockery 2=alpine 3=wall 4=container 5=groundcover */
int arb_planning(int lc,int cc,int pc,int lr,int sh,int bc,int sr){return add(arbps,&st.n_plan,&st.t_cluster,N,lc,cc,pc,lr,sh,bc,sr);}
int arb_execution(int lc,int cc,int pc,int lr,int sh,int bc,int sr){return add(arbes,&st.n_exec,&st.t_petal,N-2,lc,cc,pc,lr,sh,bc,sr);}
int arb_evaluation(int lc,int cc,int pc,int lr,int sh,int bc,int sr){return add(arbvs,&st.n_eval,&st.t_leaf,N-4,lc,cc,pc,lr,sh,bc,sr);}
int arb_division(int lc,int cc,int pc,int lr,int sh,int bc,int sr){return add(arbdv,&st.n_div,&st.t_stem,N-6,lc,cc,pc,lr,sh,bc,sr);}
int arb_market(int lc,int cc,int pc,int lr,int sh,int bc,int sr){return add(arbms,&st.n_mkt,&st.t_spread,N-6,lc,cc,pc,lr,sh,bc,sr);}
void arb_report(void){ps("[ARB] Plan: ");pi(st.n_plan);ps(" cluster=");pi(st.t_cluster);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nDiv: ");pi(st.n_div);ps(" stem=");pi(st.t_stem);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void arb_state(void){ps("[ARB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Arabis (Rock Cress) Admin Demo ===\n\n");arb_init();
ps("Arabis planning (alpine layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;arb_planning(lc,8+(i*3),4+(i%4),6+(i*2),10+(i*3),(i%6)+1,5+(i%4));}
ps("\nArabis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;arb_execution(lc,10+(i*2),5+(i%3),7+(i*2),12+(i*3),(i%6)+1,6+(i%3));}
ps("\nArabis evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;arb_evaluation(lc,12+(i*2),6+(i%3),8+(i*2),15+(i*2),(i%5)+2,7+(i%3));}
ps("\nArabis division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;arb_division(lc,6+(i*3),3+(i%3),5+(i*2),8+(i*3),(i%4)+1,4+(i%4));}
ps("\nArabis groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;arb_market(lc,14+(i*2),7+(i%2),9+(i*2),18+(i*2),(i%5)+2,8+(i%3));}
ps("\n");arb_report();arb_state();ps("\n=== Demo Complete ===\n");return 0;}
