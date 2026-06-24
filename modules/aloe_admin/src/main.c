/* aloe_admin: Aloe succulent plant and medicinal gel source management (v1.0)
 * Aloe planning, planting, evaluation, harvest, market
 * Features: leaf length, gel content, offset count, drought tolerance, sun exposure
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_len,gel_content,offset_ct,drought_tol,sun_exp,harvest_mo,active;} alo_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_leaf,t_gel,t_offset,t_drought,t_sun;} alo_state_t;
static alo_t alops[N],aloes[N-2],alovs[N-4],alohv[N-6],aloms[N-6]; static alo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alo_t*a,int*cnt,int*sum,int mx,int lc,int ll,int gc,int oc,int dt,int se,int hm){if(*cnt>=mx)return -1;alo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_len=ll;x->gel_content=gc;x->offset_ct=oc;x->drought_tol=dt;x->sun_exp=se;x->harvest_mo=hm;x->active=1;*sum+=ll;(*cnt)++;ps("[ALO] Aloe ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ll=");pi(ll);ps(" gc=");pi(gc);ps(" oc=");pi(oc);ps(" dt=");pi(dt);ps(" se=");pi(se);ps("\n");return *cnt-1;}
int alo_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_leaf=0;st.t_gel=0;st.t_offset=0;st.t_drought=0;st.t_sun=0;for(int i=0;i<N;i++)alops[i].active=0;for(int i=0;i<N-2;i++)aloes[i].active=0;for(int i=0;i<N-4;i++)alovs[i].active=0;for(int i=0;i<N-6;i++)alohv[i].active=0;for(int i=0;i<N-6;i++)aloms[i].active=0;init=1;ps("[ALO] Aloe initialized\n");return 0;}
/* 1=desert 2=container 3=rock_garden 4=xeriscape 5=medicinal */
int alo_planning(int lc,int ll,int gc,int oc,int dt,int se,int hm){return add(alops,&st.n_plan,&st.t_leaf,N,lc,ll,gc,oc,dt,se,hm);}
int alo_execution(int lc,int ll,int gc,int oc,int dt,int se,int hm){return add(aloes,&st.n_exec,&st.t_gel,N-2,lc,ll,gc,oc,dt,se,hm);}
int alo_evaluation(int lc,int ll,int gc,int oc,int dt,int se,int hm){return add(alovs,&st.n_eval,&st.t_offset,N-4,lc,ll,gc,oc,dt,se,hm);}
int alo_harvest(int lc,int ll,int gc,int oc,int dt,int se,int hm){return add(alohv,&st.n_harv,&st.t_drought,N-6,lc,ll,gc,oc,dt,se,hm);}
int alo_market(int lc,int ll,int gc,int oc,int dt,int se,int hm){return add(aloms,&st.n_mkt,&st.t_sun,N-6,lc,ll,gc,oc,dt,se,hm);}
void alo_report(void){ps("[ALO] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" gel=");pi(st.t_gel);ps("\nEval: ");pi(st.n_eval);ps(" offset=");pi(st.t_offset);ps("\nHarv: ");pi(st.n_harv);ps(" drought=");pi(st.t_drought);ps("\nMkt: ");pi(st.n_mkt);ps(" sun=");pi(st.t_sun);ps("\n");}
void alo_state(void){ps("[ALO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Aloe Admin Demo ===\n\n");alo_init();
ps("Aloe planning (succulent layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;alo_planning(lc,20+(i*5),40+(i*8),3+(i%4),(i%5)+1,8+(i*2),(i%3)+1);}
ps("\nAloe execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;alo_execution(lc,25+(i*4),45+(i*6),4+(i%3),(i%5)+1,9+(i*2),(i%3)+1);}
ps("\nAloe evaluation (gel check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;alo_evaluation(lc,30+(i*3),50+(i*5),5+(i%3),(i%4)+2,10+(i*2),(i%2)+2);}
ps("\nAloe leaf harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;alo_harvest(lc,18+(i*4),35+(i*6),2+(i%3),(i%5)+1,7+(i*3),(i%3)+1);}
ps("\nAloe medicinal gel market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;alo_market(lc,35+(i*3),55+(i*4),6+(i*2),(i%4)+2,11+(i*2),(i%3)+2);}
ps("\n");alo_report();alo_state();ps("\n=== Demo Complete ===\n");return 0;}
