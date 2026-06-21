/* hoya_admin: Hoya management technology administration (v1.0)
 * Hoya planning, hoya execution, hoya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hoy_t;
typedef struct{int n_hop,n_hoe,n_hov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hoy_state_t;
static hoy_t hopsy[N],hoes[N-2],hovs[N-4],acs[N-6],mks[N-6]; static hoy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hoy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hoy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hoy_init(void){if(init)return -1;st.n_hop=0;st.n_hoe=0;st.n_hov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hopsy[i].active=0;for(int i=0;i<N-2;i++)hoes[i].active=0;for(int i=0;i<N-4;i++)hovs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HOY] Hoya initialized\n");return 0;}
int hoy_planning(int t,int c,int a,int b,int d,int e,int y){return add(hopsy,&st.n_hop,&st.t_f1,N,t,c,a,b,d,e,y);}
int hoy_execution(int t,int c,int a,int b,int d,int e,int y){return add(hoes,&st.n_hoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hoy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hovs,&st.n_hov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hoy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hoy_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hoy_report(void){ps("[HOY] Hop: ");pi(st.n_hop);ps(" PCS=");pi(st.t_f1);ps("\nHoe: ");pi(st.n_hoe);ps(" PCS=");pi(st.t_f2);ps("\nHov: ");pi(st.n_hov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hoy_state(void){ps("[HOY] Hop=");pi(st.n_hop);ps(" Hoe=");pi(st.n_hoe);ps(" Hov=");pi(st.n_hov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hoya Admin Demo ===\n\n");hoy_init();
ps("Hoya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hoy_planning(t,c,604+(i*17),593+(i*14),573+(i*10),555+(i*6),2020+(i%5));}
ps("\nHoya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hoy_execution(t,c,593+(i*15),582+(i*12),564+(i*8),551+(i*5),2021+(i%4));}
ps("\nHoya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hoy_evaluation(t,c,585+(i*13),574+(i*10),558+(i*7),547+(i*4),2022+(i%3));}
ps("\nHoya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hoy_accessory(t,c,577+(i*11),568+(i*9),554+(i*6),544+(i*3),2023+(i%2));}
ps("\nHoya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hoy_market(t,c,571+(i*9),562+(i*7),549+(i*5),541+(i*3),2024);}
ps("\n");hoy_report();hoy_state();ps("\n=== Demo Complete ===\n");return 0;}
