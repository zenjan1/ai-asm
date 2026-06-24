/* begonia_admin: Begonia (Wax Begonia) shade bedding management (v1.0)
 * Begonia planning, planting, evaluation, pinching, market
 * Features: flower size, leaf width, flower color, leaf color, plant height, bloom period
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_sz,leaf_wd,flower_color,leaf_color,plant_ht,bloom_pd,active;} beg_t;
typedef struct{int n_plan,n_exec,n_eval,n_pinch,n_mkt,t_flower,t_leaf,t_color,t_foliage,t_ht;} beg_state_t;
static beg_t begps[N],beges[N-2],begvs[N-4],begpn[N-6],begms[N-6]; static beg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beg_t*a,int*cnt,int*sum,int mx,int lc,int fs,int lw,int fc,int lcc,int ph,int bp){if(*cnt>=mx)return -1;beg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_sz=fs;x->leaf_wd=lw;x->flower_color=fc;x->leaf_color=lcc;x->plant_ht=ph;x->bloom_pd=bp;x->active=1;*sum+=fs;(*cnt)++;ps("[BEG] Begonia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" lw=");pi(lw);ps(" fc=");pi(fc);ps(" lcc=");pi(lcc);ps(" ph=");pi(ph);ps("\n");return *cnt-1;}
int beg_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_pinch=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_color=0;st.t_foliage=0;st.t_ht=0;for(int i=0;i<N;i++)begps[i].active=0;for(int i=0;i<N-2;i++)beges[i].active=0;for(int i=0;i<N-4;i++)begvs[i].active=0;for(int i=0;i<N-6;i++)begpn[i].active=0;for(int i=0;i<N-6;i++)begms[i].active=0;init=1;ps("[BEG] Begonia initialized\n");return 0;}
/* 1=shade_bed 2=container 3=hanging_basket 4=border 5=patrio */
int beg_planning(int lc,int fs,int lw,int fc,int lcc,int ph,int bp){return add(begps,&st.n_plan,&st.t_flower,N,lc,fs,lw,fc,lcc,ph,bp);}
int beg_execution(int lc,int fs,int lw,int fc,int lcc,int ph,int bp){return add(beges,&st.n_exec,&st.t_leaf,N-2,lc,fs,lw,fc,lcc,ph,bp);}
int beg_evaluation(int lc,int fs,int lw,int fc,int lcc,int ph,int bp){return add(begvs,&st.n_eval,&st.t_color,N-4,lc,fs,lw,fc,lcc,ph,bp);}
int beg_pinching(int lc,int fs,int lw,int fc,int lcc,int ph,int bp){return add(begpn,&st.n_pinch,&st.t_foliage,N-6,lc,fs,lw,fc,lcc,ph,bp);}
int beg_market(int lc,int fs,int lw,int fc,int lcc,int ph,int bp){return add(begms,&st.n_mkt,&st.t_ht,N-6,lc,fs,lw,fc,lcc,ph,bp);}
void beg_report(void){ps("[BEG] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nPinch: ");pi(st.n_pinch);ps(" foliage=");pi(st.t_foliage);ps("\nMkt: ");pi(st.n_mkt);ps(" ht=");pi(st.t_ht);ps("\n");}
void beg_state(void){ps("[BEG] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Pinch=");pi(st.n_pinch);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Begonia (Wax Begonia) Admin Demo ===\n\n");beg_init();
ps("Begonia planning (shade bed layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;beg_planning(lc,2+(i*2),4+(i%4),(i%5)+1,3+(i%3),12+(i*3),20+(i%6));}
ps("\nBegonia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;beg_execution(lc,3+(i*2),5+(i%3),(i%5)+1,4+(i%3),14+(i*2),22+(i%5));}
ps("\nBegonia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;beg_evaluation(lc,4+(i*2),6+(i%3),(i%4)+2,5+(i%2),16+(i*2),24+(i%4));}
ps("\nBegonia pinching...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;beg_pinching(lc,1+(i*2),3+(i%4),(i%5)+1,2+(i%3),10+(i*3),18+(i%5));}
ps("\nBegonia shade bedding market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;beg_market(lc,5+(i*2),7+(i%2),(i%4)+2,6+(i%2),18+(i*2),26+(i%3));}
ps("\n");beg_report();beg_state();ps("\n=== Demo Complete ===\n");return 0;}
