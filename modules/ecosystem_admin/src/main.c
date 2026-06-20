/* ecosystem_admin: Ecosystem management technology administration (v1.0)
 * Ecosystem planning, ecosystem building, ecosystem evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ecm_t;
typedef struct{int n_ep,n_eb,n_evl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ecm_state_t;
static ecm_t eps[N],ebs[N-2],evls[N-4],acs[N-6],mks[N-6]; static ecm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ecm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ecm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ecm_init(void){if(init)return -1;st.n_ep=0;st.n_eb=0;st.n_evl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eps[i].active=0;for(int i=0;i<N-2;i++)ebs[i].active=0;for(int i=0;i<N-4;i++)evls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ECM] Ecosystem initialized\n");return 0;}
int ecm_planning(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ecm_building(int t,int c,int a,int b,int d,int e,int y){return add(ebs,&st.n_eb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ecm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(evls,&st.n_evl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ecm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ecm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ecm_report(void){ps("[ECM] Ep: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f1);ps("\nEb: ");pi(st.n_eb);ps(" PCS=");pi(st.t_f2);ps("\nEvl: ");pi(st.n_evl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ecm_state(void){ps("[ECM] Ep=");pi(st.n_ep);ps(" Eb=");pi(st.n_eb);ps(" Evl=");pi(st.n_evl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ecosystem Admin Demo ===\n\n");ecm_init();
ps("Ecosystem planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ecm_planning(t,c,485+(i*17),474+(i*14),454+(i*10),436+(i*6),2020+(i%5));}
ps("\nEcosystem building...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ecm_building(t,c,474+(i*15),463+(i*12),445+(i*8),432+(i*5),2021+(i%4));}
ps("\nEcosystem evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ecm_evaluation(t,c,466+(i*13),455+(i*10),439+(i*7),428+(i*4),2022+(i%3));}
ps("\nEcosystem accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecm_accessory(t,c,458+(i*11),449+(i*9),435+(i*6),425+(i*3),2023+(i%2));}
ps("\nEcosystem marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ecm_market(t,c,452+(i*9),443+(i*7),430+(i*5),422+(i*3),2024);}
ps("\n");ecm_report();ecm_state();ps("\n=== Demo Complete ===\n");return 0;}
