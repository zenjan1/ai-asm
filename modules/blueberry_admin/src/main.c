/* blueberry_admin: Blueberry (Vaccinium) fruit production management (v1.0)
 * Blueberry planning, planting, evaluation, harvesting, market
 * Features: berry diameter, leaf width, bush height, berry color, sugar brix, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,berry_dia,leaf_wd,bush_ht,berry_color,sugar_bx,harv_wk,active;} blub_t;
typedef struct{int n_plan,n_exec,n_eval,n_harv,n_mkt,t_berry,t_leaf,t_bush,t_color,t_sugar;} blub_state_t;
static blub_t blubps[N],blubes[N-2],blubvs[N-4],blubhr[N-6],blubms[N-6]; static blub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blub_t*a,int*cnt,int*sum,int mx,int lc,int bd,int lw,int bh,int bc,int sb,int hw){if(*cnt>=mx)return -1;blub_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->berry_dia=bd;x->leaf_wd=lw;x->bush_ht=bh;x->berry_color=bc;x->sugar_bx=sb;x->harv_wk=hw;x->active=1;*sum+=bd;(*cnt)++;ps("[BLUB] Blueberry ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bd=");pi(bd);ps(" lw=");pi(lw);ps(" bh=");pi(bh);ps(" bc=");pi(bc);ps(" sb=");pi(sb);ps("\n");return *cnt-1;}
int blub_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_harv=0;st.n_mkt=0;st.t_berry=0;st.t_leaf=0;st.t_bush=0;st.t_color=0;st.t_sugar=0;for(int i=0;i<N;i++)blubps[i].active=0;for(int i=0;i<N-2;i++)blubes[i].active=0;for(int i=0;i<N-4;i++)blubvs[i].active=0;for(int i=0;i<N-6;i++)blubhr[i].active=0;for(int i=0;i<N-6;i++)blubms[i].active=0;init=1;ps("[BLUB] Blueberry initialized\n");return 0;}
/* 1=highbush 2=lowbush 3=halfhigh 4=rabbiteye 5=southern */
int blub_planning(int lc,int bd,int lw,int bh,int bc,int sb,int hw){return add(blubps,&st.n_plan,&st.t_berry,N,lc,bd,lw,bh,bc,sb,hw);}
int blub_execution(int lc,int bd,int lw,int bh,int bc,int sb,int hw){return add(blubes,&st.n_exec,&st.t_leaf,N-2,lc,bd,lw,bh,bc,sb,hw);}
int blub_evaluation(int lc,int bd,int lw,int bh,int bc,int sb,int hw){return add(blubvs,&st.n_eval,&st.t_bush,N-4,lc,bd,lw,bh,bc,sb,hw);}
int blub_harvesting(int lc,int bd,int lw,int bh,int bc,int sb,int hw){return add(blubhr,&st.n_harv,&st.t_color,N-6,lc,bd,lw,bh,bc,sb,hw);}
int blub_market(int lc,int bd,int lw,int bh,int bc,int sb,int hw){return add(blubms,&st.n_mkt,&st.t_sugar,N-6,lc,bd,lw,bh,bc,sb,hw);}
void blub_report(void){ps("[BLUB] Plan: ");pi(st.n_plan);ps(" berry=");pi(st.t_berry);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf);ps("\nEval: ");pi(st.n_eval);ps(" bush=");pi(st.t_bush);ps("\nHarv: ");pi(st.n_harv);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" sugar=");pi(st.t_sugar);ps("\n");}
void blub_state(void){ps("[BLUB] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Blueberry (Vaccinium) Admin Demo ===\n\n");blub_init();
ps("Blueberry planning (variety layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blub_planning(lc,8+(i*2),4+(i*2),60+(i*10),(i%4)+1,10+(i*2),32+(i%6));}
ps("\nBlueberry execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blub_execution(lc,9+(i*2),5+(i*2),65+(i*8),(i%4)+1,11+(i*2),34+(i%5));}
ps("\nBlueberry evaluation (fruit check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blub_evaluation(lc,10+(i*2),6+(i*2),70+(i*6),(i%3)+2,12+(i*2),36+(i%4));}
ps("\nBlueberry harvesting...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blub_harvesting(lc,7+(i*2),3+(i*2),55+(i*9),(i%4)+1,9+(i*2),30+(i%5));}
ps("\nBlueberry market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blub_market(lc,11+(i*2),7+(i*2),75+(i*5),(i%3)+3,13+(i*2),38+(i%3));}
ps("\n");blub_report();blub_state();ps("\n=== Demo Complete ===\n");return 0;}
