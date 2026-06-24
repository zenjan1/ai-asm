/* coral_admin: Coral (Acropora millepora) reef ecosystem management (v1.0)
 * Coral planting, feeding, water management, harvest, market
 * Features: colony_dia_cm, height_cm, polyp_count, color_idx, water_temp, growth_rate
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,clny_dia,ht_cm,polp_ct,clr_idx,wt_tmp,gr_rate,active;} crl_t;
typedef struct{int n_plant,n_feed,n_water,n_harv,n_mkt,t_dia,t_ht,t_polp,t_clr,t_gr;} crl_state_t;
static crl_t crlps[N],crlfd[N-2],crlwt[N-4],crlhv[N-6],crlmk[N-6]; static crl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crl_t*a,int*cnt,int*sum,int mx,int lc,int cd,int hc,int pc,int ci,int wt,int gr){if(*cnt>=mx)return -1;crl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->clny_dia=cd;x->ht_cm=hc;x->polp_ct=pc;x->clr_idx=ci;x->wt_tmp=wt;x->gr_rate=gr;x->active=1;*sum+=cd;(*cnt)++;ps("[CRL] Coral ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cd=");pi(cd);ps(" hc=");pi(hc);ps(" pc=");pi(pc);ps(" ci=");pi(ci);ps(" wt=");pi(wt);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int crl_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_dia=0;st.t_ht=0;st.t_polp=0;st.t_clr=0;st.t_gr=0;for(int i=0;i<N;i++)crlps[i].active=0;for(int i=0;i<N-2;i++)crlfd[i].active=0;for(int i=0;i<N-4;i++)crlwt[i].active=0;for(int i=0;i<N-6;i++)crlhv[i].active=0;for(int i=0;i<N-6;i++)crlmk[i].active=0;init=1;ps("[CRL] Coral initialized\n");return 0;}
int crl_planting(int lc,int cd,int hc,int pc,int ci,int wt,int gr){return add(crlps,&st.n_plant,&st.t_dia,N,lc,cd,hc,pc,ci,wt,gr);}
int crl_feeding(int lc,int cd,int hc,int pc,int ci,int wt,int gr){return add(crlfd,&st.n_feed,&st.t_ht,N-2,lc,cd,hc,pc,ci,wt,gr);}
int crl_water_mgmt(int lc,int cd,int hc,int pc,int ci,int wt,int gr){return add(crlwt,&st.n_water,&st.t_polp,N-4,lc,cd,hc,pc,ci,wt,gr);}
int crl_harvest(int lc,int cd,int hc,int pc,int ci,int wt,int gr){return add(crlhv,&st.n_harv,&st.t_clr,N-6,lc,cd,hc,pc,ci,wt,gr);}
int crl_market(int lc,int cd,int hc,int pc,int ci,int wt,int gr){return add(crlmk,&st.n_mkt,&st.t_gr,N-6,lc,cd,hc,pc,ci,wt,gr);}
void crl_report(void){ps("[CRL] Plant: ");pi(st.n_plant);ps(" Dia=");pi(st.t_dia);ps("\nFeed: ");pi(st.n_feed);ps(" Ht=");pi(st.t_ht);ps("\nWater: ");pi(st.n_water);ps(" Polp=");pi(st.t_polp);ps("\nHarv: ");pi(st.n_harv);ps(" Clr=");pi(st.t_clr);ps("\nMkt: ");pi(st.n_mkt);ps(" Gr=");pi(st.t_gr);ps("\n");}
void crl_state(void){ps("[CRL] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coral Admin Demo ===\n\n");crl_init();
/* 1=reef 2=lagoon 3=aquarium 4=nursery 5=market */
ps("Coral planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,cd=10+(i*3),hc=5+(i*2),pc=50+(i*20),ci=(i%7)+1,wt=24+(i%6),gr=1+(i%5);crl_planting(lc,cd,hc,pc,ci,wt,gr);}
ps("\nCoral feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,cd=12+(i*3),hc=6+(i*2),pc=55+(i*18),ci=(i%6)+1,wt=25+(i%5),gr=1+(i%4);crl_feeding(lc,cd,hc,pc,ci,wt,gr);}
ps("\nCoral water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,cd=14+(i*2),hc=7+(i*2),pc=60+(i*15),ci=(i%5)+1,wt=26+(i%4),gr=2+(i%3);crl_water_mgmt(lc,cd,hc,pc,ci,wt,gr);}
ps("\nCoral harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,cd=8+(i*4),hc=4+(i*3),pc=45+(i*25),ci=(i%8)+1,wt=23+(i%8),gr=1+(i%6);crl_harvest(lc,cd,hc,pc,ci,wt,gr);}
ps("\nCoral market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,cd=16+(i*2),hc=8+(i*2),pc=65+(i*12),ci=(i%4)+4,wt=27+(i%3),gr=2+(i%2);crl_market(lc,cd,hc,pc,ci,wt,gr);}
ps("\n");crl_report();crl_state();ps("\n=== Demo Complete ===\n");return 0;}
