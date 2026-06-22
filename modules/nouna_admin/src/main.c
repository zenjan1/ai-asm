/* nouna_admin: Nouna management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} noun_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} noun_state_t;
static noun_t nounp[N],nounx[N-2],noun2[N-4],nounac[N-6],nounm[N-6]; static noun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(noun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;noun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int noun_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nounp[i].active=0;for(int i=0;i<N-2;i++)nounx[i].active=0;for(int i=0;i<N-4;i++)noun2[i].active=0;for(int i=0;i<N-6;i++)nounac[i].active=0;for(int i=0;i<N-6;i++)nounm[i].active=0;init=1;ps("[NOUN] Nouna initialized\n");return 0;}
int noun_planning(int t,int c,int a,int b,int d,int e,int y){return add(nounp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int noun_execution(int t,int c,int a,int b,int d,int e,int y){return add(nounx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int noun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(noun2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int noun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nounac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int noun_market(int t,int c,int a,int b,int d,int e,int y){return add(nounm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void noun_report(void){ps("[NOUN] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void noun_state(void){ps("[NOUN] Pl=");pi(st.n_planning);ps(" Ex=");pi(st.n_execution);ps(" Ev=");pi(st.n_evaluation);ps(" Ac=");pi(st.n_accessory);ps(" Mk=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Nouna Admin Demo ===\n\n");noun_init();
ps("Nouna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;noun_planning(t,c,1615+(i*17),1604+(i*14),1584+(i*10),1566+(i*6),2020+(i%5));}
ps("\nNouna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;noun_execution(t,c,1604+(i*15),1593+(i*12),1575+(i*8),1562+(i*5),2021+(i%4));}
ps("\nNouna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;noun_evaluation(t,c,1596+(i*13),1585+(i*10),1569+(i*7),1558+(i*4),2022+(i%3));}
ps("\nNouna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;noun_accessory(t,c,1588+(i*11),1579+(i*9),1565+(i*6),1555+(i*3),2023+(i%2));}
ps("\nNouna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;noun_market(t,c,1582+(i*9),1573+(i*7),1560+(i*5),1552+(i*3),2024);}
ps("\n");noun_report();noun_state();ps("\n=== Demo Complete ===\n");return 0;}
