/* anarrhinum_admin: Anarrhinum (Goosewort) rocky slope groundcover management (v1.0)
 * Anarrhinum planning, planting, evaluation, propagation, market
 * Features: stem length, flower count, leaf width, bloom color, root depth, spread rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_len,flower_ct,leaf_wd,bloom_color,root_dep,spread_rt,active;} anr_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_stem,t_flower,t_leaf,t_root,t_spread;} anr_state_t;
static anr_t anrps[N],anres[N-2],anrvs[N-4],anrpr[N-6],anrms[N-6]; static anr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(anr_t*a,int*cnt,int*sum,int mx,int lc,int sl,int fc,int lw,int bc,int rd,int sr){if(*cnt>=mx)return -1;anr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_len=sl;x->flower_ct=fc;x->leaf_wd=lw;x->bloom_color=bc;x->root_dep=rd;x->spread_rt=sr;x->active=1;*sum+=sl;(*cnt)++;ps("[ANR] Anarrhinum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps(" bc=");pi(bc);ps(" rd=");pi(rd);ps(" sr=");pi(sr);ps("\n");return *cnt-1;}
int anr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_stem=0;st.t_flower=0;st.t_leaf=0;st.t_root=0;st.t_spread=0;for(int i=0;i<N;i++)anrps[i].active=0;for(int i=0;i<N-2;i++)anres[i].active=0;for(int i=0;i<N-4;i++)anrvs[i].active=0;for(int i=0;i<N-6;i++)anrpr[i].active=0;for(int i=0;i<N-6;i++)anrms[i].active=0;init=1;ps("[ANR] Anarrhinum initialized\n");return 0;}
/* 1=rockery 2=slope 3=wall 4=alpine 5=container */
int anr_planning(int lc,int sl,int fc,int lw,int bc,int rd,int sr){return add(anrps,&st.n_plan,&st.t_stem,N,lc,sl,fc,lw,bc,rd,sr);}
int anr_execution(int lc,int sl,int fc,int lw,int bc,int rd,int sr){return add(anres,&st.n_exec,&st.t_flower,N-2,lc,sl,fc,lw,bc,rd,sr);}
int anr_evaluation(int lc,int sl,int fc,int lw,int bc,int rd,int sr){return add(anrvs,&st.n_eval,&st.t_leaf,N-4,lc,sl,fc,lw,bc,rd,sr);}
int anr_propagation(int lc,int sl,int fc,int lw,int bc,int rd,int sr){return add(anrpr,&st.n_prop,&st.t_root,N-6,lc,sl,fc,lw,bc,rd,sr);}
int anr_market(int lc,int sl,int fc,int lw,int bc,int rd,int sr){return add(anrms,&st.n_mkt,&st.t_spread,N-6,lc,sl,fc,lw,bc,rd,sr);}
void anr_report(void){ps("[ANR] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nProp: ");pi(st.n_prop);ps(" root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void anr_state(void){ps("[ANR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anarrhinum (Goosewort) Admin Demo ===\n\n");anr_init();
ps("Anarrhinum planning (rocky slope layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;anr_planning(lc,20+(i*5),6+(i%4),5+(i*2),(i%6)+1,15+(i*4),3+(i%5));}
ps("\nAnarrhinum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;anr_execution(lc,25+(i*4),8+(i%3),6+(i*2),(i%6)+1,18+(i*3),4+(i%4));}
ps("\nAnarrhinum evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;anr_evaluation(lc,30+(i*3),10+(i%3),7+(i*2),(i%5)+2,20+(i*3),5+(i%3));}
ps("\nAnarrhinum propagation...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;anr_propagation(lc,15+(i*4),5+(i%3),4+(i*2),(i%4)+1,12+(i*4),3+(i%4));}
ps("\nAnarrhinum groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;anr_market(lc,35+(i*3),12+(i%2),8+(i*2),(i%6)+2,22+(i*3),6+(i%3));}
ps("\n");anr_report();anr_state();ps("\n=== Demo Complete ===\n");return 0;}
