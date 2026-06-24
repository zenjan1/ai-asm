/* trillium_admin: Trillium (Wake-robin) woodland wildflower conservation (v1.0)
 * Trillium planning, execution, evaluation, rhizome propagation, seed conservation
 * Features: rhizome age tracking, petal color variants, seed dispersal, mycorrhizal dependency
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,species,age_yr,petal_col,seed_set,mycorr_idx,canopy,shade_t,active;} tri_t;
typedef struct{int n_plan,n_exec,n_eval,n_rhiz,n_seed,t_age,t_petal,t_seed_s,t_mycorr,t_canopy;} tri_state_t;
static tri_t trips[N],tries[N-2],trivs[N-4],trirs[N-6],triss[N-6]; static tri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tri_t*a,int*cnt,int*sum,int mx,int sp,int ay,int pc,int ss,int mi,int cn,int sh){if(*cnt>=mx)return -1;tri_t*x=&a[*cnt];x->id=*cnt;x->species=sp;x->age_yr=ay;x->petal_col=pc;x->seed_set=ss;x->mycorr_idx=mi;x->canopy=cn;x->shade_t=sh;x->active=1;*sum+=ay;(*cnt)++;ps("[TRI] Trillium ");pi(*cnt-1);ps(" sp=");pi(sp);ps(" age=");pi(ay);ps(" pc=");pi(pc);ps(" ss=");pi(ss);ps(" my=");pi(mi);ps("\n");return *cnt-1;}
int tri_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_rhiz=0;st.n_seed=0;st.t_age=0;st.t_petal=0;st.t_seed_s=0;st.t_mycorr=0;st.t_canopy=0;for(int i=0;i<N;i++)trips[i].active=0;for(int i=0;i<N-2;i++)tries[i].active=0;for(int i=0;i<N-4;i++)trivs[i].active=0;for(int i=0;i<N-6;i++)trirs[i].active=0;for(int i=0;i<N-6;i++)triss[i].active=0;init=1;ps("[TRI] Trillium (wake-robin) initialized\n");return 0;}
/* 1=grandiflorum(white) 2=erectum(red) 3=luteum(yellow) 4=cernuum(nodding) 5=camtschaticum */
int tri_planning(int sp,int ay,int pc,int ss,int mi,int cn,int sh){return add(trips,&st.n_plan,&st.t_age,N,sp,ay,pc,ss,mi,cn,sh);}
int tri_execution(int sp,int ay,int pc,int ss,int mi,int cn,int sh){return add(tries,&st.n_exec,&st.t_petal,N-2,sp,ay,pc,ss,mi,cn,sh);}
int tri_evaluation(int sp,int ay,int pc,int ss,int mi,int cn,int sh){return add(trivs,&st.n_eval,&st.t_seed_s,N-4,sp,ay,pc,ss,mi,cn,sh);}
int tri_rhizome(int sp,int ay,int pc,int ss,int mi,int cn,int sh){return add(trirs,&st.n_rhiz,&st.t_mycorr,N-6,sp,ay,pc,ss,mi,cn,sh);}
int tri_seed(int sp,int ay,int pc,int ss,int mi,int cn,int sh){return add(triss,&st.n_seed,&st.t_canopy,N-6,sp,ay,pc,ss,mi,cn,sh);}
void tri_report(void){ps("[TRI] Plan: ");pi(st.n_plan);ps(" age=");pi(st.t_age);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" seed=");pi(st.t_seed_s);ps("\nRhiz: ");pi(st.n_rhiz);ps(" mycorr=");pi(st.t_mycorr);ps("\nSeed: ");pi(st.n_seed);ps(" canopy=");pi(st.t_canopy);ps("\n");}
void tri_state(void){ps("[TRI] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Rhiz=");pi(st.n_rhiz);ps(" Seed=");pi(st.n_seed);ps("\n");}
int main(void){
ps("=== Trillium (Wake-robin) Admin Demo ===\n\n");tri_init();
ps("Trillium planning (woodland survey)...\n");
for(int i=0;i<N;i++){int sp=(i%5)+1;tri_planning(sp,3+(i%8),(i%4)+1,50+(i*5),(i%5)+1,60+(i*4),(i%3)+1);}
ps("\nTrillium execution (planting)...\n");
for(int i=0;i<N-2;i++){int sp=(i%4)+2;tri_execution(sp,4+(i%7),(i%3)+2,55+(i*4),(i%4)+2,65+(i*3),(i%3)+1);}
ps("\nTrillium evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int sp=(i%3)+1;tri_evaluation(sp,5+(i%6),(i%3)+2,60+(i*3),(i%3)+2,70+(i*3),(i%2)+2);}
ps("\nTrillium rhizome propagation...\n");
for(int i=0;i<N-6;i++){int sp=(i%5)+1;tri_rhizome(sp,2+(i%5),(i%4)+1,45+(i*4),(i%5)+1,55+(i*4),1);}
ps("\nTrillium seed conservation...\n");
for(int i=0;i<N-6;i++){int sp=(i%4)+1;tri_seed(sp,6+(i%6),(i%3)+2,65+(i*3),(i%3)+2,75+(i*2),(i%3)+1);}
ps("\n");tri_report();tri_state();ps("\n=== Demo Complete ===\n");return 0;}
