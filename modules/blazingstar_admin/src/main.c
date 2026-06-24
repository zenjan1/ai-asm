/* blazingstar_admin: Blazingstar (Liatris) prairie flower management (v1.0)
 * Blazingstar planning, planting, evaluation, corm division, market
 * Features: spike height, flower count, leaf width, flower color, corm size, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spike_ht,flower_ct,leaf_wd,flower_color,corm_sz,bloom_wk,active;} blaz_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_spike,t_flower,t_leaf,t_color,t_corm;} blaz_state_t;
static blaz_t blazps[N],blazes[N-2],blazvs[N-4],blazdv[N-6],blazms[N-6]; static blaz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blaz_t*a,int*cnt,int*sum,int mx,int lc,int sh,int fc,int lw,int flc,int cs,int bw){if(*cnt>=mx)return -1;blaz_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spike_ht=sh;x->flower_ct=fc;x->leaf_wd=lw;x->flower_color=flc;x->corm_sz=cs;x->bloom_wk=bw;x->active=1;*sum+=sh;(*cnt)++;ps("[BLAZ] Blazingstar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps(" flc=");pi(flc);ps(" cs=");pi(cs);ps("\n");return *cnt-1;}
int blaz_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_spike=0;st.t_flower=0;st.t_leaf=0;st.t_color=0;st.t_corm=0;for(int i=0;i<N;i++)blazps[i].active=0;for(int i=0;i<N-2;i++)blazes[i].active=0;for(int i=0;i<N-4;i++)blazvs[i].active=0;for(int i=0;i<N-6;i++)blazdv[i].active=0;for(int i=0;i<N-6;i++)blazms[i].active=0;init=1;ps("[BLAZ] Blazingstar initialized\n");return 0;}
/* 1=prairie 2=meadow 3=border 4=wildflower_garden 5=naturalized */
int blaz_planning(int lc,int sh,int fc,int lw,int flc,int cs,int bw){return add(blazps,&st.n_plan,&st.t_spike,N,lc,sh,fc,lw,flc,cs,bw);}
int blaz_execution(int lc,int sh,int fc,int lw,int flc,int cs,int bw){return add(blazes,&st.n_exec,&st.t_flower,N-2,lc,sh,fc,lw,flc,cs,bw);}
int blaz_evaluation(int lc,int sh,int fc,int lw,int flc,int cs,int bw){return add(blazvs,&st.n_eval,&st.t_leaf,N-4,lc,sh,fc,lw,flc,cs,bw);}
int blaz_corm_division(int lc,int sh,int fc,int lw,int flc,int cs,int bw){return add(blazdv,&st.n_div,&st.t_color,N-6,lc,sh,fc,lw,flc,cs,bw);}
int blaz_market(int lc,int sh,int fc,int lw,int flc,int cs,int bw){return add(blazms,&st.n_mkt,&st.t_corm,N-6,lc,sh,fc,lw,flc,cs,bw);}
void blaz_report(void){ps("[BLAZ] Plan: ");pi(st.n_plan);ps(" spike=");pi(st.t_spike);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" corm=");pi(st.t_corm);ps("\n");}
void blaz_state(void){ps("[BLAZ] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blazingstar (Liatris) Admin Demo ===\n\n");blaz_init();
ps("Blazingstar planning (prairie layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blaz_planning(lc,60+(i*10),12+(i*4),3+(i*2),(i%5)+1,4+(i*3),30+(i%6));}
ps("\nBlazingstar execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blaz_execution(lc,65+(i*8),14+(i*3),4+(i*2),(i%5)+1,5+(i*2),32+(i%5));}
ps("\nBlazingstar evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blaz_evaluation(lc,70+(i*7),16+(i*3),5+(i*2),(i%4)+2,6+(i%2),34+(i%4));}
ps("\nBlazingstar corm division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blaz_corm_division(lc,50+(i*9),10+(i*4),3+(i*2),(i%5)+1,3+(i*3),28+(i%5));}
ps("\nBlazingstar prairie flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blaz_market(lc,75+(i*6),18+(i*3),6+(i*2),(i%3)+3,7+(i%2),36+(i%3));}
ps("\n");blaz_report();blaz_state();ps("\n=== Demo Complete ===\n");return 0;}
