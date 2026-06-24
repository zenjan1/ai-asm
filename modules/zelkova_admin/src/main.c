/* zelkova_admin: Zelkova ornamental shade tree and bonsai subject management (v1.0)
 * Zelkova planning, planting, evaluation, pruning, market
 * Features: trunk diameter, canopy spread, leaf density, bark texture, growth rate, pruning freq
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,trunk_dia,canopy_sp,leaf_dens,bark_tx,growth_rt,prune_fr,active;} zelk_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_trunk,t_canopy,t_leaf,t_bark,t_growth;} zelk_state_t;
static zelk_t zelkps[N],zelkes[N-2],zelkvs[N-4],zelkpm[N-6],zelkms[N-6]; static zelk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zelk_t*a,int*cnt,int*sum,int mx,int lc,int td,int cs,int ld,int bt,int gr,int pf){if(*cnt>=mx)return -1;zelk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->trunk_dia=td;x->canopy_sp=cs;x->leaf_dens=ld;x->bark_tx=bt;x->growth_rt=gr;x->prune_fr=pf;x->active=1;*sum+=td;(*cnt)++;ps("[ZELK] Zelkova ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" td=");pi(td);ps(" cs=");pi(cs);ps(" ld=");pi(ld);ps(" bt=");pi(bt);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int zelk_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_trunk=0;st.t_canopy=0;st.t_leaf=0;st.t_bark=0;st.t_growth=0;for(int i=0;i<N;i++)zelkps[i].active=0;for(int i=0;i<N-2;i++)zelkes[i].active=0;for(int i=0;i<N-4;i++)zelkvs[i].active=0;for(int i=0;i<N-6;i++)zelkpm[i].active=0;for(int i=0;i<N-6;i++)zelkms[i].active=0;init=1;ps("[ZELK] Zelkova initialized\n");return 0;}
/* 1=street 2=park 3=bonsai 4=specimen 5=avenue */
int zelk_planning(int lc,int td,int cs,int ld,int bt,int gr,int pf){return add(zelkps,&st.n_plan,&st.t_trunk,N,lc,td,cs,ld,bt,gr,pf);}
int zelk_execution(int lc,int td,int cs,int ld,int bt,int gr,int pf){return add(zelkes,&st.n_exec,&st.t_canopy,N-2,lc,td,cs,ld,bt,gr,pf);}
int zelk_evaluation(int lc,int td,int cs,int ld,int bt,int gr,int pf){return add(zelkvs,&st.n_eval,&st.t_leaf,N-4,lc,td,cs,ld,bt,gr,pf);}
int zelk_pruning(int lc,int td,int cs,int ld,int bt,int gr,int pf){return add(zelkpm,&st.n_prune,&st.t_bark,N-6,lc,td,cs,ld,bt,gr,pf);}
int zelk_market(int lc,int td,int cs,int ld,int bt,int gr,int pf){return add(zelkms,&st.n_mkt,&st.t_growth,N-6,lc,td,cs,ld,bt,gr,pf);}
void zelk_report(void){ps("[ZELK] Plan: ");pi(st.n_plan);ps(" trunk=");pi(st.t_trunk);ps("\nExec: ");pi(st.n_exec);ps(" canopy=");pi(st.t_canopy);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nPrune: ");pi(st.n_prune);ps(" bark=");pi(st.t_bark);ps("\nMkt: ");pi(st.n_mkt);ps(" growth=");pi(st.t_growth);ps("\n");}
void zelk_state(void){ps("[ZELK] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zelkova Admin Demo ===\n\n");zelk_init();
ps("Zelkova planning (shade tree layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;zelk_planning(lc,10+(i*5),80+(i*20),(i%6)+1,(i%4)+1,5+(i%4),(i%3)+1);}
ps("\nZelkova execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;zelk_execution(lc,12+(i*4),90+(i*18),(i%6)+1,(i%4)+1,6+(i%3),(i%3)+1);}
ps("\nZelkova evaluation (canopy check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;zelk_evaluation(lc,15+(i*3),100+(i*15),(i%5)+2,(i%3)+2,7+(i%3),(i%2)+2);}
ps("\nZelkova pruning management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;zelk_pruning(lc,8+(i*4),70+(i*15),(i%4)+1,(i%4)+1,4+(i%3),(i%3)+1);}
ps("\nZelkova ornamental tree market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;zelk_market(lc,18+(i*3),110+(i*12),(i%6)+2,(i%3)+2,8+(i%2),(i%2)+2);}
ps("\n");zelk_report();zelk_state();ps("\n=== Demo Complete ===\n");return 0;}
