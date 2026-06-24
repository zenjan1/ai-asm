/* amaryllis_admin: Amaryllis (Hippeastrum) bulb flower and indoor flowering plant management (v1.0)
 * Amaryllis planning, planting, evaluation, forcing, market
 * Features: flower count per stem, petal width, bloom color, bulb size, stem height, forcing time
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_ct,petal_wd,bloom_color,bulb_sz,stem_ht,forcing_wk,active;} ama_t;
typedef struct{int n_plan,n_exec,n_eval,n_force,n_mkt,t_flower,t_petal,t_color,t_bulb,t_stem;} ama_state_t;
static ama_t amaps[N],amaes[N-2],amavs[N-4],amafr[N-6],amams[N-6]; static ama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ama_t*a,int*cnt,int*sum,int mx,int lc,int fc,int pw,int bc,int bs,int sh,int fw){if(*cnt>=mx)return -1;ama_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_ct=fc;x->petal_wd=pw;x->bloom_color=bc;x->bulb_sz=bs;x->stem_ht=sh;x->forcing_wk=fw;x->active=1;*sum+=fc;(*cnt)++;ps("[AMA] Amaryllis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fc=");pi(fc);ps(" pw=");pi(pw);ps(" bc=");pi(bc);ps(" bs=");pi(bs);ps(" sh=");pi(sh);ps("\n");return *cnt-1;}
int ama_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_force=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_color=0;st.t_bulb=0;st.t_stem=0;for(int i=0;i<N;i++)amaps[i].active=0;for(int i=0;i<N-2;i++)amaes[i].active=0;for(int i=0;i<N-4;i++)amavs[i].active=0;for(int i=0;i<N-6;i++)amafr[i].active=0;for(int i=0;i<N-6;i++)amams[i].active=0;init=1;ps("[AMA] Amaryllis (hippeastrum) initialized\n");return 0;}
/* 1=indoor 2=container 3=forcing 4=garden 5=cutting */
int ama_planning(int lc,int fc,int pw,int bc,int bs,int sh,int fw){return add(amaps,&st.n_plan,&st.t_flower,N,lc,fc,pw,bc,bs,sh,fw);}
int ama_execution(int lc,int fc,int pw,int bc,int bs,int sh,int fw){return add(amaes,&st.n_exec,&st.t_petal,N-2,lc,fc,pw,bc,bs,sh,fw);}
int ama_evaluation(int lc,int fc,int pw,int bc,int bs,int sh,int fw){return add(amavs,&st.n_eval,&st.t_color,N-4,lc,fc,pw,bc,bs,sh,fw);}
int ama_forcing(int lc,int fc,int pw,int bc,int bs,int sh,int fw){return add(amafr,&st.n_force,&st.t_bulb,N-6,lc,fc,pw,bc,bs,sh,fw);}
int ama_market(int lc,int fc,int pw,int bc,int bs,int sh,int fw){return add(amams,&st.n_mkt,&st.t_stem,N-6,lc,fc,pw,bc,bs,sh,fw);}
void ama_report(void){ps("[AMA] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nForce: ");pi(st.n_force);ps(" bulb=");pi(st.t_bulb);ps("\nMkt: ");pi(st.n_mkt);ps(" stem=");pi(st.t_stem);ps("\n");}
void ama_state(void){ps("[AMA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Force=");pi(st.n_force);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Amaryllis (Hippeastrum) Admin Demo ===\n\n");ama_init();
ps("Amaryllis planning (bulb layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ama_planning(lc,4+(i%3),8+(i*2),(i%6)+1,12+(i*3),40+(i*8),6+(i%4));}
ps("\nAmaryllis execution (potting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ama_execution(lc,5+(i%2),10+(i*2),(i%6)+1,14+(i*2),45+(i*6),7+(i%3));}
ps("\nAmaryllis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ama_evaluation(lc,6+(i%2),12+(i*2),(i%5)+2,16+(i*2),50+(i*5),8+(i%2));}
ps("\nAmaryllis forcing schedule...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ama_forcing(lc,3+(i%3),6+(i*2),(i%4)+1,10+(i*3),35+(i*6),5+(i%4));}
ps("\nAmaryllis bulb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ama_market(lc,7+(i%2),14+(i*2),(i%6)+2,18+(i*2),55+(i*4),9+(i%2));}
ps("\n");ama_report();ama_state();ps("\n=== Demo Complete ===\n");return 0;}
