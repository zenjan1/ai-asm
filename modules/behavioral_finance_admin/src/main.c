/* behavioral_finance_admin: Behavioral finance administration (v1.0)
 * Cognitive biases, prospect theory, market anomalies, investor behavior, behavioral corporate finance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bfn_t;
typedef struct{int n_cb,n_pt,n_ma,n_ib,n_cf,t_f1,t_f2,t_f3,t_f4,t_f5;} bfn_state_t;
static bfn_t cbs[N],pts[N-2],mas[N-4],ibs[N-6],cfs[N-6]; static bfn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bfn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bfn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BFN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bfn_init(void){if(init)return -1;st.n_cb=0;st.n_pt=0;st.n_ma=0;st.n_ib=0;st.n_cf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cbs[i].active=0;for(int i=0;i<N-2;i++)pts[i].active=0;for(int i=0;i<N-4;i++)mas[i].active=0;for(int i=0;i<N-6;i++)ibs[i].active=0;for(int i=0;i<N-6;i++)cfs[i].active=0;init=1;ps("[BFN] Behavioral finance initialized\n");return 0;}
int bfn_bias(int t,int c,int a,int b,int d,int e,int y){return add(cbs,&st.n_cb,&st.t_f1,N,t,c,a,b,d,e,y);}
int bfn_prospect(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bfn_anomaly(int t,int c,int a,int b,int d,int e,int y){return add(mas,&st.n_ma,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bfn_investor(int t,int c,int a,int b,int d,int e,int y){return add(ibs,&st.n_ib,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bfn_corporate(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bfn_report(void){ps("[BFN] Bias: ");pi(st.n_cb);ps(" OverC=");pi(st.t_f1);ps("\nProspect: ");pi(st.n_pt);ps(" LossA=");pi(st.t_f2);ps("\nAnomaly: ");pi(st.n_ma);ps(" Calen=");pi(st.t_f3);ps("\nInvest: ");pi(st.n_ib);ps(" Herd=");pi(st.t_f4);ps("\nCorp: ");pi(st.n_cf);ps(" IPO=");pi(st.t_f5);ps("\n");}
void bfn_state(void){ps("[BFN] Cb=");pi(st.n_cb);ps(" Pt=");pi(st.n_pt);ps(" Ma=");pi(st.n_ma);ps(" Ib=");pi(st.n_ib);ps(" Cf=");pi(st.n_cf);ps("\n");}
int main(void){
ps("=== Behavioral Finance Admin Demo ===\n\n");bfn_init();
ps("Cognitive biases...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bfn_bias(t,c,98+(i*17),83+(i*14),63+(i*10),46+(i*6),2020+(i%5));}
ps("\nProspect theory...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bfn_prospect(t,c,87+(i*15),73+(i*12),55+(i*8),42+(i*5),2021+(i%4));}
ps("\nMarket anomalies...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bfn_anomaly(t,c,79+(i*13),65+(i*10),49+(i*7),38+(i*4),2022+(i%3));}
ps("\nInvestor behavior...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bfn_investor(t,c,71+(i*11),59+(i*9),45+(i*6),35+(i*3),2023+(i%2));}
ps("\nBehavioral corporate finance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bfn_corporate(t,c,65+(i*9),54+(i*7),41+(i*5),33+(i*3),2024);}
ps("\n");bfn_report();bfn_state();ps("\n=== Demo Complete ===\n");return 0;}
