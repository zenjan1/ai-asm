/* martinella_admin: Martinella management technology administration (v1.0)
 * Martinella planning, martinella execution, martinella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mart_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} mart_state_t;
static mart_t martp[N],martx[N-2],mart2[N-4],martac[N-6],martm[N-6]; static mart_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mart_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mart_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MART] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mart_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)martp[i].active=0;for(int i=0;i<N-2;i++)martx[i].active=0;for(int i=0;i<N-4;i++)mart2[i].active=0;for(int i=0;i<N-6;i++)martac[i].active=0;for(int i=0;i<N-6;i++)martm[i].active=0;init=1;ps("[MART] Martinella initialized\n");return 0;}
int mart_planning(int t,int c,int a,int b,int d,int e,int y){return add(martp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int mart_execution(int t,int c,int a,int b,int d,int e,int y){return add(martx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mart_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mart2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mart_accessory(int t,int c,int a,int b,int d,int e,int y){return add(martac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mart_market(int t,int c,int a,int b,int d,int e,int y){return add(martm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mart_report(void){ps("[MART] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void mart_state(void){ps("[MART] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Martinella Admin Demo ===\n\n");mart_init();
ps("Martinella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mart_planning(t,c,1912+(i*17),1901+(i*14),1881+(i*10),1863+(i*6),2020+(i%5));}
ps("\nMartinella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mart_execution(t,c,1901+(i*15),1890+(i*12),1872+(i*8),1859+(i*5),2021+(i%4));}
ps("\nMartinella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mart_evaluation(t,c,1893+(i*13),1882+(i*10),1866+(i*7),1855+(i*4),2022+(i%3));}
ps("\nMartinella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mart_accessory(t,c,1885+(i*11),1876+(i*9),1862+(i*6),1852+(i*3),2023+(i%2));}
ps("\nMartinella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mart_market(t,c,1879+(i*9),1870+(i*7),1857+(i*5),1849+(i*3),2024);}
ps("\n");mart_report();mart_state();ps("\n=== Demo Complete ===\n");return 0;}
