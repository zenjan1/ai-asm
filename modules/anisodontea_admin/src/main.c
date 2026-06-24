/* anisodontea_admin: Anisodontea (Cape Mallow) flowering shrub management (v1.0)
 * Anisodontea planning, planting, evaluation, pruning, market
 * Features: flower diameter, petal count, bush height, bloom color, leaf shape, branch count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,bush_ht,bloom_color,leaf_shp,branch_ct,active;} asd_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_flower,t_petal,t_bush,t_bloom,t_branch;} asd_state_t;
static asd_t asdps[N],asdes[N-2],asdvs[N-4],asdpr[N-6],asdms[N-6]; static asd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asd_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){if(*cnt>=mx)return -1;asd_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->bush_ht=bh;x->bloom_color=bc;x->leaf_shp=ls;x->branch_ct=bc_ct;x->active=1;*sum+=fd;(*cnt)++;ps("[ASD] Anisodontea ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" bh=");pi(bh);ps(" bc=");pi(bc);ps(" ls=");pi(ls);ps("\n");return *cnt-1;}
int asd_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_bush=0;st.t_bloom=0;st.t_branch=0;for(int i=0;i<N;i++)asdps[i].active=0;for(int i=0;i<N-2;i++)asdes[i].active=0;for(int i=0;i<N-4;i++)asdvs[i].active=0;for(int i=0;i<N-6;i++)asdpr[i].active=0;for(int i=0;i<N-6;i++)asdms[i].active=0;init=1;ps("[ASD] Anisodontea initialized\n");return 0;}
/* 1=garden 2=hedge 3=container 4=border 5=trellis */
int asd_planning(int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){return add(asdps,&st.n_plan,&st.t_flower,N,lc,fd,pc,bh,bc,ls,bc_ct);}
int asd_execution(int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){return add(asdes,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,bh,bc,ls,bc_ct);}
int asd_evaluation(int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){return add(asdvs,&st.n_eval,&st.t_bush,N-4,lc,fd,pc,bh,bc,ls,bc_ct);}
int asd_pruning(int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){return add(asdpr,&st.n_prune,&st.t_bloom,N-6,lc,fd,pc,bh,bc,ls,bc_ct);}
int asd_market(int lc,int fd,int pc,int bh,int bc,int ls,int bc_ct){return add(asdms,&st.n_mkt,&st.t_branch,N-6,lc,fd,pc,bh,bc,ls,bc_ct);}
void asd_report(void){ps("[ASD] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" bush=");pi(st.t_bush);ps("\nPrune: ");pi(st.n_prune);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" branch=");pi(st.t_branch);ps("\n");}
void asd_state(void){ps("[ASD] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anisodontea (Cape Mallow) Admin Demo ===\n\n");asd_init();
ps("Anisodontea planning (shrub layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;asd_planning(lc,6+(i*2),5+(i%4),60+(i*12),(i%7)+1,3+(i%3),8+(i*3));}
ps("\nAnisodontea execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;asd_execution(lc,7+(i*2),6+(i%3),70+(i*10),(i%7)+1,4+(i%3),10+(i*2));}
ps("\nAnisodontea evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;asd_evaluation(lc,8+(i*2),7+(i%3),80+(i*8),(i%6)+2,5+(i%2),12+(i*2));}
ps("\nAnisodontea pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;asd_pruning(lc,5+(i*2),4+(i%3),50+(i*10),(i%5)+1,3+(i%3),6+(i*3));}
ps("\nAnisodontea shrub market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;asd_market(lc,9+(i*2),8+(i%2),90+(i*6),(i%6)+2,6+(i%2),14+(i*2));}
ps("\n");asd_report();asd_state();ps("\n=== Demo Complete ===\n");return 0;}
