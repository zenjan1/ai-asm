/* ajuga_admin: Ajuga (Bugleweed) groundcover and creeping perennial management (v1.0)
 * Ajuga planning, planting, evaluation, trimming, market
 * Features: spread rate, leaf color variation, flower spike count, shade tolerance, root depth
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spread_rt,leaf_color,flower_spikes,shade_tol,root_dep,trim_freq,active;} ajg_t;
typedef struct{int n_plan,n_exec,n_eval,n_trim,n_mkt,t_spread,t_leaf,t_flower,t_shade,t_root;} ajg_state_t;
static ajg_t ajgps[N],ajges[N-2],ajgvs[N-4],ajgtm[N-6],ajgms[N-6]; static ajg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ajg_t*a,int*cnt,int*sum,int mx,int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){if(*cnt>=mx)return -1;ajg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spread_rt=sr;x->leaf_color=lc_col;x->flower_spikes=fs;x->shade_tol=st_tol;x->root_dep=rd;x->trim_freq=tf;x->active=1;*sum+=sr;(*cnt)++;ps("[AJG] Ajuga ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sr=");pi(sr);ps(" lc_col=");pi(lc_col);ps(" fs=");pi(fs);ps(" st=");pi(st_tol);ps(" rd=");pi(rd);ps("\n");return *cnt-1;}
int ajg_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trim=0;st.n_mkt=0;st.t_spread=0;st.t_leaf=0;st.t_flower=0;st.t_shade=0;st.t_root=0;for(int i=0;i<N;i++)ajgps[i].active=0;for(int i=0;i<N-2;i++)ajges[i].active=0;for(int i=0;i<N-4;i++)ajgvs[i].active=0;for(int i=0;i<N-6;i++)ajgtm[i].active=0;for(int i=0;i<N-6;i++)ajgms[i].active=0;init=1;ps("[AJG] Ajuga (bugleweed) initialized\n");return 0;}
/* 1=groundcover 2=under_tree 3=shade_border 4=rock_garden 5=edging */
int ajg_planning(int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){return add(ajgps,&st.n_plan,&st.t_spread,N,lc,sr,lc_col,fs,st_tol,rd,tf);}
int ajg_execution(int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){return add(ajges,&st.n_exec,&st.t_leaf,N-2,lc,sr,lc_col,fs,st_tol,rd,tf);}
int ajg_evaluation(int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){return add(ajgvs,&st.n_eval,&st.t_flower,N-4,lc,sr,lc_col,fs,st_tol,rd,tf);}
int ajg_trimming(int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){return add(ajgtm,&st.n_trim,&st.t_shade,N-6,lc,sr,lc_col,fs,st_tol,rd,tf);}
int ajg_market(int lc,int sr,int lc_col,int fs,int st_tol,int rd,int tf){return add(ajgms,&st.n_mkt,&st.t_root,N-6,lc,sr,lc_col,fs,st_tol,rd,tf);}
void ajg_report(void){ps("[AJG] Plan: ");pi(st.n_plan);ps(" spread=");pi(st.t_spread);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nTrim: ");pi(st.n_trim);ps(" shade=");pi(st.t_shade);ps("\nMkt: ");pi(st.n_mkt);ps(" root=");pi(st.t_root);ps("\n");}
void ajg_state(void){ps("[AJG] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trim=");pi(st.n_trim);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ajuga (Bugleweed) Admin Demo ===\n\n");ajg_init();
ps("Ajuga planning (groundcover layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ajg_planning(lc,10+(i*5),(i%6)+1,5+(i*2),(i%4)+1,8+(i*3),(i%3)+1);}
ps("\nAjuga execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ajg_execution(lc,12+(i*4),(i%6)+1,6+(i*2),(i%4)+1,10+(i*2),(i%3)+1);}
ps("\nAjuga evaluation (flower check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ajg_evaluation(lc,15+(i*3),(i%5)+2,7+(i*2),(i%3)+2,12+(i*2),(i%2)+2);}
ps("\nAjuga trimming management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ajg_trimming(lc,8+(i*4),(i%4)+1,4+(i*2),(i%4)+1,6+(i*3),(i%3)+1);}
ps("\nAjuga groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ajg_market(lc,18+(i*3),(i%6)+2,8+(i*2),(i%3)+2,14+(i*2),(i%3)+2);}
ps("\n");ajg_report();ajg_state();ps("\n=== Demo Complete ===\n");return 0;}
