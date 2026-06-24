/* bidens_admin: Bidens (Beggarticks) wildflower meadow management (v1.0)
 * Bidens planning, planting, evaluation, seed collection, market
 * Features: flower diameter, petal count, stem height, flower color, seed spine, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,stem_ht,flower_color,seed_sp,bloom_wk,active;} bide_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_petal,t_stem,t_color,t_seed_sp;} bide_state_t;
static bide_t bideps[N],bidees[N-2],bidevs[N-4],bidesd[N-6],bidems[N-6]; static bide_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bide_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int sh,int fc,int ss,int bw){if(*cnt>=mx)return -1;bide_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->stem_ht=sh;x->flower_color=fc;x->seed_sp=ss;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BIDE] Bidens ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps("\n");return *cnt-1;}
int bide_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_color=0;st.t_seed_sp=0;for(int i=0;i<N;i++)bideps[i].active=0;for(int i=0;i<N-2;i++)bidees[i].active=0;for(int i=0;i<N-4;i++)bidevs[i].active=0;for(int i=0;i<N-6;i++)bidesd[i].active=0;for(int i=0;i<N-6;i++)bidems[i].active=0;init=1;ps("[BIDE] Bidens initialized\n");return 0;}
/* 1=meadow 2=wildflower_patch 3=pollinator 4=roadside 5=prairie */
int bide_planning(int lc,int fd,int pc,int sh,int fc,int ss,int bw){return add(bideps,&st.n_plan,&st.t_flower,N,lc,fd,pc,sh,fc,ss,bw);}
int bide_execution(int lc,int fd,int pc,int sh,int fc,int ss,int bw){return add(bidees,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,sh,fc,ss,bw);}
int bide_evaluation(int lc,int fd,int pc,int sh,int fc,int ss,int bw){return add(bidevs,&st.n_eval,&st.t_stem,N-4,lc,fd,pc,sh,fc,ss,bw);}
int bide_seed_collection(int lc,int fd,int pc,int sh,int fc,int ss,int bw){return add(bidesd,&st.n_seed,&st.t_color,N-6,lc,fd,pc,sh,fc,ss,bw);}
int bide_market(int lc,int fd,int pc,int sh,int fc,int ss,int bw){return add(bidems,&st.n_mkt,&st.t_seed_sp,N-6,lc,fd,pc,sh,fc,ss,bw);}
void bide_report(void){ps("[BIDE] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed_sp=");pi(st.t_seed_sp);ps("\n");}
void bide_state(void){ps("[BIDE] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bidens (Beggarticks) Admin Demo ===\n\n");bide_init();
ps("Bidens planning (meadow layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bide_planning(lc,3+(i*2),5+(i%5),30+(i*6),(i%5)+1,4+(i%3),28+(i%6));}
ps("\nBidens execution (sowing)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bide_execution(lc,4+(i*2),6+(i%4),35+(i*5),(i%5)+1,5+(i%3),30+(i%5));}
ps("\nBidens evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bide_evaluation(lc,5+(i*2),7+(i%3),40+(i*4),(i%4)+2,6+(i%2),32+(i%4));}
ps("\nBidens seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bide_seed_collection(lc,2+(i*2),4+(i%4),25+(i*6),(i%5)+1,3+(i%3),26+(i%5));}
ps("\nBidens wildflower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bide_market(lc,6+(i*2),8+(i%3),(i%4)+2,45+(i*3),7+(i%2),34+(i%3));}
ps("\n");bide_report();bide_state();ps("\n=== Demo Complete ===\n");return 0;}
