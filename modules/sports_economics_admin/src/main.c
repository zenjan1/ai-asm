/* sports_economics_admin: Sports economics administration (v1.0)
 * Sports demand, sports supply, sports market, sports industry, sports policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spo_t;
typedef struct{int n_sd,n_ss,n_sm,n_si,n_so,t_f1,t_f2,t_f3,t_f4,t_f5;} spo_state_t;
static spo_t sds[N],sss[N-2],sms[N-4],sis[N-6],sos[N-6]; static spo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spo_init(void){if(init)return -1;st.n_sd=0;st.n_ss=0;st.n_sm=0;st.n_si=0;st.n_so=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sds[i].active=0;for(int i=0;i<N-2;i++)sss[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)sis[i].active=0;for(int i=0;i<N-6;i++)sos[i].active=0;init=1;ps("[SPO] Sports economics initialized\n");return 0;}
int spo_demand(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f1,N,t,c,a,b,d,e,y);}
int spo_supply(int t,int c,int a,int b,int d,int e,int y){return add(sss,&st.n_ss,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spo_market(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spo_industry(int t,int c,int a,int b,int d,int e,int y){return add(sis,&st.n_si,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spo_policy(int t,int c,int a,int b,int d,int e,int y){return add(sos,&st.n_so,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spo_report(void){ps("[SPO] Demand: ");pi(st.n_sd);ps(" Partic=");pi(st.t_f1);ps("\nSupply: ");pi(st.n_ss);ps(" Facility=");pi(st.t_f2);ps("\nMarket: ");pi(st.n_sm);ps(" Media=");pi(st.t_f3);ps("\nIndustry: ");pi(st.n_si);ps(" League=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_so);ps(" Olympic=");pi(st.t_f5);ps("\n");}
void spo_state(void){ps("[SPO] Sd=");pi(st.n_sd);ps(" Ss=");pi(st.n_ss);ps(" Sm=");pi(st.n_sm);ps(" Si=");pi(st.n_si);ps(" So=");pi(st.n_so);ps("\n");}
int main(void){
ps("=== Sports Economics Admin Demo ===\n\n");spo_init();
ps("Sports demand...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spo_demand(t,c,108+(i*17),93+(i*14),73+(i*10),56+(i*6),2020+(i%5));}
ps("\nSports supply...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spo_supply(t,c,97+(i*15),83+(i*12),65+(i*8),52+(i*5),2021+(i%4));}
ps("\nSports market...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spo_market(t,c,89+(i*13),75+(i*10),59+(i*7),48+(i*4),2022+(i%3));}
ps("\nSports industry...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spo_industry(t,c,81+(i*11),69+(i*9),55+(i*6),45+(i*3),2023+(i%2));}
ps("\nSports policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spo_policy(t,c,75+(i*9),64+(i*7),51+(i*5),43+(i*3),2024);}
ps("\n");spo_report();spo_state();ps("\n=== Demo Complete ===\n");return 0;}
