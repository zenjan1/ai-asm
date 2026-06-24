/* asarum_admin: Asarum (Wild Ginger) shade groundcover management (v1.0)
 * Asarum planning, planting, evaluation, root division, market
 * Features: leaf span, root spread, flower type, leaf vein pattern, ground cover, shade tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,leaf_sp,root_sp,flower_tp,leaf_vn,ground_cv,shade_tl,active;} asr_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_leaf,t_root,t_flower,t_vein,t_ground;} asr_state_t;
static asr_t asrps[N],asres[N-2],asrvs[N-4],asrdv[N-6],asrms[N-6]; static asr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asr_t*a,int*cnt,int*sum,int mx,int lc,int ls,int rs,int ft,int lv,int gc,int slt){if(*cnt>=mx)return -1;asr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->leaf_sp=ls;x->root_sp=rs;x->flower_tp=ft;x->leaf_vn=lv;x->ground_cv=gc;x->shade_tl=slt;x->active=1;*sum+=ls;(*cnt)++;ps("[ASR] Asarum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ls=");pi(ls);ps(" rs=");pi(rs);ps(" ft=");pi(ft);ps(" lv=");pi(lv);ps(" gc=");pi(gc);ps("\n");return *cnt-1;}
int asr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_leaf=0;st.t_root=0;st.t_flower=0;st.t_vein=0;st.t_ground=0;for(int i=0;i<N;i++)asrps[i].active=0;for(int i=0;i<N-2;i++)asres[i].active=0;for(int i=0;i<N-4;i++)asrvs[i].active=0;for(int i=0;i<N-6;i++)asrdv[i].active=0;for(int i=0;i<N-6;i++)asrms[i].active=0;init=1;ps("[ASR] Asarum initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=groundcover 4=container 5=naturalized */
int asr_planning(int lc,int ls,int rs,int ft,int lv,int gc,int slt){return add(asrps,&st.n_plan,&st.t_leaf,N,lc,ls,rs,ft,lv,gc,slt);}
int asr_execution(int lc,int ls,int rs,int ft,int lv,int gc,int slt){return add(asres,&st.n_exec,&st.t_root,N-2,lc,ls,rs,ft,lv,gc,slt);}
int asr_evaluation(int lc,int ls,int rs,int ft,int lv,int gc,int slt){return add(asrvs,&st.n_eval,&st.t_flower,N-4,lc,ls,rs,ft,lv,gc,slt);}
int asr_root_division(int lc,int ls,int rs,int ft,int lv,int gc,int slt){return add(asrdv,&st.n_div,&st.t_vein,N-6,lc,ls,rs,ft,lv,gc,slt);}
int asr_market(int lc,int ls,int rs,int ft,int lv,int gc,int slt){return add(asrms,&st.n_mkt,&st.t_ground,N-6,lc,ls,rs,ft,lv,gc,slt);}
void asr_report(void){ps("[ASR] Plan: ");pi(st.n_plan);ps(" leaf=");pi(st.t_leaf);ps("\nExec: ");pi(st.n_exec);ps(" root=");pi(st.t_root);ps("\nEval: ");pi(st.n_eval);ps(" flower=");pi(st.t_flower);ps("\nDiv: ");pi(st.n_div);ps(" vein=");pi(st.t_vein);ps("\nMkt: ");pi(st.n_mkt);ps(" ground=");pi(st.t_ground);ps("\n");}
void asr_state(void){ps("[ASR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Asarum (Wild Ginger) Admin Demo ===\n\n");asr_init();
ps("Asarum planning (shade layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;asr_planning(lc,12+(i*3),15+(i*4),(i%4)+1,5+(i%3),60+(i*5),(i%5)+1);}
ps("\nAsarum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;asr_execution(lc,14+(i*2),18+(i*3),(i%4)+1,6+(i%3),65+(i*4),(i%5)+1);}
ps("\nAsarum evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;asr_evaluation(lc,16+(i*2),20+(i*3),(i%3)+2,7+(i%2),70+(i*3),(i%4)+2);}
ps("\nAsarum root division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;asr_root_division(lc,10+(i*3),12+(i*3),(i%3)+1,4+(i%3),55+(i*4),(i%5)+1);}
ps("\nAsarum groundcover market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;asr_market(lc,18+(i*2),22+(i*2),(i%4)+2,8+(i%2),75+(i*3),(i%4)+2);}
ps("\n");asr_report();asr_state();ps("\n=== Demo Complete ===\n");return 0;}
