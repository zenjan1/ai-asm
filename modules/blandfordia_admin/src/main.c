/* blandfordia_admin: Blandfordia (Christmas Bells) ornamental perennial management (v1.0)
 * Blandfordia planning, planting, evaluation, seed collection, market
 * Features: flower length, leaf width, stem height, flower color, seed count, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_ln,leaf_wd,stem_ht,flower_color,seed_ct,bloom_wk,active;} blan_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_leaf,t_stem,t_color,t_seed;} blan_state_t;
static blan_t blanps[N],blanes[N-2],blanvs[N-4],blansd[N-6],blanms[N-6]; static blan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blan_t*a,int*cnt,int*sum,int mx,int lc,int fl,int lw,int sh,int fc,int sc,int bw){if(*cnt>=mx)return -1;blan_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_ln=fl;x->leaf_wd=lw;x->stem_ht=sh;x->flower_color=fc;x->seed_ct=sc;x->bloom_wk=bw;x->active=1;*sum+=fl;(*cnt)++;ps("[BLAN] Blandfordia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fl=");pi(fl);ps(" lw=");pi(lw);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" sc=");pi(sc);ps("\n");return *cnt-1;}
int blan_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_seed=0;for(int i=0;i<N;i++)blanps[i].active=0;for(int i=0;i<N-2;i++)blanes[i].active=0;for(int i=0;i<N-4;i++)blanvs[i].active=0;for(int i=0;i<N-6;i++)blansd[i].active=0;for(int i=0;i<N-6;i++)blanms[i].active=0;init=1;ps("[BLAN] Blandfordia initialized\n");return 0;}
/* 1=garden_bed 2=rockery 3=greenhouse 4=border 5=container */
int blan_planning(int lc,int fl,int lw,int sh,int fc,int sc,int bw){return add(blanps,&st.n_plan,&st.t_flower,N,lc,fl,lw,sh,fc,sc,bw);}
int blan_execution(int lc,int fl,int lw,int sh,int fc,int sc,int bw){return add(blanes,&st.n_exec,&st.t_leaf,N-2,lc,fl,lw,sh,fc,sc,bw);}
int blan_evaluation(int lc,int fl,int lw,int sh,int fc,int sc,int bw){return add(blanvs,&st.n_eval,&st.t_stem,N-4,lc,fl,lw,sh,fc,sc,bw);}
int blan_seed_collection(int lc,int fl,int lw,int sh,int fc,int sc,int bw){return add(blansd,&st.n_seed,&st.t_color,N-6,lc,fl,lw,sh,fc,sc,bw);}
int blan_market(int lc,int fl,int lw,int sh,int fc,int sc,int bw){return add(blanms,&st.n_mkt,&st.t_seed,N-6,lc,fl,lw,sh,fc,sc,bw);}
void blan_report(void){ps("[BLAN] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void blan_state(void){ps("[BLAN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blandfordia (Christmas Bells) Admin Demo ===\n\n");blan_init();
ps("Blandfordia planning (garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blan_planning(lc,40+(i*5),12+(i*3),50+(i*8),(i%4)+1,80+(i*15),26+(i%6));}
ps("\nBlandfordia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blan_execution(lc,45+(i*4),14+(i*2),55+(i*7),(i%4)+1,90+(i*12),28+(i%5));}
ps("\nBlandfordia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blan_evaluation(lc,50+(i*3),16+(i*2),60+(i*6),(i%3)+2,100+(i*10),30+(i%4));}
ps("\nBlandfordia seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blan_seed_collection(lc,35+(i*4),10+(i*3),45+(i*7),(i%4)+1,70+(i*12),24+(i%5));}
ps("\nBlandfordia ornamental market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blan_market(lc,55+(i*3),18+(i*2),65+(i*5),(i%3)+3,110+(i*8),32+(i%3));}
ps("\n");blan_report();blan_state();ps("\n=== Demo Complete ===\n");return 0;}
