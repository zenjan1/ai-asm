/* azalea_admin: Azalea (Flowering Shrub) woodland garden management (v1.0)
 * Azalea planning, planting, evaluation, pruning, market
 * Features: flower diameter, petal count, bloom color, leaf length, acid need, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_ct,bloom_color,leaf_len,acid_nd,bloom_wk,active;} aza_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_flower,t_petal,t_bloom,t_leaf,t_acid;} aza_state_t;
static aza_t azaps[N],azaes[N-2],azavs[N-4],azapr[N-6],azams[N-6]; static aza_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aza_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pc,int bc,int ll,int an,int bw){if(*cnt>=mx)return -1;aza_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_ct=pc;x->bloom_color=bc;x->leaf_len=ll;x->acid_nd=an;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[AZA] Azalea ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pc=");pi(pc);ps(" bc=");pi(bc);ps(" ll=");pi(ll);ps(" an=");pi(an);ps("\n");return *cnt-1;}
int aza_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_bloom=0;st.t_leaf=0;st.t_acid=0;for(int i=0;i<N;i++)azaps[i].active=0;for(int i=0;i<N-2;i++)azaes[i].active=0;for(int i=0;i<N-4;i++)azavs[i].active=0;for(int i=0;i<N-6;i++)azapr[i].active=0;for(int i=0;i<N-6;i++)azams[i].active=0;init=1;ps("[AZA] Azalea initialized\n");return 0;}
/* 1=woodland 2=shade_garden 3=container 4=patio 5=acid_bed */
int aza_planning(int lc,int fd,int pc,int bc,int ll,int an,int bw){return add(azaps,&st.n_plan,&st.t_flower,N,lc,fd,pc,bc,ll,an,bw);}
int aza_execution(int lc,int fd,int pc,int bc,int ll,int an,int bw){return add(azaes,&st.n_exec,&st.t_petal,N-2,lc,fd,pc,bc,ll,an,bw);}
int aza_evaluation(int lc,int fd,int pc,int bc,int ll,int an,int bw){return add(azavs,&st.n_eval,&st.t_bloom,N-4,lc,fd,pc,bc,ll,an,bw);}
int aza_pruning(int lc,int fd,int pc,int bc,int ll,int an,int bw){return add(azapr,&st.n_prune,&st.t_leaf,N-6,lc,fd,pc,bc,ll,an,bw);}
int aza_market(int lc,int fd,int pc,int bc,int ll,int an,int bw){return add(azams,&st.n_mkt,&st.t_acid,N-6,lc,fd,pc,bc,ll,an,bw);}
void aza_report(void){ps("[AZA] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" bloom=");pi(st.t_bloom);ps("\nPrune: ");pi(st.n_prune);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" acid=");pi(st.t_acid);ps("\n");}
void aza_state(void){ps("[AZA] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Azalea (Flowering Shrub) Admin Demo ===\n\n");aza_init();
ps("Azalea planning (woodland layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aza_planning(lc,6+(i*3),5+(i%6),(i%7)+1,8+(i*2),45+(i*5),18+(i%6));}
ps("\nAzalea execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aza_execution(lc,8+(i*2),6+(i%5),(i%7)+1,10+(i*2),48+(i*4),20+(i%5));}
ps("\nAzalea evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aza_evaluation(lc,10+(i*2),7+(i%4),(i%6)+2,12+(i*2),50+(i*3),22+(i%4));}
ps("\nAzalea pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aza_pruning(lc,5+(i*3),4+(i%5),(i%5)+1,7+(i*2),42+(i*5),16+(i%5));}
ps("\nAzalea flowering shrub market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aza_market(lc,12+(i*2),8+(i%3),(i%6)+2,14+(i*2),55+(i*3),24+(i%3));}
ps("\n");aza_report();aza_state();ps("\n=== Demo Complete ===\n");return 0;}
