/* bluegum_admin: Bluegum (Eucalyptus globulus) tree management (v1.0)
 * Bluegum planning, planting, evaluation, harvesting, market
 * Features: tree height, trunk diameter, leaf length, bark color, oil content, growth rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trunk_dia,leaf_ln,bark_color,oil_ct,growth_rt,active;} bgm_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_ht,t_trunk,t_leaf,t_bark,t_oil;} bgm_state_t;
static bgm_t bgmps[N],bgmes[N-2],bgmvs[N-4],bgmhr[N-6],bgmms[N-6]; static bgm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bgm_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int ll,int bc,int oc,int gr){if(*cnt>=mx)return -1;bgm_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trunk_dia=td;x->leaf_ln=ll;x->bark_color=bc;x->oil_ct=oc;x->growth_rt=gr;x->active=1;*sum+=th;(*cnt)++;ps("[BGM] Bluegum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" ll=");pi(ll);ps(" bc=");pi(bc);ps(" oc=");pi(oc);ps("\n");return *cnt-1;}
int bgm_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_trunk=0;st.t_leaf=0;st.t_bark=0;st.t_oil=0;for(int i=0;i<N;i++)bgmps[i].active=0;for(int i=0;i<N-2;i++)bgmes[i].active=0;for(int i=0;i<N-4;i++)bgmvs[i].active=0;for(int i=0;i<N-6;i++)bgmhr[i].active=0;for(int i=0;i<N-6;i++)bgmms[i].active=0;init=1;ps("[BGM] Bluegum initialized\n");return 0;}
/* 1=plantation 2=windbreak 3=ornamental 4=timber 5=coastal */
int bgm_planning(int lc,int th,int td,int ll,int bc,int oc,int gr){return add(bgmps,&st.n_plan,&st.t_ht,N,lc,th,td,ll,bc,oc,gr);}
int bgm_execution(int lc,int th,int td,int ll,int bc,int oc,int gr){return add(bgmes,&st.n_exec,&st.t_trunk,N-2,lc,th,td,ll,bc,oc,gr);}
int bgm_evaluation(int lc,int th,int td,int ll,int bc,int oc,int gr){return add(bgmvs,&st.n_eval,&st.t_leaf,N-4,lc,th,td,ll,bc,oc,gr);}
int bgm_harvesting(int lc,int th,int td,int ll,int bc,int oc,int gr){return add(bgmhr,&st.n_harv,&st.t_bark,N-6,lc,th,td,ll,bc,oc,gr);}
int bgm_market(int lc,int th,int td,int ll,int bc,int oc,int gr){return add(bgmms,&st.n_mkt,&st.t_oil,N-6,lc,th,td,ll,bc,oc,gr);}
void bgm_report(void){ps("[BGM] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" trunk=");pi(st.t_trunk);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nHarv: ");pi(st.n_harv);ps(" bark=");pi(st.t_bark);ps("\nMkt: ");pi(st.n_mkt);ps(" oil=");pi(st.t_oil);ps("\n");}
void bgm_state(void){ps("[BGM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluegum (Eucalyptus globulus) Admin Demo ===\n\n");bgm_init();
ps("Bluegum planning (plantation layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bgm_planning(lc,300+(i*50),15+(i*5),12+(i*3),(i%4)+1,2+(i*1),20+(i*4));}
ps("\nBluegum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bgm_execution(lc,350+(i*40),18+(i*4),14+(i*2),(i%4)+1,3+(i*1),22+(i*3));}
ps("\nBluegum evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bgm_evaluation(lc,400+(i*35),20+(i*3),16+(i*2),(i%3)+2,4+(i*1),24+(i*3));}
ps("\nBluegum harvesting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bgm_harvesting(lc,280+(i*45),12+(i*5),10+(i*3),(i%4)+1,1+(i*1),18+(i*4));}
ps("\nBluegum market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bgm_market(lc,420+(i*30),22+(i*3),18+(i*2),(i%3)+3,5+(i*1),26+(i*3));}
ps("\n");bgm_report();bgm_state();ps("\n=== Demo Complete ===\n");return 0;}
