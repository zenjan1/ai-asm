/* asclepias_admin: Asclepias (Milkweed) monarch butterfly habitat management (v1.0)
 * Asclepias planning, planting, evaluation, seed collection, market
 * Features: stem height, flower cluster count, petal color, seed pod count, plant spread, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_ht,flower_cl,petal_color,seed_pod,plant_sp,bloom_wk,active;} asc_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_stem,t_flower,t_petal,t_seed_p,t_spread;} asc_state_t;
static asc_t ascps[N],ascss[N-2],ascvs[N-4],ascsc[N-6],ascms[N-6]; static asc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asc_t*a,int*cnt,int*sum,int mx,int lc,int sh,int fc,int pc,int sp,int plsp,int bw){if(*cnt>=mx)return -1;asc_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_ht=sh;x->flower_cl=fc;x->petal_color=pc;x->seed_pod=sp;x->plant_sp=plsp;x->bloom_wk=bw;x->active=1;*sum+=sh;(*cnt)++;ps("[ASC] Asclepias ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" pc=");pi(pc);ps(" sp=");pi(sp);ps(" plsp=");pi(plsp);ps("\n");return *cnt-1;}
int asc_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_stem=0;st.t_flower=0;st.t_petal=0;st.t_seed_p=0;st.t_spread=0;for(int i=0;i<N;i++)ascps[i].active=0;for(int i=0;i<N-2;i++)ascss[i].active=0;for(int i=0;i<N-4;i++)ascvs[i].active=0;for(int i=0;i<N-6;i++)ascsc[i].active=0;for(int i=0;i<N-6;i++)ascms[i].active=0;init=1;ps("[ASC] Asclepias initialized\n");return 0;}
/* 1=monarch_habitat 2=wildflower 3=meadow 4=container 5=restoration */
int asc_planning(int lc,int sh,int fc,int pc,int sp,int plsp,int bw){return add(ascps,&st.n_plan,&st.t_stem,N,lc,sh,fc,pc,sp,plsp,bw);}
int asc_execution(int lc,int sh,int fc,int pc,int sp,int plsp,int bw){return add(ascss,&st.n_exec,&st.t_flower,N-2,lc,sh,fc,pc,sp,plsp,bw);}
int asc_evaluation(int lc,int sh,int fc,int pc,int sp,int plsp,int bw){return add(ascvs,&st.n_eval,&st.t_petal,N-4,lc,sh,fc,pc,sp,plsp,bw);}
int asc_seed_collection(int lc,int sh,int fc,int pc,int sp,int plsp,int bw){return add(ascsc,&st.n_seed,&st.t_seed_p,N-6,lc,sh,fc,pc,sp,plsp,bw);}
int asc_market(int lc,int sh,int fc,int pc,int sp,int plsp,int bw){return add(ascms,&st.n_mkt,&st.t_spread,N-6,lc,sh,fc,pc,sp,plsp,bw);}
void asc_report(void){ps("[ASC] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" petal=");pi(st.t_petal);ps("\nSeed: ");pi(st.n_seed);ps(" seed_pod=");pi(st.t_seed_p);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void asc_state(void){ps("[ASC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Asclepias (Milkweed) Admin Demo ===\n\n");asc_init();
ps("Asclepias planning (monarch habitat)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;asc_planning(lc,60+(i*12),4+(i%4),(i%6)+1,3+(i%3),20+(i*5),22+(i%6));}
ps("\nAsclepias execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;asc_execution(lc,65+(i*10),5+(i%3),(i%6)+1,4+(i%3),25+(i*4),24+(i%5));}
ps("\nAsclepias evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;asc_evaluation(lc,70+(i*8),6+(i%3),(i%5)+2,5+(i%2),28+(i*3),26+(i%4));}
ps("\nAsclepias seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;asc_seed_collection(lc,55+(i*10),3+(i%3),(i%4)+1,2+(i%3),18+(i*4),20+(i%5));}
ps("\nAsclepias native plant market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;asc_market(lc,75+(i*6),7+(i%2),(i%5)+2,6+(i%2),30+(i*3),28+(i%3));}
ps("\n");asc_report();asc_state();ps("\n=== Demo Complete ===\n");return 0;}
