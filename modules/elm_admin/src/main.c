/* elm_admin: Elm management technology administration (v1.0)
 * Elm planning, elm execution, elm evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} elm_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} elm_state_t;
static elm_t elmp[N],elmx[N-2],elm2[N-4],elmac[N-6],elmm[N-6]; static elm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(elm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;elm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELM_] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int elm_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)elmp[i].active=0;for(int i=0;i<N-2;i++)elmx[i].active=0;for(int i=0;i<N-4;i++)elm2[i].active=0;for(int i=0;i<N-6;i++)elmac[i].active=0;for(int i=0;i<N-6;i++)elmm[i].active=0;init=1;ps("[ELM_] Elm initialized\n");return 0;}
int elm_planning(int t,int c,int a,int b,int d,int e,int y){return add(elmp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int elm_execution(int t,int c,int a,int b,int d,int e,int y){return add(elmx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int elm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(elm2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int elm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(elmac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int elm_market(int t,int c,int a,int b,int d,int e,int y){return add(elmm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void elm_report(void){ps("[ELM_] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void elm_state(void){ps("[ELM_] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Elm Admin Demo ===\n\n");elm_init();
ps("Elm planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;elm_planning(t,c,1713+(i*17),1702+(i*14),1682+(i*10),1664+(i*6),2020+(i%5));}
ps("\nElm execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;elm_execution(t,c,1702+(i*15),1691+(i*12),1673+(i*8),1660+(i*5),2021+(i%4));}
ps("\nElm evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;elm_evaluation(t,c,1694+(i*13),1683+(i*10),1667+(i*7),1656+(i*4),2022+(i%3));}
ps("\nElm accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elm_accessory(t,c,1686+(i*11),1677+(i*9),1663+(i*6),1653+(i*3),2023+(i%2));}
ps("\nElm marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elm_market(t,c,1680+(i*9),1671+(i*7),1658+(i*5),1650+(i*3),2024);}
ps("\n");elm_report();elm_state();ps("\n=== Demo Complete ===\n");return 0;}
