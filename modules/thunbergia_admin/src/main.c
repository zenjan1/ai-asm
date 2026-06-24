/* thunbergia_admin: Thunbergia (Black-eyed Susan Vine) climbing vine cultivation (v1.0)
 * Thunbergia planning, execution, evaluation, trellis management, flower market
 * Features: vine growth tracking, flower color varieties, support structure, pruning, pest monitoring
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,vine_len,flower_ct,color,sup_ht,bloom_wk,pest_lv,active;} thu_t;
typedef struct{int n_plan,n_exec,n_eval,n_trel,n_mkt,t_vine,t_flower,t_sup,t_bloom,t_pest;} thu_state_t;
static thu_t thups[N],thues[N-2],thuvs[N-4],thuts[N-6],thums[N-6]; static thu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thu_t*a,int*cnt,int*sum,int mx,int cv,int vl,int fc,int cl,int sh,int bw,int pl){if(*cnt>=mx)return -1;thu_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->vine_len=vl;x->flower_ct=fc;x->color=cl;x->sup_ht=sh;x->bloom_wk=bw;x->pest_lv=pl;x->active=1;*sum+=vl;(*cnt)++;ps("[THU] Thunbergia ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" vl=");pi(vl);ps(" fl=");pi(fc);ps(" cl=");pi(cl);ps(" sh=");pi(sh);ps("\n");return *cnt-1;}
int thu_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trel=0;st.n_mkt=0;st.t_vine=0;st.t_flower=0;st.t_sup=0;st.t_bloom=0;st.t_pest=0;for(int i=0;i<N;i++)thups[i].active=0;for(int i=0;i<N-2;i++)thues[i].active=0;for(int i=0;i<N-4;i++)thuvs[i].active=0;for(int i=0;i<N-6;i++)thuts[i].active=0;for(int i=0;i<N-6;i++)thums[i].active=0;init=1;ps("[THU] Thunbergia (black-eyed Susan vine) initialized\n");return 0;}
/* 1=alata(yellow) 2=grandiflora(blue) 3=cordifolia(white) 4=battiscombei(purple) 5=friedricii(orange) */
int thu_planning(int cv,int vl,int fc,int cl,int sh,int bw,int pl){return add(thups,&st.n_plan,&st.t_vine,N,cv,vl,fc,cl,sh,bw,pl);}
int thu_execution(int cv,int vl,int fc,int cl,int sh,int bw,int pl){return add(thues,&st.n_exec,&st.t_flower,N-2,cv,vl,fc,cl,sh,bw,pl);}
int thu_evaluation(int cv,int vl,int fc,int cl,int sh,int bw,int pl){return add(thuvs,&st.n_eval,&st.t_sup,N-4,cv,vl,fc,cl,sh,bw,pl);}
int thu_trellis(int cv,int vl,int fc,int cl,int sh,int bw,int pl){return add(thuts,&st.n_trel,&st.t_bloom,N-6,cv,vl,fc,cl,sh,bw,pl);}
int thu_market(int cv,int vl,int fc,int cl,int sh,int bw,int pl){return add(thums,&st.n_mkt,&st.t_pest,N-6,cv,vl,fc,cl,sh,bw,pl);}
void thu_report(void){ps("[THU] Plan: ");pi(st.n_plan);ps(" vine=");pi(st.t_vine);ps("\nExec: ");pi(st.n_exec);ps(" fl=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" sup=");pi(st.t_sup);ps("\nTrel: ");pi(st.n_trel);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" pest=");pi(st.t_pest);ps("\n");}
void thu_state(void){ps("[THU] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trel=");pi(st.n_trel);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Thunbergia (Black-eyed Susan Vine) Admin Demo ===\n\n");thu_init();
ps("Thunbergia planning (trellis layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;thu_planning(cv,120+(i*25),8+(i*3),(i%5)+1,180+(i*15),18+(i%8),(i%3));}
ps("\nThunbergia execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;thu_execution(cv,140+(i*20),10+(i*2),(i%4)+1,200+(i*12),20+(i%6),(i%3));}
ps("\nThunbergia evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;thu_evaluation(cv,160+(i*18),12+(i*2),(i%3)+2,220+(i*10),22+(i%5),(i%2));}
ps("\nThunbergia trellis management...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;thu_trellis(cv,100+(i*22),6+(i*3),(i%5)+1,160+(i*14),16+(i%7),1);}
ps("\nThunbergia flower market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;thu_market(cv,180+(i*15),14+(i*2),(i%4)+2,240+(i*8),24+(i%4),(i%3));}
ps("\n");thu_report();thu_state();ps("\n=== Demo Complete ===\n");return 0;}
