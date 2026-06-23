/* fittonia_admin: Fittonia management technology administration (v1.0)
 * Fittonia planning, fittonia execution, fittonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fitt_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} fitt_state_t;
static fitt_t fittp[N],fittx[N-2],fitt2[N-4],fittac[N-6],fittm[N-6]; static fitt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(fitt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fitt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FITT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fitt_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fittp[i].active=0;for(int i=0;i<N-2;i++)fittx[i].active=0;for(int i=0;i<N-4;i++)fitt2[i].active=0;for(int i=0;i<N-6;i++)fittac[i].active=0;for(int i=0;i<N-6;i++)fittm[i].active=0;init=1;ps("[FITT] Fittonia initialized\n");return 0;}
int fitt_planning(int t,int c,int a,int b,int d,int e,int y){return add(fittp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int fitt_execution(int t,int c,int a,int b,int d,int e,int y){return add(fittx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fitt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fitt2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fitt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fittac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fitt_market(int t,int c,int a,int b,int d,int e,int y){return add(fittm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fitt_report(void){ps("[FITT] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void fitt_state(void){ps("[FITT] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Fittonia Admin Demo ===\n\n");fitt_init();
ps("Fittonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fitt_planning(t,c,1847+(i*17),1836+(i*14),1816+(i*10),1798+(i*6),2020+(i%5));}
ps("\nFittonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fitt_execution(t,c,1836+(i*15),1825+(i*12),1807+(i*8),1794+(i*5),2021+(i%4));}
ps("\nFittonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fitt_evaluation(t,c,1828+(i*13),1817+(i*10),1801+(i*7),1790+(i*4),2022+(i%3));}
ps("\nFittonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fitt_accessory(t,c,1820+(i*11),1811+(i*9),1797+(i*6),1787+(i*3),2023+(i%2));}
ps("\nFittonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fitt_market(t,c,1814+(i*9),1805+(i*7),1792+(i*5),1784+(i*3),2024);}
ps("\n");fitt_report();fitt_state();ps("\n=== Demo Complete ===\n");return 0;}
