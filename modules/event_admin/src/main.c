/* event_admin: Event management technology administration (v1.0)
 * Event planning, event execution, event evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} evx_t;
typedef struct{int n_ep,n_ee,n_ev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} evx_state_t;
static evx_t eps[N],ees[N-2],evs[N-4],acs[N-6],mks[N-6]; static evx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(evx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;evx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EVX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int evx_init(void){if(init)return -1;st.n_ep=0;st.n_ee=0;st.n_ev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eps[i].active=0;for(int i=0;i<N-2;i++)ees[i].active=0;for(int i=0;i<N-4;i++)evs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EVX] Event initialized\n");return 0;}
int evx_planning(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f1,N,t,c,a,b,d,e,y);}
int evx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ees,&st.n_ee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int evx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(evs,&st.n_ev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int evx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int evx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void evx_report(void){ps("[EVX] Ep: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f1);ps("\nEe: ");pi(st.n_ee);ps(" PCS=");pi(st.t_f2);ps("\nEv: ");pi(st.n_ev);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void evx_state(void){ps("[EVX] Ep=");pi(st.n_ep);ps(" Ee=");pi(st.n_ee);ps(" Ev=");pi(st.n_ev);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Event Admin Demo ===\n\n");evx_init();
ps("Event planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;evx_planning(t,c,466+(i*17),455+(i*14),435+(i*10),417+(i*6),2020+(i%5));}
ps("\nEvent execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;evx_execution(t,c,455+(i*15),444+(i*12),426+(i*8),413+(i*5),2021+(i%4));}
ps("\nEvent evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;evx_evaluation(t,c,447+(i*13),436+(i*10),420+(i*7),409+(i*4),2022+(i%3));}
ps("\nEvent accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;evx_accessory(t,c,439+(i*11),430+(i*9),416+(i*6),406+(i*3),2023+(i%2));}
ps("\nEvent marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;evx_market(t,c,433+(i*9),424+(i*7),411+(i*5),403+(i*3),2024);}
ps("\n");evx_report();evx_state();ps("\n=== Demo Complete ===\n");return 0;}
