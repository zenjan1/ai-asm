/* tofieldia_admin: Tofieldia management technology administration (v1.0)
 * Tofieldia planning, tofieldia execution, tofieldia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tofi_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} tofi_state_t;
static tofi_t tofip[N],tofis[N-2],tofi2[N-4],tofiac[N-6],tofim[N-6]; static tofi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(tofi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tofi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOFI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tofi_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tofip[i].active=0;for(int i=0;i<N-2;i++)tofis[i].active=0;for(int i=0;i<N-4;i++)tofi2[i].active=0;for(int i=0;i<N-6;i++)tofiac[i].active=0;for(int i=0;i<N-6;i++)tofim[i].active=0;init=1;ps("[TOFI] Tofieldia initialized\n");return 0;}
int tofi_planning(int t,int c,int a,int b,int d,int e,int y){return add(tofip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int tofi_execution(int t,int c,int a,int b,int d,int e,int y){return add(tofis,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tofi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tofi2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tofi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tofiac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tofi_market(int t,int c,int a,int b,int d,int e,int y){return add(tofim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tofi_report(void){ps("[TOFI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void tofi_state(void){ps("[TOFI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Tofieldia Admin Demo ===\n\n");tofi_init();
ps("Tofieldia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tofi_planning(t,c,1683+(i*17),1672+(i*14),1652+(i*10),1634+(i*6),2020+(i%5));}
ps("\nTofieldia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tofi_execution(t,c,1672+(i*15),1661+(i*12),1643+(i*8),1630+(i*5),2021+(i%4));}
ps("\nTofieldia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tofi_evaluation(t,c,1664+(i*13),1653+(i*10),1637+(i*7),1626+(i*4),2022+(i%3));}
ps("\nTofieldia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tofi_accessory(t,c,1656+(i*11),1647+(i*9),1633+(i*6),1623+(i*3),2023+(i%2));}
ps("\nTofieldia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tofi_market(t,c,1650+(i*9),1641+(i*7),1628+(i*5),1620+(i*3),2024);}
ps("\n");tofi_report();tofi_state();ps("\n=== Demo Complete ===\n");return 0;}
