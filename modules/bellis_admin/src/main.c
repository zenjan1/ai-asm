/* bellis_admin: Bellis (English Daisy) spring lawn management (v1.0)
 * Bellis planning, planting, evaluation, division, market
 * Features: flower diameter, petal count, flower color, stem height, leaf rosette, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,flower_color,stem_ht,leaf_ros,bloom_wk,active;} bell_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_petal,t_color,t_stem,t_leaf;} bell_state_t;
static bell_t bellps[N],belles[N-2],bellvs[N-4],belldv[N-6],bellms[N-6]; static bell_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bell_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int fc,int sh,int lr,int bw){if(*cnt>=mx)return -1;bell_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->flower_color=fc;x->stem_ht=sh;x->leaf_ros=lr;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BELL] Bellis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" sh=");pi(sh);ps(" lr=");pi(lr);ps("\n");return *cnt-1;}
int bell_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_color=0;st.t_stem=0;st.t_leaf=0;for(int i=0;i<N;i++)bellps[i].active=0;for(int i=0;i<N-2;i++)belles[i].active=0;for(int i=0;i<N-4;i++)bellvs[i].active=0;for(int i=0;i<N-6;i++)belldv[i].active=0;for(int i=0;i<N-6;i++)bellms[i].active=0;init=1;ps("[BELL] Bellis initialized\n");return 0;}
/* 1=lawn 2=meadow 3=border 4=container 5=spring_garden */
int bell_planning(int lc,int fd,int pc,int fc,int sh,int lr,int bw){return add(bellps,&st.n_plan,&st.t_flower,N,lc,fd,pc,fc,sh,lr,bw);}
int bell_execution(int lc,int fd,int pc,int fc,int sh,int lr,int bw){return add(belles,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,fc,sh,lr,bw);}
int bell_evaluation(int lc,int fd,int pc,int fc,int sh,int lr,int bw){return add(bellvs,&st.n_eval,&st.t_color,N-4,lc,fd,pc,fc,sh,lr,bw);}
int bell_division(int lc,int fd,int pc,int fc,int sh,int lr,int bw){return add(belldv,&st.n_div,&st.t_stem,N-6,lc,fd,pc,fc,sh,lr,bw);}
int bell_market(int lc,int fd,int pc,int fc,int sh,int lr,int bw){return add(bellms,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pc,fc,sh,lr,bw);}
void bell_report(void){ps("[BELL] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nDiv: ");pi(st.n_div);ps(" stem=");pi(st.t_stem);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void bell_state(void){ps("[BELL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bellis (English Daisy) Admin Demo ===\n\n");bell_init();
ps("Bellis planning (lawn layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bell_planning(lc,2+(i*2),8+(i%5),(i%5)+1,6+(i*2),4+(i%3),10+(i%6));}
ps("\nBellis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bell_execution(lc,3+(i*2),9+(i%4),(i%5)+1,7+(i*2),5+(i%3),12+(i%5));}
ps("\nBellis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bell_evaluation(lc,4+(i*2),10+(i%3),(i%4)+2,8+(i*2),6+(i%2),14+(i%4));}
ps("\nBellis division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bell_division(lc,1+(i*2),7+(i%4),(i%5)+1,5+(i*2),3+(i%3),8+(i%5));}
ps("\nBellis spring daisy market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bell_market(lc,5+(i*2),11+(i%3),(i%4)+2,9+(i*2),7+(i%2),16+(i%3));}
ps("\n");bell_report();bell_state();ps("\n=== Demo Complete ===\n");return 0;}
