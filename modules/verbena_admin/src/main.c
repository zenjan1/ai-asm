/* verbena_admin: Verbena flowering herb and medicinal tea plant management (v1.0)
 * Verbena planning, planting, evaluation, propagation, market
 * Features: flower cluster count, fragrance level, medicinal compound, bloom duration, propagation rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_ct,fragrance,compound_mg,bloom_dk,prop_rt,harvest_ct,active;} vrb_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_flower,t_frag,t_compound,t_bloom,t_prop;} vrb_state_t;
static vrb_t vrbps[N],vrbes[N-2],vrbvs[N-4],vrbpr[N-6],vrbms[N-6]; static vrb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vrb_t*a,int*cnt,int*sum,int mx,int lc,int fc,int fg,int cm,int bd,int pr,int hc){if(*cnt>=mx)return -1;vrb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_ct=fc;x->fragrance=fg;x->compound_mg=cm;x->bloom_dk=bd;x->prop_rt=pr;x->harvest_ct=hc;x->active=1;*sum+=fc;(*cnt)++;ps("[VRB] Verbena ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fc=");pi(fc);ps(" fg=");pi(fg);ps(" cm=");pi(cm);ps(" bd=");pi(bd);ps(" pr=");pi(pr);ps("\n");return *cnt-1;}
int vrb_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_flower=0;st.t_frag=0;st.t_compound=0;st.t_bloom=0;st.t_prop=0;for(int i=0;i<N;i++)vrbps[i].active=0;for(int i=0;i<N-2;i++)vrbes[i].active=0;for(int i=0;i<N-4;i++)vrbvs[i].active=0;for(int i=0;i<N-6;i++)vrbpr[i].active=0;for(int i=0;i<N-6;i++)vrbms[i].active=0;init=1;ps("[VRB] Verbena initialized\n");return 0;}
/* 1=border 2=container 3=hanging_basket 4=herb_garden 5=wildflower */
int vrb_planning(int lc,int fc,int fg,int cm,int bd,int pr,int hc){return add(vrbps,&st.n_plan,&st.t_flower,N,lc,fc,fg,cm,bd,pr,hc);}
int vrb_execution(int lc,int fc,int fg,int cm,int bd,int pr,int hc){return add(vrbes,&st.n_exec,&st.t_frag,N-2,lc,fc,fg,cm,bd,pr,hc);}
int vrb_evaluation(int lc,int fc,int fg,int cm,int bd,int pr,int hc){return add(vrbvs,&st.n_eval,&st.t_compound,N-4,lc,fc,fg,cm,bd,pr,hc);}
int vrb_propagation(int lc,int fc,int fg,int cm,int bd,int pr,int hc){return add(vrbpr,&st.n_prop,&st.t_bloom,N-6,lc,fc,fg,cm,bd,pr,hc);}
int vrb_market(int lc,int fc,int fg,int cm,int bd,int pr,int hc){return add(vrbms,&st.n_mkt,&st.t_prop,N-6,lc,fc,fg,cm,bd,pr,hc);}
void vrb_report(void){ps("[VRB] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" frag=");pi(st.t_frag);ps("\nEval: ");pi(st.n_eval);ps(" comp=");pi(st.t_compound);ps("\nProp: ");pi(st.n_prop);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" prop=");pi(st.t_prop);ps("\n");}
void vrb_state(void){ps("[VRB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Verbena Admin Demo ===\n\n");vrb_init();
ps("Verbena planning (garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vrb_planning(lc,8+(i*3),(i%7)+1,12+(i*4),30+(i*5),(i%4)+1,4+(i*2));}
ps("\nVerbena execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vrb_execution(lc,10+(i*2),(i%7)+1,15+(i*3),35+(i*4),(i%4)+1,5+(i*2));}
ps("\nVerbena evaluation (compound analysis)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vrb_evaluation(lc,12+(i*2),(i%6)+2,18+(i*3),40+(i*3),(i%3)+2,6+(i*2));}
ps("\nVerbena propagation (cuttings)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vrb_propagation(lc,6+(i*3),(i%5)+1,10+(i*4),25+(i*4),(i%4)+1,3+(i*2));}
ps("\nVerbena herbal tea market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vrb_market(lc,14+(i*2),(i%7)+2,20+(i*2),45+(i*3),(i%3)+2,7+(i*2));}
ps("\n");vrb_report();vrb_state();ps("\n=== Demo Complete ===\n");return 0;}
