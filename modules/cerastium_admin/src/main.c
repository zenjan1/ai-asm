/* cerastium_admin: Cerastium management technology administration (v1.0)
 * Cerastium planning, cerastium execution, cerastium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cera_t;
typedef struct{int n_cerap,n_cera,n_cera2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cera_state_t;
static cera_t ceraps[N],cerae[N-2],cera2[N-4],ceraac[N-6],ceram[N-6]; static cera_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cera_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cera_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CERA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cera_init(void){if(init)return -1;st.n_cerap=0;st.n_cera=0;st.n_cera2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ceraps[i].active=0;for(int i=0;i<N-2;i++)cerae[i].active=0;for(int i=0;i<N-4;i++)cera2[i].active=0;for(int i=0;i<N-6;i++)ceraac[i].active=0;for(int i=0;i<N-6;i++)ceram[i].active=0;init=1;ps("[CERA] Cerastium initialized\n");return 0;}
int cera_planning(int t,int c,int a,int b,int d,int e,int y){return add(ceraps,&st.n_cerap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cera_execution(int t,int c,int a,int b,int d,int e,int y){return add(cerae,&st.n_cera,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cera_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cera2,&st.n_cera2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cera_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ceraac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cera_market(int t,int c,int a,int b,int d,int e,int y){return add(ceram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cera_report(void){ps("[CERA] Erap: ");pi(st.n_cerap);ps(" PCS=");pi(st.t_f1);ps("\nErae: ");pi(st.n_cera);ps(" PCS=");pi(st.t_f2);ps("\nEra2: ");pi(st.n_cera2);ps(" PCS=");pi(st.t_f3);ps("\nErac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cera_state(void){ps("[CERA] Erap=");pi(st.n_cerap);ps(" Era=");pi(st.n_cera);ps(" Era2=");pi(st.n_cera2);ps(" Erac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cerastium Admin Demo ===\n\n");cera_init();
ps("Cerastium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cera_planning(t,c,1469+(i*17),1458+(i*14),1438+(i*10),1420+(i*6),2020+(i%5));}
ps("\nCerastium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cera_execution(t,c,1458+(i*15),1447+(i*12),1429+(i*8),1416+(i*5),2021+(i%4));}
ps("\nCerastium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cera_evaluation(t,c,1450+(i*13),1439+(i*10),1423+(i*7),1412+(i*4),2022+(i%3));}
ps("\nCerastium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cera_accessory(t,c,1442+(i*11),1433+(i*9),1419+(i*6),1409+(i*3),2023+(i%2));}
ps("\nCerastium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cera_market(t,c,1436+(i*9),1427+(i*7),1414+(i*5),1406+(i*3),2024);}
ps("\n");cera_report();cera_state();ps("\n=== Demo Complete ===\n");return 0;}
