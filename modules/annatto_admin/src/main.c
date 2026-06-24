/* annatto_admin: Annatto (Achiote) dye tree and spice management (v1.0)
 * Annatto planning, planting, evaluation, dye extraction, market
 * Features: seed yield, dye content, tree height, fruit weight, pigment strength, harvest month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,seed_yield,dye_ct,tree_ht,fruit_wt,pigment_st,harv_mo,active;} ant_t;
typedef struct{int n_plan,n_exec,n_eval,n_dye,n_mkt,t_seed,t_dye_c,t_tree,t_fruit,t_pigment;} ant_state_t;
static ant_t antps[N],antes[N-2],antvs[N-4],antdv[N-6],antms[N-6]; static ant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ant_t*a,int*cnt,int*sum,int mx,int lc,int sy,int dc,int th,int fw,int ps_val,int hm){if(*cnt>=mx)return -1;ant_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->seed_yield=sy;x->dye_ct=dc;x->tree_ht=th;x->fruit_wt=fw;x->pigment_st=ps_val;x->harv_mo=hm;x->active=1;*sum+=sy;(*cnt)++;ps("[ANT] Annatto ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sy=");pi(sy);ps(" dc=");pi(dc);ps(" th=");pi(th);ps(" fw=");pi(fw);ps(" ps=");pi(ps_val);ps("\n");return *cnt-1;}
int ant_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dye=0;st.n_mkt=0;st.t_seed=0;st.t_dye_c=0;st.t_tree=0;st.t_fruit=0;st.t_pigment=0;for(int i=0;i<N;i++)antps[i].active=0;for(int i=0;i<N-2;i++)antes[i].active=0;for(int i=0;i<N-4;i++)antvs[i].active=0;for(int i=0;i<N-6;i++)antdv[i].active=0;for(int i=0;i<N-6;i++)antms[i].active=0;init=1;ps("[ANT] Annatto initialized\n");return 0;}
/* 1=plantation 2=dye_farm 3=spice_garden 4=container 5=tropical */
int ant_planning(int lc,int sy,int dc,int th,int fw,int ps_val,int hm){return add(antps,&st.n_plan,&st.t_seed,N,lc,sy,dc,th,fw,ps_val,hm);}
int ant_execution(int lc,int sy,int dc,int th,int fw,int ps_val,int hm){return add(antes,&st.n_exec,&st.t_dye_c,N-2,lc,sy,dc,th,fw,ps_val,hm);}
int ant_evaluation(int lc,int sy,int dc,int th,int fw,int ps_val,int hm){return add(antvs,&st.n_eval,&st.t_tree,N-4,lc,sy,dc,th,fw,ps_val,hm);}
int ant_dye_extraction(int lc,int sy,int dc,int th,int fw,int ps_val,int hm){return add(antdv,&st.n_dye,&st.t_fruit,N-6,lc,sy,dc,th,fw,ps_val,hm);}
int ant_market(int lc,int sy,int dc,int th,int fw,int ps_val,int hm){return add(antms,&st.n_mkt,&st.t_pigment,N-6,lc,sy,dc,th,fw,ps_val,hm);}
void ant_report(void){ps("[ANT] Plan: ");pi(st.n_plan);ps(" seed=");pi(st.t_seed);ps("\nExec: ");pi(st.n_exec);ps(" dye=");pi(st.t_dye_c);ps("\nEval: ");pi(st.n_eval);ps(" tree=");pi(st.t_tree);ps("\nDye: ");pi(st.n_dye);ps(" fruit=");pi(st.t_fruit);ps("\nMkt: ");pi(st.n_mkt);ps(" pigment=");pi(st.t_pigment);ps("\n");}
void ant_state(void){ps("[ANT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dye=");pi(st.n_dye);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Annatto (Achiote) Admin Demo ===\n\n");ant_init();
ps("Annatto planning (dye tree layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ant_planning(lc,20+(i*8),30+(i*6),200+(i*30),15+(i*5),40+(i*8),8+(i%4));}
ps("\nAnnatto execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ant_execution(lc,25+(i*6),35+(i*5),230+(i*25),18+(i*4),45+(i*6),9+(i%3));}
ps("\nAnnatto evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ant_evaluation(lc,30+(i*5),40+(i*4),260+(i*20),20+(i*3),50+(i*5),10+(i%2));}
ps("\nAnnatto dye extraction...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ant_dye_extraction(lc,15+(i*6),25+(i*5),180+(i*25),12+(i*4),35+(i*7),7+(i%4));}
ps("\nAnnatto dye market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ant_market(lc,35+(i*4),45+(i*3),290+(i*15),22+(i*3),55+(i*4),11+(i%2));}
ps("\n");ant_report();ant_state();ps("\n=== Demo Complete ===\n");return 0;}
