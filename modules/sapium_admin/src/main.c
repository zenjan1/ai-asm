/* sapium_admin: Sapium management technology administration (v1.0)
 * Sapium planning, sapium execution, sapium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sapi_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} sapi_state_t;
static sapi_t sapip[N],sapix[N-2],sapi2[N-4],sapiac[N-6],sapim[N-6]; static sapi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(sapi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sapi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SAPI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sapi_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sapip[i].active=0;for(int i=0;i<N-2;i++)sapix[i].active=0;for(int i=0;i<N-4;i++)sapi2[i].active=0;for(int i=0;i<N-6;i++)sapiac[i].active=0;for(int i=0;i<N-6;i++)sapim[i].active=0;init=1;ps("[SAPI] Sapium initialized\n");return 0;}
int sapi_planning(int t,int c,int a,int b,int d,int e,int y){return add(sapip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int sapi_execution(int t,int c,int a,int b,int d,int e,int y){return add(sapix,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sapi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sapi2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sapi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sapiac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sapi_market(int t,int c,int a,int b,int d,int e,int y){return add(sapim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sapi_report(void){ps("[SAPI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void sapi_state(void){ps("[SAPI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Sapium Admin Demo ===\n\n");sapi_init();
ps("Sapium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sapi_planning(t,c,1643+(i*17),1632+(i*14),1612+(i*10),1594+(i*6),2020+(i%5));}
ps("\nSapium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sapi_execution(t,c,1632+(i*15),1621+(i*12),1603+(i*8),1590+(i*5),2021+(i%4));}
ps("\nSapium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sapi_evaluation(t,c,1624+(i*13),1613+(i*10),1597+(i*7),1586+(i*4),2022+(i%3));}
ps("\nSapium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sapi_accessory(t,c,1616+(i*11),1607+(i*9),1593+(i*6),1583+(i*3),2023+(i%2));}
ps("\nSapium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sapi_market(t,c,1610+(i*9),1601+(i*7),1588+(i*5),1580+(i*3),2024);}
ps("\n");sapi_report();sapi_state();ps("\n=== Demo Complete ===\n");return 0;}
