/* anagallis_admin: Anagallis (Pimpernel) small flowering herb management (v1.0)
 * Anagallis planning, planting, evaluation, seed collection, market
 * Features: petal count, flower color, leaf size, stem spread, bloom week, seed capsule
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,petal_ct,flower_color,leaf_sz,stem_spread,bloom_wk,seed_cap,active;} ang_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_petal,t_color,t_leaf,t_spread,t_seed;} ang_state_t;
static ang_t angps[N],angss[N-2],angvs[N-4],angsd[N-6],angms[N-6]; static ang_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ang_t*a,int*cnt,int*sum,int mx,int lc,int pc,int fc,int ls,int ss,int bw,int sc){if(*cnt>=mx)return -1;ang_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->petal_ct=pc;x->flower_color=fc;x->leaf_sz=ls;x->stem_spread=ss;x->bloom_wk=bw;x->seed_cap=sc;x->active=1;*sum+=pc;(*cnt)++;ps("[ANG] Anagallis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" pc=");pi(pc);ps(" fc=");pi(fc);ps(" ls=");pi(ls);ps(" ss=");pi(ss);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int ang_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_petal=0;st.t_color=0;st.t_leaf=0;st.t_spread=0;st.t_seed=0;for(int i=0;i<N;i++)angps[i].active=0;for(int i=0;i<N-2;i++)angss[i].active=0;for(int i=0;i<N-4;i++)angvs[i].active=0;for(int i=0;i<N-6;i++)angsd[i].active=0;for(int i=0;i<N-6;i++)angms[i].active=0;init=1;ps("[ANG] Anagallis initialized\n");return 0;}
/* 1=meadow 2=rockery 3=border 4=container 5=wildflower */
int ang_planning(int lc,int pc,int fc,int ls,int ss,int bw,int sc){return add(angps,&st.n_plan,&st.t_petal,N,lc,pc,fc,ls,ss,bw,sc);}
int ang_execution(int lc,int pc,int fc,int ls,int ss,int bw,int sc){return add(angss,&st.n_exec,&st.t_color,N-2,lc,pc,fc,ls,ss,bw,sc);}
int ang_evaluation(int lc,int pc,int fc,int ls,int ss,int bw,int sc){return add(angvs,&st.n_eval,&st.t_leaf,N-4,lc,pc,fc,ls,ss,bw,sc);}
int ang_seed_collection(int lc,int pc,int fc,int ls,int ss,int bw,int sc){return add(angsd,&st.n_seed,&st.t_spread,N-6,lc,pc,fc,ls,ss,bw,sc);}
int ang_market(int lc,int pc,int fc,int ls,int ss,int bw,int sc){return add(angms,&st.n_mkt,&st.t_seed,N-6,lc,pc,fc,ls,ss,bw,sc);}
void ang_report(void){ps("[ANG] Plan: ");pi(st.n_plan);ps(" petal=");pi(st.t_petal);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nSeed: ");pi(st.n_seed);ps(" spread=");pi(st.t_spread);ps("\nMkt: ");pi(st.n_mkt);ps(" seed=");pi(st.t_seed);ps("\n");}
void ang_state(void){ps("[ANG] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anagallis (Pimpernel) Admin Demo ===\n\n");ang_init();
ps("Anagallis planning (herb layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ang_planning(lc,5+(i%3),(i%7)+1,8+(i*2),15+(i*3),14+(i%8),20+(i*5));}
ps("\nAnagallis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ang_execution(lc,6+(i%3),(i%7)+1,10+(i*2),18+(i*2),16+(i%6),25+(i*4));}
ps("\nAnagallis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ang_evaluation(lc,7+(i%2),(i%6)+2,12+(i*2),20+(i*2),18+(i%5),30+(i*3));}
ps("\nAnagallis seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ang_seed_collection(lc,4+(i%3),(i%5)+1,6+(i*2),12+(i*3),12+(i%7),15+(i*4));}
ps("\nAnagallis herb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ang_market(lc,8+(i%2),(i%6)+2,14+(i*2),22+(i*2),20+(i%4),35+(i*3));}
ps("\n");ang_report();ang_state();ps("\n=== Demo Complete ===\n");return 0;}
