/* thespesia_admin: Thespesia management technology administration (v1.0)
 * Thespesia planning, thespesia execution, thespesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} thes_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} thes_state_t;
static thes_t thesp[N],thesx[N-2],thes2[N-4],thesac[N-6],thesm[N-6]; static thes_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(thes_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;thes_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int thes_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)thesp[i].active=0;for(int i=0;i<N-2;i++)thesx[i].active=0;for(int i=0;i<N-4;i++)thes2[i].active=0;for(int i=0;i<N-6;i++)thesac[i].active=0;for(int i=0;i<N-6;i++)thesm[i].active=0;init=1;ps("[THES] Thespesia initialized\n");return 0;}
int thes_planning(int t,int c,int a,int b,int d,int e,int y){return add(thesp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int thes_execution(int t,int c,int a,int b,int d,int e,int y){return add(thesx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int thes_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(thes2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int thes_accessory(int t,int c,int a,int b,int d,int e,int y){return add(thesac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int thes_market(int t,int c,int a,int b,int d,int e,int y){return add(thesm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void thes_report(void){ps("[THES] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void thes_state(void){ps("[THES] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Thespesia Admin Demo ===\n\n");thes_init();
ps("Thespesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;thes_planning(t,c,1674+(i*17),1663+(i*14),1643+(i*10),1625+(i*6),2020+(i%5));}
ps("\nThespesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;thes_execution(t,c,1663+(i*15),1652+(i*12),1634+(i*8),1621+(i*5),2021+(i%4));}
ps("\nThespesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;thes_evaluation(t,c,1655+(i*13),1644+(i*10),1628+(i*7),1617+(i*4),2022+(i%3));}
ps("\nThespesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thes_accessory(t,c,1647+(i*11),1638+(i*9),1624+(i*6),1614+(i*3),2023+(i%2));}
ps("\nThespesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thes_market(t,c,1641+(i*9),1632+(i*7),1619+(i*5),1611+(i*3),2024);}
ps("\n");thes_report();thes_state();ps("\n=== Demo Complete ===\n");return 0;}
