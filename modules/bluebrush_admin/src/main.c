/* bluebrush_admin: Bluebrush (Alyogyne) hibiscus shrub management (v1.0)
 * Bluebrush planning, planting, evaluation, pruning, market
 * Features: flower diameter, petal count, bush height, flower color, leaf width, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,bush_ht,flower_color,leaf_wd,bloom_wk,active;} blbr_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_flower,t_petal,t_bush,t_color,t_leaf;} blbr_state_t;
static blbr_t blbrps[N],blbres[N-2],blbrvs[N-4],blbrpr[N-6],blbrms[N-6]; static blbr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blbr_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int bh,int fc,int lw,int bw){if(*cnt>=mx)return -1;blbr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->bush_ht=bh;x->flower_color=fc;x->leaf_wd=lw;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLBR] Bluebrush ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" bh=");pi(bh);ps(" fc=");pi(fc);ps(" lw=");pi(lw);ps("\n");return *cnt-1;}
int blbr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_bush=0;st.t_color=0;st.t_leaf=0;for(int i=0;i<N;i++)blbrps[i].active=0;for(int i=0;i<N-2;i++)blbres[i].active=0;for(int i=0;i<N-4;i++)blbrvs[i].active=0;for(int i=0;i<N-6;i++)blbrpr[i].active=0;for(int i=0;i<N-6;i++)blbrms[i].active=0;init=1;ps("[BLBR] Bluebrush initialized\n");return 0;}
/* 1=garden 2=hedge 3=container 4=specimen 5=coastal */
int blbr_planning(int lc,int fd,int pc,int bh,int fc,int lw,int bw){return add(blbrps,&st.n_plan,&st.t_flower,N,lc,fd,pc,bh,fc,lw,bw);}
int blbr_execution(int lc,int fd,int pc,int bh,int fc,int lw,int bw){return add(blbres,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,bh,fc,lw,bw);}
int blbr_evaluation(int lc,int fd,int pc,int bh,int fc,int lw,int bw){return add(blbrvs,&st.n_eval,&st.t_bush,N-4,lc,fd,pc,bh,fc,lw,bw);}
int blbr_pruning(int lc,int fd,int pc,int bh,int fc,int lw,int bw){return add(blbrpr,&st.n_prune,&st.t_color,N-6,lc,fd,pc,bh,fc,lw,bw);}
int blbr_market(int lc,int fd,int pc,int bh,int fc,int lw,int bw){return add(blbrms,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pc,bh,fc,lw,bw);}
void blbr_report(void){ps("[BLBR] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" bush=");pi(st.t_bush);ps("\nPrune: ");pi(st.n_prune);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void blbr_state(void){ps("[BLBR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluebrush (Alyogyne) Admin Demo ===\n\n");blbr_init();
ps("Bluebrush planning (garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blbr_planning(lc,6+(i*2),5+(i%5),80+(i*12),(i%4)+1,8+(i*3),28+(i%6));}
ps("\nBluebrush execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blbr_execution(lc,7+(i*2),6+(i%4),90+(i*10),(i%4)+1,9+(i*2),30+(i%5));}
ps("\nBluebrush evaluation (shrub check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blbr_evaluation(lc,8+(i*2),7+(i%3),100+(i*8),(i%3)+2,10+(i*2),32+(i%4));}
ps("\nBluebrush pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blbr_pruning(lc,5+(i*2),4+(i%4),70+(i*11),(i%4)+1,7+(i*3),26+(i%5));}
ps("\nBluebrush market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blbr_market(lc,9+(i*2),8+(i%3),110+(i*6),(i%3)+3,11+(i*2),34+(i%3));}
ps("\n");blbr_report();blbr_state();ps("\n=== Demo Complete ===\n");return 0;}
