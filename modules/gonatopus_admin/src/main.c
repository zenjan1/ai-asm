/* gonatopus_admin: Gonatopus management technology administration (v1.0)
 * Gonatopus planning, gonatopus execution, gonatopus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gon_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} gon_state_t;
static gon_t gon_p[N],gon_x[N-2],gon2[N-4],gonac[N-6],gonm[N-6]; static gon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gon_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gon_p[i].active=0;for(int i=0;i<N-2;i++)gon_x[i].active=0;for(int i=0;i<N-4;i++)gon2[i].active=0;for(int i=0;i<N-6;i++)gonac[i].active=0;for(int i=0;i<N-6;i++)gonm[i].active=0;init=1;ps("[GON] Gonatopus initialized\n");return 0;}
int gon_planning(int t,int c,int a,int b,int d,int e,int y){return add(gon_p,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int gon_execution(int t,int c,int a,int b,int d,int e,int y){return add(gon_x,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gon2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gonac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gon_market(int t,int c,int a,int b,int d,int e,int y){return add(gonm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gon_report(void){ps("[GON] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void gon_state(void){ps("[GON] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Gonatopus Admin Demo ===\n\n");gon_init();
ps("Gonatopus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gon_planning(t,c,2068+(i*17),2057+(i*14),2037+(i*10),2019+(i*6),2020+(i%5));}
ps("\nGonatopus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gon_execution(t,c,2057+(i*15),2046+(i*12),2028+(i*8),2015+(i*5),2021+(i%4));}
ps("\nGonatopus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gon_evaluation(t,c,2049+(i*13),2038+(i*10),2022+(i*7),2011+(i*4),2022+(i%3));}
ps("\nGonatopus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gon_accessory(t,c,2041+(i*11),2032+(i*9),2018+(i*6),2008+(i*3),2023+(i%2));}
ps("\nGonatopus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gon_market(t,c,2035+(i*9),2026+(i*7),2015+(i*5),2007+(i*3),2024);}
ps("\n");gon_report();gon_state();ps("\n=== Demo Complete ===\n");return 0;}
