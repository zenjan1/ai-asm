/* currant_admin: Currant (Ribes nigrum) berry bush cultivation (v1.0)
 * Currant planting, pruning, pollination, harvest, market
 * Features: bush_ht_cm, berry_cluster, fruit_wt_g, sugar_brix, vitamin_c_mg, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bush_ht,brry_cl,frt_wt,sugar_bx,vit_c,harv_wk,active;} curr_t;
typedef struct{int n_plant,n_prune,n_poll,n_harv,n_mkt,t_ht,t_brry,t_wt,t_sugar,t_vit;} curr_state_t;
static curr_t currps[N],currds[N-2],currpl[N-4],currh[N-6],currm[N-6]; static curr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(curr_t*a,int*cnt,int*sum,int mx,int lc,int bh,int bc,int fw,int sb,int vc,int hw){if(*cnt>=mx)return -1;curr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bush_ht=bh;x->brry_cl=bc;x->frt_wt=fw;x->sugar_bx=sb;x->vit_c=vc;x->harv_wk=hw;x->active=1;*sum+=bh;(*cnt)++;ps("[CURR] Currant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bh=");pi(bh);ps(" bc=");pi(bc);ps(" fw=");pi(fw);ps(" sb=");pi(sb);ps(" vc=");pi(vc);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int curr_init(void){if(init)return -1;st.n_plant=0;st.n_prune=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_brry=0;st.t_wt=0;st.t_sugar=0;st.t_vit=0;for(int i=0;i<N;i++)currps[i].active=0;for(int i=0;i<N-2;i++)currds[i].active=0;for(int i=0;i<N-4;i++)currpl[i].active=0;for(int i=0;i<N-6;i++)currh[i].active=0;for(int i=0;i<N-6;i++)currm[i].active=0;init=1;ps("[CURR] Currant initialized\n");return 0;}
int curr_planting(int lc,int bh,int bc,int fw,int sb,int vc,int hw){return add(currps,&st.n_plant,&st.t_ht,N,lc,bh,bc,fw,sb,vc,hw);}
int curr_pruning(int lc,int bh,int bc,int fw,int sb,int vc,int hw){return add(currds,&st.n_prune,&st.t_brry,N-2,lc,bh,bc,fw,sb,vc,hw);}
int curr_pollination(int lc,int bh,int bc,int fw,int sb,int vc,int hw){return add(currpl,&st.n_poll,&st.t_wt,N-4,lc,bh,bc,fw,sb,vc,hw);}
int curr_harvest(int lc,int bh,int bc,int fw,int sb,int vc,int hw){return add(currh,&st.n_harv,&st.t_sugar,N-6,lc,bh,bc,fw,sb,vc,hw);}
int curr_market(int lc,int bh,int bc,int fw,int sb,int vc,int hw){return add(currm,&st.n_mkt,&st.t_vit,N-6,lc,bh,bc,fw,sb,vc,hw);}
void curr_report(void){ps("[CURR] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" Brry=");pi(st.t_brry);ps("\nPoll: ");pi(st.n_poll);ps(" Wt=");pi(st.t_wt);ps("\nHarv: ");pi(st.n_harv);ps(" Sugar=");pi(st.t_sugar);ps("\nMkt: ");pi(st.n_mkt);ps(" Vit=");pi(st.t_vit);ps("\n");}
void curr_state(void){ps("[CURR] Plant=");pi(st.n_plant);ps(" Prune=");pi(st.n_prune);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Currant Admin Demo ===\n\n");curr_init();
/* 1=garden 2=orchard 3=hedgerow 4=organic 5=market */
ps("Currant planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bh=80+(i*8),bc=5+(i*3),fw=3+(i*2),sb=8+(i*2),vc=100+(i*30),hw=14+(i%6);curr_planting(lc,bh,bc,fw,sb,vc,hw);}
ps("\nCurrant pruning...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bh=85+(i*7),bc=6+(i*2),fw=3+(i*2),sb=9+(i*2),vc=110+(i*25),hw=15+(i%5);curr_pruning(lc,bh,bc,fw,sb,vc,hw);}
ps("\nCurrant pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bh=90+(i*6),bc=7+(i*2),fw=4+(i*2),sb=10+(i*2),vc=120+(i*20),hw=16+(i%4);curr_pollination(lc,bh,bc,fw,sb,vc,hw);}
ps("\nCurrant harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bh=75+(i*10),bc=4+(i*4),fw=2+(i*3),sb=7+(i*3),vc=90+(i*35),hw=12+(i%7);curr_harvest(lc,bh,bc,fw,sb,vc,hw);}
ps("\nCurrant market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bh=95+(i*5),bc=8+(i*2),fw=5+(i*2),sb=11+(i*2),vc=130+(i*15),hw=17+(i%3);curr_market(lc,bh,bc,fw,sb,vc,hw);}
ps("\n");curr_report();curr_state();ps("\n=== Demo Complete ===\n");return 0;}
