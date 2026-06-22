/* nard_admin: Nard management technology administration (v1.0)
 * Nard planning, nard execution, nard evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nard_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} nard_state_t;
static nard_t nardp[N],nardx[N-2],nard2[N-4],nardac[N-6],nardm[N-6]; static nard_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nard_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nard_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NARD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nard_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nardp[i].active=0;for(int i=0;i<N-2;i++)nardx[i].active=0;for(int i=0;i<N-4;i++)nard2[i].active=0;for(int i=0;i<N-6;i++)nardac[i].active=0;for(int i=0;i<N-6;i++)nardm[i].active=0;init=1;ps("[NARD] Nard initialized\n");return 0;}
int nard_planning(int t,int c,int a,int b,int d,int e,int y){return add(nardp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int nard_execution(int t,int c,int a,int b,int d,int e,int y){return add(nardx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nard_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nard2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nard_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nardac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nard_market(int t,int c,int a,int b,int d,int e,int y){return add(nardm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nard_report(void){ps("[NARD] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void nard_state(void){ps("[NARD] Pl=");pi(st.n_planning);ps(" Ex=");pi(st.n_execution);ps(" Ev=");pi(st.n_evaluation);ps(" Ac=");pi(st.n_accessory);ps(" Mk=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Nard Admin Demo ===\n\n");nard_init();
ps("Nard planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nard_planning(t,c,1591+(i*17),1580+(i*14),1560+(i*10),1542+(i*6),2020+(i%5));}
ps("\nNard execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nard_execution(t,c,1580+(i*15),1569+(i*12),1551+(i*8),1538+(i*5),2021+(i%4));}
ps("\nNard evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nard_evaluation(t,c,1572+(i*13),1561+(i*10),1545+(i*7),1534+(i*4),2022+(i%3));}
ps("\nNard accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nard_accessory(t,c,1564+(i*11),1555+(i*9),1541+(i*6),1531+(i*3),2023+(i%2));}
ps("\nNard marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nard_market(t,c,1558+(i*9),1549+(i*7),1536+(i*5),1528+(i*3),2024);}
ps("\n");nard_report();nard_state();ps("\n=== Demo Complete ===\n");return 0;}
