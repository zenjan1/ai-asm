/* swertia_admin: Swertia management technology administration (v1.0)
 * Swertia planning, swertia execution, swertia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} swer_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} swer_state_t;
static swer_t swerp[N],swerx[N-2],swer2[N-4],swerac[N-6],swerm[N-6]; static swer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(swer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;swer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SWER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int swer_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)swerp[i].active=0;for(int i=0;i<N-2;i++)swerx[i].active=0;for(int i=0;i<N-4;i++)swer2[i].active=0;for(int i=0;i<N-6;i++)swerac[i].active=0;for(int i=0;i<N-6;i++)swerm[i].active=0;init=1;ps("[SWER] Swertia initialized\n");return 0;}
int swer_planning(int t,int c,int a,int b,int d,int e,int y){return add(swerp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int swer_execution(int t,int c,int a,int b,int d,int e,int y){return add(swerx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int swer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(swer2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int swer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(swerac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int swer_market(int t,int c,int a,int b,int d,int e,int y){return add(swerm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void swer_report(void){ps("[SWER] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void swer_state(void){ps("[SWER] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Swertia Admin Demo ===\n\n");swer_init();
ps("Swertia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;swer_planning(t,c,1666+(i*17),1655+(i*14),1635+(i*10),1617+(i*6),2020+(i%5));}
ps("\nSwertia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;swer_execution(t,c,1655+(i*15),1644+(i*12),1626+(i*8),1613+(i*5),2021+(i%4));}
ps("\nSwertia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;swer_evaluation(t,c,1647+(i*13),1636+(i*10),1620+(i*7),1609+(i*4),2022+(i%3));}
ps("\nSwertia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swer_accessory(t,c,1639+(i*11),1630+(i*9),1616+(i*6),1606+(i*3),2023+(i%2));}
ps("\nSwertia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;swer_market(t,c,1633+(i*9),1624+(i*7),1611+(i*5),1603+(i*3),2024);}
ps("\n");swer_report();swer_state();ps("\n=== Demo Complete ===\n");return 0;}
