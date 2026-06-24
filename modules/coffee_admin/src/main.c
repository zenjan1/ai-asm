/* coffee_admin: Coffee (Coffea arabica) coffee tree management (v1.0)
 * Coffee planting, pruning, roasting, harvest, market
 * Features: tree_height_m, cherry_count, bean_wt_g, caffeine_mg, roast_idx, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,chry_ct,bean_wt,caf_mg,rost_idx,harv_mo,active;} cfee_t;
typedef struct{int n_plant,n_prune,n_roast,n_harv,n_mkt,t_ht,t_chry,t_bean,t_caf,t_rost;} cfee_state_t;
static cfee_t cfeps[N],cfers[N-2],cfrts[N-4],cfehs[N-6],cfems[N-6]; static cfee_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cfee_t*a,int*cnt,int*sum,int mx,int lc,int th,int cc,int bw,int cm,int ri,int hm){if(*cnt>=mx)return -1;cfee_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->chry_ct=cc;x->bean_wt=bw;x->caf_mg=cm;x->rost_idx=ri;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CFEE] Coffee ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" cc=");pi(cc);ps(" bw=");pi(bw);ps(" cm=");pi(cm);ps(" ri=");pi(ri);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int cfee_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_roast=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_chry=0;st.t_bean=0;st.t_caf=0;st.t_rost=0;for(int i=0;i<N;i++)cfeps[i].active=0;for(int i=0;i<N-2;i++)cfers[i].active=0;for(int i=0;i<N-4;i++)cfrts[i].active=0;for(int i=0;i<N-6;i++)cfehs[i].active=0;for(int i=0;i<N-6;i++)cfems[i].active=0;init=1;ps("[CFEE] Coffee initialized\n");return 0;}
int cfee_planting(int lc,int th,int cc,int bw,int cm,int ri,int hm){return add(cfeps,&st.n_plant,&st.t_ht,N,lc,th,cc,bw,cm,ri,hm);}
int cfee_pruning(int lc,int th,int cc,int bw,int cm,int ri,int hm){return add(cfers,&st.n_prune,&st.t_chry,N-2,lc,th,cc,bw,cm,ri,hm);}
int cfee_roasting(int lc,int th,int cc,int bw,int cm,int ri,int hm){return add(cfrts,&st.n_roast,&st.t_bean,N-4,lc,th,cc,bw,cm,ri,hm);}
int cfee_harvest(int lc,int th,int cc,int bw,int cm,int ri,int hm){return add(cfehs,&st.n_harv,&st.t_caf,N-6,lc,th,cc,bw,cm,ri,hm);}
int cfee_market(int lc,int th,int cc,int bw,int cm,int ri,int hm){return add(cfems,&st.n_mkt,&st.t_rost,N-6,lc,th,cc,bw,cm,ri,hm);}
void cfee_report(void){ps("[CFEE] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Chry=");pi(st.t_chry);ps("\nRoast: ");pi(st.n_roast);ps(" Bean=");pi(st.t_bean);ps("\nHarv: ");pi(st.n_harv);ps(" Caf=");pi(st.t_caf);ps("\nMkt: ");pi(st.n_mkt);ps(" Rost=");pi(st.t_rost);ps("\n");}
void cfee_state(void){ps("[CFEE] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Roast=");pi(st.n_roast);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coffee Admin Demo ===\n\n");cfee_init();
/* 1=plantation 2=orchard 3=greenhouse 4=terrace 5=market */
ps("Coffee planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=2+(i*2),cc=100+(i*30),bw=1+(i%3),cm=80+(i*20),ri=(i%5)+1,hm=10+(i%4);cfee_planting(lc,th,cc,bw,cm,ri,hm);}
ps("\nCoffee pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=3+(i*2),cc=110+(i*25),bw=1+(i%2),cm=85+(i*18),ri=(i%4)+1,hm=11+(i%3);cfee_pruning(lc,th,cc,bw,cm,ri,hm);}
ps("\nCoffee roasting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=4+(i*2),cc=120+(i*20),bw=2+(i%2),cm=90+(i*15),ri=(i%3)+1,hm=12+(i%2);cfee_roasting(lc,th,cc,bw,cm,ri,hm);}
ps("\nCoffee harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=1+(i*3),cc=80+(i*35),bw=1+(i%4),cm=70+(i*25),ri=(i%6)+1,hm=9+(i%5);cfee_harvest(lc,th,cc,bw,cm,ri,hm);}
ps("\nCoffee market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=5+(i*2),cc=130+(i*18),bw=2+(i%2),cm=95+(i*12),ri=(i%3)+3,hm=1+(i%3);cfee_market(lc,th,cc,bw,cm,ri,hm);}
ps("\n");cfee_report();cfee_state();ps("\n=== Demo Complete ===\n");return 0;}
