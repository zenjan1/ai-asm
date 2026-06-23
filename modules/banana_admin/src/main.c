/* banana_admin: Banana management technology administration (v1.0)
 * Banana planning, banana execution, banana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bana_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} bana_state_t;
static bana_t banap[N],banax[N-2],bana2[N-4],banac[N-6],banam[N-6]; static bana_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bana_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bana_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BANA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bana_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)banap[i].active=0;for(int i=0;i<N-2;i++)banax[i].active=0;for(int i=0;i<N-4;i++)bana2[i].active=0;for(int i=0;i<N-6;i++)banac[i].active=0;for(int i=0;i<N-6;i++)banam[i].active=0;init=1;ps("[BANA] Banana initialized\n");return 0;}
int bana_planning(int t,int c,int a,int b,int d,int e,int y){return add(banap,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int bana_execution(int t,int c,int a,int b,int d,int e,int y){return add(banax,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bana_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bana2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bana_accessory(int t,int c,int a,int b,int d,int e,int y){return add(banac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bana_market(int t,int c,int a,int b,int d,int e,int y){return add(banam,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bana_report(void){ps("[BANA] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void bana_state(void){ps("[BANA] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Banana Admin Demo ===\n\n");bana_init();
ps("Banana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bana_planning(t,c,1782+(i*17),1771+(i*14),1751+(i*10),1733+(i*6),2020+(i%5));}
ps("\nBanana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bana_execution(t,c,1771+(i*15),1760+(i*12),1742+(i*8),1729+(i*5),2021+(i%4));}
ps("\nBanana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bana_evaluation(t,c,1763+(i*13),1752+(i*10),1736+(i*7),1725+(i*4),2022+(i%3));}
ps("\nBanana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bana_accessory(t,c,1755+(i*11),1746+(i*9),1732+(i*6),1722+(i*3),2023+(i%2));}
ps("\nBanana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bana_market(t,c,1749+(i*9),1740+(i*7),1727+(i*5),1719+(i*3),2024);}
ps("\n");bana_report();bana_state();ps("\n=== Demo Complete ===\n");return 0;}
