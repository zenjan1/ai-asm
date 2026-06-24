/* betonica_admin: Betonica (Wood Betony) medicinal herb management (v1.0)
 * Betonica planning, planting, evaluation, harvest, market
 * Features: flower spike, leaf width, stem height, flower color, medicinal index, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_sp,leaf_wd,stem_ht,flower_color,med_idx,bloom_wk,active;} beto_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_flower,t_leaf,t_stem,t_color,t_med;} beto_state_t;
static beto_t betops[N],beto_es[N-2],betovs[N-4],beto_hv[N-6],betoms[N-6]; static beto_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beto_t*a,int*cnt,int*sum,int mx,int lc,int fs,int lw,int sh,int fc,int mi,int bw){if(*cnt>=mx)return -1;beto_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_sp=fs;x->leaf_wd=lw;x->stem_ht=sh;x->flower_color=fc;x->med_idx=mi;x->bloom_wk=bw;x->active=1;*sum+=fs;(*cnt)++;ps("[BETO] Betonica ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" lw=");pi(lw);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" mi=");pi(mi);ps("\n");return *cnt-1;}
int beto_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_stem=0;st.t_color=0;st.t_med=0;for(int i=0;i<N;i++)betops[i].active=0;for(int i=0;i<N-2;i++)beto_es[i].active=0;for(int i=0;i<N-4;i++)betovs[i].active=0;for(int i=0;i<N-6;i++)beto_hv[i].active=0;for(int i=0;i<N-6;i++)betoms[i].active=0;init=1;ps("[BETO] Betonica initialized\n");return 0;}
/* 1=herb_garden 2=woodland_edge 3=medicinal 4=cottage_garden 5=container */
int beto_planning(int lc,int fs,int lw,int sh,int fc,int mi,int bw){return add(betops,&st.n_plan,&st.t_flower,N,lc,fs,lw,sh,fc,mi,bw);}
int beto_execution(int lc,int fs,int lw,int sh,int fc,int mi,int bw){return add(beto_es,&st.n_exec,&st.t_leaf,N-2,lc,fs,lw,sh,fc,mi,bw);}
int beto_evaluation(int lc,int fs,int lw,int sh,int fc,int mi,int bw){return add(betovs,&st.n_eval,&st.t_stem,N-4,lc,fs,lw,sh,fc,mi,bw);}
int beto_harvest(int lc,int fs,int lw,int sh,int fc,int mi,int bw){return add(beto_hv,&st.n_harv,&st.t_color,N-6,lc,fs,lw,sh,fc,mi,bw);}
int beto_market(int lc,int fs,int lw,int sh,int fc,int mi,int bw){return add(betoms,&st.n_mkt,&st.t_med,N-6,lc,fs,lw,sh,fc,mi,bw);}
void beto_report(void){ps("[BETO] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nHarv: ");pi(st.n_harv);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" med=");pi(st.t_med);ps("\n");}
void beto_state(void){ps("[BETO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Betonica (Wood Betony) Admin Demo ===\n\n");beto_init();
ps("Betonica planning (herb garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;beto_planning(lc,8+(i*3),4+(i%4),25+(i*5),(i%5)+1,50+(i*5),24+(i%6));}
ps("\nBetonica execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;beto_execution(lc,10+(i*2),5+(i%3),28+(i*4),(i%5)+1,55+(i*4),26+(i%5));}
ps("\nBetonica evaluation (quality check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;beto_evaluation(lc,12+(i*2),6+(i%3),30+(i*3),(i%4)+2,58+(i%3),28+(i%4));}
ps("\nBetonica harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;beto_harvest(lc,6+(i*3),3+(i%4),22+(i*5),(i%5)+1,45+(i%5),22+(i%5));}
ps("\nBetonica medicinal herb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;beto_market(lc,14+(i*2),7+(i%2),(i%6)+2,65+(i%3),30+(i%3),60+(i%4));}
ps("\n");beto_report();beto_state();ps("\n=== Demo Complete ===\n");return 0;}
