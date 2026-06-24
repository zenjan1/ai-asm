/* cucumber_admin: Cucumber (Cucumis sativus) vine vegetable cultivation (v1.0)
 * Cucumber planting, trellising, pollination, harvest, market
 * Features: vine_len_cm, fruit_len_cm, fruit_wt_g, spine_count, water_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vn_ln,frt_ln,frt_wt,spn_ct,wt_pct,harv_wk,active;} ccmb_t;
typedef struct{int n_plant,n_trell,n_poll,n_harv,n_mkt,t_vn,t_frt,t_wt,t_spn,t_water;} ccmb_state_t;
static ccmb_t ccmbps[N],ccmbt[N-2],ccmbpl[N-4],ccmbh[N-6],ccmbm[N-6]; static ccmb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ccmb_t*a,int*cnt,int*sum,int mx,int lc,int vl,int fl,int fw,int sc,int wp,int hw){if(*cnt>=mx)return -1;ccmb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vn_ln=vl;x->frt_ln=fl;x->frt_wt=fw;x->spn_ct=sc;x->wt_pct=wp;x->harv_wk=hw;x->active=1;*sum+=vl;(*cnt)++;ps("[CCMB] Cucumber ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" fl=");pi(fl);ps(" fw=");pi(fw);ps(" sc=");pi(sc);ps(" wp=");pi(wp);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int ccmb_init(void){if(init)return -1;st.n_plant=0;st.n_trell=0;st.n_poll=0;st.n_harv=0;st.n_mkt=0;st.t_vn=0;st.t_frt=0;st.t_wt=0;st.t_spn=0;st.t_water=0;for(int i=0;i<N;i++)ccmbps[i].active=0;for(int i=0;i<N-2;i++)ccmbt[i].active=0;for(int i=0;i<N-4;i++)ccmbpl[i].active=0;for(int i=0;i<N-6;i++)ccmbh[i].active=0;for(int i=0;i<N-6;i++)ccmbm[i].active=0;init=1;ps("[CCMB] Cucumber initialized\n");return 0;}
int ccmb_planting(int lc,int vl,int fl,int fw,int sc,int wp,int hw){return add(ccmbps,&st.n_plant,&st.t_vn,N,lc,vl,fl,fw,sc,wp,hw);}
int ccmb_trellising(int lc,int vl,int fl,int fw,int sc,int wp,int hw){return add(ccmbt,&st.n_trell,&st.t_frt,N-2,lc,vl,fl,fw,sc,wp,hw);}
int ccmb_pollination(int lc,int vl,int fl,int fw,int sc,int wp,int hw){return add(ccmbpl,&st.n_poll,&st.t_wt,N-4,lc,vl,fl,fw,sc,wp,hw);}
int ccmb_harvest(int lc,int vl,int fl,int fw,int sc,int wp,int hw){return add(ccmbh,&st.n_harv,&st.t_spn,N-6,lc,vl,fl,fw,sc,wp,hw);}
int ccmb_market(int lc,int vl,int fl,int fw,int sc,int wp,int hw){return add(ccmbm,&st.n_mkt,&st.t_water,N-6,lc,vl,fl,fw,sc,wp,hw);}
void ccmb_report(void){ps("[CCMB] Plant: ");pi(st.n_plant);ps(" Vine=");pi(st.t_vn);ps("\nTrell: ");pi(st.n_trell);ps(" Frt=");pi(st.t_frt);ps("\nPoll: ");pi(st.n_poll);ps(" Wt=");pi(st.t_wt);ps("\nHarv: ");pi(st.n_harv);ps(" Spn=");pi(st.t_spn);ps("\nMkt: ");pi(st.n_mkt);ps(" Water=");pi(st.t_water);ps("\n");}
void ccmb_state(void){ps("[CCMB] Plant=");pi(st.n_plant);ps(" Trell=");pi(st.n_trell);ps(" Poll=");pi(st.n_poll);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cucumber Admin Demo ===\n\n");ccmb_init();
/* 1=garden 2=greenhouse 3=raised_bed 4=field 5=market */
ps("Cucumber planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,vl=50+(i*10),fl=15+(i*3),fw=150+(i*30),sc=(i%8),wp=90+(i%8),hw=10+(i%6);ccmb_planting(lc,vl,fl,fw,sc,wp,hw);}
ps("\nCucumber trellising...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,vl=55+(i*9),fl=16+(i*3),fw=160+(i*25),sc=(i%7),wp=91+(i%7),hw=11+(i%5);ccmb_trellising(lc,vl,fl,fw,sc,wp,hw);}
ps("\nCucumber pollination...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,vl=60+(i*8),fl=17+(i*2),fw=170+(i*20),sc=(i%6),wp=92+(i%6),hw=12+(i%4);ccmb_pollination(lc,vl,fl,fw,sc,wp,hw);}
ps("\nCucumber harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,vl=45+(i*12),fl=14+(i*4),fw=140+(i*35),sc=(i%9),wp=88+(i%10),hw=9+(i%7);ccmb_harvest(lc,vl,fl,fw,sc,wp,hw);}
ps("\nCucumber market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,vl=65+(i*7),fl=18+(i*2),fw=180+(i*18),sc=(i%5),wp=93+(i%5),hw=13+(i%3);ccmb_market(lc,vl,fl,fw,sc,wp,hw);}
ps("\n");ccmb_report();ccmb_state();ps("\n=== Demo Complete ===\n");return 0;}
