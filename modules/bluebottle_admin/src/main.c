/* bluebottle_admin: Bluebottle (Centaurea cyanus) cornflower meadow management (v1.0)
 * Bluebottle planning, planting, evaluation, seed collection, market
 * Features: flower diameter, petal count, stem height, flower color, seed count, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,stem_ht,flower_color,seed_ct,bloom_wk,active;} blbt_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_petal,t_stem,t_color,t_seed;} blbt_state_t;
static blbt_t blbtps[N],blbtes[N-2],blbtvs[N-4],blbtsd[N-6],blbtms[N-6]; static blbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(blbt_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int sh,int fc,int sc,int bw){if(*cnt>=mx)return -1;blbt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->stem_ht=sh;x->flower_color=fc;x->seed_ct=sc;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[BLBT] Bluebottle ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" sc=");pi(sc);ps("\n");return *cnt-1;}
int blbt_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_color=0;st.t_seed=0;for(int i=0;i<N;i++)blbtps[i].active=0;for(int i=0;i<N-2;i++)blbtes[i].active=0;for(int i=0;i<N-4;i++)blbtvs[i].active=0;for(int i=0;i<N-6;i++)blbtsd[i].active=0;for(int i=0;i<N-6;i++)blbtms[i].active=0;init=1;ps("[BLBT] Bluebottle initialized\n");return 0;}
/* 1=meadow 2=cornfield 3=wildflower_patch 4=roadsides 5=garden_border */
int blbt_planning(int lc,int fd,int pc,int sh,int fc,int sc,int bw){return add(blbtps,&st.n_plan,&st.t_flower,N,lc,fd,pc,sh,fc,sc,bw);}
int blbt_execution(int lc,int fd,int pc,int sh,int fc,int sc,int bw){return add(blbtes,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,sh,fc,sc,bw);}
int blbt_evaluation(int lc,int fd,int pc,int sh,int fc,int sc,int bw){return add(blbtvs,&st.n_eval,&st.t_stem,N-4,lc,fd,pc,sh,fc,sc,bw);}
int blbt_seed_collection(int lc,int fd,int pc,int sh,int fc,int sc,int bw){return add(blbtsd,&st.n_seed,&st.t_color,N-6,lc,fd,pc,sh,fc,sc,bw);}
int blbt_market(int lc,int fd,int pc,int sh,int fc,int sc,int bw){return add(blbtms,&st.n_mkt,&st.t_seed,N-6,lc,fd,pc,sh,fc,sc,bw);}
void blbt_report(void){ps("[BLBT] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nSeed: ");pi(st.n_seed);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void blbt_state(void){ps("[BLBT] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bluebottle (Centaurea) Admin Demo ===\n\n");blbt_init();
ps("Bluebottle planning (meadow layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;blbt_planning(lc,2+(i*2),8+(i*3),30+(i*5),(i%4)+1,15+(i*8),24+(i%6));}
ps("\nBluebottle execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;blbt_execution(lc,3+(i*2),10+(i*2),35+(i*4),(i%4)+1,18+(i*6),26+(i%5));}
ps("\nBluebottle evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;blbt_evaluation(lc,4+(i*2),12+(i*2),40+(i*3),(i%3)+2,20+(i*5),28+(i%4));}
ps("\nBluebottle seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;blbt_seed_collection(lc,2+(i*2),6+(i*3),25+(i*5),(i%4)+1,12+(i*7),22+(i%5));}
ps("\nBluebottle meadow market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;blbt_market(lc,5+(i*2),14+(i*2),45+(i*3),(i%3)+3,22+(i*4),30+(i%3));}
ps("\n");blbt_report();blbt_state();ps("\n=== Demo Complete ===\n");return 0;}
