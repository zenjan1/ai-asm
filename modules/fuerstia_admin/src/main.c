/* fuerstia_admin: Fuerstia management technology administration (v1.0)
 * Fuerstia planning, fuerstia execution, fuerstia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fuer_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} fuer_state_t;
static fuer_t fuer_p[N],fuer_x[N-2],fuer2[N-4],fuerac[N-6],fuerm[N-6]; static fuer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(fuer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fuer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FUER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fuer_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fuer_p[i].active=0;for(int i=0;i<N-2;i++)fuer_x[i].active=0;for(int i=0;i<N-4;i++)fuer2[i].active=0;for(int i=0;i<N-6;i++)fuerac[i].active=0;for(int i=0;i<N-6;i++)fuerm[i].active=0;init=1;ps("[FUER] Fuerstia initialized\n");return 0;}
int fuer_planning(int t,int c,int a,int b,int d,int e,int y){return add(fuer_p,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int fuer_execution(int t,int c,int a,int b,int d,int e,int y){return add(fuer_x,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fuer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fuer2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fuer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fuerac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fuer_market(int t,int c,int a,int b,int d,int e,int y){return add(fuerm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fuer_report(void){ps("[FUER] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void fuer_state(void){ps("[FUER] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Fuerstia Admin Demo ===\n\n");fuer_init();
ps("Fuerstia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fuer_planning(t,c,2062+(i*17),2051+(i*14),2031+(i*10),2013+(i*6),2020+(i%5));}
ps("\nFuerstia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fuer_execution(t,c,2051+(i*15),2040+(i*12),2022+(i*8),2009+(i*5),2021+(i%4));}
ps("\nFuerstia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fuer_evaluation(t,c,2043+(i*13),2032+(i*10),2016+(i*7),2005+(i*4),2022+(i%3));}
ps("\nFuerstia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fuer_accessory(t,c,2035+(i*11),2026+(i*9),2012+(i*6),2002+(i*3),2023+(i%2));}
ps("\nFuerstia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fuer_market(t,c,2029+(i*9),2020+(i*7),2009+(i*5),2001+(i*3),2024);}
ps("\n");fuer_report();fuer_state();ps("\n=== Demo Complete ===\n");return 0;}
