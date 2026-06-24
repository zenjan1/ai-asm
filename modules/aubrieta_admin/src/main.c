/* aubrieta_admin: Aubrieta (Rock Cress) cascading groundcover management (v1.0)
 * Aubrieta planning, planting, evaluation, trimming, market
 * Features: flower carpet size, petal color, stem length, leaf coverage, bloom period, spread rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,carpet_sz,petal_color,stem_len,leaf_cov,bloom_pd,spread_rt,active;} aub_t;
typedef struct{int n_plan,n_exec,n_eval,n_trim,n_mkt,t_carpet,t_petal,t_stem,t_leaf,t_bloom;} aub_state_t;
static aub_t aubps[N],aubes[N-2],aubvs[N-4],aubtr[N-6],aubms[N-6]; static aub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aub_t*a,int*cnt,int*sum,int mx,int lc,int cs,int pc,int sl,int lcc,int bp,int sr){if(*cnt>=mx)return -1;aub_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->carpet_sz=cs;x->petal_color=pc;x->stem_len=sl;x->leaf_cov=lcc;x->bloom_pd=bp;x->spread_rt=sr;x->active=1;*sum+=cs;(*cnt)++;ps("[AUB] Aubrieta ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cs=");pi(cs);ps(" pc=");pi(pc);ps(" sl=");pi(sl);ps(" lcc=");pi(lcc);ps(" bp=");pi(bp);ps("\n");return *cnt-1;}
int aub_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trim=0;st.n_mkt=0;st.t_carpet=0;st.t_petal=0;st.t_stem=0;st.t_leaf=0;st.t_bloom=0;for(int i=0;i<N;i++)aubps[i].active=0;for(int i=0;i<N-2;i++)aubes[i].active=0;for(int i=0;i<N-4;i++)aubvs[i].active=0;for(int i=0;i<N-6;i++)aubtr[i].active=0;for(int i=0;i<N-6;i++)aubms[i].active=0;init=1;ps("[AUB] Aubrieta initialized\n");return 0;}
/* 1=wall 2=rockery 3=cascading 4=container 5=groundcover */
int aub_planning(int lc,int cs,int pc,int sl,int lcc,int bp,int sr){return add(aubps,&st.n_plan,&st.t_carpet,N,lc,cs,pc,sl,lcc,bp,sr);}
int aub_execution(int lc,int cs,int pc,int sl,int lcc,int bp,int sr){return add(aubes,&st.n_exec,&st.t_petal,N-2,lc,cs,pc,sl,lcc,bp,sr);}
int aub_evaluation(int lc,int cs,int pc,int sl,int lcc,int bp,int sr){return add(aubvs,&st.n_eval,&st.t_stem,N-4,lc,cs,pc,sl,lcc,bp,sr);}
int aub_trimming(int lc,int cs,int pc,int sl,int lcc,int bp,int sr){return add(aubtr,&st.n_trim,&st.t_leaf,N-6,lc,cs,pc,sl,lcc,bp,sr);}
int aub_market(int lc,int cs,int pc,int sl,int lcc,int bp,int sr){return add(aubms,&st.n_mkt,&st.t_bloom,N-6,lc,cs,pc,sl,lcc,bp,sr);}
void aub_report(void){ps("[AUB] Plan: ");pi(st.n_plan);ps(" carpet=");pi(st.t_carpet);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nTrim: ");pi(st.n_trim);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" bloom=");pi(st.t_bloom);ps("\n");}
void aub_state(void){ps("[AUB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trim=");pi(st.n_trim);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aubrieta (Rock Cress) Admin Demo ===\n\n");aub_init();
ps("Aubrieta planning (cascading layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aub_planning(lc,20+(i*5),(i%7)+1,10+(i*3),50+(i*6),14+(i%6),4+(i%4));}
ps("\nAubrieta execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aub_execution(lc,25+(i*4),(i%7)+1,12+(i*2),55+(i*5),16+(i%5),5+(i%3));}
ps("\nAubrieta evaluation (carpet check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aub_evaluation(lc,30+(i*3),(i%6)+2,14+(i*2),60+(i*4),18+(i%4),6+(i%3));}
ps("\nAubrieta trimming...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aub_trimming(lc,18+(i*4),(i%5)+1,8+(i*3),45+(i*5),12+(i%5),3+(i%4));}
ps("\nAubrieta groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aub_market(lc,35+(i*3),(i%6)+2,16+(i*2),65+(i*3),20+(i%3),7+(i%3));}
ps("\n");aub_report();aub_state();ps("\n=== Demo Complete ===\n");return 0;}
