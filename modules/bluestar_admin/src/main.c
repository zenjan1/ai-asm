/* bluestar_admin: Bluestar (Amsonia) native wildflower management (v1.0)
 * Bluestar planning, planting, evaluation, seed collection, market
 * Features: flower diameter, leaf width, stem height, flower color, seed pod, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,leaf_wd,stem_ht,flower_color,seed_pd,bloom_wk,active;} blst_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_leaf,t_stem,t_color,t_seed;} blst_state_t;
static blst_t blstps[N],blstes[N-2],blstvs[N-4],blstsd[N-6],blstms[N-6]; static blst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blst_t*a,int*cnt,int*sum,int mx,int lc,int fd,int lw,int sh,int fc,int sp,int bw){if(*cnt>=mx)return -1;blst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->leaf_wd=lw;x->stem_ht=sh;x->flower_color=fc;x->seed_pd=sp;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLST] Bluestar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" lw=");pi(lw);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" sp=");pi(sp);ps("\n");return *cnt-1;}
int blst_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_seed=0;for(int i=0;i<N;i++)blstps[i].active=0;for(int i=0;i<N-2;i++)blstes[i].active=0;for(int i=0;i<N-4;i++)blstvs[i].active=0;for(int i=0;i<N-6;i++)blstsd[i].active=0;for(int i=0;i<N-6;i++)blstms[i].active=0;init=1;ps("[BLST] Bluestar initialized\n");return 0;}
/* 1=native_garden 2=prairie 3=woodland_edge 4=meadow 5=border */
int blst_planning(int lc,int fd,int lw,int sh,int fc,int sp,int bw){return add(blstps,&st.n_plan,&st.t_flower,N,lc,fd,lw,sh,fc,sp,bw);}
int blst_execution(int lc,int fd,int lw,int sh,int fc,int sp,int bw){return add(blstes,&st.n_exec,&st.t_leaf,N-2,lc,fd,lw,sh,fc,sp,bw);}
int blst_evaluation(int lc,int fd,int lw,int sh,int fc,int sp,int bw){return add(blstvs,&st.n_eval,&st.t_stem,N-4,lc,fd,lw,sh,fc,sp,bw);}
int blst_seed_collection(int lc,int fd,int lw,int sh,int fc,int sp,int bw){return add(blstsd,&st.n_seed,&st.t_color,N-6,lc,fd,lw,sh,fc,sp,bw);}
int blst_market(int lc,int fd,int lw,int sh,int fc,int sp,int bw){return add(blstms,&st.n_mkt,&st.t_seed,N-6,lc,fd,lw,sh,fc,sp,bw);}
void blst_report(void){ps("[BLST] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void blst_state(void){ps("[BLST] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluestar (Amsonia) Admin Demo ===\n\n");blst_init();
ps("Bluestar planning (native garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blst_planning(lc,1+(i*1),3+(i*2),30+(i*5),(i%4)+1,2+(i*2),20+(i%6));}
ps("\nBluestar execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blst_execution(lc,2+(i*1),4+(i*2),35+(i*4),(i%4)+1,3+(i*2),22+(i%5));}
ps("\nBluestar evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blst_evaluation(lc,3+(i*1),5+(i*2),40+(i*3),(i%3)+2,4+(i*2),24+(i%4));}
ps("\nBluestar seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blst_seed_collection(lc,1+(i*1),2+(i*2),25+(i*5),(i%4)+1,1+(i*2),18+(i%5));}
ps("\nBluestar market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blst_market(lc,4+(i*1),6+(i*2),45+(i*3),(i%3)+3,5+(i*2),26+(i%3));}
ps("\n");blst_report();blst_state();ps("\n=== Demo Complete ===\n");return 0;}
