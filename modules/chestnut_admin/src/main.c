/* chestnut_admin: Chestnut (Castanea sativa) nut tree management (v1.0)
 * Chestnut planting, cultivation, roasting, storage, market
 * Features: tree_height_m, nut_yield_kg, burr_count, nut_weight_g, starch_pct, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,nut_yd,burr_ct,nut_wt,starch_pct,harv_mo,active;} chst_t;
typedef struct{int n_plant,n_cult,n_roast,n_store,n_mkt,t_tree,t_nut,t_burr,t_wt,t_starch;} chst_state_t;
static chst_t chstps[N],chstcs[N-2],chstrs[N-4],chstss[N-6],chstms[N-6]; static chst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chst_t*a,int*cnt,int*sum,int mx,int lc,int th,int ny,int bc,int nw,int sp,int hm){if(*cnt>=mx)return -1;chst_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->nut_yd=ny;x->burr_ct=bc;x->nut_wt=nw;x->starch_pct=sp;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CHST] Chestnut ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" ny=");pi(ny);ps(" bc=");pi(bc);ps(" nw=");pi(nw);ps(" sp=");pi(sp);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int chst_init(void){if(init)return -1;st.n_plant=0;st.n_cult=0;st.n_roast=0;st.n_store=0;st.n_mkt=0;st.t_tree=0;st.t_nut=0;st.t_burr=0;st.t_wt=0;st.t_starch=0;for(int i=0;i<N;i++)chstps[i].active=0;for(int i=0;i<N-2;i++)chstcs[i].active=0;for(int i=0;i<N-4;i++)chstrs[i].active=0;for(int i=0;i<N-6;i++)chstss[i].active=0;for(int i=0;i<N-6;i++)chstms[i].active=0;init=1;ps("[CHST] Chestnut initialized\n");return 0;}
int chst_planting(int lc,int th,int ny,int bc,int nw,int sp,int hm){return add(chstps,&st.n_plant,&st.t_tree,N,lc,th,ny,bc,nw,sp,hm);}
int chst_cultivation(int lc,int th,int ny,int bc,int nw,int sp,int hm){return add(chstcs,&st.n_cult,&st.t_nut,N-2,lc,th,ny,bc,nw,sp,hm);}
int chst_roasting(int lc,int th,int ny,int bc,int nw,int sp,int hm){return add(chstrs,&st.n_roast,&st.t_burr,N-4,lc,th,ny,bc,nw,sp,hm);}
int chst_storage(int lc,int th,int ny,int bc,int nw,int sp,int hm){return add(chstss,&st.n_store,&st.t_wt,N-6,lc,th,ny,bc,nw,sp,hm);}
int chst_market(int lc,int th,int ny,int bc,int nw,int sp,int hm){return add(chstms,&st.n_mkt,&st.t_starch,N-6,lc,th,ny,bc,nw,sp,hm);}
void chst_report(void){ps("[CHST] Plant: ");pi(st.n_plant);ps(" Tree=");pi(st.t_tree);ps("\nCult: ");pi(st.n_cult);ps(" Nut=");pi(st.t_nut);ps("\nRoast: ");pi(st.n_roast);ps(" Burr=");pi(st.t_burr);ps("\nStore: ");pi(st.n_store);ps(" Wt=");pi(st.t_wt);ps("\nMkt: ");pi(st.n_mkt);ps(" Starch=");pi(st.t_starch);ps("\n");}
void chst_state(void){ps("[CHST] Plant=");pi(st.n_plant);ps(" Cult=");pi(st.n_cult);ps(" Roast=");pi(st.n_roast);ps(" Store=");pi(st.n_store);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Chestnut Admin Demo ===\n\n");chst_init();
/* 1=orchard 2=forest 3=grove 4=plantation 5=wild */
ps("Chestnut planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=5+(i%8),ny=8+(i*4),bc=20+(i*10),nw=10+(i*3),sp=30+(i*3),hm=9+(i%4);chst_planting(lc,th,ny,bc,nw,sp,hm);}
ps("\nChestnut cultivation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=6+(i%6),ny=12+(i*3),bc=25+(i*8),nw=12+(i*2),sp=35+(i%3),hm=10+(i%3);chst_cultivation(lc,th,ny,bc,nw,sp,hm);}
ps("\nChestnut roasting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=7+(i%5),ny=15+(i*3),bc=30+(i%6),nw=14+(i%2),sp=40+(i%2),hm=11+(i%2);chst_roasting(lc,th,ny,bc,nw,sp,hm);}
ps("\nChestnut storage...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=4+(i%9),ny=6+(i*5),bc=15+(i*12),nw=8+(i*4),sp=25+(i*4),hm=8+(i%5);chst_storage(lc,th,ny,bc,nw,sp,hm);}
ps("\nChestnut market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=8+(i%4),ny=18+(i*3),bc=35+(i%5),nw=16+(i%2),sp=45+(i%2),hm=12+(i%1);chst_market(lc,th,ny,bc,nw,sp,hm);}
ps("\n");chst_report();chst_state();ps("\n=== Demo Complete ===\n");return 0;}
