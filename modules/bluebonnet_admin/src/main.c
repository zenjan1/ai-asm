/* bluebonnet_admin: Bluebonnet (Lupinus texensis) wildflower management (v1.0)
 * Bluebonnet planning, planting, evaluation, seed collection, market
 * Features: petal count, spike height, leaf width, flower color, seed pod, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,spike_ht,leaf_wd,flower_color,seed_pd,bloom_wk,active;} blun_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_petal,t_spike,t_leaf,t_color,t_seed;} blun_state_t;
static blun_t blunps[N],blunes[N-2],blunvs[N-4],blunsd[N-6],blunms[N-6]; static blun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blun_t*a,int*cnt,int*sum,int mx,int lc,int pc,int sh,int lw,int fc,int sd,int bw){if(*cnt>=mx)return -1;blun_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->spike_ht=sh;x->leaf_wd=lw;x->flower_color=fc;x->seed_pd=sd;x->bloom_wk=bw;x->active=1;*sum+=pc;(*cnt)++;ps("[BLUN] Bluebonnet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" lw=");pi(lw);ps(" fc=");pi(fc);ps(" sd=");pi(sd);ps("\n");return *cnt-1;}
int blun_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_petal=0;st.t_spike=0;st.t_leaf=0;st.t_color=0;st.t_seed=0;for(int i=0;i<N;i++)blunps[i].active=0;for(int i=0;i<N-2;i++)blunes[i].active=0;for(int i=0;i<N-4;i++)blunvs[i].active=0;for(int i=0;i<N-6;i++)blunsd[i].active=0;for(int i=0;i<N-6;i++)blunms[i].active=0;init=1;ps("[BLUN] Bluebonnet initialized\n");return 0;}
/* 1=fields 2=roadsides 3=meadows 4=pastures 5=wildflower_garden */
int blun_planning(int lc,int pc,int sh,int lw,int fc,int sd,int bw){return add(blunps,&st.n_plan,&st.t_petal,N,lc,pc,sh,lw,fc,sd,bw);}
int blun_execution(int lc,int pc,int sh,int lw,int fc,int sd,int bw){return add(blunes,&st.n_exec,&st.t_spike,N-2,lc,pc,sh,lw,fc,sd,bw);}
int blun_evaluation(int lc,int pc,int sh,int lw,int fc,int sd,int bw){return add(blunvs,&st.n_eval,&st.t_leaf,N-4,lc,pc,sh,lw,fc,sd,bw);}
int blun_seed_collection(int lc,int pc,int sh,int lw,int fc,int sd,int bw){return add(blunsd,&st.n_seed,&st.t_color,N-6,lc,pc,sh,lw,fc,sd,bw);}
int blun_market(int lc,int pc,int sh,int lw,int fc,int sd,int bw){return add(blunms,&st.n_mkt,&st.t_seed,N-6,lc,pc,sh,lw,fc,sd,bw);}
void blun_report(void){ps("[BLUN] Plan: ");pi(st.n_plan);ps(" petal=");pi(st.t_petal);ps("\nExec: ");pi(st.n_exec);ps(" spike=");pi(st.t_spike);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void blun_state(void){ps("[BLUN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluebonnet (Lupinus) Admin Demo ===\n\n");blun_init();
ps("Bluebonnet planning (field layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blun_planning(lc,10+(i*3),15+(i*4),5+(i*2),(i%4)+1,3+(i*2),14+(i%6));}
ps("\nBluebonnet execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blun_execution(lc,12+(i*2),18+(i*3),6+(i*2),(i%4)+1,4+(i*2),16+(i%5));}
ps("\nBluebonnet evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blun_evaluation(lc,14+(i*2),20+(i*3),7+(i*2),(i%3)+2,5+(i*2),18+(i%4));}
ps("\nBluebonnet seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blun_seed_collection(lc,8+(i*3),12+(i*4),4+(i*2),(i%4)+1,2+(i*2),12+(i%5));}
ps("\nBluebonnet wildflower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blun_market(lc,16+(i*2),22+(i*3),8+(i*2),(i%3)+3,6+(i*2),20+(i%3));}
ps("\n");blun_report();blun_state();ps("\n=== Demo Complete ===\n");return 0;}
