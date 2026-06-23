/* cnidium_admin: Cnidium management technology administration (v1.0)
 * Cnidium planning, cnidium execution, cnidium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cnid_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} cnid_state_t;
static cnid_t cnidp[N],cnidx[N-2],cnid2[N-4],cnidac[N-6],cnidm[N-6]; static cnid_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cnid_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cnid_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CNID] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cnid_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cnidp[i].active=0;for(int i=0;i<N-2;i++)cnidx[i].active=0;for(int i=0;i<N-4;i++)cnid2[i].active=0;for(int i=0;i<N-6;i++)cnidac[i].active=0;for(int i=0;i<N-6;i++)cnidm[i].active=0;init=1;ps("[CNID] Cnidium initialized\n");return 0;}
int cnid_planning(int t,int c,int a,int b,int d,int e,int y){return add(cnidp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int cnid_execution(int t,int c,int a,int b,int d,int e,int y){return add(cnidx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cnid_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cnid2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cnid_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cnidac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cnid_market(int t,int c,int a,int b,int d,int e,int y){return add(cnidm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cnid_report(void){ps("[CNID] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void cnid_state(void){ps("[CNID] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Cnidium Admin Demo ===\n\n");cnid_init();
ps("Cnidium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cnid_planning(t,c,1980+(i*17),1969+(i*14),1949+(i*10),1931+(i*6),2020+(i%5));}
ps("\nCnidium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cnid_execution(t,c,1969+(i*15),1958+(i*12),1940+(i*8),1927+(i*5),2021+(i%4));}
ps("\nCnidium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cnid_evaluation(t,c,1961+(i*13),1950+(i*10),1934+(i*7),1923+(i*4),2022+(i%3));}
ps("\nCnidium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnid_accessory(t,c,1953+(i*11),1944+(i*9),1930+(i*6),1920+(i*3),2023+(i%2));}
ps("\nCnidium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnid_market(t,c,1947+(i*9),1938+(i*7),1925+(i*5),1917+(i*3),2024);}
ps("\n");cnid_report();cnid_state();ps("\n=== Demo Complete ===\n");return 0;}
