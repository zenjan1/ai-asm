/* brackish_admin: Brackish water estuary ecosystem management (v1.0)
 * Tidal, estuary, conservation, monitoring, market
 * Features: salinity_ppt, tide_m, turbidity_ntu, mangrove_pct, fish_species, bird_count
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sal_ppt,tide_m,turb_ntu,mang_pct,fish_sp,bird_ct,active;} brack_t;
typedef struct{int n_tidal,n_est,n_cons,n_monitor,n_mkt,t_sal,t_tide,t_turb,t_mang,t_fish;} brack_state_t;
static brack_t brackps[N],brackes[N-2],brackcs[N-4],brackmons[N-6],brackms[N-6]; static brack_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brack_t*a,int*cnt,int*sum,int mx,int lc,int sal,int tid,int turb,int mang,int fsh,int bird){if(*cnt>=mx)return -1;brack_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sal_ppt=sal;x->tide_m=tid;x->turb_ntu=turb;x->mang_pct=mang;x->fish_sp=fsh;x->bird_ct=bird;x->active=1;*sum+=sal;(*cnt)++;ps("[BRACK] Brack ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sal=");pi(sal);ps(" tid=");pi(tid);ps(" turb=");pi(turb);ps(" mang=");pi(mang);ps(" fsh=");pi(fsh);ps(" bird=");pi(bird);ps("\n");return *cnt-1;}
int brack_init(void){if(init)return -1;st.n_tidal=0;st.n_est=0;st.n_cons=0;st.n_monitor=0;st.n_mkt=0;st.t_sal=0;st.t_tide=0;st.t_turb=0;st.t_mang=0;st.t_fish=0;for(int i=0;i<N;i++)brackps[i].active=0;for(int i=0;i<N-2;i++)brackes[i].active=0;for(int i=0;i<N-4;i++)brackcs[i].active=0;for(int i=0;i<N-6;i++)brackmons[i].active=0;for(int i=0;i<N-6;i++)brackms[i].active=0;init=1;ps("[BRACK] Brackish initialized\n");return 0;}
int brack_tidal(int lc,int sal,int tid,int turb,int mang,int fsh,int bird){return add(brackps,&st.n_tidal,&st.t_sal,N,lc,sal,tid,turb,mang,fsh,bird);}
int brack_estuary(int lc,int sal,int tid,int turb,int mang,int fsh,int bird){return add(brackes,&st.n_est,&st.t_tide,N-2,lc,sal,tid,turb,mang,fsh,bird);}
int brack_conservation(int lc,int sal,int tid,int turb,int mang,int fsh,int bird){return add(brackcs,&st.n_cons,&st.t_turb,N-4,lc,sal,tid,turb,mang,fsh,bird);}
int brack_monitoring(int lc,int sal,int tid,int turb,int mang,int fsh,int bird){return add(brackmons,&st.n_monitor,&st.t_mang,N-6,lc,sal,tid,turb,mang,fsh,bird);}
int brack_market(int lc,int sal,int tid,int turb,int mang,int fsh,int bird){return add(brackms,&st.n_mkt,&st.t_fish,N-6,lc,sal,tid,turb,mang,fsh,bird);}
void brack_report(void){ps("[BRACK] Tidal: ");pi(st.n_tidal);ps(" Sal=");pi(st.t_sal);ps("\nEst: ");pi(st.n_est);ps(" Tide=");pi(st.t_tide);ps("\nCons: ");pi(st.n_cons);ps(" Turb=");pi(st.t_turb);ps("\nMonitor: ");pi(st.n_monitor);ps(" Mang=");pi(st.t_mang);ps("\nMkt: ");pi(st.n_mkt);ps(" Fish=");pi(st.t_fish);ps("\n");}
void brack_state(void){ps("[BRACK] Tidal=");pi(st.n_tidal);ps(" Est=");pi(st.n_est);ps(" Cons=");pi(st.n_cons);ps(" Monitor=");pi(st.n_monitor);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Brackish Admin Demo ===\n\n");brack_init();
/* 1=mangrove 2=saltmarsh 3=lagoon 4=delta 5=fjord */
ps("Tidal survey...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sal=5+(i*2),tid=1+(i%5),turb=10+(i*5),mang=20+(i*3),fsh=8+(i*2),bird=15+(i*5);brack_tidal(lc,sal,tid,turb,mang,fsh,bird);}
ps("\nEstuary survey...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sal=8+(i*2),tid=2+(i%4),turb=15+(i*4),mang=25+(i*2),fsh=12+(i*2),bird=20+(i*3);brack_estuary(lc,sal,tid,turb,mang,fsh,bird);}
ps("\nConservation survey...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sal=10+(i*1),tid=3+(i%3),turb=8+(i*6),mang=30+(i*2),fsh=15+(i*2),bird=25+(i*3);brack_conservation(lc,sal,tid,turb,mang,fsh,bird);}
ps("\nMonitoring survey...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sal=6+(i*2),tid=1+(i%5),turb=20+(i*3),mang=18+(i*4),fsh=10+(i*2),bird=12+(i*4);brack_monitoring(lc,sal,tid,turb,mang,fsh,bird);}
ps("\nMarket survey...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sal=12+(i*1),tid=4+(i*2),turb=5+(i*7),mang=35+(i*2),fsh=18+(i*2),bird=30+(i*3);brack_market(lc,sal,tid,turb,mang,fsh,bird);}
ps("\n");brack_report();brack_state();ps("\n=== Demo Complete ===\n");return 0;}
