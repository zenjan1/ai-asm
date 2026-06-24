/* thyme_admin: Thyme (Thymus vulgaris) culinary and medicinal herb (v1.0)
 * Thyme planning, execution, evaluation, oil extraction, herb market
 * Features: variety tracking, essential oil content, harvest cycles, drought tolerance, ground cover
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,oil_ml,harv_kg,cover_pct,drought,age_mo,thymol_pct,active;} thm_t;
typedef struct{int n_plan,n_exec,n_eval,n_oil,n_mkt,t_oil,t_harv,t_cover,t_drought,t_age;} thm_state_t;
static thm_t thmps[N],thmes[N-2],thmvs[N-4],thmos[N-6],thmms[N-6]; static thm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thm_t*a,int*cnt,int*sum,int mx,int cv,int ol,int hv,int cv2,int dr,int ag,int tp){if(*cnt>=mx)return -1;thm_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->oil_ml=ol;x->harv_kg=hv;x->cover_pct=cv2;x->drought=dr;x->age_mo=ag;x->thymol_pct=tp;x->active=1;*sum+=ol;(*cnt)++;ps("[THM] Thyme ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" oil=");pi(ol);ps(" hv=");pi(hv);ps(" cv%=");pi(cv2);ps(" dr=");pi(dr);ps("\n");return *cnt-1;}
int thm_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_oil=0;st.n_mkt=0;st.t_oil=0;st.t_harv=0;st.t_cover=0;st.t_drought=0;st.t_age=0;for(int i=0;i<N;i++)thmps[i].active=0;for(int i=0;i<N-2;i++)thmes[i].active=0;for(int i=0;i<N-4;i++)thmvs[i].active=0;for(int i=0;i<N-6;i++)thmos[i].active=0;for(int i=0;i<N-6;i++)thmms[i].active=0;init=1;ps("[THM] Thyme (culinary herb) initialized\n");return 0;}
/* 1=vulgaris(common) 2=citriodorus(lemon) 3=serpyllum(creeping) 4=herba-barona(caraway) 5=nitidus */
int thm_planning(int cv,int ol,int hv,int cp,int dr,int ag,int tp){return add(thmps,&st.n_plan,&st.t_oil,N,cv,ol,hv,cp,dr,ag,tp);}
int thm_execution(int cv,int ol,int hv,int cp,int dr,int ag,int tp){return add(thmes,&st.n_exec,&st.t_harv,N-2,cv,ol,hv,cp,dr,ag,tp);}
int thm_evaluation(int cv,int ol,int hv,int cp,int dr,int ag,int tp){return add(thmvs,&st.n_eval,&st.t_cover,N-4,cv,ol,hv,cp,dr,ag,tp);}
int thm_oil(int cv,int ol,int hv,int cp,int dr,int ag,int tp){return add(thmos,&st.n_oil,&st.t_drought,N-6,cv,ol,hv,cp,dr,ag,tp);}
int thm_market(int cv,int ol,int hv,int cp,int dr,int ag,int tp){return add(thmms,&st.n_mkt,&st.t_age,N-6,cv,ol,hv,cp,dr,ag,tp);}
void thm_report(void){ps("[THM] Plan: ");pi(st.n_plan);ps(" oil=");pi(st.t_oil);ps("\nExec: ");pi(st.n_exec);ps(" harv=");pi(st.t_harv);ps("\nEval: ");pi(st.n_eval);ps(" cover=");pi(st.t_cover);ps("\nOil: ");pi(st.n_oil);ps(" dr=");pi(st.t_drought);ps("\nMkt: ");pi(st.n_mkt);ps(" age=");pi(st.t_age);ps("\n");}
void thm_state(void){ps("[THM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Oil=");pi(st.n_oil);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Thyme (Culinary Herb) Admin Demo ===\n\n");thm_init();
ps("Thyme planning (bed layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;thm_planning(cv,8+(i*3),2+(i%3),60+(i*5),(i%4)+1,6+(i*3),35+(i*5));}
ps("\nThyme execution (harvest)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;thm_execution(cv,10+(i*2),3+(i%2),65+(i*4),(i%3)+2,8+(i*2),38+(i*4));}
ps("\nThyme evaluation (quality check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;thm_evaluation(cv,12+(i*2),4+(i%2),70+(i*3),(i%3)+2,10+(i%2),40+(i*3));}
ps("\nThyme essential oil extraction...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;thm_oil(cv,6+(i*3),1+(i%3),50+(i*5),(i%4)+1,4+(i*3),30+(i*5));}
ps("\nThyme herb market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;thm_market(cv,14+(i*2),5+(i%1),75+(i*3),(i%3)+2,12+(i%2),42+(i*3));}
ps("\n");thm_report();thm_state();ps("\n=== Demo Complete ===\n");return 0;}
