/* psoralea_admin: Psoralea management technology administration (v1.0)
 * Psoralea planning, psoralea execution, psoralea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pso_t;
typedef struct{int n_psop,n_psoe,n_pso2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pso_state_t;
static pso_t psops[N],psoss[N-2],psovss[N-4],psoas[N-6],psomks[N-6]; static pso_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pso_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pso_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PSO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pso_init(void){if(init)return -1;st.n_psop=0;st.n_psoe=0;st.n_pso2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)psops[i].active=0;for(int i=0;i<N-2;i++)psoss[i].active=0;for(int i=0;i<N-4;i++)psovss[i].active=0;for(int i=0;i<N-6;i++)psoas[i].active=0;for(int i=0;i<N-6;i++)psomks[i].active=0;init=1;ps("[PSO] Psoralea initialized\n");return 0;}
int pso_planning(int t,int c,int a,int b,int d,int e,int y){return add(psops,&st.n_psop,&st.t_f1,N,t,c,a,b,d,e,y);}
int pso_execution(int t,int c,int a,int b,int d,int e,int y){return add(psoss,&st.n_psoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pso_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(psovss,&st.n_pso2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pso_accessory(int t,int c,int a,int b,int d,int e,int y){return add(psoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pso_market(int t,int c,int a,int b,int d,int e,int y){return add(psomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pso_report(void){ps("[PSO] Psopp: ");pi(st.n_psop);ps(" PCS=");pi(st.t_f1);ps("\nPsoe: ");pi(st.n_psoe);ps(" PCS=");pi(st.t_f2);ps("\nPsov: ");pi(st.n_pso2);ps(" PCS=");pi(st.t_f3);ps("\nPsoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pso_state(void){ps("[PSO] Psopp=");pi(st.n_psop);ps(" Psoe=");pi(st.n_psoe);ps(" Psov=");pi(st.n_pso2);ps(" Psoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Psoralea Admin Demo ===\n\n");pso_init();
ps("Psoralea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pso_planning(t,c,1174+(i*17),1163+(i*14),1143+(i*10),1125+(i*6),2020+(i%5));}
ps("\nPsoralea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pso_execution(t,c,1163+(i*15),1152+(i*12),1134+(i*8),1121+(i*5),2021+(i%4));}
ps("\nPsoralea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pso_evaluation(t,c,1155+(i*13),1144+(i*10),1128+(i*7),1117+(i*4),2022+(i%3));}
ps("\nPsoralea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pso_accessory(t,c,1147+(i*11),1138+(i*9),1124+(i*6),1114+(i*3),2023+(i%2));}
ps("\nPsoralea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pso_market(t,c,1141+(i*9),1132+(i*7),1119+(i*5),1111+(i*3),2024);}
ps("\n");pso_report();pso_state();ps("\n=== Demo Complete ===\n");return 0;}
