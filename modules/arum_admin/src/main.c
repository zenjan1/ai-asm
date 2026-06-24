/* arum_admin: Arum (Lords and Ladies) woodland tuber management (v1.0)
 * Arum planning, planting, evaluation, tuber division, market
 * Features: spathe height, spadix length, leaf span, bloom color, berry cluster, dormancy month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spathe_ht,spadix_len,leaf_span,bloom_color,berry_cl,dorm_mo,active;} aru_t;
typedef struct{int n_plan,n_exec,n_eval,n_tuber,n_mkt,t_spathe,t_spadix,t_leaf,t_bloom,t_berry;} aru_state_t;
static aru_t arups[N],arues[N-2],aruvs[N-4],arutb[N-6],arums[N-6]; static aru_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aru_t*a,int*cnt,int*sum,int mx,int lc,int sh,int sl,int ls,int bc,int brcl,int dm){if(*cnt>=mx)return -1;aru_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spathe_ht=sh;x->spadix_len=sl;x->leaf_span=ls;x->bloom_color=bc;x->berry_cl=brcl;x->dorm_mo=dm;x->active=1;*sum+=sh;(*cnt)++;ps("[ARU] Arum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" sl=");pi(sl);ps(" ls=");pi(ls);ps(" bc=");pi(bc);ps(" brcl=");pi(brcl);ps("\n");return *cnt-1;}
int aru_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_tuber=0;st.n_mkt=0;st.t_spathe=0;st.t_spadix=0;st.t_leaf=0;st.t_bloom=0;st.t_berry=0;for(int i=0;i<N;i++)arups[i].active=0;for(int i=0;i<N-2;i++)arues[i].active=0;for(int i=0;i<N-4;i++)aruvs[i].active=0;for(int i=0;i<N-6;i++)arutb[i].active=0;for(int i=0;i<N-6;i++)arums[i].active=0;init=1;ps("[ARU] Arum initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=border 4=container 5=wild_garden */
int aru_planning(int lc,int sh,int sl,int ls,int bc,int brcl,int dm){return add(arups,&st.n_plan,&st.t_spathe,N,lc,sh,sl,ls,bc,brcl,dm);}
int aru_execution(int lc,int sh,int sl,int ls,int bc,int brcl,int dm){return add(arues,&st.n_exec,&st.t_spadix,N-2,lc,sh,sl,ls,bc,brcl,dm);}
int aru_evaluation(int lc,int sh,int sl,int ls,int bc,int brcl,int dm){return add(aruvs,&st.n_eval,&st.t_leaf,N-4,lc,sh,sl,ls,bc,brcl,dm);}
int aru_tuber_division(int lc,int sh,int sl,int ls,int bc,int brcl,int dm){return add(arutb,&st.n_tuber,&st.t_bloom,N-6,lc,sh,sl,ls,bc,brcl,dm);}
int aru_market(int lc,int sh,int sl,int ls,int bc,int brcl,int dm){return add(arums,&st.n_mkt,&st.t_berry,N-6,lc,sh,sl,ls,bc,brcl,dm);}
void aru_report(void){ps("[ARU] Plan: ");pi(st.n_plan);ps(" spathe=");pi(st.t_spathe);ps("\nExec: ");pi(st.n_exec);ps(" spadix=");pi(st.t_spadix);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nTuber: ");pi(st.n_tuber);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" berry=");pi(st.t_berry);ps("\n");}
void aru_state(void){ps("[ARU] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Tuber=");pi(st.n_tuber);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Arum (Lords and Ladies) Admin Demo ===\n\n");aru_init();
ps("Arum planning (woodland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aru_planning(lc,20+(i*5),8+(i*3),25+(i*6),(i%5)+1,6+(i%4),10+(i%3));}
ps("\nArum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aru_execution(lc,22+(i*4),10+(i*2),28+(i*5),(i%5)+1,7+(i%3),11+(i%2));}
ps("\nArum evaluation (leaf check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aru_evaluation(lc,25+(i*3),12+(i*2),30+(i*4),(i%4)+2,8+(i%3),11+(i%2));}
ps("\nArum tuber division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aru_tuber_division(lc,18+(i*4),6+(i*3),22+(i*5),(i%4)+1,5+(i%3),9+(i%3));}
ps("\nArum woodland plant market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aru_market(lc,28+(i*3),14+(i*2),35+(i*3),(i%5)+2,9+(i%2),12+(i%2));}
ps("\n");aru_report();aru_state();ps("\n=== Demo Complete ===\n");return 0;}
