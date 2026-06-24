/* basilicum_admin: Basilicum (Holy Basil / Tulsi) sacred herb management (v1.0)
 * Basilicum planning, planting, evaluation, cutting, market
 * Features: flower whorl, leaf scent, stem height, eugenol pct, adaptogen index, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_wh,leaf_scent,stem_ht,eugenol_pc,adapt_idx,bloom_wk,active;} basi_t;
typedef struct{int n_plan,n_exec,n_eval,n_cut,n_mkt,t_flower,t_scent,t_stem,t_eugenol,t_adapt;} basi_state_t;
static basi_t basips[N],basies[N-2],basivs[N-4],basict[N-6],basims[N-6]; static basi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(basi_t*a,int*cnt,int*sum,int mx,int lc,int fw,int ls,int sh,int ep,int ai,int bw){if(*cnt>=mx)return -1;basi_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_wh=fw;x->leaf_scent=ls;x->stem_ht=sh;x->eugenol_pc=ep;x->adapt_idx=ai;x->bloom_wk=bw;x->active=1;*sum+=fw;(*cnt)++;ps("[BASI] Basilicum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fw=");pi(fw);ps(" ls=");pi(ls);ps(" sh=");pi(sh);ps(" ep=");pi(ep);ps(" ai=");pi(ai);ps("\n");return *cnt-1;}
int basi_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_cut=0;st.n_mkt=0;st.t_flower=0;st.t_scent=0;st.t_stem=0;st.t_eugenol=0;st.t_adapt=0;for(int i=0;i<N;i++)basips[i].active=0;for(int i=0;i<N-2;i++)basies[i].active=0;for(int i=0;i<N-4;i++)basivs[i].active=0;for(int i=0;i<N-6;i++)basict[i].active=0;for(int i=0;i<N-6;i++)basims[i].active=0;init=1;ps("[BASI] Basilicum initialized\n");return 0;}
/* 1=sacred_garden 2=ayurvedic 3=container 4=temple 5=medicinal */
int basi_planning(int lc,int fw,int ls,int sh,int ep,int ai,int bw){return add(basips,&st.n_plan,&st.t_flower,N,lc,fw,ls,sh,ep,ai,bw);}
int basi_execution(int lc,int fw,int ls,int sh,int ep,int ai,int bw){return add(basies,&st.n_exec,&st.t_scent,N-2,lc,fw,ls,sh,ep,ai,bw);}
int basi_evaluation(int lc,int fw,int ls,int sh,int ep,int ai,int bw){return add(basivs,&st.n_eval,&st.t_stem,N-4,lc,fw,ls,sh,ep,ai,bw);}
int basi_cutting(int lc,int fw,int ls,int sh,int ep,int ai,int bw){return add(basict,&st.n_cut,&st.t_eugenol,N-6,lc,fw,ls,sh,ep,ai,bw);}
int basi_market(int lc,int fw,int ls,int sh,int ep,int ai,int bw){return add(basims,&st.n_mkt,&st.t_adapt,N-6,lc,fw,ls,sh,ep,ai,bw);}
void basi_report(void){ps("[BASI] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" scent=");pi(st.t_scent);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nCut: ");pi(st.n_cut);ps(" eugenol=");pi(st.t_eugenol);ps("\nMkt: ");pi(st.n_mkt);ps(" adapt=");pi(st.t_adapt);ps("\n");}
void basi_state(void){ps("[BASI] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Cut=");pi(st.n_cut);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Basilicum (Holy Basil / Tulsi) Admin Demo ===\n\n");basi_init();
ps("Basilicum planning (sacred garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;basi_planning(lc,6+(i*3),50+(i*5),25+(i*4),30+(i*4),70+(i*3),24+(i%6));}
ps("\nBasilicum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;basi_execution(lc,8+(i*2),55+(i*4),28+(i*3),32+(i*3),72+(i*3),26+(i%5));}
ps("\nBasilicum evaluation (quality check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;basi_evaluation(lc,10+(i*2),60+(i*3),30+(i*3),35+(i*3),75+(i%3),28+(i%4));}
ps("\nBasilicum cutting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;basi_cutting(lc,5+(i*3),45+(i%5),22+(i%4),28+(i%4),65+(i%3),22+(i%5));}
ps("\nBasilicum sacred herb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;basi_market(lc,12+(i*2),65+(i%3),(i%6)+2,40+(i%3),80+(i%2),30+(i%3));}
ps("\n");basi_report();basi_state();ps("\n=== Demo Complete ===\n");return 0;}
