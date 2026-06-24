/* zinnia_admin: Zinnia colorful annual flower and pollinator garden plant management (v1.0)
 * Zinnia planning, planting, evaluation, seed collection, market
 * Features: flower diameter, petal layers, color variety, bloom duration, heat tolerance, stem height
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_layers,color_var,bloom_dk,heat_tol,stem_ht,active;} zin_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_flower,t_petal,t_color,t_bloom,t_heat;} zin_state_t;
static zin_t zinps[N],zines[N-2],zinvs[N-4],zinsd[N-6],zinms[N-6]; static zin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zin_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pl,int cv,int bd,int ht,int sh){if(*cnt>=mx)return -1;zin_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_layers=pl;x->color_var=cv;x->bloom_dk=bd;x->heat_tol=ht;x->stem_ht=sh;x->active=1;*sum+=fd;(*cnt)++;ps("[ZIN] Zinnia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pl=");pi(pl);ps(" cv=");pi(cv);ps(" bd=");pi(bd);ps(" ht=");pi(ht);ps("\n");return *cnt-1;}
int zin_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_color=0;st.t_bloom=0;st.t_heat=0;for(int i=0;i<N;i++)zinps[i].active=0;for(int i=0;i<N-2;i++)zines[i].active=0;for(int i=0;i<N-4;i++)zinvs[i].active=0;for(int i=0;i<N-6;i++)zinsd[i].active=0;for(int i=0;i<N-6;i++)zinms[i].active=0;init=1;ps("[ZIN] Zinnia initialized\n");return 0;}
/* 1=flower_bed 2=container 3=cutting_garden 4=pollinator_garden 5=border */
int zin_planning(int lc,int fd,int pl,int cv,int bd,int ht,int sh){return add(zinps,&st.n_plan,&st.t_flower,N,lc,fd,pl,cv,bd,ht,sh);}
int zin_execution(int lc,int fd,int pl,int cv,int bd,int ht,int sh){return add(zines,&st.n_exec,&st.t_petal,N-2,lc,fd,pl,cv,bd,ht,sh);}
int zin_evaluation(int lc,int fd,int pl,int cv,int bd,int ht,int sh){return add(zinvs,&st.n_eval,&st.t_color,N-4,lc,fd,pl,cv,bd,ht,sh);}
int zin_seed_collect(int lc,int fd,int pl,int cv,int bd,int ht,int sh){return add(zinsd,&st.n_seed,&st.t_bloom,N-6,lc,fd,pl,cv,bd,ht,sh);}
int zin_market(int lc,int fd,int pl,int cv,int bd,int ht,int sh){return add(zinms,&st.n_mkt,&st.t_heat,N-6,lc,fd,pl,cv,bd,ht,sh);}
void zin_report(void){ps("[ZIN] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nSeed: ");pi(st.n_seed);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" heat=");pi(st.t_heat);ps("\n");}
void zin_state(void){ps("[ZIN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zinnia Admin Demo ===\n\n");zin_init();
ps("Zinnia planning (colorful layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;zin_planning(lc,5+(i*2),3+(i%4),(i%7)+1,30+(i*5),(i%5)+1,40+(i*8));}
ps("\nZinnia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;zin_execution(lc,6+(i*2),3+(i%3),(i%7)+1,32+(i*4),(i%5)+1,45+(i*7));}
ps("\nZinnia evaluation (color check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;zin_evaluation(lc,7+(i*2),4+(i%3),(i%6)+2,35+(i*3),(i%4)+2,50+(i*6));}
ps("\nZinnia seed collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;zin_seed_collect(lc,4+(i*2),2+(i%3),(i%5)+1,25+(i*4),(i%5)+1,35+(i*6));}
ps("\nZinnia cut flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;zin_market(lc,8+(i*2),4+(i%2),(i%7)+2,38+(i*3),(i%4)+2,55+(i*5));}
ps("\n");zin_report();zin_state();ps("\n=== Demo Complete ===\n");return 0;}
