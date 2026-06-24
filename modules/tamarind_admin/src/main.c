/* tamarind_admin: Tamarind (Tamarindus indica) tropical fruit tree (v1.0)
 * Tamarind planning, execution, evaluation, pulp processing, market
 * Features: pod yield tracking, pulp quality, canopy management, irrigation, shade tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,pod_kg,pulp_pct,ht_m,canopy_m,irr_mm,shade,active;} tam_t;
typedef struct{int n_plan,n_exec,n_eval,n_pulp,n_mkt,t_pod,t_pulp,t_ht,t_canopy,t_irr;} tam_state_t;
static tam_t tamps[N],tames[N-2],tamvs[N-4],tamps2[N-6],tamms[N-6]; static tam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tam_t*a,int*cnt,int*sum,int mx,int cv,int pk,int pp,int ht,int cn,int im,int sh){if(*cnt>=mx)return -1;tam_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->pod_kg=pk;x->pulp_pct=pp;x->ht_m=ht;x->canopy_m=cn;x->irr_mm=im;x->shade=sh;x->active=1;*sum+=pk;(*cnt)++;ps("[TAM] Tamarind ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" pod=");pi(pk);ps(" pp=");pi(pp);ps(" ht=");pi(ht);ps(" cn=");pi(cn);ps("\n");return *cnt-1;}
int tam_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_pulp=0;st.n_mkt=0;st.t_pod=0;st.t_pulp=0;st.t_ht=0;st.t_canopy=0;st.t_irr=0;for(int i=0;i<N;i++)tamps[i].active=0;for(int i=0;i<N-2;i++)tames[i].active=0;for(int i=0;i<N-4;i++)tamvs[i].active=0;for(int i=0;i<N-6;i++)tamps2[i].active=0;for(int i=0;i<N-6;i++)tamms[i].active=0;init=1;ps("[TAM] Tamarind (tropical fruit tree) initialized\n");return 0;}
/* 1=Sweet 2=Sour 3=Manila 4=Kenyan 5=Australian */
int tam_planning(int cv,int pk,int pp,int ht,int cn,int im,int sh){return add(tamps,&st.n_plan,&st.t_pod,N,cv,pk,pp,ht,cn,im,sh);}
int tam_execution(int cv,int pk,int pp,int ht,int cn,int im,int sh){return add(tames,&st.n_exec,&st.t_pulp,N-2,cv,pk,pp,ht,cn,im,sh);}
int tam_evaluation(int cv,int pk,int pp,int ht,int cn,int im,int sh){return add(tamvs,&st.n_eval,&st.t_ht,N-4,cv,pk,pp,ht,cn,im,sh);}
int tam_pulp(int cv,int pk,int pp,int ht,int cn,int im,int sh){return add(tamps2,&st.n_pulp,&st.t_canopy,N-6,cv,pk,pp,ht,cn,im,sh);}
int tam_market(int cv,int pk,int pp,int ht,int cn,int im,int sh){return add(tamms,&st.n_mkt,&st.t_irr,N-6,cv,pk,pp,ht,cn,im,sh);}
void tam_report(void){ps("[TAM] Plan: ");pi(st.n_plan);ps(" pod=");pi(st.t_pod);ps("\nExec: ");pi(st.n_exec);ps(" pulp=");pi(st.t_pulp);ps("\nEval: ");pi(st.n_eval);ps(" ht=");pi(st.t_ht);ps("\nPulp: ");pi(st.n_pulp);ps(" canopy=");pi(st.t_canopy);ps("\nMkt: ");pi(st.n_mkt);ps(" irr=");pi(st.t_irr);ps("\n");}
void tam_state(void){ps("[TAM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Pulp=");pi(st.n_pulp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tamarind (Tropical Fruit Tree) Admin Demo ===\n\n");tam_init();
ps("Tamarind planning (orchard layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;tam_planning(cv,45+(i*12),38+(i%8),8+(i*2),6+(i*2),120+(i*30),(i%3)+1);}
ps("\nTamarind execution (harvesting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;tam_execution(cv,50+(i*10),40+(i%7),9+(i*2),7+(i*2),130+(i*25),(i%3)+1);}
ps("\nTamarind evaluation (pulp quality)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;tam_evaluation(cv,55+(i*9),42+(i%6),10+(i*2),8+(i*1),140+(i*20),(i%2)+2);}
ps("\nTamarind pulp processing...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;tam_pulp(cv,40+(i*11),35+(i%7),7+(i*2),5+(i*2),110+(i*28),1);}
ps("\nTamarind market (export)...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;tam_market(cv,60+(i*8),45+(i%5),11+(i*1),9+(i*1),150+(i*18),(i%3)+1);}
ps("\n");tam_report();tam_state();ps("\n=== Demo Complete ===\n");return 0;}
