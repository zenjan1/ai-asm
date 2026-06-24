/* cassava_admin: Cassava (Manihot esculenta) tuber crop management (v1.0)
 * Cassava planting, cultivation, processing, storage, market
 * Features: root weight kg, plant height cm, starch pct, cyanide ppm, stem cuttings, harvest month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,root_wt,plnt_ht,starch_pct,cyan_ppm,stm_ct,harv_mo,active;} cass_t;
typedef struct{int n_plant,n_cult,n_proc,n_store,n_mkt,t_root,t_ht,t_starch,t_cyan,t_stm;} cass_state_t;
static cass_t cassps[N],casscs[N-2],cassprs[N-4],cassss[N-6],cassms[N-6]; static cass_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cass_t*a,int*cnt,int*sum,int mx,int lc,int rw,int ph,int sp,int cp,int sc,int hm){if(*cnt>=mx)return -1;cass_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->root_wt=rw;x->plnt_ht=ph;x->starch_pct=sp;x->cyan_ppm=cp;x->stm_ct=sc;x->harv_mo=hm;x->active=1;*sum+=rw;(*cnt)++;ps("[CASS] Cassava ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" rw=");pi(rw);ps(" ph=");pi(ph);ps(" sp=");pi(sp);ps(" cp=");pi(cp);ps(" sc=");pi(sc);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int cass_init(void){if(init)return -1;st.n_plant=0;st.n_cult=0;st.n_proc=0;st.n_store=0;st.n_mkt=0;st.t_root=0;st.t_ht=0;st.t_starch=0;st.t_cyan=0;st.t_stm=0;for(int i=0;i<N;i++)cassps[i].active=0;for(int i=0;i<N-2;i++)casscs[i].active=0;for(int i=0;i<N-4;i++)cassprs[i].active=0;for(int i=0;i<N-6;i++)cassss[i].active=0;for(int i=0;i<N-6;i++)cassms[i].active=0;init=1;ps("[CASS] Cassava initialized\n");return 0;}
int cass_planting(int lc,int rw,int ph,int sp,int cp,int sc,int hm){return add(cassps,&st.n_plant,&st.t_root,N,lc,rw,ph,sp,cp,sc,hm);}
int cass_cultivation(int lc,int rw,int ph,int sp,int cp,int sc,int hm){return add(casscs,&st.n_cult,&st.t_ht,N-2,lc,rw,ph,sp,cp,sc,hm);}
int cass_processing(int lc,int rw,int ph,int sp,int cp,int sc,int hm){return add(cassprs,&st.n_proc,&st.t_starch,N-4,lc,rw,ph,sp,cp,sc,hm);}
int cass_storage(int lc,int rw,int ph,int sp,int cp,int sc,int hm){return add(cassss,&st.n_store,&st.t_cyan,N-6,lc,rw,ph,sp,cp,sc,hm);}
int cass_market(int lc,int rw,int ph,int sp,int cp,int sc,int hm){return add(cassms,&st.n_mkt,&st.t_stm,N-6,lc,rw,ph,sp,cp,sc,hm);}
void cass_report(void){ps("[CASS] Plant: ");pi(st.n_plant);ps(" Root=");pi(st.t_root);ps("\nCult: ");pi(st.n_cult);ps(" Ht=");pi(st.t_ht);ps("\nProc: ");pi(st.n_proc);ps(" Starch=");pi(st.t_starch);ps("\nStore: ");pi(st.n_store);ps(" Cyan=");pi(st.t_cyan);ps("\nMkt: ");pi(st.n_mkt);ps(" Stm=");pi(st.t_stm);ps("\n");}
void cass_state(void){ps("[CASS] Plant=");pi(st.n_plant);ps(" Cult=");pi(st.n_cult);ps(" Proc=");pi(st.n_proc);ps(" Store=");pi(st.n_store);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cassava Admin Demo ===\n\n");cass_init();
/* 1=tropical_field 2=farm 3=garden 4=plantation 5=subsistence */
ps("Cassava planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,rw=2+(i*1),ph=80+(i*10),sp=15+(i%5),cp=20+(i*10),sc=3+(i%2),hm=8+(i%2);cass_planting(lc,rw,ph,sp,cp,sc,hm);}
ps("\nCassava cultivation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,rw=3+(i*1),ph=90+(i*8),sp=18+(i%4),cp=25+(i*8),sc=4+(i%2),hm=9+(i%2);cass_cultivation(lc,rw,ph,sp,cp,sc,hm);}
ps("\nCassava processing...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,rw=4+(i*1),ph=100+(i*6),sp=20+(i%3),cp=30+(i*6),sc=5+(i%2),hm=10+(i%1);cass_processing(lc,rw,ph,sp,cp,sc,hm);}
ps("\nCassava storage...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,rw=1+(i*2),ph=70+(i*12),sp=12+(i%6),cp=15+(i*12),sc=2+(i%3),hm=7+(i%2);cass_storage(lc,rw,ph,sp,cp,sc,hm);}
ps("\nCassava market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,rw=5+(i*1),ph=110+(i*5),sp=22+(i%3),cp=35+(i*5),sc=6+(i%2),hm=11+(i%1);cass_market(lc,rw,ph,sp,cp,sc,hm);}
ps("\n");cass_report();cass_state();ps("\n=== Demo Complete ===\n");return 0;}
