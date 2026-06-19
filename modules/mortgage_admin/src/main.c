/* mortgage_admin: Mortgage administration (v1.0)
 * Loan application, loan products, repayment management, risk management, post-loan management
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mga_t;
typedef struct{int n_la,n_lp,n_rp,n_rm,n_pl,t_f1,t_f2,t_f3,t_f4,t_f5;} mga_state_t;
static mga_t las[N],lps[N-2],rps[N-4],rms[N-6],pls[N-6]; static mga_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mga_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mga_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MGA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mga_init(void){if(init)return -1;st.n_la=0;st.n_lp=0;st.n_rp=0;st.n_rm=0;st.n_pl=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)las[i].active=0;for(int i=0;i<N-2;i++)lps[i].active=0;for(int i=0;i<N-4;i++)rps[i].active=0;for(int i=0;i<N-6;i++)rms[i].active=0;for(int i=0;i<N-6;i++)pls[i].active=0;init=1;ps("[MGA] Mortgage initialized\n");return 0;}
int mga_apply(int t,int c,int a,int b,int d,int e,int y){return add(las,&st.n_la,&st.t_f1,N,t,c,a,b,d,e,y);}
int mga_product(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mga_repay(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mga_risk(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mga_postloan(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mga_report(void){ps("[MGA] Apply: ");pi(st.n_la);ps(" App=");pi(st.t_f1);ps("\nProd: ");pi(st.n_lp);ps(" LTV=");pi(st.t_f2);ps("\nRepay: ");pi(st.n_rp);ps(" PMI=");pi(st.t_f3);ps("\nRisk: ");pi(st.n_rm);ps(" NPL=");pi(st.t_f4);ps("\nPost: ");pi(st.n_pl);ps(" REO=");pi(st.t_f5);ps("\n");}
void mga_state(void){ps("[MGA] La=");pi(st.n_la);ps(" Lp=");pi(st.n_lp);ps(" Rp=");pi(st.n_rp);ps(" Rm=");pi(st.n_rm);ps(" Pl=");pi(st.n_pl);ps("\n");}
int main(void){
ps("=== Mortgage Admin Demo ===\n\n");mga_init();
ps("Loan application...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mga_apply(t,c,128+(i*17),113+(i*14),93+(i*10),76+(i*6),2020+(i%5));}
ps("\nLoan products...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mga_product(t,c,117+(i*15),103+(i*12),85+(i*8),72+(i*5),2021+(i%4));}
ps("\nRepayment management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mga_repay(t,c,109+(i*13),95+(i*10),79+(i*7),68+(i*4),2022+(i%3));}
ps("\nRisk management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mga_risk(t,c,101+(i*11),89+(i*9),75+(i*6),65+(i*3),2023+(i%2));}
ps("\nPost-loan management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mga_postloan(t,c,95+(i*9),84+(i*7),71+(i*5),63+(i*3),2024);}
ps("\n");mga_report();mga_state();ps("\n=== Demo Complete ===\n");return 0;}
