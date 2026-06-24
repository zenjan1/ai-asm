/* aquilegia_admin: Aquilegia (Columbine) woodland perennial management (v1.0)
 * Aquilegia planning, planting, evaluation, seed collection, market
 * Features: spur length, petal count, flower color, leaf shape, stem height, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spur_len,petal_ct,flower_color,leaf_shp,stem_ht,bloom_wk,active;} aql_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_spur,t_petal,t_color,t_leaf,t_stem;} aql_state_t;
static aql_t aqlps[N],aqles[N-2],aqlvs[N-4],aqlsd[N-6],aqlms[N-6]; static aql_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aql_t*a,int*cnt,int*sum,int mx,int lc,int sl,int pc,int fc,int ls,int sh,int bw){if(*cnt>=mx)return -1;aql_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spur_len=sl;x->petal_ct=pc;x->flower_color=fc;x->leaf_shp=ls;x->stem_ht=sh;x->bloom_wk=bw;x->active=1;*sum+=sl;(*cnt)++;ps("[AQL] Aquilegia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" ls=");pi(ls);ps(" sh=");pi(sh);ps("\n");return *cnt-1;}
int aql_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_spur=0;st.t_petal=0;st.t_color=0;st.t_leaf=0;st.t_stem=0;for(int i=0;i<N;i++)aqlps[i].active=0;for(int i=0;i<N-2;i++)aqles[i].active=0;for(int i=0;i<N-4;i++)aqlvs[i].active=0;for(int i=0;i<N-6;i++)aqlsd[i].active=0;for(int i=0;i<N-6;i++)aqlms[i].active=0;init=1;ps("[AQL] Aquilegia initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=rockery 4=border 5=container */
int aql_planning(int lc,int sl,int pc,int fc,int ls,int sh,int bw){return add(aqlps,&st.n_plan,&st.t_spur,N,lc,sl,pc,fc,ls,sh,bw);}
int aql_execution(int lc,int sl,int pc,int fc,int ls,int sh,int bw){return add(aqles,&st.n_exec,&st.t_petal,N-2,lc,sl,pc,fc,ls,sh,bw);}
int aql_evaluation(int lc,int sl,int pc,int fc,int ls,int sh,int bw){return add(aqlvs,&st.n_eval,&st.t_color,N-4,lc,sl,pc,fc,ls,sh,bw);}
int aql_seed_collection(int lc,int sl,int pc,int fc,int ls,int sh,int bw){return add(aqlsd,&st.n_seed,&st.t_leaf,N-6,lc,sl,pc,fc,ls,sh,bw);}
int aql_market(int lc,int sl,int pc,int fc,int ls,int sh,int bw){return add(aqlms,&st.n_mkt,&st.t_stem,N-6,lc,sl,pc,fc,ls,sh,bw);}
void aql_report(void){ps("[AQL] Plan: ");pi(st.n_plan);ps(" spur=");pi(st.t_spur);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nSeed: ");pi(st.n_seed);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" stem=");pi(st.t_stem);ps("\n");}
void aql_state(void){ps("[AQL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aquilegia (Columbine) Admin Demo ===\n\n");aql_init();
ps("Aquilegia planning (woodland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aql_planning(lc,20+(i*5),5+(i%4),(i%7)+1,3+(i%3),30+(i*6),18+(i%6));}
ps("\nAquilegia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aql_execution(lc,22+(i*4),6+(i%3),(i%7)+1,4+(i%3),35+(i*5),20+(i%5));}
ps("\nAquilegia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aql_evaluation(lc,25+(i*3),7+(i%3),(i%6)+2,5+(i%2),40+(i*4),22+(i%4));}
ps("\nAquilegia seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aql_seed_collection(lc,18+(i*4),4+(i%3),(i%5)+1,3+(i%3),25+(i*5),16+(i%5));}
ps("\nAquilegia perennial market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aql_market(lc,28+(i*3),8+(i%2),(i%6)+2,6+(i%2),45+(i*3),24+(i%3));}
ps("\n");aql_report();aql_state();ps("\n=== Demo Complete ===\n");return 0;}
