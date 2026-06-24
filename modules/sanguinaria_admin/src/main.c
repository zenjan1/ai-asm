/* sanguinaria_admin: Sanguinaria (Bloodroot) woodland conservation (v1.0)
 * Sanguinaria planning, execution, evaluation, rhizome propagation, seed dispersal
 * Features: rhizome tracking, alkaloid content, shade management, mycorrhizal association
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,loc,rhizome_ct,alkaloid,canopy,bloom_wk,seed_rt,mycorr,active;} san_t;
typedef struct{int n_plan,n_exec,n_eval,n_rhiz,n_seed,t_rhiz,t_alk,t_canopy,t_bloom,t_seed;} san_state_t;
static san_t sanps[N],sanes[N-2],sanvs[N-4],sanrs[N-6],sanss[N-6]; static san_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(san_t*a,int*cnt,int*sum,int mx,int lc,int rh,int al,int cn,int bw,int sr,int my){if(*cnt>=mx)return -1;san_t*x=&a[*cnt];x->id=*cnt;x->loc=lc;x->rhizome_ct=rh;x->alkaloid=al;x->canopy=cn;x->bloom_wk=bw;x->seed_rt=sr;x->mycorr=my;x->active=1;*sum+=rh;(*cnt)++;ps("[SAN] Sanguinaria ");pi(*cnt-1);ps(" loc=");pi(lc);ps(" rh=");pi(rh);ps(" al=");pi(al);ps(" cn=");pi(cn);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int san_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_rhiz=0;st.n_seed=0;st.t_rhiz=0;st.t_alk=0;st.t_canopy=0;st.t_bloom=0;st.t_seed=0;for(int i=0;i<N;i++)sanps[i].active=0;for(int i=0;i<N-2;i++)sanes[i].active=0;for(int i=0;i<N-4;i++)sanvs[i].active=0;for(int i=0;i<N-6;i++)sanrs[i].active=0;for(int i=0;i<N-6;i++)sanss[i].active=0;init=1;ps("[SAN] Sanguinaria (bloodroot) initialized\n");return 0;}
/* 1=deep_woodland 2=partial_shade 3=riparian 4=hillside 5=clearing_edge */
int san_planning(int lc,int rh,int al,int cn,int bw,int sr,int my){return add(sanps,&st.n_plan,&st.t_rhiz,N,lc,rh,al,cn,bw,sr,my);}
int san_execution(int lc,int rh,int al,int cn,int bw,int sr,int my){return add(sanes,&st.n_exec,&st.t_alk,N-2,lc,rh,al,cn,bw,sr,my);}
int san_evaluation(int lc,int rh,int al,int cn,int bw,int sr,int my){return add(sanvs,&st.n_eval,&st.t_canopy,N-4,lc,rh,al,cn,bw,sr,my);}
int san_rhizome(int lc,int rh,int al,int cn,int bw,int sr,int my){return add(sanrs,&st.n_rhiz,&st.t_bloom,N-6,lc,rh,al,cn,bw,sr,my);}
int san_seed(int lc,int rh,int al,int cn,int bw,int sr,int my){return add(sanss,&st.n_seed,&st.t_seed,N-6,lc,rh,al,cn,bw,sr,my);}
void san_report(void){ps("[SAN] Plan: ");pi(st.n_plan);ps(" rhiz=");pi(st.t_rhiz);ps("\nExec: ");pi(st.n_exec);ps(" alk=");pi(st.t_alk);ps("\nEval: ");pi(st.n_eval);ps(" canopy=");pi(st.t_canopy);ps("\nRhiz: ");pi(st.n_rhiz);ps(" bloom=");pi(st.t_bloom);ps("\nSeed: ");pi(st.n_seed);ps(" seed=");pi(st.t_seed);ps("\n");}
void san_state(void){ps("[SAN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Rhiz=");pi(st.n_rhiz);ps(" Seed=");pi(st.n_seed);ps("\n");}
int main(void){
ps("=== Sanguinaria (Bloodroot) Admin Demo ===\n\n");san_init();
ps("Sanguinaria planning (woodland survey)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;san_planning(lc,5+(i*2),35+(i*8),60+(i%30),12+(i%6),40+(i*5),(i%4)+1);}
ps("\nSanguinaria execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;san_execution(lc,7+(i*2),40+(i*7),65+(i%25),14+(i%5),45+(i*4),(i%3)+2);}
ps("\nSanguinaria evaluation (alkaloid testing)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;san_evaluation(lc,9+(i*2),45+(i*6),70+(i%20),16+(i%4),50+(i*3),(i%3)+2);}
ps("\nSanguinaria rhizome propagation...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;san_rhizome(lc,4+(i*2),30+(i*7),55+(i%28),10+(i%5),35+(i*4),1);}
ps("\nSanguinaria seed dispersal...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;san_seed(lc,6+(i*2),38+(i*6),62+(i%22),13+(i%4),42+(i*3),(i%3)+1);}
ps("\n");san_report();san_state();ps("\n=== Demo Complete ===\n");return 0;}
