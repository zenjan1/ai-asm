/* augouardia_admin: Augouardia (Tropical Legume) ornamental tree management (v1.0)
 * Augouardia planning, planting, evaluation, bark harvest, market
 * Features: pod length, seed count, flower color, bark type, rain need, flower week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,pod_len,seed_ct,flower_color,bark_tp,rain_nd,flower_wk,active;} augo_t;
typedef struct{int n_plan,n_exec,n_eval,n_bark,n_mkt,t_pod,t_seed,t_flower,t_bark_t,t_rain;} augo_state_t;
static augo_t augops[N],augoes[N-2],augovs[N-4],augobk[N-6],augoms[N-6]; static augo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(augo_t*a,int*cnt,int*sum,int mx,int lc,int pl,int sc,int fc,int bt,int rn,int fw){if(*cnt>=mx)return -1;augo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->pod_len=pl;x->seed_ct=sc;x->flower_color=fc;x->bark_tp=bt;x->rain_nd=rn;x->flower_wk=fw;x->active=1;*sum+=pl;(*cnt)++;ps("[AUGO] Augouardia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pl=");pi(pl);ps(" sc=");pi(sc);ps(" fc=");pi(fc);ps(" bt=");pi(bt);ps(" rn=");pi(rn);ps("\n");return *cnt-1;}
int augo_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_bark=0;st.n_mkt=0;st.t_pod=0;st.t_seed=0;st.t_flower=0;st.t_bark_t=0;st.t_rain=0;for(int i=0;i<N;i++)augops[i].active=0;for(int i=0;i<N-2;i++)augoes[i].active=0;for(int i=0;i<N-4;i++)augovs[i].active=0;for(int i=0;i<N-6;i++)augobk[i].active=0;for(int i=0;i<N-6;i++)augoms[i].active=0;init=1;ps("[AUGO] Augouardia initialized\n");return 0;}
/* 1=tropical_garden 2=avenue 3=orchard 4=container 5=rainforest */
int augo_planning(int lc,int pl,int sc,int fc,int bt,int rn,int fw){return add(augops,&st.n_plan,&st.t_pod,N,lc,pl,sc,fc,bt,rn,fw);}
int augo_execution(int lc,int pl,int sc,int fc,int bt,int rn,int fw){return add(augoes,&st.n_exec,&st.t_seed,N-2,lc,pl,sc,fc,bt,rn,fw);}
int augo_evaluation(int lc,int pl,int sc,int fc,int bt,int rn,int fw){return add(augovs,&st.n_eval,&st.t_flower,N-4,lc,pl,sc,fc,bt,rn,fw);}
int augo_bark_harvest(int lc,int pl,int sc,int fc,int bt,int rn,int fw){return add(augobk,&st.n_bark,&st.t_bark_t,N-6,lc,pl,sc,fc,bt,rn,fw);}
int augo_market(int lc,int pl,int sc,int fc,int bt,int rn,int fw){return add(augoms,&st.n_mkt,&st.t_rain,N-6,lc,pl,sc,fc,bt,rn,fw);}
void augo_report(void){ps("[AUGO] Plan: ");pi(st.n_plan);ps(" pod=");pi(st.t_pod);ps("\nExec: ");pi(st.n_exec);ps(" seed=");pi(st.t_seed);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nBark: ");pi(st.n_bark);ps(" bark_t=");pi(st.t_bark_t);ps("\nMkt: ");pi(st.n_mkt);ps(" rain=");pi(st.t_rain);ps("\n");}
void augo_state(void){ps("[AUGO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Bark=");pi(st.n_bark);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Augouardia (Tropical Legume) Admin Demo ===\n\n");augo_init();
ps("Augouardia planning (tropical layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;augo_planning(lc,8+(i*3),4+(i%6),(i%7)+1,3+(i%3),50+(i*8),18+(i%6));}
ps("\nAugouardia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;augo_execution(lc,10+(i*2),5+(i%5),(i%7)+1,4+(i%3),55+(i*7),20+(i%5));}
ps("\nAugouardia evaluation (flower check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;augo_evaluation(lc,12+(i*2),6+(i%4),(i%6)+2,5+(i%2),60+(i*6),22+(i%4));}
ps("\nAugouardia bark harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;augo_bark_harvest(lc,7+(i*3),3+(i%5),(i%5)+1,3+(i%3),48+(i*7),16+(i%5));}
ps("\nAugouardia ornamental tree market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;augo_market(lc,14+(i*2),7+(i%3),(i%6)+2,6+(i%2),65+(i*5),24+(i%3));}
ps("\n");augo_report();augo_state();ps("\n=== Demo Complete ===\n");return 0;}
