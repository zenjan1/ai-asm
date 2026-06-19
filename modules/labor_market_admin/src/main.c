/* labor_market_admin: Labor market administration system (v1.0)
 * Labor supply, labor demand, employment theory, human capital, labor policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lab_t;
typedef struct{int n_ls,n_ld,n_et,n_hc,n_lp,t_f1,t_f2,t_f3,t_f4,t_f5;} lab_state_t;
static lab_t lss[N],lds[N-2],ets[N-4],hcs[N-6],lps[N-6]; static lab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lab_init(void){if(init)return -1;st.n_ls=0;st.n_ld=0;st.n_et=0;st.n_hc=0;st.n_lp=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lss[i].active=0;for(int i=0;i<N-2;i++)lds[i].active=0;for(int i=0;i<N-4;i++)ets[i].active=0;for(int i=0;i<N-6;i++)hcs[i].active=0;for(int i=0;i<N-6;i++)lps[i].active=0;init=1;ps("[LAB] Labor market initialized\n");return 0;}
int lab_supply(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f1,N,t,c,a,b,d,e,y);}
int lab_demand(int t,int c,int a,int b,int d,int e,int y){return add(lds,&st.n_ld,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lab_employment(int t,int c,int a,int b,int d,int e,int y){return add(ets,&st.n_et,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lab_humancap(int t,int c,int a,int b,int d,int e,int y){return add(hcs,&st.n_hc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lab_policy(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lab_report(void){ps("[LAB] Supply: ");pi(st.n_ls);ps(" Work=");pi(st.t_f1);ps("\nDemand: ");pi(st.n_ld);ps(" Wage=");pi(st.t_f2);ps("\nEmploy: ");pi(st.n_et);ps(" Unemp=");pi(st.t_f3);ps("\nHumanCap: ");pi(st.n_hc);ps(" Edu=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_lp);ps(" Protect=");pi(st.t_f5);ps("\n");}
void lab_state(void){ps("[LAB] Ls=");pi(st.n_ls);ps(" Ld=");pi(st.n_ld);ps(" Et=");pi(st.n_et);ps(" Hc=");pi(st.n_hc);ps(" Lp=");pi(st.n_lp);ps("\n");}
int main(void){
ps("=== Labor Market Admin Demo ===\n\n");lab_init();
ps("Labor supply...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lab_supply(t,c,83+(i*18),69+(i*15),49+(i*11),32+(i*7),2020+(i%5));}
ps("\nLabor demand...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lab_demand(t,c,73+(i*16),59+(i*13),41+(i*9),28+(i*6),2021+(i%4));}
ps("\nEmployment theory...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lab_employment(t,c,65+(i*14),51+(i*11),35+(i*7),24+(i*4),2022+(i%3));}
ps("\nHuman capital...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lab_humancap(t,c,57+(i*12),45+(i*9),31+(i*6),21+(i*3),2023+(i%2));}
ps("\nLabor policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lab_policy(t,c,51+(i*10),40+(i*8),27+(i*5),19+(i*3),2024);}
ps("\n");lab_report();lab_state();ps("\n=== Demo Complete ===\n");return 0;}
