/* factoring_admin: Factoring administration (v1.0)
 * Accounts receivable, factoring finance, credit assessment, risk management, factoring accounting
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fac_t;
typedef struct{int n_ar,n_ff,n_ca,n_rm,n_fa,t_f1,t_f2,t_f3,t_f4,t_f5;} fac_state_t;
static fac_t ars[N],ffs[N-2],cas[N-4],rms[N-6],fas[N-6]; static fac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fac_init(void){if(init)return -1;st.n_ar=0;st.n_ff=0;st.n_ca=0;st.n_rm=0;st.n_fa=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ars[i].active=0;for(int i=0;i<N-2;i++)ffs[i].active=0;for(int i=0;i<N-4;i++)cas[i].active=0;for(int i=0;i<N-6;i++)rms[i].active=0;for(int i=0;i<N-6;i++)fas[i].active=0;init=1;ps("[FAC] Factoring initialized\n");return 0;}
int fac_receivable(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f1,N,t,c,a,b,d,e,y);}
int fac_finance(int t,int c,int a,int b,int d,int e,int y){return add(ffs,&st.n_ff,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fac_credit(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fac_risk(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fac_accounting(int t,int c,int a,int b,int d,int e,int y){return add(fas,&st.n_fa,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fac_report(void){ps("[FAC] AR: ");pi(st.n_ar);ps(" Recv=");pi(st.t_f1);ps("\nFin: ");pi(st.n_ff);ps(" Rate=");pi(st.t_f2);ps("\nCredit: ");pi(st.n_ca);ps(" Score=");pi(st.t_f3);ps("\nRisk: ");pi(st.n_rm);ps(" Loss=");pi(st.t_f4);ps("\nAcc: ");pi(st.n_fa);ps(" Bad=");pi(st.t_f5);ps("\n");}
void fac_state(void){ps("[FAC] Ar=");pi(st.n_ar);ps(" Ff=");pi(st.n_ff);ps(" Ca=");pi(st.n_ca);ps(" Rm=");pi(st.n_rm);ps(" Fa=");pi(st.n_fa);ps("\n");}
int main(void){
ps("=== Factoring Admin Demo ===\n\n");fac_init();
ps("Accounts receivable...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fac_receivable(t,c,124+(i*17),109+(i*14),89+(i*10),72+(i*6),2020+(i%5));}
ps("\nFactoring finance...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fac_finance(t,c,113+(i*15),99+(i*12),81+(i*8),68+(i*5),2021+(i%4));}
ps("\nCredit assessment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fac_credit(t,c,105+(i*13),91+(i*10),75+(i*7),64+(i*4),2022+(i%3));}
ps("\nRisk management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fac_risk(t,c,97+(i*11),85+(i*9),71+(i*6),61+(i*3),2023+(i%2));}
ps("\nFactoring accounting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fac_accounting(t,c,91+(i*9),80+(i*7),67+(i*5),59+(i*3),2024);}
ps("\n");fac_report();fac_state();ps("\n=== Demo Complete ===\n");return 0;}
