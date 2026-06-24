/* cumin_admin: Cumin (Cuminum cyminum) spice seed crop (v1.0)
 * Cumin planting, irrigation, drying, harvest, market
 * Features: plant_ht_cm, seed_yield_g, umbel_count, oil_pct, flavor_idx, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,seed_yd,umb_ct,oil_pct,flv_idx,harv_wk,active;} cumn_t;
typedef struct{int n_plant,n_irrig,n_dry,n_harv,n_mkt,t_ht,t_seed,t_umb,t_oil,t_flv;} cumn_state_t;
static cumn_t cumnps[N],cumni[N-2],cumnd[N-4],cumnh[N-6],cumnm[N-6]; static cumn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cumn_t*a,int*cnt,int*sum,int mx,int lc,int ph,int sy,int uc,int op,int fi,int hw){if(*cnt>=mx)return -1;cumn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->seed_yd=sy;x->umb_ct=uc;x->oil_pct=op;x->flv_idx=fi;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CUMN] Cumin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" sy=");pi(sy);ps(" uc=");pi(uc);ps(" op=");pi(op);ps(" fi=");pi(fi);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cumn_init(void){if(init)return -1;st.n_plant=0;st.n_irrig=0;st.n_dry=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_seed=0;st.t_umb=0;st.t_oil=0;st.t_flv=0;for(int i=0;i<N;i++)cumnps[i].active=0;for(int i=0;i<N-2;i++)cumni[i].active=0;for(int i=0;i<N-4;i++)cumnd[i].active=0;for(int i=0;i<N-6;i++)cumnh[i].active=0;for(int i=0;i<N-6;i++)cumnm[i].active=0;init=1;ps("[CUMN] Cumin initialized\n");return 0;}
int cumn_planting(int lc,int ph,int sy,int uc,int op,int fi,int hw){return add(cumnps,&st.n_plant,&st.t_ht,N,lc,ph,sy,uc,op,fi,hw);}
int cumn_irrigation(int lc,int ph,int sy,int uc,int op,int fi,int hw){return add(cumni,&st.n_irrig,&st.t_seed,N-2,lc,ph,sy,uc,op,fi,hw);}
int cumn_drying(int lc,int ph,int sy,int uc,int op,int fi,int hw){return add(cumnd,&st.n_dry,&st.t_umb,N-4,lc,ph,sy,uc,op,fi,hw);}
int cumn_harvest(int lc,int ph,int sy,int uc,int op,int fi,int hw){return add(cumnh,&st.n_harv,&st.t_oil,N-6,lc,ph,sy,uc,op,fi,hw);}
int cumn_market(int lc,int ph,int sy,int uc,int op,int fi,int hw){return add(cumnm,&st.n_mkt,&st.t_flv,N-6,lc,ph,sy,uc,op,fi,hw);}
void cumn_report(void){ps("[CUMN] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nIrrig: ");pi(st.n_irrig);ps(" Seed=");pi(st.t_seed);ps("\nDry: ");pi(st.n_dry);ps(" Umb=");pi(st.t_umb);ps("\nHarv: ");pi(st.n_harv);ps(" Oil=");pi(st.t_oil);ps("\nMkt: ");pi(st.n_mkt);ps(" Flv=");pi(st.t_flv);ps("\n");}
void cumn_state(void){ps("[CUMN] Plant=");pi(st.n_plant);ps(" Irrig=");pi(st.n_irrig);ps(" Dry=");pi(st.n_dry);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cumin Admin Demo ===\n\n");cumn_init();
/* 1=field 2=arid 3=irrigated 4=organic 5=market */
ps("Cumin planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=25+(i*4),sy=5+(i*3),uc=3+(i*2),op=3+(i%5),fi=(i%8)+1,hw=14+(i%6);cumn_planting(lc,ph,sy,uc,op,fi,hw);}
ps("\nCumin irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=28+(i*3),sy=6+(i*2),uc=4+(i*2),op=4+(i%4),fi=(i%7)+1,hw=15+(i%5);cumn_irrigation(lc,ph,sy,uc,op,fi,hw);}
ps("\nCumin drying...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=30+(i*3),sy=7+(i*2),uc=5+(i*2),op=5+(i%3),fi=(i%6)+1,hw=16+(i%4);cumn_drying(lc,ph,sy,uc,op,fi,hw);}
ps("\nCumin harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=22+(i*5),sy=4+(i*4),uc=2+(i*3),op=2+(i%6),fi=(i%9)+1,hw=13+(i%7);cumn_harvest(lc,ph,sy,uc,op,fi,hw);}
ps("\nCumin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=32+(i*3),sy=8+(i*2),uc=6+(i*2),op=6+(i%3),fi=(i%4)+5,hw=17+(i%3);cumn_market(lc,ph,sy,uc,op,fi,hw);}
ps("\n");cumn_report();cumn_state();ps("\n=== Demo Complete ===\n");return 0;}
