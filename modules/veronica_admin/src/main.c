/* veronica_admin: Veronica (Speedwell) flowering perennial and border plant management (v1.0)
 * Veronica planning, planting, evaluation, propagation, market
 * Features: spike height, flower color, bloom duration, leaf pattern, pollinator visits
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spike_ht,flower_color,bloom_dk,leaf_pat,pollinator_v,spread_rt,active;} vrn_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_spike,t_color,t_bloom,t_leaf,t_pollinator;} vrn_state_t;
static vrn_t vrnps[N],vrnes[N-2],vrnvs[N-4],vrnpr[N-6],vrnms[N-6]; static vrn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vrn_t*a,int*cnt,int*sum,int mx,int lc,int sh,int fc,int bd,int lp,int pv,int sr){if(*cnt>=mx)return -1;vrn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spike_ht=sh;x->flower_color=fc;x->bloom_dk=bd;x->leaf_pat=lp;x->pollinator_v=pv;x->spread_rt=sr;x->active=1;*sum+=sh;(*cnt)++;ps("[VRN] Veronica ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" bd=");pi(bd);ps(" lp=");pi(lp);ps(" pv=");pi(pv);ps("\n");return *cnt-1;}
int vrn_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_spike=0;st.t_color=0;st.t_bloom=0;st.t_leaf=0;st.t_pollinator=0;for(int i=0;i<N;i++)vrnps[i].active=0;for(int i=0;i<N-2;i++)vrnes[i].active=0;for(int i=0;i<N-4;i++)vrnvs[i].active=0;for(int i=0;i<N-6;i++)vrnpr[i].active=0;for(int i=0;i<N-6;i++)vrnms[i].active=0;init=1;ps("[VRN] Veronica (speedwell) initialized\n");return 0;}
/* 1=border 2=rockery 3=cottage_garden 4=meadow 5=container */
int vrn_planning(int lc,int sh,int fc,int bd,int lp,int pv,int sr){return add(vrnps,&st.n_plan,&st.t_spike,N,lc,sh,fc,bd,lp,pv,sr);}
int vrn_execution(int lc,int sh,int fc,int bd,int lp,int pv,int sr){return add(vrnes,&st.n_exec,&st.t_color,N-2,lc,sh,fc,bd,lp,pv,sr);}
int vrn_evaluation(int lc,int sh,int fc,int bd,int lp,int pv,int sr){return add(vrnvs,&st.n_eval,&st.t_bloom,N-4,lc,sh,fc,bd,lp,pv,sr);}
int vrn_propagation(int lc,int sh,int fc,int bd,int lp,int pv,int sr){return add(vrnpr,&st.n_prop,&st.t_leaf,N-6,lc,sh,fc,bd,lp,pv,sr);}
int vrn_market(int lc,int sh,int fc,int bd,int lp,int pv,int sr){return add(vrnms,&st.n_mkt,&st.t_pollinator,N-6,lc,sh,fc,bd,lp,pv,sr);}
void vrn_report(void){ps("[VRN] Plan: ");pi(st.n_plan);ps(" spike=");pi(st.t_spike);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" bloom=");pi(st.t_bloom);ps("\nProp: ");pi(st.n_prop);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" poll=");pi(st.t_pollinator);ps("\n");}
void vrn_state(void){ps("[VRN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Veronica (Speedwell) Admin Demo ===\n\n");vrn_init();
ps("Veronica planning (border layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vrn_planning(lc,30+(i*8),(i%6)+1,20+(i*5),(i%4)+1,10+(i*3),(i%3)+1);}
ps("\nVeronica execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vrn_execution(lc,35+(i*7),(i%6)+1,22+(i*4),(i%4)+1,12+(i*2),(i%3)+1);}
ps("\nVeronica evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vrn_evaluation(lc,40+(i*6),(i%5)+2,25+(i*3),(i%3)+2,14+(i*2),(i%2)+2);}
ps("\nVeronica propagation (division)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vrn_propagation(lc,25+(i*6),(i%4)+1,18+(i*4),(i%4)+1,8+(i*3),(i%3)+1);}
ps("\nVeronica perennial market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vrn_market(lc,45+(i*5),(i%6)+2,28+(i*3),(i%3)+2,16+(i*2),(i%3)+2);}
ps("\n");vrn_report();vrn_state();ps("\n=== Demo Complete ===\n");return 0;}
