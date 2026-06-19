/* credit_card_admin: Credit card administration (v1.0)
 * Card issuance, transaction processing, risk management, customer service, marketing analytics
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cca_t;
typedef struct{int n_ci,n_tp,n_rm,n_cs,n_ma,t_f1,t_f2,t_f3,t_f4,t_f5;} cca_state_t;
static cca_t cis[N],tps[N-2],rms[N-4],css[N-6],mas[N-6]; static cca_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cca_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cca_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CCA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cca_init(void){if(init)return -1;st.n_ci=0;st.n_tp=0;st.n_rm=0;st.n_cs=0;st.n_ma=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cis[i].active=0;for(int i=0;i<N-2;i++)tps[i].active=0;for(int i=0;i<N-4;i++)rms[i].active=0;for(int i=0;i<N-6;i++)css[i].active=0;for(int i=0;i<N-6;i++)mas[i].active=0;init=1;ps("[CCA] Credit card initialized\n");return 0;}
int cca_issue(int t,int c,int a,int b,int d,int e,int y){return add(cis,&st.n_ci,&st.t_f1,N,t,c,a,b,d,e,y);}
int cca_transaction(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cca_risk(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cca_service(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cca_marketing(int t,int c,int a,int b,int d,int e,int y){return add(mas,&st.n_ma,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cca_report(void){ps("[CCA] Issue: ");pi(st.n_ci);ps(" Card=");pi(st.t_f1);ps("\nTrans: ");pi(st.n_tp);ps(" Vol=");pi(st.t_f2);ps("\nRisk: ");pi(st.n_rm);ps(" Fraud=");pi(st.t_f3);ps("\nServ: ");pi(st.n_cs);ps(" Sat=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_ma);ps(" ROI=");pi(st.t_f5);ps("\n");}
void cca_state(void){ps("[CCA] Ci=");pi(st.n_ci);ps(" Tp=");pi(st.n_tp);ps(" Rm=");pi(st.n_rm);ps(" Cs=");pi(st.n_cs);ps(" Ma=");pi(st.n_ma);ps("\n");}
int main(void){
ps("=== Credit Card Admin Demo ===\n\n");cca_init();
ps("Card issuance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cca_issue(t,c,127+(i*17),112+(i*14),92+(i*10),75+(i*6),2020+(i%5));}
ps("\nTransaction processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cca_transaction(t,c,116+(i*15),102+(i*12),84+(i*8),71+(i*5),2021+(i%4));}
ps("\nRisk management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cca_risk(t,c,108+(i*13),94+(i*10),78+(i*7),67+(i*4),2022+(i%3));}
ps("\nCustomer service...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cca_service(t,c,100+(i*11),88+(i*9),74+(i*6),64+(i*3),2023+(i%2));}
ps("\nMarketing analytics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cca_marketing(t,c,94+(i*9),83+(i*7),70+(i*5),62+(i*3),2024);}
ps("\n");cca_report();cca_state();ps("\n=== Demo Complete ===\n");return 0;}
