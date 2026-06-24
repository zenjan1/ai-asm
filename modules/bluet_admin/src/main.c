/* bluet_admin: Bluet (Houstonia) woodland wildflower management (v1.0)
 * Bluet planning, planting, evaluation, seed collection, market
 * Features: flower diameter, leaf width, plant height, flower color, seed count, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,leaf_wd,plant_ht,flower_color,seed_ct,bloom_wk,active;} blt_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_leaf,t_plant,t_color,t_seed;} blt_state_t;
static blt_t bltps[N],bltes[N-2],bltvs[N-4],bltsd[N-6],bltms[N-6]; static blt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blt_t*a,int*cnt,int*sum,int mx,int lc,int fd,int lw,int ph,int fc,int sc,int bw){if(*cnt>=mx)return -1;blt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->leaf_wd=lw;x->plant_ht=ph;x->flower_color=fc;x->seed_ct=sc;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLT] Bluet ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" lw=");pi(lw);ps(" ph=");pi(ph);ps(" fc=");pi(fc);ps(" sc=");pi(sc);ps("\n");return *cnt-1;}
int blt_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_leaf=0;st.t_plant=0;st.t_color=0;st.t_seed=0;for(int i=0;i<N;i++)bltps[i].active=0;for(int i=0;i<N-2;i++)bltes[i].active=0;for(int i=0;i<N-4;i++)bltvs[i].active=0;for(int i=0;i<N-6;i++)bltsd[i].active=0;for(int i=0;i<N-6;i++)bltms[i].active=0;init=1;ps("[BLT] Bluet initialized\n");return 0;}
/* 1=woodland 2=rock_garden 3=alpine 4=shade_border 5=container */
int blt_planning(int lc,int fd,int lw,int ph,int fc,int sc,int bw){return add(bltps,&st.n_plan,&st.t_flower,N,lc,fd,lw,ph,fc,sc,bw);}
int blt_execution(int lc,int fd,int lw,int ph,int fc,int sc,int bw){return add(bltes,&st.n_exec,&st.t_leaf,N-2,lc,fd,lw,ph,fc,sc,bw);}
int blt_evaluation(int lc,int fd,int lw,int ph,int fc,int sc,int bw){return add(bltvs,&st.n_eval,&st.t_plant,N-4,lc,fd,lw,ph,fc,sc,bw);}
int blt_seed_collection(int lc,int fd,int lw,int ph,int fc,int sc,int bw){return add(bltsd,&st.n_seed,&st.t_color,N-6,lc,fd,lw,ph,fc,sc,bw);}
int blt_market(int lc,int fd,int lw,int ph,int fc,int sc,int bw){return add(bltms,&st.n_mkt,&st.t_seed,N-6,lc,fd,lw,ph,fc,sc,bw);}
void blt_report(void){ps("[BLT] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" plant=");pi(st.t_plant);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void blt_state(void){ps("[BLT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluet (Houstonia) Admin Demo ===\n\n");blt_init();
ps("Bluet planning (variety layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blt_planning(lc,1+(i*1),1+(i*1),5+(i*2),(i%4)+1,10+(i*5),16+(i%6));}
ps("\nBluet execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blt_execution(lc,1+(i*1),2+(i*1),6+(i*2),(i%4)+1,12+(i*4),18+(i%5));}
ps("\nBluet evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blt_evaluation(lc,2+(i*1),3+(i*1),7+(i*2),(i%3)+2,14+(i*3),20+(i%4));}
ps("\nBluet seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blt_seed_collection(lc,1+(i*1),1+(i*1),4+(i*2),(i%4)+1,8+(i*5),14+(i%5));}
ps("\nBluet market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blt_market(lc,2+(i*1),4+(i*1),8+(i*2),(i%3)+3,16+(i*3),22+(i%3));}
ps("\n");blt_report();blt_state();ps("\n=== Demo Complete ===\n");return 0;}
