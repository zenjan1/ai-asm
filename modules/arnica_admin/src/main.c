/* arnica_admin: Arnica (Leopard's Bane) medicinal herb management (v1.0)
 * Arnica planning, planting, evaluation, flower harvest, market
 * Features: flower diameter, petal count, stem height, leaf pair count, compound mg, altitude
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,stem_ht,leaf_pair,compound_mg,altitude,active;} arn_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_flower,t_petal,t_stem,t_leaf,t_compound;} arn_state_t;
static arn_t arnps[N],arnes[N-2],arnvs[N-4],arnhv[N-6],arnms[N-6]; static arn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(arn_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){if(*cnt>=mx)return -1;arn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->stem_ht=sh;x->leaf_pair=lp;x->compound_mg=cm;x->altitude=alt_val;x->active=1;*sum+=fd;(*cnt)++;ps("[ARN] Arnica ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" lp=");pi(lp);ps(" cm=");pi(cm);ps("\n");return *cnt-1;}
int arn_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_leaf=0;st.t_compound=0;for(int i=0;i<N;i++)arnps[i].active=0;for(int i=0;i<N-2;i++)arnes[i].active=0;for(int i=0;i<N-4;i++)arnvs[i].active=0;for(int i=0;i<N-6;i++)arnhv[i].active=0;for(int i=0;i<N-6;i++)arnms[i].active=0;init=1;ps("[ARN] Arnica initialized\n");return 0;}
/* 1=mountain 2=meadow 3=medicinal 4=alpine 5=container */
int arn_planning(int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){return add(arnps,&st.n_plan,&st.t_flower,N,lc,fd,pc,sh,lp,cm,alt_val);}
int arn_execution(int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){return add(arnes,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,sh,lp,cm,alt_val);}
int arn_evaluation(int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){return add(arnvs,&st.n_eval,&st.t_stem,N-4,lc,fd,pc,sh,lp,cm,alt_val);}
int arn_flower_harvest(int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){return add(arnhv,&st.n_harv,&st.t_leaf,N-6,lc,fd,pc,sh,lp,cm,alt_val);}
int arn_market(int lc,int fd,int pc,int sh,int lp,int cm,int alt_val){return add(arnms,&st.n_mkt,&st.t_compound,N-6,lc,fd,pc,sh,lp,cm,alt_val);}
void arn_report(void){ps("[ARN] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nHarv: ");pi(st.n_harv);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" compound=");pi(st.t_compound);ps("\n");}
void arn_state(void){ps("[ARN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Arnica (Leopard's Bane) Admin Demo ===\n\n");arn_init();
ps("Arnica planning (mountain layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;arn_planning(lc,6+(i*2),8+(i%5),25+(i*5),3+(i%3),40+(i*10),1500+(i*200));}
ps("\nArnica execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;arn_execution(lc,7+(i*2),9+(i%4),28+(i*4),4+(i%3),45+(i*8),1700+(i*150));}
ps("\nArnica evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;arn_evaluation(lc,8+(i*2),10+(i%3),32+(i*3),5+(i%2),50+(i*7),1900+(i*100));}
ps("\nArnica flower harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;arn_flower_harvest(lc,5+(i*2),7+(i%4),20+(i*4),3+(i%3),35+(i*8),1300+(i*200));}
ps("\nArnica medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;arn_market(lc,9+(i*2),11+(i%3),35+(i*3),6+(i%2),55+(i*6),2100+(i*100));}
ps("\n");arn_report();arn_state();ps("\n=== Demo Complete ===\n");return 0;}
