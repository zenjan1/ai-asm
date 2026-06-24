/* valeriana_admin: Valeriana (Valerian) medicinal herb and sedative plant management (v1.0)
 * Valeriana planning, planting, evaluation, processing, market
 * Features: root age tracking, valerenic acid content, harvest timing, drying method, potency grade
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,root_age,val_acid,harv_mo,dry_meth,potency_gr,storage_mo,active;} vlr_t;
typedef struct{int n_plan,n_exec,n_eval,n_proc,n_mkt,t_root,t_acid,t_harv,t_dry,t_potency;} vlr_state_t;
static vlr_t vlrps[N],vlres[N-2],vlrvs[N-4],vlrprs[N-6],vlrms[N-6]; static vlr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vlr_t*a,int*cnt,int*sum,int mx,int lc,int ra,int va,int hm,int dm,int pg,int sm){if(*cnt>=mx)return -1;vlr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->root_age=ra;x->val_acid=va;x->harv_mo=hm;x->dry_meth=dm;x->potency_gr=pg;x->storage_mo=sm;x->active=1;*sum+=ra;(*cnt)++;ps("[VLR] Valeriana ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ra=");pi(ra);ps(" va=");pi(va);ps(" hm=");pi(hm);ps(" dm=");pi(dm);ps(" pg=");pi(pg);ps("\n");return *cnt-1;}
int vlr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_proc=0;st.n_mkt=0;st.t_root=0;st.t_acid=0;st.t_harv=0;st.t_dry=0;st.t_potency=0;for(int i=0;i<N;i++)vlrps[i].active=0;for(int i=0;i<N-2;i++)vlres[i].active=0;for(int i=0;i<N-4;i++)vlrvs[i].active=0;for(int i=0;i<N-6;i++)vlrprs[i].active=0;for(int i=0;i<N-6;i++)vlrms[i].active=0;init=1;ps("[VLR] Valeriana (valerian) initialized\n");return 0;}
/* 1=air_dry 2=shade_dry 3=low_heat 4=freeze_dry 5=sun_dry */
int vlr_planning(int lc,int ra,int va,int hm,int dm,int pg,int sm){return add(vlrps,&st.n_plan,&st.t_root,N,lc,ra,va,hm,dm,pg,sm);}
int vlr_execution(int lc,int ra,int va,int hm,int dm,int pg,int sm){return add(vlres,&st.n_exec,&st.t_acid,N-2,lc,ra,va,hm,dm,pg,sm);}
int vlr_evaluation(int lc,int ra,int va,int hm,int dm,int pg,int sm){return add(vlrvs,&st.n_eval,&st.t_harv,N-4,lc,ra,va,hm,dm,pg,sm);}
int vlr_processing(int lc,int ra,int va,int hm,int dm,int pg,int sm){return add(vlrprs,&st.n_proc,&st.t_dry,N-6,lc,ra,va,hm,dm,pg,sm);}
int vlr_market(int lc,int ra,int va,int hm,int dm,int pg,int sm){return add(vlrms,&st.n_mkt,&st.t_potency,N-6,lc,ra,va,hm,dm,pg,sm);}
void vlr_report(void){ps("[VLR] Plan: ");pi(st.n_plan);ps(" root=");pi(st.t_root);ps("\nExec: ");pi(st.n_exec);ps(" acid=");pi(st.t_acid);ps("\nEval: ");pi(st.n_eval);ps(" harv=");pi(st.t_harv);ps("\nProc: ");pi(st.n_proc);ps(" dry=");pi(st.t_dry);ps("\nMkt: ");pi(st.n_mkt);ps(" potency=");pi(st.t_potency);ps("\n");}
void vlr_state(void){ps("[VLR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Proc=");pi(st.n_proc);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Valeriana (Valerian) Admin Demo ===\n\n");vlr_init();
ps("Valeriana planning (root survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vlr_planning(lc,2+(i%4),30+(i*5),9+(i%3),(i%5)+1,(i%4)+1,6+(i%3));}
ps("\nValeriana execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vlr_execution(lc,3+(i%3),35+(i*4),10+(i%2),(i%5)+1,(i%4)+1,7+(i%2));}
ps("\nValeriana evaluation (acid analysis)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vlr_evaluation(lc,4+(i%3),40+(i*3),10+(i%2),(i%5)+2,(i%4)+2,8+(i%2));}
ps("\nValeriana processing (drying)...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vlr_processing(lc,2+(i%4),45+(i*3),11+(i%2),(i%5)+1,(i%3)+1,9+(i%2));}
ps("\nValeriana medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vlr_market(lc,5+(i%3),50+(i*3),10+(i%2),(i%5)+2,(i%4)+1,10+(i%2));}
ps("\n");vlr_report();vlr_state();ps("\n=== Demo Complete ===\n");return 0;}
