/* tussilago_admin: Tussilago (Coltsfoot) medicinal herb and early spring wildflower (v1.0)
 * Tussilago planning, planting, evaluation, medicinal harvest, market
 * Features: flower timing, leaf emergence, medicinal compound monitoring, habitat, spread control
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_wk,leaf_area,compound_mg,habitat,spread_rt,harv_wk,active;} tus_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_flower,t_leaf,t_compound,t_habitat,t_spread;} tus_state_t;
static tus_t tusps[N],tuses[N-2],tusvs[N-4],tushs[N-6],tusms[N-6]; static tus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tus_t*a,int*cnt,int*sum,int mx,int lc,int fw,int la,int cm,int hb,int sp,int hw){if(*cnt>=mx)return -1;tus_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_wk=fw;x->leaf_area=la;x->compound_mg=cm;x->habitat=hb;x->spread_rt=sp;x->harv_wk=hw;x->active=1;*sum+=fw;(*cnt)++;ps("[TUS] Tussilago ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fw=");pi(fw);ps(" la=");pi(la);ps(" cm=");pi(cm);ps(" hb=");pi(hb);ps(" sp=");pi(sp);ps("\n");return *cnt-1;}
int tus_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_compound=0;st.t_habitat=0;st.t_spread=0;for(int i=0;i<N;i++)tusps[i].active=0;for(int i=0;i<N-2;i++)tuses[i].active=0;for(int i=0;i<N-4;i++)tusvs[i].active=0;for(int i=0;i<N-6;i++)tushs[i].active=0;for(int i=0;i<N-6;i++)tusms[i].active=0;init=1;ps("[TUS] Tussilago (coltsfoot) initialized\n");return 0;}
/* 1=woodland 2=riverside 3=hillside 4=meadow 5=disturbed_ground */
int tus_planning(int lc,int fw,int la,int cm,int hb,int sp,int hw){return add(tusps,&st.n_plan,&st.t_flower,N,lc,fw,la,cm,hb,sp,hw);}
int tus_execution(int lc,int fw,int la,int cm,int hb,int sp,int hw){return add(tuses,&st.n_exec,&st.t_leaf,N-2,lc,fw,la,cm,hb,sp,hw);}
int tus_evaluation(int lc,int fw,int la,int cm,int hb,int sp,int hw){return add(tusvs,&st.n_eval,&st.t_compound,N-4,lc,fw,la,cm,hb,sp,hw);}
int tus_harvest(int lc,int fw,int la,int cm,int hb,int sp,int hw){return add(tushs,&st.n_harv,&st.t_habitat,N-6,lc,fw,la,cm,hb,sp,hw);}
int tus_market(int lc,int fw,int la,int cm,int hb,int sp,int hw){return add(tusms,&st.n_mkt,&st.t_spread,N-6,lc,fw,la,cm,hb,sp,hw);}
void tus_report(void){ps("[TUS] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" comp=");pi(st.t_compound);ps("\nHarv: ");pi(st.n_harv);ps(" hab=");pi(st.t_habitat);ps("\nMkt: ");pi(st.n_mkt);ps(" spread=");pi(st.t_spread);ps("\n");}
void tus_state(void){ps("[TUS] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tussilago (Coltsfoot) Admin Demo ===\n\n");tus_init();
ps("Tussilago planning (site survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;tus_planning(lc,6+(i%4),20+(i*8),15+(i*5),(i%3)+1,3+(i%4),10+(i*3));}
ps("\nTussilago execution (transplanting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;tus_execution(lc,7+(i%3),25+(i*7),18+(i*4),(i%3)+1,4+(i%3),11+(i*2));}
ps("\nTussilago evaluation (compound analysis)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;tus_evaluation(lc,8+(i%3),30+(i*6),22+(i*3),(i%3)+2,5+(i%3),12+(i*2));}
ps("\nTussilago medicinal harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;tus_harvest(lc,5+(i%3),15+(i*5),25+(i*4),(i%3)+1,2+(i%3),8+(i*2));}
ps("\nTussilago herbal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;tus_market(lc,9+(i%3),35+(i*5),28+(i*3),(i%3)+2,6+(i%2),13+(i*2));}
ps("\n");tus_report();tus_state();ps("\n=== Demo Complete ===\n");return 0;}
