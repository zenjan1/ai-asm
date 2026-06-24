/* fevillea_admin: Fevillea management technology administration (v1.0)
 * Fevillea planning, fevillea execution, fevillea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fevi_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} fevi_state_t;
static fevi_t fevip[N],fevix[N-2],fevi2[N-4],feviac[N-6],fevim[N-6]; static fevi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(fevi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fevi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FEVI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fevi_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fevip[i].active=0;for(int i=0;i<N-2;i++)fevix[i].active=0;for(int i=0;i<N-4;i++)fevi2[i].active=0;for(int i=0;i<N-6;i++)feviac[i].active=0;for(int i=0;i<N-6;i++)fevim[i].active=0;init=1;ps("[FEVI] Fevillea initialized\n");return 0;}
int fevi_planning(int t,int c,int a,int b,int d,int e,int y){return add(fevip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int fevi_execution(int t,int c,int a,int b,int d,int e,int y){return add(fevix,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fevi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fevi2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fevi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(feviac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fevi_market(int t,int c,int a,int b,int d,int e,int y){return add(fevim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fevi_report(void){ps("[FEVI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void fevi_state(void){ps("[FEVI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Fevillea Admin Demo ===\n\n");fevi_init();
ps("Fevillea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fevi_planning(t,c,2044+(i*17),2033+(i*14),2013+(i*10),1995+(i*6),2020+(i%5));}
ps("\nFevillea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fevi_execution(t,c,2033+(i*15),2022+(i*12),2004+(i*8),1991+(i*5),2021+(i%4));}
ps("\nFevillea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fevi_evaluation(t,c,2025+(i*13),2014+(i*10),1998+(i*7),1987+(i*4),2022+(i%3));}
ps("\nFevillea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fevi_accessory(t,c,2017+(i*11),2008+(i*9),1994+(i*6),1984+(i*3),2023+(i%2));}
ps("\nFevillea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fevi_market(t,c,2011+(i*9),2002+(i*7),1991+(i*5),1983+(i*3),2024);}
ps("\n");fevi_report();fevi_state();ps("\n=== Demo Complete ===\n");return 0;}
