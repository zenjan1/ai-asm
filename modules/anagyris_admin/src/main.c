/* anagyris_admin: Anagyris (Bean Caper) medicinal shrub management (v1.0)
 * Anagyris planning, planting, evaluation, bark harvest, market
 * Features: leaf length, seed pod count, bark texture, root depth, compound mg, drought tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_len,seed_pod,bark_tx,root_dep,compound_mg,drought_tol,active;} ana_t;
typedef struct{int n_plan,n_exec,n_eval,n_bark,n_mkt,t_leaf,t_seed,t_bark_tx,t_root,t_compound;} ana_state_t;
static ana_t anaps[N],anaes[N-2],anavs[N-4],anabk[N-6],anams[N-6]; static ana_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ana_t*a,int*cnt,int*sum,int mx,int lc,int ll,int sp,int bt,int rd,int cm,int dt){if(*cnt>=mx)return -1;ana_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_len=ll;x->seed_pod=sp;x->bark_tx=bt;x->root_dep=rd;x->compound_mg=cm;x->drought_tol=dt;x->active=1;*sum+=ll;(*cnt)++;ps("[ANA] Anagyris ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" sp=");pi(sp);ps(" bt=");pi(bt);ps(" rd=");pi(rd);ps(" cm=");pi(cm);ps("\n");return *cnt-1;}
int ana_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_bark=0;st.n_mkt=0;st.t_leaf=0;st.t_seed=0;st.t_bark_tx=0;st.t_root=0;st.t_compound=0;for(int i=0;i<N;i++)anaps[i].active=0;for(int i=0;i<N-2;i++)anaes[i].active=0;for(int i=0;i<N-4;i++)anavs[i].active=0;for(int i=0;i<N-6;i++)anabk[i].active=0;for(int i=0;i<N-6;i++)anams[i].active=0;init=1;ps("[ANA] Anagyris initialized\n");return 0;}
/* 1=arid 2=rock_garden 3=medicinal 4=xeriscape 5=container */
int ana_planning(int lc,int ll,int sp,int bt,int rd,int cm,int dt){return add(anaps,&st.n_plan,&st.t_leaf,N,lc,ll,sp,bt,rd,cm,dt);}
int ana_execution(int lc,int ll,int sp,int bt,int rd,int cm,int dt){return add(anaes,&st.n_exec,&st.t_seed,N-2,lc,ll,sp,bt,rd,cm,dt);}
int ana_evaluation(int lc,int ll,int sp,int bt,int rd,int cm,int dt){return add(anavs,&st.n_eval,&st.t_bark_tx,N-4,lc,ll,sp,bt,rd,cm,dt);}
int ana_bark_harvest(int lc,int ll,int sp,int bt,int rd,int cm,int dt){return add(anabk,&st.n_bark,&st.t_root,N-6,lc,ll,sp,bt,rd,cm,dt);}
int ana_market(int lc,int ll,int sp,int bt,int rd,int cm,int dt){return add(anams,&st.n_mkt,&st.t_compound,N-6,lc,ll,sp,bt,rd,cm,dt);}
void ana_report(void){ps("[ANA] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" seed=");pi(st.t_seed);ps("\nEval: ");pi(st.n_eval);ps(" bark=");pi(st.t_bark_tx);ps("\nBark: ");pi(st.n_bark);ps(" root=");pi(st.t_root);ps("\nMkt: ");pi(st.n_mkt);ps(" compound=");pi(st.t_compound);ps("\n");}
void ana_state(void){ps("[ANA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Bark=");pi(st.n_bark);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anagyris (Bean Caper) Admin Demo ===\n\n");ana_init();
ps("Anagyris planning (shrub layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ana_planning(lc,15+(i*4),4+(i%5),(i%6)+1,30+(i*8),50+(i*12),(i%5)+1);}
ps("\nAnagyris execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ana_execution(lc,18+(i*3),5+(i%4),(i%6)+1,35+(i*6),60+(i*10),(i%5)+1);}
ps("\nAnagyris evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ana_evaluation(lc,20+(i*3),6+(i%3),(i%5)+2,40+(i*5),70+(i*8),(i%4)+2);}
ps("\nAnagyris bark harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ana_bark_harvest(lc,12+(i*4),3+(i%4),(i%4)+1,25+(i*6),40+(i*10),(i%5)+1);}
ps("\nAnagyris medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ana_market(lc,22+(i*3),7+(i%3),(i%6)+2,45+(i*4),80+(i*7),(i%4)+2);}
ps("\n");ana_report();ana_state();ps("\n=== Demo Complete ===\n");return 0;}
