/* avens_admin: Avens (Geum) perennial border management (v1.0)
 * Avens planning, planting, evaluation, division, market
 * Features: flower diameter, petal count, stem height, bloom color, leaf type, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,stem_ht,bloom_color,leaf_tp,bloom_wk,active;} ave_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_petal,t_stem,t_color,t_leaf;} ave_state_t;
static ave_t aveps[N],aveses[N-2],avevs[N-4],avedv[N-6],avems[N-6]; static ave_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ave_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int sh,int bc,int lt,int bw){if(*cnt>=mx)return -1;ave_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->stem_ht=sh;x->bloom_color=bc;x->leaf_tp=lt;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[AVE] Avens ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" lt=");pi(lt);ps("\n");return *cnt-1;}
int ave_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_color=0;st.t_leaf=0;for(int i=0;i<N;i++)aveps[i].active=0;for(int i=0;i<N-2;i++)aveses[i].active=0;for(int i=0;i<N-4;i++)avevs[i].active=0;for(int i=0;i<N-6;i++)avedv[i].active=0;for(int i=0;i<N-6;i++)avems[i].active=0;init=1;ps("[AVE] Avens initialized\n");return 0;}
/* 1=border 2=cottage_garden 3=meadow 4=container 5=woodland_edge */
int ave_planning(int lc,int fd,int pc,int sh,int bc,int lt,int bw){return add(aveps,&st.n_plan,&st.t_flower,N,lc,fd,pc,sh,bc,lt,bw);}
int ave_execution(int lc,int fd,int pc,int sh,int bc,int lt,int bw){return add(aveses,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,sh,bc,lt,bw);}
int ave_evaluation(int lc,int fd,int pc,int sh,int bc,int lt,int bw){return add(avevs,&st.n_eval,&st.t_stem,N-4,lc,fd,pc,sh,bc,lt,bw);}
int ave_division(int lc,int fd,int pc,int sh,int bc,int lt,int bw){return add(avedv,&st.n_div,&st.t_color,N-6,lc,fd,pc,sh,bc,lt,bw);}
int ave_market(int lc,int fd,int pc,int sh,int bc,int lt,int bw){return add(avems,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pc,sh,bc,lt,bw);}
void ave_report(void){ps("[AVE] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void ave_state(void){ps("[AVE] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Avens (Geum) Admin Demo ===\n\n");ave_init();
ps("Avens planning (border layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ave_planning(lc,4+(i*2),5+(i%5),30+(i*6),(i%7)+1,3+(i%3),20+(i%6));}
ps("\nAvens execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ave_execution(lc,5+(i*2),6+(i%4),35+(i*5),(i%7)+1,4+(i%3),22+(i%5));}
ps("\nAvens evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ave_evaluation(lc,6+(i*2),7+(i%3),40+(i*4),(i%6)+2,5+(i%2),24+(i%4));}
ps("\nAvens division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ave_division(lc,3+(i*2),4+(i%4),25+(i*6),(i%5)+1,3+(i%3),18+(i%5));}
ps("\nAvens perennial flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ave_market(lc,7+(i*2),8+(i%3),(i%6)+2,6+(i%2),50+(i*4),26+(i%3));}
ps("\n");ave_report();ave_state();ps("\n=== Demo Complete ===\n");return 0;}
