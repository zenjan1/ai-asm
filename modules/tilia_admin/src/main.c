/* tilia_admin: Tilia management technology administration (v1.0)
 * Tilia planning, tilia execution, tilia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tili_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} tili_state_t;
static tili_t tilip[N],tilix[N-2],tili2[N-4],tiliac[N-6],tilim[N-6]; static tili_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(tili_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tili_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TILI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tili_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tilip[i].active=0;for(int i=0;i<N-2;i++)tilix[i].active=0;for(int i=0;i<N-4;i++)tili2[i].active=0;for(int i=0;i<N-6;i++)tiliac[i].active=0;for(int i=0;i<N-6;i++)tilim[i].active=0;init=1;ps("[TILI] Tilia initialized\n");return 0;}
int tili_planning(int t,int c,int a,int b,int d,int e,int y){return add(tilip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int tili_execution(int t,int c,int a,int b,int d,int e,int y){return add(tilix,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tili_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tili2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tili_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tiliac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tili_market(int t,int c,int a,int b,int d,int e,int y){return add(tilim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tili_report(void){ps("[TILI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void tili_state(void){ps("[TILI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Tilia Admin Demo ===\n\n");tili_init();
ps("Tilia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tili_planning(t,c,1680+(i*17),1669+(i*14),1649+(i*10),1631+(i*6),2020+(i%5));}
ps("\nTilia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tili_execution(t,c,1669+(i*15),1658+(i*12),1640+(i*8),1627+(i*5),2021+(i%4));}
ps("\nTilia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tili_evaluation(t,c,1661+(i*13),1650+(i*10),1634+(i*7),1623+(i*4),2022+(i%3));}
ps("\nTilia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tili_accessory(t,c,1653+(i*11),1644+(i*9),1630+(i*6),1620+(i*3),2023+(i%2));}
ps("\nTilia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tili_market(t,c,1647+(i*9),1638+(i*7),1625+(i*5),1617+(i*3),2024);}
ps("\n");tili_report();tili_state();ps("\n=== Demo Complete ===\n");return 0;}
