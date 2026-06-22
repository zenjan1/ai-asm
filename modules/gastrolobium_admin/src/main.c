/* gastrolobium_admin: Gastrolobium management technology administration (v1.0)
 * Gastrolobium planning, gastrolobium execution, gastrolobium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gas_t;
typedef struct{int n_gasp,n_gase,n_gas2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gas_state_t;
static gas_t gasps[N],gasss[N-2],gasvss[N-4],gasas[N-6],gasmks[N-6]; static gas_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gas_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gas_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gas_init(void){if(init)return -1;st.n_gasp=0;st.n_gase=0;st.n_gas2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gasps[i].active=0;for(int i=0;i<N-2;i++)gasss[i].active=0;for(int i=0;i<N-4;i++)gasvss[i].active=0;for(int i=0;i<N-6;i++)gasas[i].active=0;for(int i=0;i<N-6;i++)gasmks[i].active=0;init=1;ps("[GAS] Gastrolobium initialized\n");return 0;}
int gas_planning(int t,int c,int a,int b,int d,int e,int y){return add(gasps,&st.n_gasp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gas_execution(int t,int c,int a,int b,int d,int e,int y){return add(gasss,&st.n_gase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gas_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gasvss,&st.n_gas2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gas_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gasas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gas_market(int t,int c,int a,int b,int d,int e,int y){return add(gasmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gas_report(void){ps("[GAS] Gasp: ");pi(st.n_gasp);ps(" PCS=");pi(st.t_f1);ps("\nGase: ");pi(st.n_gase);ps(" PCS=");pi(st.t_f2);ps("\nGasv: ");pi(st.n_gas2);ps(" PCS=");pi(st.t_f3);ps("\nGasc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gas_state(void){ps("[GAS] Gasp=");pi(st.n_gasp);ps(" Gase=");pi(st.n_gase);ps(" Gasv=");pi(st.n_gas2);ps(" Gasc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gastrolobium Admin Demo ===\n\n");gas_init();
ps("Gastrolobium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gas_planning(t,c,1285+(i*17),1274+(i*14),1254+(i*10),1236+(i*6),2020+(i%5));}
ps("\nGastrolobium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gas_execution(t,c,1274+(i*15),1263+(i*12),1245+(i*8),1232+(i*5),2021+(i%4));}
ps("\nGastrolobium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gas_evaluation(t,c,1266+(i*13),1255+(i*10),1239+(i*7),1228+(i*4),2022+(i%3));}
ps("\nGastrolobium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gas_accessory(t,c,1258+(i*11),1249+(i*9),1235+(i*6),1225+(i*3),2023+(i%2));}
ps("\nGastrolobium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gas_market(t,c,1252+(i*9),1243+(i*7),1230+(i*5),1222+(i*3),2024);}
ps("\n");gas_report();gas_state();ps("\n=== Demo Complete ===\n");return 0;}
