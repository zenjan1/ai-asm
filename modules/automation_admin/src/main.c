/* automation_admin: Automation management technology administration (v1.0)
 * Automation planning, automation execution, automation evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} atx_t;
typedef struct{int n_ap,n_ae,n_av,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} atx_state_t;
static atx_t aps[N],aes[N-2],avs[N-4],acs[N-6],mks[N-6]; static atx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(atx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;atx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ATX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int atx_init(void){if(init)return -1;st.n_ap=0;st.n_ae=0;st.n_av=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aps[i].active=0;for(int i=0;i<N-2;i++)aes[i].active=0;for(int i=0;i<N-4;i++)avs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ATX] Automation initialized\n");return 0;}
int atx_planning(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f1,N,t,c,a,b,d,e,y);}
int atx_execution(int t,int c,int a,int b,int d,int e,int y){return add(aes,&st.n_ae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int atx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(avs,&st.n_av,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int atx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int atx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void atx_report(void){ps("[ATX] Ap: ");pi(st.n_ap);ps(" PCS=");pi(st.t_f1);ps("\nAe: ");pi(st.n_ae);ps(" PCS=");pi(st.t_f2);ps("\nAv: ");pi(st.n_av);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void atx_state(void){ps("[ATX] Ap=");pi(st.n_ap);ps(" Ae=");pi(st.n_ae);ps(" Av=");pi(st.n_av);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Automation Admin Demo ===\n\n");atx_init();
ps("Automation planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;atx_planning(t,c,481+(i*17),470+(i*14),450+(i*10),432+(i*6),2020+(i%5));}
ps("\nAutomation execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;atx_execution(t,c,470+(i*15),459+(i*12),441+(i*8),428+(i*5),2021+(i%4));}
ps("\nAutomation evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;atx_evaluation(t,c,462+(i*13),451+(i*10),435+(i*7),424+(i*4),2022+(i%3));}
ps("\nAutomation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atx_accessory(t,c,454+(i*11),445+(i*9),431+(i*6),421+(i*3),2023+(i%2));}
ps("\nAutomation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atx_market(t,c,448+(i*9),439+(i*7),426+(i*5),418+(i*3),2024);}
ps("\n");atx_report();atx_state();ps("\n=== Demo Complete ===\n");return 0;}
