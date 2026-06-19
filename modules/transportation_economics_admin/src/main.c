/* transportation_economics_admin: Transportation economics administration (v1.0)
 * Transportation demand, transportation supply, transportation pricing, transportation market, transportation policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tra_t;
typedef struct{int n_td,n_ts,n_tp,n_tm,n_to,t_f1,t_f2,t_f3,t_f4,t_f5;} tra_state_t;
static tra_t tds[N],tss[N-2],tps[N-4],tms[N-6],tos[N-6]; static tra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tra_init(void){if(init)return -1;st.n_td=0;st.n_ts=0;st.n_tp=0;st.n_tm=0;st.n_to=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tds[i].active=0;for(int i=0;i<N-2;i++)tss[i].active=0;for(int i=0;i<N-4;i++)tps[i].active=0;for(int i=0;i<N-6;i++)tms[i].active=0;for(int i=0;i<N-6;i++)tos[i].active=0;init=1;ps("[TRA] Transportation economics initialized\n");return 0;}
int tra_demand(int t,int c,int a,int b,int d,int e,int y){return add(tds,&st.n_td,&st.t_f1,N,t,c,a,b,d,e,y);}
int tra_supply(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tra_pricing(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tra_market(int t,int c,int a,int b,int d,int e,int y){return add(tms,&st.n_tm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tra_policy(int t,int c,int a,int b,int d,int e,int y){return add(tos,&st.n_to,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tra_report(void){ps("[TRA] Demand: ");pi(st.n_td);ps(" Travel=");pi(st.t_f1);ps("\nSupply: ");pi(st.n_ts);ps(" Infra=");pi(st.t_f2);ps("\nPricing: ");pi(st.n_tp);ps(" Toll=");pi(st.t_f3);ps("\nMarket: ");pi(st.n_tm);ps(" Compete=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_to);ps(" Green=");pi(st.t_f5);ps("\n");}
void tra_state(void){ps("[TRA] Td=");pi(st.n_td);ps(" Ts=");pi(st.n_ts);ps(" Tp=");pi(st.n_tp);ps(" Tm=");pi(st.n_tm);ps(" To=");pi(st.n_to);ps("\n");}
int main(void){
ps("=== Transportation Economics Admin Demo ===\n\n");tra_init();
ps("Transportation demand...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tra_demand(t,c,103+(i*17),88+(i*14),68+(i*10),51+(i*6),2020+(i%5));}
ps("\nTransportation supply...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tra_supply(t,c,92+(i*15),78+(i*12),60+(i*8),47+(i*5),2021+(i%4));}
ps("\nTransportation pricing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tra_pricing(t,c,84+(i*13),70+(i*10),54+(i*7),43+(i*4),2022+(i%3));}
ps("\nTransportation market...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tra_market(t,c,76+(i*11),64+(i*9),50+(i*6),40+(i*3),2023+(i%2));}
ps("\nTransportation policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tra_policy(t,c,70+(i*9),59+(i*7),46+(i*5),38+(i*3),2024);}
ps("\n");tra_report();tra_state();ps("\n=== Demo Complete ===\n");return 0;}
