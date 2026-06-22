/* stauntonia_admin: Stauntonia management technology administration (v1.0)
 * Stauntonia planning, stauntonia execution, stauntonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} stau_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} stau_state_t;
static stau_t staup[N],staux[N-2],stau2[N-4],stauac[N-6],staum[N-6]; static stau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(stau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;stau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int stau_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)staup[i].active=0;for(int i=0;i<N-2;i++)staux[i].active=0;for(int i=0;i<N-4;i++)stau2[i].active=0;for(int i=0;i<N-6;i++)stauac[i].active=0;for(int i=0;i<N-6;i++)staum[i].active=0;init=1;ps("[STAU] Stauntonia initialized\n");return 0;}
int stau_planning(int t,int c,int a,int b,int d,int e,int y){return add(staup,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int stau_execution(int t,int c,int a,int b,int d,int e,int y){return add(staux,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int stau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(stau2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int stau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(stauac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int stau_market(int t,int c,int a,int b,int d,int e,int y){return add(staum,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void stau_report(void){ps("[STAU] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void stau_state(void){ps("[STAU] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Stauntonia Admin Demo ===\n\n");stau_init();
ps("Stauntonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;stau_planning(t,c,1660+(i*17),1649+(i*14),1629+(i*10),1611+(i*6),2020+(i%5));}
ps("\nStauntonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;stau_execution(t,c,1649+(i*15),1638+(i*12),1620+(i*8),1607+(i*5),2021+(i%4));}
ps("\nStauntonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;stau_evaluation(t,c,1641+(i*13),1630+(i*10),1614+(i*7),1603+(i*4),2022+(i%3));}
ps("\nStauntonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;stau_accessory(t,c,1633+(i*11),1624+(i*9),1610+(i*6),1600+(i*3),2023+(i%2));}
ps("\nStauntonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;stau_market(t,c,1627+(i*9),1618+(i*7),1605+(i*5),1597+(i*3),2024);}
ps("\n");stau_report();stau_state();ps("\n=== Demo Complete ===\n");return 0;}
