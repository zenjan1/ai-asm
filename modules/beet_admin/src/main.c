/* beet_admin: Beet (Beta vulgaris) root vegetable crop management (v1.0)
 * Beet planning, planting, evaluation, harvest, market
 * Features: root diameter, leaf length, root color, sugar brix, growth rate, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,root_dia,leaf_ln,root_color,sugar_bx,growth_rt,harvest_wk,active;} beet_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_root,t_leaf,t_color,t_sugar,t_growth;} beet_state_t;
static beet_t beetps[N],beetes[N-2],beetvs[N-4],beethv[N-6],beetms[N-6]; static beet_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beet_t*a,int*cnt,int*sum,int mx,int lc,int rd,int ll,int rc,int sb,int gr,int hw){if(*cnt>=mx)return -1;beet_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->root_dia=rd;x->leaf_ln=ll;x->root_color=rc;x->sugar_bx=sb;x->growth_rt=gr;x->harvest_wk=hw;x->active=1;*sum+=rd;(*cnt)++;ps("[BEET] Beet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rd=");pi(rd);ps(" ll=");pi(ll);ps(" rc=");pi(rc);ps(" sb=");pi(sb);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int beet_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_root=0;st.t_leaf=0;st.t_color=0;st.t_sugar=0;st.t_growth=0;for(int i=0;i<N;i++)beetps[i].active=0;for(int i=0;i<N-2;i++)beetes[i].active=0;for(int i=0;i<N-4;i++)beetvs[i].active=0;for(int i=0;i<N-6;i++)beethv[i].active=0;for(int i=0;i<N-6;i++)beetms[i].active=0;init=1;ps("[BEET] Beet initialized\n");return 0;}
/* 1=garden_bed 2=raised_bed 3=field 4=container 5=market_garden */
int beet_planning(int lc,int rd,int ll,int rc,int sb,int gr,int hw){return add(beetps,&st.n_plan,&st.t_root,N,lc,rd,ll,rc,sb,gr,hw);}
int beet_execution(int lc,int rd,int ll,int rc,int sb,int gr,int hw){return add(beetes,&st.n_exec,&st.t_leaf,N-2,lc,rd,ll,rc,sb,gr,hw);}
int beet_evaluation(int lc,int rd,int ll,int rc,int sb,int gr,int hw){return add(beetvs,&st.n_eval,&st.t_color,N-4,lc,rd,ll,rc,sb,gr,hw);}
int beet_harvest(int lc,int rd,int ll,int rc,int sb,int gr,int hw){return add(beethv,&st.n_harv,&st.t_sugar,N-6,lc,rd,ll,rc,sb,gr,hw);}
int beet_market(int lc,int rd,int ll,int rc,int sb,int gr,int hw){return add(beetms,&st.n_mkt,&st.t_growth,N-6,lc,rd,ll,rc,sb,gr,hw);}
void beet_report(void){ps("[BEET] Plan: ");pi(st.n_plan);ps(" root=");pi(st.t_root);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nHarv: ");pi(st.n_harv);ps(" sugar=");pi(st.t_sugar);ps("\nMkt: ");pi(st.n_mkt);ps(" growth=");pi(st.t_growth);ps("\n");}
void beet_state(void){ps("[BEET] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Beet (Beta vulgaris) Admin Demo ===\n\n");beet_init();
ps("Beet planning (garden bed layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;beet_planning(lc,4+(i*2),12+(i*3),(i%5)+1,80+(i*5),6+(i*3),30+(i%6));}
ps("\nBeet execution (sowing)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;beet_execution(lc,5+(i*2),14+(i*2),(i%5)+1,85+(i*4),7+(i*2),32+(i%5));}
ps("\nBeet evaluation (root check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;beet_evaluation(lc,6+(i*2),16+(i*2),(i%4)+2,90+(i*3),8+(i%2),34+(i%4));}
ps("\nBeet harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;beet_harvest(lc,3+(i*2),10+(i*3),(i%5)+1,75+(i*5),5+(i*3),28+(i%5));}
ps("\nBeet root vegetable market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;beet_market(lc,7+(i*2),18+(i*2),(i%4)+2,95+(i%3),9+(i%2),36+(i%3));}
ps("\n");beet_report();beet_state();ps("\n=== Demo Complete ===\n");return 0;}
