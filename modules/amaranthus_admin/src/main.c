/* amaranthus_admin: Amaranthus ornamental grain and colorful foliage plant management (v1.0)
 * Amaranthus planning, planting, evaluation, seed harvest, market
 * Features: flower spike length, seed yield, leaf color intensity, plant height, protein content
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spike_len,seed_yield,leaf_color,plant_ht,protein_ct,harvest_mo,active;} amt_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_spike,t_seed_y,t_leaf,t_plant,t_protein;} amt_state_t;
static amt_t amtps[N],amtes[N-2],amtvs[N-4],amtsd[N-6],amtms[N-6]; static amt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(amt_t*a,int*cnt,int*sum,int mx,int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){if(*cnt>=mx)return -1;amt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spike_len=sl;x->seed_yield=sy;x->leaf_color=lc_col;x->plant_ht=ph;x->protein_ct=pc;x->harvest_mo=hm;x->active=1;*sum+=sl;(*cnt)++;ps("[AMT] Amaranthus ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" sy=");pi(sy);ps(" lc_col=");pi(lc_col);ps(" ph=");pi(ph);ps(" pc=");pi(pc);ps("\n");return *cnt-1;}
int amt_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_spike=0;st.t_seed_y=0;st.t_leaf=0;st.t_plant=0;st.t_protein=0;for(int i=0;i<N;i++)amtps[i].active=0;for(int i=0;i<N-2;i++)amtes[i].active=0;for(int i=0;i<N-4;i++)amtvs[i].active=0;for(int i=0;i<N-6;i++)amtsd[i].active=0;for(int i=0;i<N-6;i++)amtms[i].active=0;init=1;ps("[AMT] Amaranthus initialized\n");return 0;}
/* 1=ornamental 2=grain 3=vegetable 4=flower_bed 5=container */
int amt_planning(int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){return add(amtps,&st.n_plan,&st.t_spike,N,lc,sl,sy,lc_col,ph,pc,hm);}
int amt_execution(int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){return add(amtes,&st.n_exec,&st.t_seed_y,N-2,lc,sl,sy,lc_col,ph,pc,hm);}
int amt_evaluation(int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){return add(amtvs,&st.n_eval,&st.t_leaf,N-4,lc,sl,sy,lc_col,ph,pc,hm);}
int amt_seed_harvest(int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){return add(amtsd,&st.n_seed,&st.t_plant,N-6,lc,sl,sy,lc_col,ph,pc,hm);}
int amt_market(int lc,int sl,int sy,int lc_col,int ph,int pc,int hm){return add(amtms,&st.n_mkt,&st.t_protein,N-6,lc,sl,sy,lc_col,ph,pc,hm);}
void amt_report(void){ps("[AMT] Plan: ");pi(st.n_plan);ps(" spike=");pi(st.t_spike);ps("\nExec: ");pi(st.n_exec);ps(" seed=");pi(st.t_seed_y);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nSeed: ");pi(st.n_seed);ps(" plant=");pi(st.t_plant);ps("\nMkt: ");pi(st.n_mkt);ps(" protein=");pi(st.t_protein);ps("\n");}
void amt_state(void){ps("[AMT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Amaranthus Admin Demo ===\n\n");amt_init();
ps("Amaranthus planning (colorful layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;amt_planning(lc,30+(i*8),20+(i*10),(i%7)+1,80+(i*15),(i*4)+10,9+(i%3));}
ps("\nAmaranthus execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;amt_execution(lc,35+(i*6),25+(i*8),(i%7)+1,90+(i*12),(i*4)+12,10+(i%2));}
ps("\nAmaranthus evaluation (color check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;amt_evaluation(lc,40+(i*5),30+(i*6),(i%6)+2,100+(i*10),(i*3)+15,10+(i%2));}
ps("\nAmaranthus seed harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;amt_seed_harvest(lc,25+(i*6),15+(i*8),(i%5)+1,70+(i*10),(i*4)+8,9+(i%3));}
ps("\nAmaranthus grain market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;amt_market(lc,45+(i*4),35+(i*5),(i%7)+2,110+(i*8),(i%3)+18,11+(i%2));}
ps("\n");amt_report();amt_state();ps("\n=== Demo Complete ===\n");return 0;}
