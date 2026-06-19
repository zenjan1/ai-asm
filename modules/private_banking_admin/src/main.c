/* private_banking_admin: Private banking administration (v1.0)
 * HNW services, investment management, trust services, tax consulting, value-added services
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pba_t;
typedef struct{int n_hs,n_im,n_ts,n_tc,n_vs,t_f1,t_f2,t_f3,t_f4,t_f5;} pba_state_t;
static pba_t hss[N],ims[N-2],tss[N-4],tcs[N-6],vss[N-6]; static pba_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pba_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pba_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PBA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pba_init(void){if(init)return -1;st.n_hs=0;st.n_im=0;st.n_ts=0;st.n_tc=0;st.n_vs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hss[i].active=0;for(int i=0;i<N-2;i++)ims[i].active=0;for(int i=0;i<N-4;i++)tss[i].active=0;for(int i=0;i<N-6;i++)tcs[i].active=0;for(int i=0;i<N-6;i++)vss[i].active=0;init=1;ps("[PBA] Private banking initialized\n");return 0;}
int pba_hnw(int t,int c,int a,int b,int d,int e,int y){return add(hss,&st.n_hs,&st.t_f1,N,t,c,a,b,d,e,y);}
int pba_invest(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pba_trust(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pba_tax(int t,int c,int a,int b,int d,int e,int y){return add(tcs,&st.n_tc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pba_value(int t,int c,int a,int b,int d,int e,int y){return add(vss,&st.n_vs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pba_report(void){ps("[PBA] HNW: ");pi(st.n_hs);ps(" UHNW=");pi(st.t_f1);ps("\nInv: ");pi(st.n_im);ps(" PE=");pi(st.t_f2);ps("\nTrust: ");pi(st.n_ts);ps(" FOT=");pi(st.t_f3);ps("\nTax: ");pi(st.n_tc);ps(" CRS=");pi(st.t_f4);ps("\nValue: ");pi(st.n_vs);ps(" Conc=");pi(st.t_f5);ps("\n");}
void pba_state(void){ps("[PBA] Hs=");pi(st.n_hs);ps(" Im=");pi(st.n_im);ps(" Ts=");pi(st.n_ts);ps(" Tc=");pi(st.n_tc);ps(" Vs=");pi(st.n_vs);ps("\n");}
int main(void){
ps("=== Private Banking Admin Demo ===\n\n");pba_init();
ps("HNW services...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pba_hnw(t,c,132+(i*17),117+(i*14),97+(i*10),80+(i*6),2020+(i%5));}
ps("\nInvestment management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pba_invest(t,c,121+(i*15),107+(i*12),89+(i*8),76+(i*5),2021+(i%4));}
ps("\nTrust services...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pba_trust(t,c,113+(i*13),99+(i*10),83+(i*7),72+(i*4),2022+(i%3));}
ps("\nTax consulting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pba_tax(t,c,105+(i*11),93+(i*9),79+(i*6),69+(i*3),2023+(i%2));}
ps("\nValue-added services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pba_value(t,c,99+(i*9),88+(i*7),75+(i*5),67+(i*3),2024);}
ps("\n");pba_report();pba_state();ps("\n=== Demo Complete ===\n");return 0;}
