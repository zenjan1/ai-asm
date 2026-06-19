/* international_finance_admin: International finance administration (v1.0)
 * Exchange rate theory, international monetary, international capital, financial architecture, open macro
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} int_t;
typedef struct{int n_er,n_im,n_ic,n_fa,n_om,t_f1,t_f2,t_f3,t_f4,t_f5;} int_state_t;
static int_t ers[N],ims[N-2],ics[N-4],fas[N-6],oms[N-6]; static int_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(int_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;int_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[INT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int int_init(void){if(init)return -1;st.n_er=0;st.n_im=0;st.n_ic=0;st.n_fa=0;st.n_om=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ers[i].active=0;for(int i=0;i<N-2;i++)ims[i].active=0;for(int i=0;i<N-4;i++)ics[i].active=0;for(int i=0;i<N-6;i++)fas[i].active=0;for(int i=0;i<N-6;i++)oms[i].active=0;init=1;ps("[INT] International finance initialized\n");return 0;}
int int_exchange(int t,int c,int a,int b,int d,int e,int y){return add(ers,&st.n_er,&st.t_f1,N,t,c,a,b,d,e,y);}
int int_monetary(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int int_capital(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int int_arch(int t,int c,int a,int b,int d,int e,int y){return add(fas,&st.n_fa,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int int_open(int t,int c,int a,int b,int d,int e,int y){return add(oms,&st.n_om,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void int_report(void){ps("[INT] Exchange: ");pi(st.n_er);ps(" PPP=");pi(st.t_f1);ps("\nMonetary: ");pi(st.n_im);ps(" Bretton=");pi(st.t_f2);ps("\nCapital: ");pi(st.n_ic);ps(" Flow=");pi(st.t_f3);ps("\nArch: ");pi(st.n_fa);ps(" IMF=");pi(st.t_f4);ps("\nOpen: ");pi(st.n_om);ps(" Trilemma=");pi(st.t_f5);ps("\n");}
void int_state(void){ps("[INT] Er=");pi(st.n_er);ps(" Im=");pi(st.n_im);ps(" Ic=");pi(st.n_ic);ps(" Fa=");pi(st.n_fa);ps(" Om=");pi(st.n_om);ps("\n");}
int main(void){
ps("=== International Finance Admin Demo ===\n\n");int_init();
ps("Exchange rate theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;int_exchange(t,c,113+(i*17),98+(i*14),78+(i*10),61+(i*6),2020+(i%5));}
ps("\nInternational monetary...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;int_monetary(t,c,102+(i*15),88+(i*12),70+(i*8),57+(i*5),2021+(i%4));}
ps("\nInternational capital...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;int_capital(t,c,94+(i*13),80+(i*10),64+(i*7),53+(i*4),2022+(i%3));}
ps("\nFinancial architecture...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;int_arch(t,c,86+(i*11),74+(i*9),60+(i*6),50+(i*3),2023+(i%2));}
ps("\nOpen macro...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;int_open(t,c,80+(i*9),69+(i*7),56+(i*5),48+(i*3),2024);}
ps("\n");int_report();int_state();ps("\n=== Demo Complete ===\n");return 0;}
