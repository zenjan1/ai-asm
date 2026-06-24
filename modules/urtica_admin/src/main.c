/* urtica_admin: Urtica (Stinging Nettle) medicinal herb and fiber plant management (v1.0)
 * Urtica planning, planting, evaluation, harvest, market
 * Features: sting potency tracking, leaf yield, medicinal compound, fiber quality, growth density
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sting_pot,leaf_yield,compound_mg,fiber_q,density,harvest_wk,active;} urt_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_sting,t_leaf,t_compound,t_fiber,t_density;} urt_state_t;
static urt_t urtps[N],turtes[N-2],urtvs[N-4],urths[N-6],urtms[N-6]; static urt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(urt_t*a,int*cnt,int*sum,int mx,int lc,int sp,int ly,int cm,int fq,int dn,int hw){if(*cnt>=mx)return -1;urt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sting_pot=sp;x->leaf_yield=ly;x->compound_mg=cm;x->fiber_q=fq;x->density=dn;x->harvest_wk=hw;x->active=1;*sum+=sp;(*cnt)++;ps("[URT] Urtica ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sp=");pi(sp);ps(" ly=");pi(ly);ps(" cm=");pi(cm);ps(" fq=");pi(fq);ps(" dn=");pi(dn);ps("\n");return *cnt-1;}
int urt_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_sting=0;st.t_leaf=0;st.t_compound=0;st.t_fiber=0;st.t_density=0;for(int i=0;i<N;i++)urtps[i].active=0;for(int i=0;i<N-2;i++)turtes[i].active=0;for(int i=0;i<N-4;i++)urtvs[i].active=0;for(int i=0;i<N-6;i++)urths[i].active=0;for(int i=0;i<N-6;i++)urtms[i].active=0;init=1;ps("[URT] Urtica (stinging nettle) initialized\n");return 0;}
/* 1=woodland_edge 2=riverside 3=hedge_row 4=wasteland 5=field_margin */
int urt_planning(int lc,int sp,int ly,int cm,int fq,int dn,int hw){return add(urtps,&st.n_plan,&st.t_sting,N,lc,sp,ly,cm,fq,dn,hw);}
int urt_execution(int lc,int sp,int ly,int cm,int fq,int dn,int hw){return add(turtes,&st.n_exec,&st.t_leaf,N-2,lc,sp,ly,cm,fq,dn,hw);}
int urt_evaluation(int lc,int sp,int ly,int cm,int fq,int dn,int hw){return add(urtvs,&st.n_eval,&st.t_compound,N-4,lc,sp,ly,cm,fq,dn,hw);}
int urt_harvest(int lc,int sp,int ly,int cm,int fq,int dn,int hw){return add(urths,&st.n_harv,&st.t_fiber,N-6,lc,sp,ly,cm,fq,dn,hw);}
int urt_market(int lc,int sp,int ly,int cm,int fq,int dn,int hw){return add(urtms,&st.n_mkt,&st.t_density,N-6,lc,sp,ly,cm,fq,dn,hw);}
void urt_report(void){ps("[URT] Plan: ");pi(st.n_plan);ps(" sting=");pi(st.t_sting);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" comp=");pi(st.t_compound);ps("\nHarv: ");pi(st.n_harv);ps(" fiber=");pi(st.t_fiber);ps("\nMkt: ");pi(st.n_mkt);ps(" density=");pi(st.t_density);ps("\n");}
void urt_state(void){ps("[URT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Urtica (Stinging Nettle) Admin Demo ===\n\n");urt_init();
ps("Urtica planning (site survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;urt_planning(lc,30+(i*4),20+(i*6),15+(i*5),(i%4)+1,10+(i*3),14+(i%8));}
ps("\nUrtica execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;urt_execution(lc,35+(i*3),25+(i*5),18+(i*4),(i%4)+1,12+(i*2),15+(i%7));}
ps("\nUrtica evaluation (compound analysis)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;urt_evaluation(lc,40+(i*3),30+(i*4),22+(i*3),(i%4)+2,14+(i*2),16+(i%6));}
ps("\nUrtica medicinal harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;urt_harvest(lc,25+(i*4),15+(i*5),20+(i*3),(i%3)+1,8+(i*3),12+(i%5));}
ps("\nUrtica fiber market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;urt_market(lc,45+(i*3),35+(i*3),25+(i*2),(i%4)+2,16+(i*2),18+(i%4));}
ps("\n");urt_report();urt_state();ps("\n=== Demo Complete ===\n");return 0;}
