/* rhapis_admin: Rhapis management technology administration (v1.0)
 * Rhapis planning, rhapis execution, rhapis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rhap_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} rhap_state_t;
static rhap_t rhapp[N],rhapx[N-2],rhap2[N-4],rhapac[N-6],rhapm[N-6]; static rhap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(rhap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rhap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RHAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rhap_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rhapp[i].active=0;for(int i=0;i<N-2;i++)rhapx[i].active=0;for(int i=0;i<N-4;i++)rhap2[i].active=0;for(int i=0;i<N-6;i++)rhapac[i].active=0;for(int i=0;i<N-6;i++)rhapm[i].active=0;init=1;ps("[RHAP] Rhapis initialized\n");return 0;}
int rhap_planning(int t,int c,int a,int b,int d,int e,int y){return add(rhapp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int rhap_execution(int t,int c,int a,int b,int d,int e,int y){return add(rhapx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rhap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rhap2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rhap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rhapac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rhap_market(int t,int c,int a,int b,int d,int e,int y){return add(rhapm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rhap_report(void){ps("[RHAP] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void rhap_state(void){ps("[RHAP] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Rhapis Admin Demo ===\n\n");rhap_init();
ps("Rhapis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rhap_planning(t,c,1642+(i*17),1631+(i*14),1611+(i*10),1593+(i*6),2020+(i%5));}
ps("\nRhapis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rhap_execution(t,c,1631+(i*15),1620+(i*12),1602+(i*8),1589+(i*5),2021+(i%4));}
ps("\nRhapis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rhap_evaluation(t,c,1623+(i*13),1612+(i*10),1596+(i*7),1585+(i*4),2022+(i%3));}
ps("\nRhapis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhap_accessory(t,c,1615+(i*11),1606+(i*9),1592+(i*6),1582+(i*3),2023+(i%2));}
ps("\nRhapis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rhap_market(t,c,1609+(i*9),1600+(i*7),1587+(i*5),1579+(i*3),2024);}
ps("\n");rhap_report();rhap_state();ps("\n=== Demo Complete ===\n");return 0;}
