/* crab_admin: Crab (Scylla serrata) mud crab aquaculture (v1.0)
 * Crab stocking, feeding, water management, harvest, market
 * Features: shell_width_mm, body_wt_g, claw_size_mm, meat_pct, water_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,sh_wd,bdy_wt,claw_sz,meat_pct,wt_tmp,harv_wk,active;} crb_t;
typedef struct{int n_stock,n_feed,n_water,n_harv,n_mkt,t_wd,t_wt,t_claw,t_meat,t_tmp;} crb_state_t;
static crb_t crbsd[N],crbfd[N-2],crbwt[N-4],crbhs[N-6],crbms[N-6]; static crb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crb_t*a,int*cnt,int*sum,int mx,int lc,int sw,int bw,int cs,int mp,int wt,int hw){if(*cnt>=mx)return -1;crb_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->sh_wd=sw;x->bdy_wt=bw;x->claw_sz=cs;x->meat_pct=mp;x->wt_tmp=wt;x->harv_wk=hw;x->active=1;*sum+=sw;(*cnt)++;ps("[CRB] Crab ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sw=");pi(sw);ps(" bw=");pi(bw);ps(" cs=");pi(cs);ps(" mp=");pi(mp);ps(" wt=");pi(wt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int crb_init(void){if(init)return -1;st.n_stock=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_wd=0;st.t_wt=0;st.t_claw=0;st.t_meat=0;st.t_tmp=0;for(int i=0;i<N;i++)crbsd[i].active=0;for(int i=0;i<N-2;i++)crbfd[i].active=0;for(int i=0;i<N-4;i++)crbwt[i].active=0;for(int i=0;i<N-6;i++)crbhs[i].active=0;for(int i=0;i<N-6;i++)crbms[i].active=0;init=1;ps("[CRB] Crab initialized\n");return 0;}
int crb_stocking(int lc,int sw,int bw,int cs,int mp,int wt,int hw){return add(crbsd,&st.n_stock,&st.t_wd,N,lc,sw,bw,cs,mp,wt,hw);}
int crb_feeding(int lc,int sw,int bw,int cs,int mp,int wt,int hw){return add(crbfd,&st.n_feed,&st.t_wt,N-2,lc,sw,bw,cs,mp,wt,hw);}
int crb_water_mgmt(int lc,int sw,int bw,int cs,int mp,int wt,int hw){return add(crbwt,&st.n_water,&st.t_claw,N-4,lc,sw,bw,cs,mp,wt,hw);}
int crb_harvest(int lc,int sw,int bw,int cs,int mp,int wt,int hw){return add(crbhs,&st.n_harv,&st.t_meat,N-6,lc,sw,bw,cs,mp,wt,hw);}
int crb_market(int lc,int sw,int bw,int cs,int mp,int wt,int hw){return add(crbms,&st.n_mkt,&st.t_tmp,N-6,lc,sw,bw,cs,mp,wt,hw);}
void crb_report(void){ps("[CRB] Stock: ");pi(st.n_stock);ps(" Wd=");pi(st.t_wd);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nWater: ");pi(st.n_water);ps(" Claw=");pi(st.t_claw);ps("\nHarv: ");pi(st.n_harv);ps(" Meat=");pi(st.t_meat);ps("\nMkt: ");pi(st.n_mkt);ps(" Tmp=");pi(st.t_tmp);ps("\n");}
void crb_state(void){ps("[CRB] Stock=");pi(st.n_stock);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Crab Admin Demo ===\n\n");crb_init();
/* 1=pond 2=mangrove 3=tank 4=coastal 5=market */
ps("Crab stocking...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sw=80+(i*8),bw=150+(i*30),cs=30+(i*5),mp=25+(i*3),wt=25+(i%8),hw=16+(i%8);crb_stocking(lc,sw,bw,cs,mp,wt,hw);}
ps("\nCrab feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sw=85+(i*7),bw=160+(i*25),cs=32+(i*4),mp=27+(i*3),wt=26+(i%7),hw=18+(i%6);crb_feeding(lc,sw,bw,cs,mp,wt,hw);}
ps("\nCrab water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sw=90+(i*6),bw=170+(i*20),cs=34+(i*4),mp=29+(i*2),wt=27+(i%6),hw=20+(i%5);crb_water_mgmt(lc,sw,bw,cs,mp,wt,hw);}
ps("\nCrab harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sw=75+(i*10),bw=140+(i*35),cs=28+(i*6),mp=23+(i*4),wt=24+(i%10),hw=14+(i%10);crb_harvest(lc,sw,bw,cs,mp,wt,hw);}
ps("\nCrab market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sw=95+(i*5),bw=180+(i*15),cs=36+(i*3),mp=31+(i*2),wt=28+(i%5),hw=22+(i%4);crb_market(lc,sw,bw,cs,mp,wt,hw);}
ps("\n");crb_report();crb_state();ps("\n=== Demo Complete ===\n");return 0;}
