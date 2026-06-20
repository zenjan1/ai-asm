/* emission_admin: Emission management technology administration (v1.0)
 * Emission planning, emission execution, emission evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ems_t;
typedef struct{int n_ep,n_ee,n_ev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ems_state_t;
static ems_t eps[N],ees[N-2],evs[N-4],acs[N-6],mks[N-6]; static ems_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ems_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ems_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EMS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ems_init(void){if(init)return -1;st.n_ep=0;st.n_ee=0;st.n_ev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eps[i].active=0;for(int i=0;i<N-2;i++)ees[i].active=0;for(int i=0;i<N-4;i++)evs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EMS] Emission initialized\n");return 0;}
int ems_planning(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ems_execution(int t,int c,int a,int b,int d,int e,int y){return add(ees,&st.n_ee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ems_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(evs,&st.n_ev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ems_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ems_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ems_report(void){ps("[EMS] Ep: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f1);ps("\nEe: ");pi(st.n_ee);ps(" PCS=");pi(st.t_f2);ps("\nEv: ");pi(st.n_ev);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ems_state(void){ps("[EMS] Ep=");pi(st.n_ep);ps(" Ee=");pi(st.n_ee);ps(" Ev=");pi(st.n_ev);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Emission Admin Demo ===\n\n");ems_init();
ps("Emission planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ems_planning(t,c,497+(i*17),486+(i*14),466+(i*10),448+(i*6),2020+(i%5));}
ps("\nEmission execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ems_execution(t,c,486+(i*15),475+(i*12),457+(i*8),444+(i*5),2021+(i%4));}
ps("\nEmission evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ems_evaluation(t,c,478+(i*13),467+(i*10),451+(i*7),440+(i*4),2022+(i%3));}
ps("\nEmission accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ems_accessory(t,c,470+(i*11),461+(i*9),447+(i*6),437+(i*3),2023+(i%2));}
ps("\nEmission marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ems_market(t,c,464+(i*9),455+(i*7),442+(i*5),434+(i*3),2024);}
ps("\n");ems_report();ems_state();ps("\n=== Demo Complete ===\n");return 0;}
