/* informatization_admin: Informatization management technology administration (v1.0)
 * Informatization planning, informatization execution, informatization evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ifx_t;
typedef struct{int n_ip,n_ie,n_iv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ifx_state_t;
static ifx_t ips[N],ies[N-2],ivs[N-4],acs[N-6],mks[N-6]; static ifx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ifx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ifx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IFX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ifx_init(void){if(init)return -1;st.n_ip=0;st.n_ie=0;st.n_iv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ips[i].active=0;for(int i=0;i<N-2;i++)ies[i].active=0;for(int i=0;i<N-4;i++)ivs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IFX] Informatization initialized\n");return 0;}
int ifx_planning(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ifx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ies,&st.n_ie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ifx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ivs,&st.n_iv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ifx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ifx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ifx_report(void){ps("[IFX] Ip: ");pi(st.n_ip);ps(" PCS=");pi(st.t_f1);ps("\nIe: ");pi(st.n_ie);ps(" PCS=");pi(st.t_f2);ps("\nIv: ");pi(st.n_iv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ifx_state(void){ps("[IFX] Ip=");pi(st.n_ip);ps(" Ie=");pi(st.n_ie);ps(" Iv=");pi(st.n_iv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Informatization Admin Demo ===\n\n");ifx_init();
ps("Informatization planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ifx_planning(t,c,480+(i*17),469+(i*14),449+(i*10),431+(i*6),2020+(i%5));}
ps("\nInformatization execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ifx_execution(t,c,469+(i*15),458+(i*12),440+(i*8),427+(i*5),2021+(i%4));}
ps("\nInformatization evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ifx_evaluation(t,c,461+(i*13),450+(i*10),434+(i*7),423+(i*4),2022+(i%3));}
ps("\nInformatization accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ifx_accessory(t,c,453+(i*11),444+(i*9),430+(i*6),420+(i*3),2023+(i%2));}
ps("\nInformatization marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ifx_market(t,c,447+(i*9),438+(i*7),425+(i*5),417+(i*3),2024);}
ps("\n");ifx_report();ifx_state();ps("\n=== Demo Complete ===\n");return 0;}
