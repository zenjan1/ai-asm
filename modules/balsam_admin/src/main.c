/* balsam_admin: Balsam (Impatiens balsamina) annual flower management (v1.0)
 * Balsam planning, planting, evaluation, deadheading, market
 * Features: flower size, petal count, flower color, stem height, seed pod, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_sz,petal_ct,flower_color,stem_ht,seed_pod,bloom_wk,active;} bal_t;
typedef struct{int n_plan,n_exec,n_eval,n_dead,n_mkt,t_flower,t_petal,t_color,t_stem,t_seed;} bal_state_t;
static bal_t balps[N],bales[N-2],balvs[N-4],baldh[N-6],balms[N-6]; static bal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bal_t*a,int*cnt,int*sum,int mx,int lc,int fs,int pc,int fc,int sh,int sp,int bw){if(*cnt>=mx)return -1;bal_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_sz=fs;x->petal_ct=pc;x->flower_color=fc;x->stem_ht=sh;x->seed_pod=sp;x->bloom_wk=bw;x->active=1;*sum+=fs;(*cnt)++;ps("[BAL] Balsam ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" sh=");pi(sh);ps(" sp=");pi(sp);ps("\n");return *cnt-1;}
int bal_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dead=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_color=0;st.t_stem=0;st.t_seed=0;for(int i=0;i<N;i++)balps[i].active=0;for(int i=0;i<N-2;i++)bales[i].active=0;for(int i=0;i<N-4;i++)balvs[i].active=0;for(int i=0;i<N-6;i++)baldh[i].active=0;for(int i=0;i<N-6;i++)balms[i].active=0;init=1;ps("[BAL] Balsam initialized\n");return 0;}
/* 1=garden_bed 2=container 3=border 4=patio 5=shade_spot */
int bal_planning(int lc,int fs,int pc,int fc,int sh,int sp,int bw){return add(balps,&st.n_plan,&st.t_flower,N,lc,fs,pc,fc,sh,sp,bw);}
int bal_execution(int lc,int fs,int pc,int fc,int sh,int sp,int bw){return add(bales,&st.n_exec,&st.t_petal,N-2,lc,fs,pc,fc,sh,sp,bw);}
int bal_evaluation(int lc,int fs,int pc,int fc,int sh,int sp,int bw){return add(balvs,&st.n_eval,&st.t_color,N-4,lc,fs,pc,fc,sh,sp,bw);}
int bal_deadheading(int lc,int fs,int pc,int fc,int sh,int sp,int bw){return add(baldh,&st.n_dead,&st.t_stem,N-6,lc,fs,pc,fc,sh,sp,bw);}
int bal_market(int lc,int fs,int pc,int fc,int sh,int sp,int bw){return add(balms,&st.n_mkt,&st.t_seed,N-6,lc,fs,pc,fc,sh,sp,bw);}
void bal_report(void){ps("[BAL] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nDead: ");pi(st.n_dead);ps(" stem=");pi(st.t_stem);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void bal_state(void){ps("[BAL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dead=");pi(st.n_dead);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Balsam (Touch-me-not) Admin Demo ===\n\n");bal_init();
ps("Balsam planning (garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bal_planning(lc,3+(i*2),5+(i%5),(i%7)+1,20+(i*5),4+(i%4),22+(i%6));}
ps("\nBalsam execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bal_execution(lc,4+(i*2),6+(i%4),(i%7)+1,22+(i*4),5+(i%3),24+(i%5));}
ps("\nBalsam evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bal_evaluation(lc,5+(i*2),7+(i%3),(i%6)+2,25+(i*3),6+(i%3),26+(i%4));}
ps("\nBalsam deadheading...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bal_deadheading(lc,2+(i*2),4+(i%4),(i%5)+1,18+(i*5),3+(i%4),20+(i%5));}
ps("\nBalsam annual flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bal_market(lc,6+(i*2),8+(i%3),(i%6)+2,28+(i*3),7+(i%3),28+(i%3));}
ps("\n");bal_report();bal_state();ps("\n=== Demo Complete ===\n");return 0;}
