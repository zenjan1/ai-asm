/* bistort_admin: Bistort (Persicaria bistorta) damp meadow management (v1.0)
 * Bistort planning, planting, evaluation, division, market
 * Features: flower spike, leaf length, stem height, flower color, moisture need, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_sp,leaf_ln,stem_ht,flower_color,moist_nd,bloom_wk,active;} bi_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_leaf,t_stem,t_color,t_moist;} bi_state_t;
static bi_t bsps[N],bsss[N-2],bsvss[N-4],bsdv[N-6],bsms[N-6]; static bi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bi_t*a,int*cnt,int*sum,int mx,int lc,int fs,int ll,int sh,int fc,int mn,int bw){if(*cnt>=mx)return -1;bi_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_sp=fs;x->leaf_ln=ll;x->stem_ht=sh;x->flower_color=fc;x->moist_nd=mn;x->bloom_wk=bw;x->active=1;*sum+=fs;(*cnt)++;ps("[BST] Bistort ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" ll=");pi(ll);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" mn=");pi(mn);ps("\n");return *cnt-1;}
int bs_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_moist=0;for(int i=0;i<N;i++)bsps[i].active=0;for(int i=0;i<N-2;i++)bsss[i].active=0;for(int i=0;i<N-4;i++)bsvss[i].active=0;for(int i=0;i<N-6;i++)bsdv[i].active=0;for(int i=0;i<N-6;i++)bsms[i].active=0;init=1;ps("[BST] Bistort initialized\n");return 0;}
/* 1=damp_meadow 2=bog_garden 3=pond_edge 4=woodland 5=streamside */
int bs_planning(int lc,int fs,int ll,int sh,int fc,int mn,int bw){return add(bsps,&st.n_plan,&st.t_flower,N,lc,fs,ll,sh,fc,mn,bw);}
int bs_execution(int lc,int fs,int ll,int sh,int fc,int mn,int bw){return add(bsss,&st.n_exec,&st.t_leaf,N-2,lc,fs,ll,sh,fc,mn,bw);}
int bs_evaluation(int lc,int fs,int ll,int sh,int fc,int mn,int bw){return add(bsvss,&st.n_eval,&st.t_stem,N-4,lc,fs,ll,sh,fc,mn,bw);}
int bs_division(int lc,int fs,int ll,int sh,int fc,int mn,int bw){return add(bsdv,&st.n_div,&st.t_color,N-6,lc,fs,ll,sh,fc,mn,bw);}
int bs_market(int lc,int fs,int ll,int sh,int fc,int mn,int bw){return add(bsms,&st.n_mkt,&st.t_moist,N-6,lc,fs,ll,sh,fc,mn,bw);}
void bs_report(void){ps("[BST] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" moist=");pi(st.t_moist);ps("\n");}
void bs_state(void){ps("[BST] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bistort (Persicaria) Admin Demo ===\n\n");bs_init();
ps("Bistort planning (damp meadow layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bs_planning(lc,6+(i*3),12+(i*3),25+(i*5),(i%5)+1,55+(i*5),22+(i%6));}
ps("\nBistort execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bs_execution(lc,8+(i*2),14+(i*2),28+(i*4),(i%5)+1,58+(i*4),24+(i%5));}
ps("\nBistort evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bs_evaluation(lc,10+(i*2),16+(i*2),30+(i*3),(i%4)+2,60+(i%3),26+(i%4));}
ps("\nBistort division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bs_division(lc,4+(i*3),10+(i%3),22+(i*5),(i%5)+1,50+(i%5),20+(i%5));}
ps("\nBistort damp meadow market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bs_market(lc,12+(i*2),18+(i*2),(i%6)+2,65+(i%3),28+(i%3),55+(i%4));}
ps("\n");bs_report();bs_state();ps("\n=== Demo Complete ===\n");return 0;}
