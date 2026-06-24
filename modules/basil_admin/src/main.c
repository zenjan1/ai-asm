/* basil_admin: Basil (Sweet Basil) culinary herb management (v1.0)
 * Basil planning, planting, evaluation, harvest, market
 * Features: leaf length, leaf width, plant height, oil content, pinch frequency, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_ln,leaf_wd,plant_ht,oil_ct,pinch_fr,growth_rt,active;} bas_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_leaf,t_width,t_ht,t_oil,t_pinch;} bas_state_t;
static bas_t basps[N],bases[N-2],basvs[N-4],bashv[N-6],basms[N-6]; static bas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bas_t*a,int*cnt,int*sum,int mx,int lc,int ll,int lw,int ph,int oc,int pf,int gr){if(*cnt>=mx)return -1;bas_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_ln=ll;x->leaf_wd=lw;x->plant_ht=ph;x->oil_ct=oc;x->pinch_fr=pf;x->growth_rt=gr;x->active=1;*sum+=ll;(*cnt)++;ps("[BAS] Basil ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" lw=");pi(lw);ps(" ph=");pi(ph);ps(" oc=");pi(oc);ps(" pf=");pi(pf);ps("\n");return *cnt-1;}
int bas_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_leaf=0;st.t_width=0;st.t_ht=0;st.t_oil=0;st.t_pinch=0;for(int i=0;i<N;i++)basps[i].active=0;for(int i=0;i<N-2;i++)bases[i].active=0;for(int i=0;i<N-4;i++)basvs[i].active=0;for(int i=0;i<N-6;i++)bashv[i].active=0;for(int i=0;i<N-6;i++)basms[i].active=0;init=1;ps("[BAS] Basil initialized\n");return 0;}
/* 1=herb_garden 2=container 3=kitchen_garden 4=greenhouse 5=raised_bed */
int bas_planning(int lc,int ll,int lw,int ph,int oc,int pf,int gr){return add(basps,&st.n_plan,&st.t_leaf,N,lc,ll,lw,ph,oc,pf,gr);}
int bas_execution(int lc,int ll,int lw,int ph,int oc,int pf,int gr){return add(bases,&st.n_exec,&st.t_width,N-2,lc,ll,lw,ph,oc,pf,gr);}
int bas_evaluation(int lc,int ll,int lw,int ph,int oc,int pf,int gr){return add(basvs,&st.n_eval,&st.t_ht,N-4,lc,ll,lw,ph,oc,pf,gr);}
int bas_harvest(int lc,int ll,int lw,int ph,int oc,int pf,int gr){return add(bashv,&st.n_harv,&st.t_oil,N-6,lc,ll,lw,ph,oc,pf,gr);}
int bas_market(int lc,int ll,int lw,int ph,int oc,int pf,int gr){return add(basms,&st.n_mkt,&st.t_pinch,N-6,lc,ll,lw,ph,oc,pf,gr);}
void bas_report(void){ps("[BAS] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" width=");pi(st.t_width);ps("\nEval: ");pi(st.n_eval);ps(" ht=");pi(st.t_ht);ps("\nHarv: ");pi(st.n_harv);ps(" oil=");pi(st.t_oil);ps("\nMkt: ");pi(st.n_mkt);ps(" pinch=");pi(st.t_pinch);ps("\n");}
void bas_state(void){ps("[BAS] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Basil (Sweet Basil) Admin Demo ===\n\n");bas_init();
ps("Basil planning (herb garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bas_planning(lc,5+(i*2),3+(i%4),20+(i*5),40+(i*5),3+(i%4),5+(i*3));}
ps("\nBasil execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bas_execution(lc,6+(i*2),4+(i%3),22+(i*4),42+(i*4),4+(i%3),6+(i*2));}
ps("\nBasil evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bas_evaluation(lc,7+(i*2),4+(i%3),25+(i*3),45+(i*3),4+(i%3),6+(i*2));}
ps("\nBasil harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bas_harvest(lc,4+(i*2),2+(i%4),18+(i*5),38+(i*5),3+(i%4),4+(i*3));}
ps("\nBasil culinary herb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bas_market(lc,8+(i*2),5+(i%3),(i%6)+2,50+(i*3),5+(i%3),8+(i%2));}
ps("\n");bas_report();bas_state();ps("\n=== Demo Complete ===\n");return 0;}
