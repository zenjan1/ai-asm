/* borehole_admin: Borehole water well drilling and groundwater management (v1.0)
 * Borehole drilling, testing, maintenance, monitoring, market
 * Features: depth_m, yield_lph, water_quality, casing_mm, pump_type, recharge_rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,dpth_m,yield_lph,wtr_qual,casing_mm,pump_ty,rchrg_rt,active;} bore_t;
typedef struct{int n_drill,n_test,n_maint,n_monitor,n_mkt,t_depth,t_yield,t_qual,t_casing,t_pump;} bore_state_t;
static bore_t boreps[N],borets[N-2],boremts[N-4],boremons[N-6],borems[N-6]; static bore_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bore_t*a,int*cnt,int*sum,int mx,int lc,int dp,int yl,int wq,int cm,int pt,int rr){if(*cnt>=mx)return -1;bore_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->dpth_m=dp;x->yield_lph=yl;x->wtr_qual=wq;x->casing_mm=cm;x->pump_ty=pt;x->rchrg_rt=rr;x->active=1;*sum+=dp;(*cnt)++;ps("[BORE] Borehole ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" dp=");pi(dp);ps(" yl=");pi(yl);ps(" wq=");pi(wq);ps(" cm=");pi(cm);ps(" pt=");pi(pt);ps(" rr=");pi(rr);ps("\n");return *cnt-1;}
int bore_init(void){if(init)return -1;st.n_drill=0;st.n_test=0;st.n_maint=0;st.n_monitor=0;st.n_mkt=0;st.t_depth=0;st.t_yield=0;st.t_qual=0;st.t_casing=0;st.t_pump=0;for(int i=0;i<N;i++)boreps[i].active=0;for(int i=0;i<N-2;i++)borets[i].active=0;for(int i=0;i<N-4;i++)boremts[i].active=0;for(int i=0;i<N-6;i++)boremons[i].active=0;for(int i=0;i<N-6;i++)borems[i].active=0;init=1;ps("[BORE] Borehole initialized\n");return 0;}
int bore_drilling(int lc,int dp,int yl,int wq,int cm,int pt,int rr){return add(boreps,&st.n_drill,&st.t_depth,N,lc,dp,yl,wq,cm,pt,rr);}
int bore_testing(int lc,int dp,int yl,int wq,int cm,int pt,int rr){return add(borets,&st.n_test,&st.t_yield,N-2,lc,dp,yl,wq,cm,pt,rr);}
int bore_maintenance(int lc,int dp,int yl,int wq,int cm,int pt,int rr){return add(boremts,&st.n_maint,&st.t_qual,N-4,lc,dp,yl,wq,cm,pt,rr);}
int bore_monitoring(int lc,int dp,int yl,int wq,int cm,int pt,int rr){return add(boremons,&st.n_monitor,&st.t_casing,N-6,lc,dp,yl,wq,cm,pt,rr);}
int bore_market(int lc,int dp,int yl,int wq,int cm,int pt,int rr){return add(borems,&st.n_mkt,&st.t_pump,N-6,lc,dp,yl,wq,cm,pt,rr);}
void bore_report(void){ps("[BORE] Drill: ");pi(st.n_drill);ps(" Depth=");pi(st.t_depth);ps("\nTest: ");pi(st.n_test);ps(" Yield=");pi(st.t_yield);ps("\nMaint: ");pi(st.n_maint);ps(" Qual=");pi(st.t_qual);ps("\nMonitor: ");pi(st.n_monitor);ps(" Casing=");pi(st.t_casing);ps("\nMkt: ");pi(st.n_mkt);ps(" Pump=");pi(st.t_pump);ps("\n");}
void bore_state(void){ps("[BORE] Drill=");pi(st.n_drill);ps(" Test=");pi(st.n_test);ps(" Maint=");pi(st.n_maint);ps(" Monitor=");pi(st.n_monitor);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Borehole Admin Demo ===\n\n");bore_init();
/* 1=residential 2=agricultural 3=industrial 4=municipal 5=remote */
ps("Borehole drilling...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,dp=20+(i*10),yl=500+(i*200),wq=6+(i%5),cm=100+(i*25),pt=(i%4)+1,rr=10+(i*5);bore_drilling(lc,dp,yl,wq,cm,pt,rr);}
ps("\nBorehole testing...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,dp=30+(i*8),yl=600+(i*150),wq=7+(i%3),cm=120+(i*20),pt=(i%4)+1,rr=12+(i*4);bore_testing(lc,dp,yl,wq,cm,pt,rr);}
ps("\nBorehole maintenance...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,dp=40+(i*6),yl=700+(i*100),wq=8+(i%2),cm=140+(i*15),pt=(i%3)+2,rr=15+(i*3);bore_maintenance(lc,dp,yl,wq,cm,pt,rr);}
ps("\nBorehole monitoring...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,dp=25+(i*8),yl=400+(i*250),wq=5+(i%4),cm=110+(i*20),pt=(i%4)+1,rr=8+(i*6);bore_monitoring(lc,dp,yl,wq,cm,pt,rr);}
ps("\nBorehole market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,dp=50+(i*5),yl=800+(i*100),wq=9+(i%1),cm=160+(i*10),pt=(i%3)+2,rr=18+(i*2);bore_market(lc,dp,yl,wq,cm,pt,rr);}
ps("\n");bore_report();bore_state();ps("\n=== Demo Complete ===\n");return 0;}
