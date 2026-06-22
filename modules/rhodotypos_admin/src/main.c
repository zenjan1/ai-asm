/* rhodotypos_admin: Rhodotypos management technology administration (v1.0)
 * Rhodotypos planning, rhodotypos execution, rhodotypos evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rho_t;
typedef struct{int n_rhop,n_rhoe,n_rho2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rho_state_t;
static rho_t rhops[N],rhoss[N-2],rhovss[N-4],rhoas[N-6],rhomks[N-6]; static rho_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rho_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rho_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rho_init(void){if(init)return -1;st.n_rhop=0;st.n_rhoe=0;st.n_rho2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhops[i].active=0;for(int i=0;i<N-2;i++)rhoss[i].active=0;for(int i=0;i<N-4;i++)rhovss[i].active=0;for(int i=0;i<N-6;i++)rhoas[i].active=0;for(int i=0;i<N-6;i++)rhomks[i].active=0;init=1;ps("[RHO] Rhodotypos initialized\n");return 0;}
int rho_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhops,&st.n_rhop,&st.t_f1,N,t,c,a,b,d,e,y);}
int rho_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhoss,&st.n_rhoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rho_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhovss,&st.n_rho2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rho_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rho_market(int t,int c,int a,int b,int d,int e,int y){return add(rhomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rho_report(void){ps("[RHO] Rhopp: ");pi(st.n_rhop);ps(" PCS=");pi(st.t_f1);ps("\nRhoe: ");pi(st.n_rhoe);ps(" PCS=");pi(st.t_f2);ps("\nRhov: ");pi(st.n_rho2);ps(" PCS=");pi(st.t_f3);ps("\nRhoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rho_state(void){ps("[RHO] Rhopp=");pi(st.n_rhop);ps(" Rhoe=");pi(st.n_rhoe);ps(" Rhov=");pi(st.n_rho2);ps(" Rhoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rhodotypos Admin Demo ===\n\n");rho_init();
ps("Rhodotypos planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rho_planning(t,c,1180+(i*17),1169+(i*14),1149+(i*10),1131+(i*6),2020+(i%5));}
ps("\nRhodotypos execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rho_execution(t,c,1169+(i*15),1158+(i*12),1140+(i*8),1127+(i*5),2021+(i%4));}
ps("\nRhodotypos evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rho_evaluation(t,c,1161+(i*13),1150+(i*10),1134+(i*7),1123+(i*4),2022+(i%3));}
ps("\nRhodotypos accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rho_accessory(t,c,1153+(i*11),1144+(i*9),1130+(i*6),1120+(i*3),2023+(i%2));}
ps("\nRhodotypos marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rho_market(t,c,1147+(i*9),1138+(i*7),1125+(i*5),1117+(i*3),2024);}
ps("\n");rho_report();rho_state();ps("\n=== Demo Complete ===\n");return 0;}
