/* anthurium_admin: Anthurium (Flamingo Flower) tropical indoor plant management (v1.0)
 * Anthurium planning, planting, evaluation, bloom forcing, market
 * Features: spathe count, spadix length, leaf length, bloom color, humidity need, bloom cycle
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spathe_ct,spadix_len,leaf_len,bloom_color,humidity_nd,bloom_cy,active;} atm_t;
typedef struct{int n_plan,n_exec,n_eval,n_force,n_mkt,t_spathe,t_spadix,t_leaf,t_bloom,t_humid;} atm_state_t;
static atm_t atmps[N],atmes[N-2],atmvs[N-4],atmfr[N-6],atmms[N-6]; static atm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(atm_t*a,int*cnt,int*sum,int mx,int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){if(*cnt>=mx)return -1;atm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spathe_ct=sc;x->spadix_len=sl;x->leaf_len=ll;x->bloom_color=bc;x->humidity_nd=hn;x->bloom_cy=bc_cy;x->active=1;*sum+=sc;(*cnt)++;ps("[ATM] Anthurium ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sc=");pi(sc);ps(" sl=");pi(sl);ps(" ll=");pi(ll);ps(" bc=");pi(bc);ps(" hn=");pi(hn);ps("\n");return *cnt-1;}
int atm_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_force=0;st.n_mkt=0;st.t_spathe=0;st.t_spadix=0;st.t_leaf=0;st.t_bloom=0;st.t_humid=0;for(int i=0;i<N;i++)atmps[i].active=0;for(int i=0;i<N-2;i++)atmes[i].active=0;for(int i=0;i<N-4;i++)atmvs[i].active=0;for(int i=0;i<N-6;i++)atmfr[i].active=0;for(int i=0;i<N-6;i++)atmms[i].active=0;init=1;ps("[ATM] Anthurium initialized\n");return 0;}
/* 1=indoor 2=greenhouse 3=tropical 4=container 5=shade_garden */
int atm_planning(int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){return add(atmps,&st.n_plan,&st.t_spathe,N,lc,sc,sl,ll,bc,hn,bc_cy);}
int atm_execution(int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){return add(atmes,&st.n_exec,&st.t_spadix,N-2,lc,sc,sl,ll,bc,hn,bc_cy);}
int atm_evaluation(int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){return add(atmvs,&st.n_eval,&st.t_leaf,N-4,lc,sc,sl,ll,bc,hn,bc_cy);}
int atm_bloom_forcing(int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){return add(atmfr,&st.n_force,&st.t_bloom,N-6,lc,sc,sl,ll,bc,hn,bc_cy);}
int atm_market(int lc,int sc,int sl,int ll,int bc,int hn,int bc_cy){return add(atmms,&st.n_mkt,&st.t_humid,N-6,lc,sc,sl,ll,bc,hn,bc_cy);}
void atm_report(void){ps("[ATM] Plan: ");pi(st.n_plan);ps(" spathe=");pi(st.t_spathe);ps("\nExec: ");pi(st.n_exec);ps(" spadix=");pi(st.t_spadix);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nForce: ");pi(st.n_force);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" humid=");pi(st.t_humid);ps("\n");}
void atm_state(void){ps("[ATM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Force=");pi(st.n_force);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anthurium (Flamingo Flower) Admin Demo ===\n\n");atm_init();
ps("Anthurium planning (indoor layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;atm_planning(lc,4+(i%5),10+(i*3),20+(i*5),(i%7)+1,60+(i*4),30+(i*3));}
ps("\nAnthurium execution (potting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;atm_execution(lc,5+(i%4),12+(i*2),22+(i*4),(i%7)+1,65+(i*3),32+(i*2));}
ps("\nAnthurium evaluation (leaf check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;atm_evaluation(lc,6+(i%3),14+(i*2),25+(i*3),(i%6)+2,70+(i*3),35+(i*2));}
ps("\nAnthurium bloom forcing...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;atm_bloom_forcing(lc,3+(i%4),8+(i*2),18+(i*4),(i%5)+1,55+(i*4),28+(i*3));}
ps("\nAnthurium tropical market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;atm_market(lc,7+(i%3),16+(i*2),28+(i*3),(i%6)+2,75+(i*3),38+(i*2));}
ps("\n");atm_report();atm_state();ps("\n=== Demo Complete ===\n");return 0;}
