/* cod_admin: Cod (Gadus morhua) Atlantic cod aquaculture (v1.0)
 * Cod stocking, feeding, water management, harvest, market
 * Features: body_len_cm, body_wt_kg, fillet_pct, omega3_ppm, water_temp, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fl_pct,omg3,wtr_tmp,harv_wk,active;} cod_t;
typedef struct{int n_stock,n_feed,n_water,n_harv,n_mkt,t_ln,t_wt,t_fl,t_omg,t_tmp;} cod_state_t;
static cod_t codps[N],codfd[N-2],codwt[N-4],codhs[N-6],codms[N-6]; static cod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cod_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fp,int o3,int wt,int hw){if(*cnt>=mx)return -1;cod_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fl_pct=fp;x->omg3=o3;x->wtr_tmp=wt;x->harv_wk=hw;x->active=1;*sum+=bl;(*cnt)++;ps("[COD] Cod ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fp=");pi(fp);ps(" o3=");pi(o3);ps(" wt=");pi(wt);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cod_init(void){if(init)return -1;st.n_stock=0;st.n_feed=0;st.n_water=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fl=0;st.t_omg=0;st.t_tmp=0;for(int i=0;i<N;i++)codps[i].active=0;for(int i=0;i<N-2;i++)codfd[i].active=0;for(int i=0;i<N-4;i++)codwt[i].active=0;for(int i=0;i<N-6;i++)codhs[i].active=0;for(int i=0;i<N-6;i++)codms[i].active=0;init=1;ps("[COD] Cod initialized\n");return 0;}
int cod_stocking(int lc,int bl,int bw,int fp,int o3,int wt,int hw){return add(codps,&st.n_stock,&st.t_ln,N,lc,bl,bw,fp,o3,wt,hw);}
int cod_feeding(int lc,int bl,int bw,int fp,int o3,int wt,int hw){return add(codfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fp,o3,wt,hw);}
int cod_water_mgmt(int lc,int bl,int bw,int fp,int o3,int wt,int hw){return add(codwt,&st.n_water,&st.t_fl,N-4,lc,bl,bw,fp,o3,wt,hw);}
int cod_harvest(int lc,int bl,int bw,int fp,int o3,int wt,int hw){return add(codhs,&st.n_harv,&st.t_omg,N-6,lc,bl,bw,fp,o3,wt,hw);}
int cod_market(int lc,int bl,int bw,int fp,int o3,int wt,int hw){return add(codms,&st.n_mkt,&st.t_tmp,N-6,lc,bl,bw,fp,o3,wt,hw);}
void cod_report(void){ps("[COD] Stock: ");pi(st.n_stock);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nWater: ");pi(st.n_water);ps(" Fl=");pi(st.t_fl);ps("\nHarv: ");pi(st.n_harv);ps(" Omg=");pi(st.t_omg);ps("\nMkt: ");pi(st.n_mkt);ps(" Tmp=");pi(st.t_tmp);ps("\n");}
void cod_state(void){ps("[COD] Stock=");pi(st.n_stock);ps(" Feed=");pi(st.n_feed);ps(" Water=");pi(st.n_water);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cod Admin Demo ===\n\n");cod_init();
/* 1=ocean_pen 2=coastal 3=tank 4=fjord 5=market */
ps("Cod stocking...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*8),bw=2+(i*2),fp=50+(i*3),o3=500+(i*100),wt=8+(i%6),hw=16+(i%8);cod_stocking(lc,bl,bw,fp,o3,wt,hw);}
ps("\nCod feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=45+(i*7),bw=3+(i*2),fp=52+(i*3),o3=520+(i*90),wt=9+(i%5),hw=18+(i%6);cod_feeding(lc,bl,bw,fp,o3,wt,hw);}
ps("\nCod water management...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=50+(i*6),bw=4+(i*2),fp=54+(i*2),o3=540+(i*80),wt=10+(i%4),hw=20+(i%5);cod_water_mgmt(lc,bl,bw,fp,o3,wt,hw);}
ps("\nCod harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=35+(i*10),bw=1+(i*3),fp=48+(i*4),o3=480+(i*120),wt=7+(i%8),hw=14+(i%10);cod_harvest(lc,bl,bw,fp,o3,wt,hw);}
ps("\nCod market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=55+(i*5),bw=5+(i*2),fp=56+(i*2),o3=560+(i*60),wt=11+(i%3),hw=22+(i%4);cod_market(lc,bl,bw,fp,o3,wt,hw);}
ps("\n");cod_report();cod_state();ps("\n=== Demo Complete ===\n");return 0;}
