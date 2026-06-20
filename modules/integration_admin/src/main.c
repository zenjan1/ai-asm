/* integration_admin: Integration management technology administration (v1.0)
 * Integration planning, integration execution, integration testing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} itx_t;
typedef struct{int n_ip,n_ie,n_it,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} itx_state_t;
static itx_t ips[N],ies[N-2],its[N-4],acs[N-6],mks[N-6]; static itx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(itx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;itx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ITX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int itx_init(void){if(init)return -1;st.n_ip=0;st.n_ie=0;st.n_it=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ips[i].active=0;for(int i=0;i<N-2;i++)ies[i].active=0;for(int i=0;i<N-4;i++)its[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ITX] Integration initialized\n");return 0;}
int itx_planning(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f1,N,t,c,a,b,d,e,y);}
int itx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ies,&st.n_ie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int itx_testing(int t,int c,int a,int b,int d,int e,int y){return add(its,&st.n_it,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int itx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int itx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void itx_report(void){ps("[ITX] Ip: ");pi(st.n_ip);ps(" PCS=");pi(st.t_f1);ps("\nIe: ");pi(st.n_ie);ps(" PCS=");pi(st.t_f2);ps("\nIt: ");pi(st.n_it);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void itx_state(void){ps("[ITX] Ip=");pi(st.n_ip);ps(" Ie=");pi(st.n_ie);ps(" It=");pi(st.n_it);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Integration Admin Demo ===\n\n");itx_init();
ps("Integration planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;itx_planning(t,c,472+(i*17),461+(i*14),441+(i*10),423+(i*6),2020+(i%5));}
ps("\nIntegration execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;itx_execution(t,c,461+(i*15),450+(i*12),432+(i*8),419+(i*5),2021+(i%4));}
ps("\nIntegration testing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;itx_testing(t,c,453+(i*13),442+(i*10),426+(i*7),415+(i*4),2022+(i%3));}
ps("\nIntegration accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;itx_accessory(t,c,445+(i*11),436+(i*9),422+(i*6),412+(i*3),2023+(i%2));}
ps("\nIntegration marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;itx_market(t,c,439+(i*9),430+(i*7),417+(i*5),409+(i*3),2024);}
ps("\n");itx_report();itx_state();ps("\n=== Demo Complete ===\n");return 0;}
