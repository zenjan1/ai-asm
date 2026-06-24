/* clove_admin: Clove (Syzygium aromaticum) spice tree management (v1.0)
 * Clove planting, pruning, drying, harvest, market
 * Features: tree_height_m, bud_count, bud_len_mm, oil_pct, dry_wt_g, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,bud_ct,bud_ln,oil_pct,dry_wt,harv_mo,active;} clv_t;
typedef struct{int n_plant,n_prune,n_dry,n_harv,n_mkt,t_ht,t_bud,t_ln,t_oil,t_dry;} clv_state_t;
static clv_t clvps[N],clvds[N-2],clvdr[N-4],clvhs[N-6],clvms[N-6]; static clv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clv_t*a,int*cnt,int*sum,int mx,int lc,int th,int bc,int bl,int op,int dw,int hm){if(*cnt>=mx)return -1;clv_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->bud_ct=bc;x->bud_ln=bl;x->oil_pct=op;x->dry_wt=dw;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CLV] Clove ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" bc=");pi(bc);ps(" bl=");pi(bl);ps(" op=");pi(op);ps(" dw=");pi(dw);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int clv_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_dry=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_bud=0;st.t_ln=0;st.t_oil=0;st.t_dry=0;for(int i=0;i<N;i++)clvps[i].active=0;for(int i=0;i<N-2;i++)clvds[i].active=0;for(int i=0;i<N-4;i++)clvdr[i].active=0;for(int i=0;i<N-6;i++)clvhs[i].active=0;for(int i=0;i<N-6;i++)clvms[i].active=0;init=1;ps("[CLV] Clove initialized\n");return 0;}
int clv_planting(int lc,int th,int bc,int bl,int op,int dw,int hm){return add(clvps,&st.n_plant,&st.t_ht,N,lc,th,bc,bl,op,dw,hm);}
int clv_pruning(int lc,int th,int bc,int bl,int op,int dw,int hm){return add(clvds,&st.n_prune,&st.t_bud,N-2,lc,th,bc,bl,op,dw,hm);}
int clv_drying(int lc,int th,int bc,int bl,int op,int dw,int hm){return add(clvdr,&st.n_dry,&st.t_ln,N-4,lc,th,bc,bl,op,dw,hm);}
int clv_harvest(int lc,int th,int bc,int bl,int op,int dw,int hm){return add(clvhs,&st.n_harv,&st.t_oil,N-6,lc,th,bc,bl,op,dw,hm);}
int clv_market(int lc,int th,int bc,int bl,int op,int dw,int hm){return add(clvms,&st.n_mkt,&st.t_dry,N-6,lc,th,bc,bl,op,dw,hm);}
void clv_report(void){ps("[CLV] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Bud=");pi(st.t_bud);ps("\nDry: ");pi(st.n_dry);ps(" Ln=");pi(st.t_ln);ps("\nHarv: ");pi(st.n_harv);ps(" Oil=");pi(st.t_oil);ps("\nMkt: ");pi(st.n_mkt);ps(" Dry=");pi(st.t_dry);ps("\n");}
void clv_state(void){ps("[CLV] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Dry=");pi(st.n_dry);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Clove Admin Demo ===\n\n");clv_init();
/* 1=orchard 2=plantation 3=greenhouse 4=terrace 5=market */
ps("Clove planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=3+(i*2),bc=50+(i*20),bl=10+(i*3),op=5+(i*2),dw=20+(i*5),hm=10+(i%4);clv_planting(lc,th,bc,bl,op,dw,hm);}
ps("\nClove pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=4+(i*2),bc=60+(i*15),bl=12+(i*2),op=6+(i*2),dw=22+(i*4),hm=11+(i%3);clv_pruning(lc,th,bc,bl,op,dw,hm);}
ps("\nClove drying...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=5+(i*2),bc=70+(i*12),bl=14+(i*2),op=7+(i*2),dw=24+(i*3),hm=12+(i%3);clv_drying(lc,th,bc,bl,op,dw,hm);}
ps("\nClove harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=2+(i*3),bc=40+(i*25),bl=8+(i*4),op=4+(i*3),dw=18+(i*6),hm=9+(i%5);clv_harvest(lc,th,bc,bl,op,dw,hm);}
ps("\nClove market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=6+(i*2),bc=80+(i*10),bl=16+(i*2),op=8+(i*2),dw=26+(i*3),hm=1+(i%3);clv_market(lc,th,bc,bl,op,dw,hm);}
ps("\n");clv_report();clv_state();ps("\n=== Demo Complete ===\n");return 0;}
