/* bauhinia_admin: Bauhinia (Orchid Tree) tropical ornamental management (v1.0)
 * Bauhinia planning, planting, evaluation, training, market
 * Features: flower diameter, petal count, vine length, flower color, leaf shape, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,vine_ln,flower_color,leaf_shp,bloom_wk,active;} bauh_t;
typedef struct{int n_plan,n_exec,n_eval,n_train,n_mkt,t_flower,t_petal,t_vine,t_color,t_leaf;} bauh_state_t;
static bauh_t bauhps[N],bauhes[N-2],bauhvs[N-4],bauhtr[N-6],bauhms[N-6]; static bauh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bauh_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int vl,int fc,int ls,int bw){if(*cnt>=mx)return -1;bauh_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->vine_ln=vl;x->flower_color=fc;x->leaf_shp=ls;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BAUH] Bauhinia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" vl=");pi(vl);ps(" fc=");pi(fc);ps(" ls=");pi(ls);ps("\n");return *cnt-1;}
int bauh_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_train=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_vine=0;st.t_color=0;st.t_leaf=0;for(int i=0;i<N;i++)bauhps[i].active=0;for(int i=0;i<N-2;i++)bauhes[i].active=0;for(int i=0;i<N-4;i++)bauhvs[i].active=0;for(int i=0;i<N-6;i++)bauhtr[i].active=0;for(int i=0;i<N-6;i++)bauhms[i].active=0;init=1;ps("[BAUH] Bauhinia initialized\n");return 0;}
/* 1=tropical_garden 2=pergola 3=fence 4=container 5=arbor */
int bauh_planning(int lc,int fd,int pc,int vl,int fc,int ls,int bw){return add(bauhps,&st.n_plan,&st.t_flower,N,lc,fd,pc,vl,fc,ls,bw);}
int bauh_execution(int lc,int fd,int pc,int vl,int fc,int ls,int bw){return add(bauhes,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,vl,fc,ls,bw);}
int bauh_evaluation(int lc,int fd,int pc,int vl,int fc,int ls,int bw){return add(bauhvs,&st.n_eval,&st.t_vine,N-4,lc,fd,pc,vl,fc,ls,bw);}
int bauh_training(int lc,int fd,int pc,int vl,int fc,int ls,int bw){return add(bauhtr,&st.n_train,&st.t_color,N-6,lc,fd,pc,vl,fc,ls,bw);}
int bauh_market(int lc,int fd,int pc,int vl,int fc,int ls,int bw){return add(bauhms,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pc,vl,fc,ls,bw);}
void bauh_report(void){ps("[BAUH] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" vine=");pi(st.t_vine);ps("\nTrain: ");pi(st.n_train);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void bauh_state(void){ps("[BAUH] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Train=");pi(st.n_train);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bauhinia (Orchid Tree) Admin Demo ===\n\n");bauh_init();
ps("Bauhinia planning (tropical layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;bauh_planning(lc,10+(i*4),5+(i%5),50+(i*15),(i%7)+1,3+(i%3),20+(i%6));}
ps("\nBauhinia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;bauh_execution(lc,12+(i*3),6+(i%4),55+(i*12),(i%7)+1,4+(i%3),22+(i%5));}
ps("\nBauhinia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;bauh_evaluation(lc,14+(i*3),7+(i%3),60+(i*10),(i%6)+2,5+(i%2),24+(i%4));}
ps("\nBauhinia training...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;bauh_training(lc,8+(i*4),4+(i%5),45+(i*15),(i%5)+1,3+(i%3),18+(i%5));}
ps("\nBauhinia tropical vine market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;bauh_market(lc,16+(i*3),8+(i%3),(i%6)+2,70+(i*10),6+(i%2),26+(i%3));}
ps("\n");bauh_report();bauh_state();ps("\n=== Demo Complete ===\n");return 0;}
