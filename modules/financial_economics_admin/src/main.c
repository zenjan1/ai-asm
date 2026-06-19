/* financial_economics_admin: Financial economics administration (v1.0)
 * Asset pricing, market microstructure, corporate finance, risk management, derivatives
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fne_t;
typedef struct{int n_ap,n_mm,n_cf,n_rm,n_dv,t_f1,t_f2,t_f3,t_f4,t_f5;} fne_state_t;
static fne_t aps[N],mms[N-2],cfs[N-4],rms[N-6],dvs[N-6]; static fne_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fne_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fne_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FNE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fne_init(void){if(init)return -1;st.n_ap=0;st.n_mm=0;st.n_cf=0;st.n_rm=0;st.n_dv=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aps[i].active=0;for(int i=0;i<N-2;i++)mms[i].active=0;for(int i=0;i<N-4;i++)cfs[i].active=0;for(int i=0;i<N-6;i++)rms[i].active=0;for(int i=0;i<N-6;i++)dvs[i].active=0;init=1;ps("[FNE] Financial economics initialized\n");return 0;}
int fne_pricing(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f1,N,t,c,a,b,d,e,y);}
int fne_micro(int t,int c,int a,int b,int d,int e,int y){return add(mms,&st.n_mm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fne_corporate(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fne_risk(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fne_deriv(int t,int c,int a,int b,int d,int e,int y){return add(dvs,&st.n_dv,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fne_report(void){ps("[FNE] Pricing: ");pi(st.n_ap);ps(" CAPM=");pi(st.t_f1);ps("\nMicro: ");pi(st.n_mm);ps(" Liquid=");pi(st.t_f2);ps("\nCorpFin: ");pi(st.n_cf);ps(" MM=");pi(st.t_f3);ps("\nRisk: ");pi(st.n_rm);ps(" VaR=");pi(st.t_f4);ps("\nDeriv: ");pi(st.n_dv);ps(" BSM=");pi(st.t_f5);ps("\n");}
void fne_state(void){ps("[FNE] Ap=");pi(st.n_ap);ps(" Mm=");pi(st.n_mm);ps(" Cf=");pi(st.n_cf);ps(" Rm=");pi(st.n_rm);ps(" Dv=");pi(st.n_dv);ps("\n");}
int main(void){
ps("=== Financial Economics Admin Demo ===\n\n");fne_init();
ps("Asset pricing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fne_pricing(t,c,85+(i*18),70+(i*15),50+(i*11),33+(i*7),2020+(i%5));}
ps("\nMarket microstructure...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fne_micro(t,c,74+(i*16),60+(i*13),42+(i*9),29+(i*6),2021+(i%4));}
ps("\nCorporate finance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fne_corporate(t,c,66+(i*14),52+(i*11),36+(i*7),25+(i*4),2022+(i%3));}
ps("\nRisk management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fne_risk(t,c,58+(i*12),46+(i*9),32+(i*6),22+(i*3),2023+(i%2));}
ps("\nDerivatives...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fne_deriv(t,c,52+(i*10),41+(i*8),28+(i*5),20+(i*3),2024);}
ps("\n");fne_report();fne_state();ps("\n=== Demo Complete ===\n");return 0;}
