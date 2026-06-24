/* coconut_admin: Coconut (Cocos nucifera) palm tree management (v1.0)
 * Coconut planting, irrigation, husking, harvest, market
 * Features: tree_height_m, nut_count, nut_wt_kg, water_ml, oil_pct, harvest_month
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,nut_ct,nut_wt,water_ml,oil_pct,harv_mo,active;} ccnt_t;
typedef struct{int n_plant,n_irrig,n_husk,n_harv,n_mkt,t_ht,t_nut,t_wt,t_water,t_oil;} ccnt_state_t;
static ccnt_t ccnps[N],ccnis[N-2],ccnhs[N-4],ccnhv[N-6],ccnms[N-6]; static ccnt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ccnt_t*a,int*cnt,int*sum,int mx,int lc,int th,int nc,int nw,int wm,int op,int hm){if(*cnt>=mx)return -1;ccnt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->nut_ct=nc;x->nut_wt=nw;x->water_ml=wm;x->oil_pct=op;x->harv_mo=hm;x->active=1;*sum+=th;(*cnt)++;ps("[CCNT] Coconut ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" nc=");pi(nc);ps(" nw=");pi(nw);ps(" wm=");pi(wm);ps(" op=");pi(op);ps(" hm=");pi(hm);ps("\n");return *cnt-1;}
int ccnt_init(void){if(init)return -1;st.n_plant=0;st.n_irrig=0;st.n_husk=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_nut=0;st.t_wt=0;st.t_water=0;st.t_oil=0;for(int i=0;i<N;i++)ccnps[i].active=0;for(int i=0;i<N-2;i++)ccnis[i].active=0;for(int i=0;i<N-4;i++)ccnhs[i].active=0;for(int i=0;i<N-6;i++)ccnhv[i].active=0;for(int i=0;i<N-6;i++)ccnms[i].active=0;init=1;ps("[CCNT] Coconut initialized\n");return 0;}
int ccnt_planting(int lc,int th,int nc,int nw,int wm,int op,int hm){return add(ccnps,&st.n_plant,&st.t_ht,N,lc,th,nc,nw,wm,op,hm);}
int ccnt_irrigation(int lc,int th,int nc,int nw,int wm,int op,int hm){return add(ccnis,&st.n_irrig,&st.t_nut,N-2,lc,th,nc,nw,wm,op,hm);}
int ccnt_husking(int lc,int th,int nc,int nw,int wm,int op,int hm){return add(ccnhs,&st.n_husk,&st.t_wt,N-4,lc,th,nc,nw,wm,op,hm);}
int ccnt_harvest(int lc,int th,int nc,int nw,int wm,int op,int hm){return add(ccnhv,&st.n_harv,&st.t_water,N-6,lc,th,nc,nw,wm,op,hm);}
int ccnt_market(int lc,int th,int nc,int nw,int wm,int op,int hm){return add(ccnms,&st.n_mkt,&st.t_oil,N-6,lc,th,nc,nw,wm,op,hm);}
void ccnt_report(void){ps("[CCNT] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nIrrig: ");pi(st.n_irrig);ps(" Nut=");pi(st.t_nut);ps("\nHusk: ");pi(st.n_husk);ps(" Wt=");pi(st.t_wt);ps("\nHarv: ");pi(st.n_harv);ps(" Water=");pi(st.t_water);ps("\nMkt: ");pi(st.n_mkt);ps(" Oil=");pi(st.t_oil);ps("\n");}
void ccnt_state(void){ps("[CCNT] Plant=");pi(st.n_plant);ps(" Irrig=");pi(st.n_irrig);ps(" Husk=");pi(st.n_husk);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coconut Admin Demo ===\n\n");ccnt_init();
/* 1=plantation 2=coastal 3=island 4=garden 5=market */
ps("Coconut planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,th=5+(i*3),nc=10+(i*5),nw=1+(i%3),wm=200+(i*50),op=30+(i*3),hm=8+(i%6);ccnt_planting(lc,th,nc,nw,wm,op,hm);}
ps("\nCoconut irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,th=6+(i*3),nc=12+(i*4),nw=2+(i%2),wm=220+(i*40),op=32+(i*3),hm=9+(i%5);ccnt_irrigation(lc,th,nc,nw,wm,op,hm);}
ps("\nCoconut husking...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,th=7+(i*3),nc=14+(i*4),nw=2+(i%2),wm=240+(i*30),op=34+(i*2),hm=10+(i%4);ccnt_husking(lc,th,nc,nw,wm,op,hm);}
ps("\nCoconut harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,th=4+(i*4),nc=8+(i*6),nw=1+(i%4),wm=180+(i*60),op=28+(i*4),hm=7+(i%7);ccnt_harvest(lc,th,nc,nw,wm,op,hm);}
ps("\nCoconut market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,th=8+(i*2),nc=16+(i*3),nw=2+(i%2),wm=260+(i*20),op=36+(i*2),hm=11+(i%3);ccnt_market(lc,th,nc,nw,wm,op,hm);}
ps("\n");ccnt_report();ccnt_state();ps("\n=== Demo Complete ===\n");return 0;}
