/* uncaria_admin: Uncaria management technology administration (v1.0)
 * Uncaria planning, uncaria execution, uncaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} unca_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} unca_state_t;
static unca_t uncup[N],uncux[N-2],uncu2[N-4],uncuac[N-6],uncum[N-6]; static unca_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(unca_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;unca_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UNCA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int unca_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uncup[i].active=0;for(int i=0;i<N-2;i++)uncux[i].active=0;for(int i=0;i<N-4;i++)uncu2[i].active=0;for(int i=0;i<N-6;i++)uncuac[i].active=0;for(int i=0;i<N-6;i++)uncum[i].active=0;init=1;ps("[UNCA] Uncaria initialized\n");return 0;}
int unca_planning(int t,int c,int a,int b,int d,int e,int y){return add(uncup,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int unca_execution(int t,int c,int a,int b,int d,int e,int y){return add(uncux,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int unca_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(uncu2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int unca_accessory(int t,int c,int a,int b,int d,int e,int y){return add(uncuac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int unca_market(int t,int c,int a,int b,int d,int e,int y){return add(uncum,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void unca_report(void){ps("[UNCA] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void unca_state(void){ps("[UNCA] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Uncaria Admin Demo ===\n\n");unca_init();
ps("Uncaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;unca_planning(t,c,1959+(i*17),1948+(i*14),1928+(i*10),1910+(i*6),2020+(i%5));}
ps("\nUncaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;unca_execution(t,c,1948+(i*15),1937+(i*12),1919+(i*8),1906+(i*5),2021+(i%4));}
ps("\nUncaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;unca_evaluation(t,c,1940+(i*13),1929+(i*10),1913+(i*7),1902+(i*4),2022+(i%3));}
ps("\nUncaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;unca_accessory(t,c,1932+(i*11),1923+(i*9),1909+(i*6),1899+(i*3),2023+(i%2));}
ps("\nUncaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;unca_market(t,c,1926+(i*9),1917+(i*7),1904+(i*5),1896+(i*3),2024);}
ps("\n");unca_report();unca_state();ps("\n=== Demo Complete ===\n");return 0;}
