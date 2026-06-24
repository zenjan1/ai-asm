/* camellia_admin: Camellia (Camellia japonica) ornamental tea plant management (v1.0)
 * Camellia planting, pruning, bloom, tea production, market
 * Features: bush height, bloom diameter, leaf count, petal count, seed oil, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bush_ht,bloom_dia,leaf_ct,petal_ct,seed_oil,bloom_wk,active;} cml_t;
typedef struct{int n_plant,n_prune,n_bloom,n_tea,n_mkt,t_bush,t_bloom,t_leaf,t_petal,t_seed;} cml_state_t;
static cml_t cmlps[N],cmlpr[N-2],cmlbs[N-4],cmlts[N-6],cmlms[N-6]; static cml_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cml_t*a,int*cnt,int*sum,int mx,int lc,int bh,int bd,int lct,int pct,int so,int bw){if(*cnt>=mx)return -1;cml_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bush_ht=bh;x->bloom_dia=bd;x->leaf_ct=lct;x->petal_ct=pct;x->seed_oil=so;x->bloom_wk=bw;x->active=1;*sum+=bh;(*cnt)++;ps("[CML] Cml ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bh=");pi(bh);ps(" bd=");pi(bd);ps(" lct=");pi(lct);ps(" pct=");pi(pct);ps(" so=");pi(so);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cml_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_bloom=0;st.n_tea=0;st.n_mkt=0;st.t_bush=0;st.t_bloom=0;st.t_leaf=0;st.t_petal=0;st.t_seed=0;for(int i=0;i<N;i++)cmlps[i].active=0;for(int i=0;i<N-2;i++)cmlpr[i].active=0;for(int i=0;i<N-4;i++)cmlbs[i].active=0;for(int i=0;i<N-6;i++)cmlts[i].active=0;for(int i=0;i<N-6;i++)cmlms[i].active=0;init=1;ps("[CML] Camellia initialized\n");return 0;}
int cml_planting(int lc,int bh,int bd,int lct,int pct,int so,int bw){return add(cmlps,&st.n_plant,&st.t_bush,N,lc,bh,bd,lct,pct,so,bw);}
int cml_pruning(int lc,int bh,int bd,int lct,int pct,int so,int bw){return add(cmlpr,&st.n_prune,&st.t_bloom,N-2,lc,bh,bd,lct,pct,so,bw);}
int cml_bloom(int lc,int bh,int bd,int lct,int pct,int so,int bw){return add(cmlbs,&st.n_bloom,&st.t_leaf,N-4,lc,bh,bd,lct,pct,so,bw);}
int cml_tea_production(int lc,int bh,int bd,int lct,int pct,int so,int bw){return add(cmlts,&st.n_tea,&st.t_petal,N-6,lc,bh,bd,lct,pct,so,bw);}
int cml_market(int lc,int bh,int bd,int lct,int pct,int so,int bw){return add(cmlms,&st.n_mkt,&st.t_seed,N-6,lc,bh,bd,lct,pct,so,bw);}
void cml_report(void){ps("[CML] Plant: ");pi(st.n_plant);ps(" Bush=");pi(st.t_bush);ps("\nPrune: ");pi(st.n_prune);ps(" Bloom=");pi(st.t_bloom);ps("\nBloom: ");pi(st.n_bloom);ps(" Leaf=");pi(st.t_leaf);ps("\nTea: ");pi(st.n_tea);ps(" Petal=");pi(st.t_petal);ps("\nMkt: ");pi(st.n_mkt);ps(" Seed=");pi(st.t_seed);ps("\n");}
void cml_state(void){ps("[CML] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Bloom=");pi(st.n_bloom);ps(" Tea=");pi(st.n_tea);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Camellia Admin Demo ===\n\n");cml_init();
/* 1=tea_garden 2=ornamental 3=greenhouse 4=hedge 5=container */
ps("Camellia planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bh=50+(i*10),bd=4+(i*2),lct=20+(i*8),pct=8+(i%6),so=5+(i*3),bw=12+(i%8);cml_planting(lc,bh,bd,lct,pct,so,bw);}
ps("\nCamellia pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bh=55+(i*8),bd=5+(i*2),lct=25+(i*6),pct=10+(i%5),so=6+(i*2),bw=14+(i%6);cml_pruning(lc,bh,bd,lct,pct,so,bw);}
ps("\nCamellia bloom...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bh=60+(i*6),bd=6+(i*2),lct=30+(i*5),pct=12+(i%4),so=7+(i*2),bw=16+(i%4);cml_bloom(lc,bh,bd,lct,pct,so,bw);}
ps("\nCamellia tea_production...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bh=45+(i*12),bd=3+(i*3),lct=15+(i*10),pct=6+(i%7),so=4+(i*4),bw=10+(i%9);cml_tea_production(lc,bh,bd,lct,pct,so,bw);}
ps("\nCamellia market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bh=65+(i*5),bd=7+(i*2),lct=35+(i*4),pct=14+(i%3),so=8+(i*2),bw=18+(i%3);cml_market(lc,bh,bd,lct,pct,so,bw);}
ps("\n");cml_report();cml_state();ps("\n=== Demo Complete ===\n");return 0;}
