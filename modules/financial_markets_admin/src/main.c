/* financial_markets_admin: Financial markets administration (v1.0)
 * Money market, capital market, derivatives market, forex market, market infrastructure
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fma_t;
typedef struct{int n_mm,n_cm,n_dm,n_fx,n_inf,t_f1,t_f2,t_f3,t_f4,t_f5;} fma_state_t;
static fma_t mms[N],cms[N-2],dms[N-4],fxs[N-6],infs[N-6]; static fma_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fma_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fma_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FMA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fma_init(void){if(init)return -1;st.n_mm=0;st.n_cm=0;st.n_dm=0;st.n_fx=0;st.n_inf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mms[i].active=0;for(int i=0;i<N-2;i++)cms[i].active=0;for(int i=0;i<N-4;i++)dms[i].active=0;for(int i=0;i<N-6;i++)fxs[i].active=0;for(int i=0;i<N-6;i++)infs[i].active=0;init=1;ps("[FMA] Financial markets initialized\n");return 0;}
int fma_money(int t,int c,int a,int b,int d,int e,int y){return add(mms,&st.n_mm,&st.t_f1,N,t,c,a,b,d,e,y);}
int fma_capital(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fma_deriv(int t,int c,int a,int b,int d,int e,int y){return add(dms,&st.n_dm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fma_forex(int t,int c,int a,int b,int d,int e,int y){return add(fxs,&st.n_fx,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fma_infra(int t,int c,int a,int b,int d,int e,int y){return add(infs,&st.n_inf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fma_report(void){ps("[FMA] Money: ");pi(st.n_mm);ps(" Libor=");pi(st.t_f1);ps("\nCapital: ");pi(st.n_cm);ps(" IPO=");pi(st.t_f2);ps("\nDeriv: ");pi(st.n_dm);ps(" BS=");pi(st.t_f3);ps("\nForex: ");pi(st.n_fx);ps(" Spot=");pi(st.t_f4);ps("\nInfra: ");pi(st.n_inf);ps(" CCP=");pi(st.t_f5);ps("\n");}
void fma_state(void){ps("[FMA] Mm=");pi(st.n_mm);ps(" Cm=");pi(st.n_cm);ps(" Dm=");pi(st.n_dm);ps(" Fx=");pi(st.n_fx);ps(" Inf=");pi(st.n_inf);ps("\n");}
int main(void){
ps("=== Financial Markets Admin Demo ===\n\n");fma_init();
ps("Money market...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fma_money(t,c,116+(i*17),101+(i*14),81+(i*10),64+(i*6),2020+(i%5));}
ps("\nCapital market...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fma_capital(t,c,105+(i*15),91+(i*12),73+(i*8),60+(i*5),2021+(i%4));}
ps("\nDerivatives market...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fma_deriv(t,c,97+(i*13),83+(i*10),67+(i*7),56+(i*4),2022+(i%3));}
ps("\nForex market...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fma_forex(t,c,89+(i*11),77+(i*9),63+(i*6),53+(i*3),2023+(i%2));}
ps("\nMarket infrastructure...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fma_infra(t,c,83+(i*9),72+(i*7),59+(i*5),51+(i*3),2024);}
ps("\n");fma_report();fma_state();ps("\n=== Demo Complete ===\n");return 0;}
