/* treasury_admin: Treasury administration (v1.0)
 * Cash management, liquidity management, funding strategy, FX risk, interest rate risk
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trs_t;
typedef struct{int n_cm,n_lm,n_fs,n_fx,n_ir,t_f1,t_f2,t_f3,t_f4,t_f5;} trs_state_t;
static trs_t cms[N],lms[N-2],fss[N-4],fxs[N-6],irs[N-6]; static trs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trs_init(void){if(init)return -1;st.n_cm=0;st.n_lm=0;st.n_fs=0;st.n_fx=0;st.n_ir=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cms[i].active=0;for(int i=0;i<N-2;i++)lms[i].active=0;for(int i=0;i<N-4;i++)fss[i].active=0;for(int i=0;i<N-6;i++)fxs[i].active=0;for(int i=0;i<N-6;i++)irs[i].active=0;init=1;ps("[TRS] Treasury initialized\n");return 0;}
int trs_cash(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f1,N,t,c,a,b,d,e,y);}
int trs_liquidity(int t,int c,int a,int b,int d,int e,int y){return add(lms,&st.n_lm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trs_funding(int t,int c,int a,int b,int d,int e,int y){return add(fss,&st.n_fs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trs_forex(int t,int c,int a,int b,int d,int e,int y){return add(fxs,&st.n_fx,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trs_rate(int t,int c,int a,int b,int d,int e,int y){return add(irs,&st.n_ir,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trs_report(void){ps("[TRS] Cash: ");pi(st.n_cm);ps(" Pool=");pi(st.t_f1);ps("\nLiq: ");pi(st.n_lm);ps(" LCR=");pi(st.t_f2);ps("\nFund: ");pi(st.n_fs);ps(" WACC=");pi(st.t_f3);ps("\nFX: ");pi(st.n_fx);ps(" Hedge=");pi(st.t_f4);ps("\nRate: ");pi(st.n_ir);ps(" Dur=");pi(st.t_f5);ps("\n");}
void trs_state(void){ps("[TRS] Cm=");pi(st.n_cm);ps(" Lm=");pi(st.n_lm);ps(" Fs=");pi(st.n_fs);ps(" Fx=");pi(st.n_fx);ps(" Ir=");pi(st.n_ir);ps("\n");}
int main(void){
ps("=== Treasury Admin Demo ===\n\n");trs_init();
ps("Cash management...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trs_cash(t,c,122+(i*17),107+(i*14),87+(i*10),70+(i*6),2020+(i%5));}
ps("\nLiquidity management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trs_liquidity(t,c,111+(i*15),97+(i*12),79+(i*8),66+(i*5),2021+(i%4));}
ps("\nFunding strategy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trs_funding(t,c,103+(i*13),89+(i*10),73+(i*7),62+(i*4),2022+(i%3));}
ps("\nFX risk...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trs_forex(t,c,95+(i*11),83+(i*9),69+(i*6),59+(i*3),2023+(i%2));}
ps("\nInterest rate risk...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trs_rate(t,c,89+(i*9),78+(i*7),65+(i*5),57+(i*3),2024);}
ps("\n");trs_report();trs_state();ps("\n=== Demo Complete ===\n");return 0;}
