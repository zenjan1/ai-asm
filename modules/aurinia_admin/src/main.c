/* aurinia_admin: Aurinia (Golden Alyssum) rock garden groundcover management (v1.0)
 * Aurinia planning, planting, evaluation, trimming, market
 * Features: flower cluster, petal color, stem height, leaf coverage, bloom period, drought tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_cl,petal_color,stem_ht,leaf_cov,bloom_pd,drought_tl,active;} auri_t;
typedef struct{int n_plan,n_exec,n_eval,n_trim,n_mkt,t_flower,t_petal,t_stem,t_leaf,t_bloom;} auri_state_t;
static auri_t aurips[N],auries[N-2],aurivs[N-4],auritm[N-6],aurims[N-6]; static auri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(auri_t*a,int*cnt,int*sum,int mx,int lc,int fc,int pc,int sh,int lcc,int bp,int dt){if(*cnt>=mx)return -1;auri_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_cl=fc;x->petal_color=pc;x->stem_ht=sh;x->leaf_cov=lcc;x->bloom_pd=bp;x->drought_tl=dt;x->active=1;*sum+=fc;(*cnt)++;ps("[AURI] Aurinia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fc=");pi(fc);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" lcc=");pi(lcc);ps(" bp=");pi(bp);ps("\n");return *cnt-1;}
int auri_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trim=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_leaf=0;st.t_bloom=0;for(int i=0;i<N;i++)aurips[i].active=0;for(int i=0;i<N-2;i++)auries[i].active=0;for(int i=0;i<N-4;i++)aurivs[i].active=0;for(int i=0;i<N-6;i++)auritm[i].active=0;for(int i=0;i<N-6;i++)aurims[i].active=0;init=1;ps("[AURI] Aurinia initialized\n");return 0;}
/* 1=rock_garden 2=wall 3=container 4=slope 5=dry_border */
int auri_planning(int lc,int fc,int pc,int sh,int lcc,int bp,int dt){return add(aurips,&st.n_plan,&st.t_flower,N,lc,fc,pc,sh,lcc,bp,dt);}
int auri_execution(int lc,int fc,int pc,int sh,int lcc,int bp,int dt){return add(auries,&st.n_exec,&st.t_petal,N-2,lc,fc,pc,sh,lcc,bp,dt);}
int auri_evaluation(int lc,int fc,int pc,int sh,int lcc,int bp,int dt){return add(aurivs,&st.n_eval,&st.t_stem,N-4,lc,fc,pc,sh,lcc,bp,dt);}
int auri_trimming(int lc,int fc,int pc,int sh,int lcc,int bp,int dt){return add(auritm,&st.n_trim,&st.t_leaf,N-6,lc,fc,pc,sh,lcc,bp,dt);}
int auri_market(int lc,int fc,int pc,int sh,int lcc,int bp,int dt){return add(aurims,&st.n_mkt,&st.t_bloom,N-6,lc,fc,pc,sh,lcc,bp,dt);}
void auri_report(void){ps("[AURI] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nTrim: ");pi(st.n_trim);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" bloom=");pi(st.t_bloom);ps("\n");}
void auri_state(void){ps("[AURI] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trim=");pi(st.n_trim);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aurinia (Golden Alyssum) Admin Demo ===\n\n");auri_init();
ps("Aurinia planning (rock garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;auri_planning(lc,6+(i*3),(i%5)+1,8+(i*2),40+(i*5),20+(i%6),70+(i*3));}
ps("\nAurinia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;auri_execution(lc,8+(i*2),(i%5)+1,10+(i*2),45+(i*4),22+(i%5),72+(i*3));}
ps("\nAurinia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;auri_evaluation(lc,10+(i*2),(i%4)+2,12+(i*2),50+(i*3),24+(i%4),75+(i%2));}
ps("\nAurinia trimming...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;auri_trimming(lc,5+(i*3),(i%5)+1,7+(i*2),35+(i*4),18+(i%5),65+(i*3));}
ps("\nAurinia groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;auri_market(lc,12+(i*2),(i%4)+2,14+(i*2),55+(i*3),26+(i%3),78+(i%2));}
ps("\n");auri_report();auri_state();ps("\n=== Demo Complete ===\n");return 0;}
