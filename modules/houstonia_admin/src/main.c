/* houstonia_admin: Houstonia management technology administration (v1.0)
 * Houstonia planning, houstonia execution, houstonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} houx_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} houx_state_t;
static houx_t houxp[N],houxx[N-2],houx2[N-4],houxac[N-6],houxm[N-6]; static houx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(houx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;houx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOUX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int houx_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)houxp[i].active=0;for(int i=0;i<N-2;i++)houxx[i].active=0;for(int i=0;i<N-4;i++)houx2[i].active=0;for(int i=0;i<N-6;i++)houxac[i].active=0;for(int i=0;i<N-6;i++)houxm[i].active=0;init=1;ps("[HOUX] Houstonia initialized\n");return 0;}
int houx_planning(int t,int c,int a,int b,int d,int e,int y){return add(houxp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int houx_execution(int t,int c,int a,int b,int d,int e,int y){return add(houxx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int houx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(houx2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int houx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(houxac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int houx_market(int t,int c,int a,int b,int d,int e,int y){return add(houxm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void houx_report(void){ps("[HOUX] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void houx_state(void){ps("[HOUX] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Houstonia Admin Demo ===\n\n");houx_init();
ps("Houstonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;houx_planning(t,c,1885+(i*17),1874+(i*14),1854+(i*10),1836+(i*6),2020+(i%5));}
ps("\nHoustonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;houx_execution(t,c,1874+(i*15),1863+(i*12),1845+(i*8),1832+(i*5),2021+(i%4));}
ps("\nHoustonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;houx_evaluation(t,c,1866+(i*13),1855+(i*10),1839+(i*7),1828+(i*4),2022+(i%3));}
ps("\nHoustonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;houx_accessory(t,c,1858+(i*11),1849+(i*9),1835+(i*6),1825+(i*3),2023+(i%2));}
ps("\nHoustonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;houx_market(t,c,1852+(i*9),1843+(i*7),1830+(i*5),1822+(i*3),2024);}
ps("\n");houx_report();houx_state();ps("\n=== Demo Complete ===\n");return 0;}
