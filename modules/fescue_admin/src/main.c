/* fescue_admin: Fescue management technology administration (v1.0)
 * Fescue planning, fescue execution, fescue evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fesc_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} fesc_state_t;
static fesc_t fescp[N],fescx[N-2],fesc2[N-4],fescac[N-6],fescm[N-6]; static fesc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(fesc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fesc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FESC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fesc_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fescp[i].active=0;for(int i=0;i<N-2;i++)fescx[i].active=0;for(int i=0;i<N-4;i++)fesc2[i].active=0;for(int i=0;i<N-6;i++)fescac[i].active=0;for(int i=0;i<N-6;i++)fescm[i].active=0;init=1;ps("[FESC] Fescue initialized\n");return 0;}
int fesc_planning(int t,int c,int a,int b,int d,int e,int y){return add(fescp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int fesc_execution(int t,int c,int a,int b,int d,int e,int y){return add(fescx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fesc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fesc2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fesc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fescac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fesc_market(int t,int c,int a,int b,int d,int e,int y){return add(fescm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fesc_report(void){ps("[FESC] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void fesc_state(void){ps("[FESC] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Fescue Admin Demo ===\n\n");fesc_init();
ps("Fescue planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fesc_planning(t,c,1846+(i*17),1835+(i*14),1815+(i*10),1797+(i*6),2020+(i%5));}
ps("\nFescue execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fesc_execution(t,c,1835+(i*15),1824+(i*12),1806+(i*8),1793+(i*5),2021+(i%4));}
ps("\nFescue evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fesc_evaluation(t,c,1827+(i*13),1816+(i*10),1800+(i*7),1789+(i*4),2022+(i%3));}
ps("\nFescue accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fesc_accessory(t,c,1819+(i*11),1810+(i*9),1796+(i*6),1786+(i*3),2023+(i%2));}
ps("\nFescue marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fesc_market(t,c,1813+(i*9),1804+(i*7),1791+(i*5),1783+(i*3),2024);}
ps("\n");fesc_report();fesc_state();ps("\n=== Demo Complete ===\n");return 0;}
