/* salvia_admin: Salvia (Sage) medicinal and ornamental cultivation (v1.0)
 * Salvia planning, execution, evaluation, essential oil extraction, market
 * Features: variety tracking, essential oil yield, bloom cycles, drought tolerance, pruning
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,oil_ml,bloom_d,ht_cm,leaf_a,zone,hardiness,active;} sal_t;
typedef struct{int n_plan,n_exec,n_eval,n_oil,n_mkt,t_oil,t_bloom,t_ht,t_leaf,t_zone;} sal_state_t;
static sal_t salps[N],sales[N-2],salvs[N-4],salos[N-6],salms[N-6]; static sal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sal_t*a,int*cnt,int*sum,int mx,int cv,int ol,int bd,int ht,int la,int zn,int hr){if(*cnt>=mx)return -1;sal_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->oil_ml=ol;x->bloom_d=bd;x->ht_cm=ht;x->leaf_a=la;x->zone=zn;x->hardiness=hr;x->active=1;*sum+=ol;(*cnt)++;ps("[SAL] Salvia ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" oil=");pi(ol);ps(" bl=");pi(bd);ps(" ht=");pi(ht);ps(" la=");pi(la);ps("\n");return *cnt-1;}
int sal_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_oil=0;st.n_mkt=0;st.t_oil=0;st.t_bloom=0;st.t_ht=0;st.t_leaf=0;st.t_zone=0;for(int i=0;i<N;i++)salps[i].active=0;for(int i=0;i<N-2;i++)sales[i].active=0;for(int i=0;i<N-4;i++)salvs[i].active=0;for(int i=0;i<N-6;i++)salos[i].active=0;for(int i=0;i<N-6;i++)salms[i].active=0;init=1;ps("[SAL] Salvia (sage) initialized\n");return 0;}
/* 1=officinalis 2=splendens 3=nemorosa 4=guaranitica 5=pratensis */
int sal_planning(int cv,int ol,int bd,int ht,int la,int zn,int hr){return add(salps,&st.n_plan,&st.t_oil,N,cv,ol,bd,ht,la,zn,hr);}
int sal_execution(int cv,int ol,int bd,int ht,int la,int zn,int hr){return add(sales,&st.n_exec,&st.t_bloom,N-2,cv,ol,bd,ht,la,zn,hr);}
int sal_evaluation(int cv,int ol,int bd,int ht,int la,int zn,int hr){return add(salvs,&st.n_eval,&st.t_ht,N-4,cv,ol,bd,ht,la,zn,hr);}
int sal_oil(int cv,int ol,int bd,int ht,int la,int zn,int hr){return add(salos,&st.n_oil,&st.t_leaf,N-6,cv,ol,bd,ht,la,zn,hr);}
int sal_market(int cv,int ol,int bd,int ht,int la,int zn,int hr){return add(salms,&st.n_mkt,&st.t_zone,N-6,cv,ol,bd,ht,la,zn,hr);}
void sal_report(void){ps("[SAL] Plan: ");pi(st.n_plan);ps(" oil=");pi(st.t_oil);ps("\nExec: ");pi(st.n_exec);ps(" bloom=");pi(st.t_bloom);ps("\nEval: ");pi(st.n_eval);ps(" ht=");pi(st.t_ht);ps("\nOil: ");pi(st.n_oil);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" zone=");pi(st.t_zone);ps("\n");}
void sal_state(void){ps("[SAL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Oil=");pi(st.n_oil);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Salvia (Sage) Admin Demo ===\n\n");sal_init();
ps("Salvia planning (cultivar selection)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;sal_planning(cv,12+(i*3),28+(i%7),45+(i*8),320+(i*40),(i%9)+1,(i%5)+1);}
ps("\nSalvia execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;sal_execution(cv,15+(i*2),30+(i%6),50+(i*7),340+(i*35),(i%8)+2,(i%4)+1);}
ps("\nSalvia evaluation (harvest quality)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;sal_evaluation(cv,18+(i*3),32+(i%5),55+(i*6),360+(i*30),(i%7)+2,(i%3)+2);}
ps("\nSalvia essential oil extraction...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;sal_oil(cv,10+(i*2),25+(i%6),40+(i*7),300+(i*35),(i%6)+1,1);}
ps("\nSalvia market (herbal products)...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;sal_market(cv,20+(i*3),35+(i%5),60+(i*5),380+(i*25),(i%7)+2,(i%3)+1);}
ps("\n");sal_report();sal_state();ps("\n=== Demo Complete ===\n");return 0;}
