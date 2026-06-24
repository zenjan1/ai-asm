/* coho_admin: Coho (Oncorhynchus kisutch) salmon fish farming (v1.0)
 * Coho stocking, feeding, water management, harvest, market
 * Features: body_len_cm, body_wt_kg, roe_pct, fat_pct, water_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,roe_pct,fat_pct,wtr_tmp,harv_wk,active;} coh_t;
typedef struct{int n_stock,n_feed,n_water,n_harv,n_mkt,t_ln,t_wt,t_roe,t_fat,t_tmp;} coh_state_t;
static coh_t cohps[N],cohfd[N-2],cohwt[N-4],cohhs[N-6],cohms[N-6]; static coh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(coh_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int rp,int fp,int wt,int hw){if(*cnt>=mx)return -1;coh_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->roe_pct=rp;x->fat_pct=fp;x->wtr_tmp=wt;x->harv_wk=hw;x->active=1;*sum+=bl;(*cnt)++;ps("[COH] Coho ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" rp=");pi(rp);ps(" fp=");pi(fp);ps(" wt=");pi(wt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int coh_init(void){if(init)return -1;st.n_stock=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_roe=0;st.t_fat=0;st.t_tmp=0;for(int i=0;i<N;i++)cohps[i].active=0;for(int i=0;i<N-2;i++)cohfd[i].active=0;for(int i=0;i<N-4;i++)cohwt[i].active=0;for(int i=0;i<N-6;i++)cohhs[i].active=0;for(int i=0;i<N-6;i++)cohms[i].active=0;init=1;ps("[COH] Coho initialized\n");return 0;}
int coh_stocking(int lc,int bl,int bw,int rp,int fp,int wt,int hw){return add(cohps,&st.n_stock,&st.t_ln,N,lc,bl,bw,rp,fp,wt,hw);}
int coh_feeding(int lc,int bl,int bw,int rp,int fp,int wt,int hw){return add(cohfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,rp,fp,wt,hw);}
int coh_water_mgmt(int lc,int bl,int bw,int rp,int fp,int wt,int hw){return add(cohwt,&st.n_water,&st.t_roe,N-4,lc,bl,bw,rp,fp,wt,hw);}
int coh_harvest(int lc,int bl,int bw,int rp,int fp,int wt,int hw){return add(cohhs,&st.n_harv,&st.t_fat,N-6,lc,bl,bw,rp,fp,wt,hw);}
int coh_market(int lc,int bl,int bw,int rp,int fp,int wt,int hw){return add(cohms,&st.n_mkt,&st.t_tmp,N-6,lc,bl,bw,rp,fp,wt,hw);}
void coh_report(void){ps("[COH] Stock: ");pi(st.n_stock);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nWater: ");pi(st.n_water);ps(" Roe=");pi(st.t_roe);ps("\nHarv: ");pi(st.n_harv);ps(" Fat=");pi(st.t_fat);ps("\nMkt: ");pi(st.n_mkt);ps(" Tmp=");pi(st.t_tmp);ps("\n");}
void coh_state(void){ps("[COH] Stock=");pi(st.n_stock);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Coho Admin Demo ===\n\n");coh_init();
/* 1=river_pen 2=hatchery 3=coastal 4=fjord 5=market */
ps("Coho stocking...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*8),bw=2+(i*2),rp=10+(i*5),fp=8+(i*3),wt=10+(i%6),hw=18+(i%8);coh_stocking(lc,bl,bw,rp,fp,wt,hw);}
ps("\nCoho feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=55+(i*7),bw=3+(i*2),rp=12+(i*4),fp=9+(i*3),wt=11+(i%5),hw=20+(i%6);coh_feeding(lc,bl,bw,rp,fp,wt,hw);}
ps("\nCoho water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=60+(i*6),bw=4+(i*2),rp=14+(i*4),fp=10+(i*2),wt=12+(i%4),hw=22+(i%5);coh_water_mgmt(lc,bl,bw,rp,fp,wt,hw);}
ps("\nCoho harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=45+(i*10),bw=1+(i*3),rp=8+(i*6),fp=7+(i*4),wt=9+(i%8),hw=16+(i%10);coh_harvest(lc,bl,bw,rp,fp,wt,hw);}
ps("\nCoho market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=65+(i*5),bw=5+(i*2),rp=16+(i*3),fp=11+(i*2),wt=13+(i%3),hw=24+(i%4);coh_market(lc,bl,bw,rp,fp,wt,hw);}
ps("\n");coh_report();coh_state();ps("\n=== Demo Complete ===\n");return 0;}
