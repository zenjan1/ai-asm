/* intelligent_admin: Intelligent management technology administration (v1.0)
 * Intelligent planning, intelligent execution, intelligent evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ilx_t;
typedef struct{int n_ip,n_ie,n_iv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ilx_state_t;
static ilx_t ips[N],ies[N-2],ivs[N-4],acs[N-6],mks[N-6]; static ilx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ilx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ilx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ILX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ilx_init(void){if(init)return -1;st.n_ip=0;st.n_ie=0;st.n_iv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ips[i].active=0;for(int i=0;i<N-2;i++)ies[i].active=0;for(int i=0;i<N-4;i++)ivs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ILX] Intelligent initialized\n");return 0;}
int ilx_planning(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ilx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ies,&st.n_ie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ilx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ivs,&st.n_iv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ilx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ilx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ilx_report(void){ps("[ILX] Ip: ");pi(st.n_ip);ps(" PCS=");pi(st.t_f1);ps("\nIe: ");pi(st.n_ie);ps(" PCS=");pi(st.t_f2);ps("\nIv: ");pi(st.n_iv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ilx_state(void){ps("[ILX] Ip=");pi(st.n_ip);ps(" Ie=");pi(st.n_ie);ps(" Iv=");pi(st.n_iv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Intelligent Admin Demo ===\n\n");ilx_init();
ps("Intelligent planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ilx_planning(t,c,482+(i*17),471+(i*14),451+(i*10),433+(i*6),2020+(i%5));}
ps("\nIntelligent execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ilx_execution(t,c,471+(i*15),460+(i*12),442+(i*8),429+(i*5),2021+(i%4));}
ps("\nIntelligent evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ilx_evaluation(t,c,463+(i*13),452+(i*10),436+(i*7),425+(i*4),2022+(i%3));}
ps("\nIntelligent accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ilx_accessory(t,c,455+(i*11),446+(i*9),432+(i*6),422+(i*3),2023+(i%2));}
ps("\nIntelligent marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ilx_market(t,c,449+(i*9),440+(i*7),427+(i*5),419+(i*3),2024);}
ps("\n");ilx_report();ilx_state();ps("\n=== Demo Complete ===\n");return 0;}
