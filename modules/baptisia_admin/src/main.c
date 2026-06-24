/* baptisia_admin: Baptisia (False Indigo) perennial border management (v1.0)
 * Baptisia planning, planting, evaluation, division, market
 * Features: flower spike, leaflet count, stem height, flower color, taproot depth, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_sp,leaf_ct,stem_ht,flower_color,taproot_dp,bloom_wk,active;} bap_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_leaf,t_stem,t_color,t_root;} bap_state_t;
static bap_t bapps[N],bapes[N-2],bapvs[N-4],bapdv[N-6],bapms[N-6]; static bap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bap_t*a,int*cnt,int*sum,int mx,int lc,int fs,int lct,int sh,int fc,int trd,int bw){if(*cnt>=mx)return -1;bap_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_sp=fs;x->leaf_ct=lct;x->stem_ht=sh;x->flower_color=fc;x->taproot_dp=trd;x->bloom_wk=bw;x->active=1;*sum+=fs;(*cnt)++;ps("[BAP] Baptisia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" lct=");pi(lct);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" trd=");pi(trd);ps("\n");return *cnt-1;}
int bap_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_root=0;for(int i=0;i<N;i++)bapps[i].active=0;for(int i=0;i<N-2;i++)bapes[i].active=0;for(int i=0;i<N-4;i++)bapvs[i].active=0;for(int i=0;i<N-6;i++)bapdv[i].active=0;for(int i=0;i<N-6;i++)bapms[i].active=0;init=1;ps("[BAP] Baptisia initialized\n");return 0;}
/* 1=border 2=prairie 3=cottage_garden 4=meadow 5=native_garden */
int bap_planning(int lc,int fs,int lct,int sh,int fc,int trd,int bw){return add(bapps,&st.n_plan,&st.t_flower,N,lc,fs,lct,sh,fc,trd,bw);}
int bap_execution(int lc,int fs,int lct,int sh,int fc,int trd,int bw){return add(bapes,&st.n_exec,&st.t_leaf,N-2,lc,fs,lct,sh,fc,trd,bw);}
int bap_evaluation(int lc,int fs,int lct,int sh,int fc,int trd,int bw){return add(bapvs,&st.n_eval,&st.t_stem,N-4,lc,fs,lct,sh,fc,trd,bw);}
int bap_division(int lc,int fs,int lct,int sh,int fc,int trd,int bw){return add(bapdv,&st.n_div,&st.t_color,N-6,lc,fs,lct,sh,fc,trd,bw);}
int bap_market(int lc,int fs,int lct,int sh,int fc,int trd,int bw){return add(bapms,&st.n_mkt,&st.t_root,N-6,lc,fs,lct,sh,fc,trd,bw);}
void bap_report(void){ps("[BAP] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" root=");pi(st.t_root);ps("\n");}
void bap_state(void){ps("[BAP] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Baptisia (False Indigo) Admin Demo ===\n\n");bap_init();
ps("Baptisia planning (border layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bap_planning(lc,12+(i*4),8+(i%5),40+(i*8),(i%5)+1,30+(i*6),22+(i%6));}
ps("\nBaptisia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bap_execution(lc,14+(i*3),9+(i%4),45+(i*7),(i%5)+1,35+(i*5),24+(i%5));}
ps("\nBaptisia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bap_evaluation(lc,16+(i*3),10+(i%3),50+(i*6),(i%4)+2,40+(i*4),26+(i%4));}
ps("\nBaptisia division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bap_division(lc,10+(i*4),7+(i%5),35+(i*8),(i%5)+1,25+(i*6),20+(i%5));}
ps("\nBaptisia perennial flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bap_market(lc,18+(i*3),11+(i%3),(i%6)+2,55+(i*5),45+(i%4),28+(i%3));}
ps("\n");bap_report();bap_state();ps("\n=== Demo Complete ===\n");return 0;}
