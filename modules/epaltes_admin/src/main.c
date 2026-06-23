/* epaltes_admin: Epaltes management technology administration (v1.0)
 * Epaltes planning, epaltes execution, epaltes evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} epal_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} epal_state_t;
static epal_t epalp[N],epalx[N-2],epal2[N-4],epalac[N-6],epalm[N-6]; static epal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(epal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;epal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int epal_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epalp[i].active=0;for(int i=0;i<N-2;i++)epalx[i].active=0;for(int i=0;i<N-4;i++)epal2[i].active=0;for(int i=0;i<N-6;i++)epalac[i].active=0;for(int i=0;i<N-6;i++)epalm[i].active=0;init=1;ps("[EPAL] Epaltes initialized\n");return 0;}
int epal_planning(int t,int c,int a,int b,int d,int e,int y){return add(epalp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int epal_execution(int t,int c,int a,int b,int d,int e,int y){return add(epalx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int epal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(epal2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int epal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(epalac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int epal_market(int t,int c,int a,int b,int d,int e,int y){return add(epalm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void epal_report(void){ps("[EPAL] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void epal_state(void){ps("[EPAL] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Epaltes Admin Demo ===\n\n");epal_init();
ps("Epaltes planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;epal_planning(t,c,2010+(i*17),1999+(i*14),1979+(i*10),1961+(i*6),2020+(i%5));}
ps("\nEpaltes execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;epal_execution(t,c,1999+(i*15),1988+(i*12),1970+(i*8),1957+(i*5),2021+(i%4));}
ps("\nEpaltes evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;epal_evaluation(t,c,1991+(i*13),1980+(i*10),1964+(i*7),1953+(i*4),2022+(i%3));}
ps("\nEpaltes accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epal_accessory(t,c,1983+(i*11),1974+(i*9),1960+(i*6),1950+(i*3),2023+(i%2));}
ps("\nEpaltes marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;epal_market(t,c,1977+(i*9),1968+(i*7),1955+(i*5),1947+(i*3),2024);}
ps("\n");epal_report();epal_state();ps("\n=== Demo Complete ===\n");return 0;}
