/* diatom_admin: Diatom (Bacillariophyta) microalgae cultivation (v1.0)
 * Diatom cultivation, feeding, harvest, processing, market
 * Features: cell_um, shell_dia_nm, silica_idx, chloro_lvl, oil_pct, colony_ct
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,cell_um,sh_dia,sl_idx,chl_lv,oil_pct,cl_ct,active;} diat_t;
typedef struct{int n_cult,n_feed,n_harv,n_proc,n_mkt,t_cl,t_sh,t_sl,t_ch,t_oil;} diat_state_t;
static diat_t dct[N],dfd[N-2],dhv[N-4],dpc[N-6],dmk[N-6]; static diat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(diat_t*a,int*cnt,int*sum,int mx,int lc,int cu,int sd,int si,int cl,int op,int cc){if(*cnt>=mx)return -1;diat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->cell_um=cu;x->sh_dia=sd;x->sl_idx=si;x->chl_lv=cl;x->oil_pct=op;x->cl_ct=cc;x->active=1;*sum+=cu;(*cnt)++;ps("[DIAT] Diatom ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" cu=");pi(cu);ps(" sd=");pi(sd);ps(" si=");pi(si);ps(" cl=");pi(cl);ps(" op=");pi(op);ps(" cc=");pi(cc);ps("\n");return *cnt-1;}
int diat_init(void){if(init)return -1;st.n_cult=0;st.n_feed=0;st.n_harv=0;st.n_proc=0;st.n_mkt=0;st.t_cl=0;st.t_sh=0;st.t_sl=0;st.t_ch=0;st.t_oil=0;for(int i=0;i<N;i++)dct[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dhv[i].active=0;for(int i=0;i<N-6;i++)dpc[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DIAT] Diatom initialized\n");return 0;}
int diat_cultivation(int lc,int cu,int sd,int si,int cl,int op,int cc){return add(dct,&st.n_cult,&st.t_cl,N,lc,cu,sd,si,cl,op,cc);}
int diat_feeding(int lc,int cu,int sd,int si,int cl,int op,int cc){return add(dfd,&st.n_feed,&st.t_sh,N-2,lc,cu,sd,si,cl,op,cc);}
int diat_harvest(int lc,int cu,int sd,int si,int cl,int op,int cc){return add(dhv,&st.n_harv,&st.t_sl,N-4,lc,cu,sd,si,cl,op,cc);}
int diat_processing(int lc,int cu,int sd,int si,int cl,int op,int cc){return add(dpc,&st.n_proc,&st.t_ch,N-6,lc,cu,sd,si,cl,op,cc);}
int diat_market(int lc,int cu,int sd,int si,int cl,int op,int cc){return add(dmk,&st.n_mkt,&st.t_oil,N-6,lc,cu,sd,si,cl,op,cc);}
void diat_report(void){ps("[DIAT] Cult: ");pi(st.n_cult);ps(" Cl=");pi(st.t_cl);ps("\nFeed: ");pi(st.n_feed);ps(" Sh=");pi(st.t_sh);ps("\nHarv: ");pi(st.n_harv);ps(" Sl=");pi(st.t_sl);ps("\nProc: ");pi(st.n_proc);ps(" Ch=");pi(st.t_ch);ps("\nMkt: ");pi(st.n_mkt);ps(" Oil=");pi(st.t_oil);ps("\n");}
void diat_state(void){ps("[DIAT] Cult=");pi(st.n_cult);ps(" Feed=");pi(st.n_feed);ps(" Harv=");pi(st.n_harv);ps(" Proc=");pi(st.n_proc);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Diatom Admin Demo ===\n\n");diat_init();
/* 1=pond 2=tank 3=bioreactor 4=lake 5=market */
ps("Diatom cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,cu=10+(i*3),sd=50+(i*10),si=(i%8)+1,cl=(i%6)+1,op=5+(i*2),cc=100+(i*50);diat_cultivation(lc,cu,sd,si,cl,op,cc);}
ps("\nDiatom feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,cu=12+(i*2),sd=55+(i*8),si=(i%7)+1,cl=(i%5)+1,op=6+(i*2),cc=120+(i*40);diat_feeding(lc,cu,sd,si,cl,op,cc);}
ps("\nDiatom harvest...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,cu=15+(i*2),sd=60+(i*7),si=(i%6)+1,cl=(i%4)+1,op=8+(i*2),cc=150+(i*30);diat_harvest(lc,cu,sd,si,cl,op,cc);}
ps("\nDiatom processing...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,cu=8+(i*4),sd=45+(i*12),si=(i%9)+1,cl=(i%7)+1,op=4+(i*3),cc=80+(i*60);diat_processing(lc,cu,sd,si,cl,op,cc);}
ps("\nDiatom market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,cu=18+(i*2),sd=70+(i*5),si=(i%4)+4,cl=(i%3)+4,op=10+(i*2),cc=200+(i*25);diat_market(lc,cu,sd,si,cl,op,cc);}
ps("\n");diat_report();diat_state();ps("\n=== Demo Complete ===\n");return 0;}
