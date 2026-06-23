/* medeola_admin: Medeola management technology administration (v1.0)
 * Medeola planning, medeola execution, medeola evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mede_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} mede_state_t;
static mede_t medep[N],medex[N-2],mede2[N-4],medeac[N-6],medem[N-6]; static mede_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mede_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mede_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MEDE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mede_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)medep[i].active=0;for(int i=0;i<N-2;i++)medex[i].active=0;for(int i=0;i<N-4;i++)mede2[i].active=0;for(int i=0;i<N-6;i++)medeac[i].active=0;for(int i=0;i<N-6;i++)medem[i].active=0;init=1;ps("[MEDE] Medeola initialized\n");return 0;}
int mede_planning(int t,int c,int a,int b,int d,int e,int y){return add(medep,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int mede_execution(int t,int c,int a,int b,int d,int e,int y){return add(medex,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mede_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mede2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mede_accessory(int t,int c,int a,int b,int d,int e,int y){return add(medeac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mede_market(int t,int c,int a,int b,int d,int e,int y){return add(medem,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mede_report(void){ps("[MEDE] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void mede_state(void){ps("[MEDE] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Medeola Admin Demo ===\n\n");mede_init();
ps("Medeola planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mede_planning(t,c,1915+(i*17),1904+(i*14),1884+(i*10),1866+(i*6),2020+(i%5));}
ps("\nMedeola execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mede_execution(t,c,1904+(i*15),1893+(i*12),1875+(i*8),1862+(i*5),2021+(i%4));}
ps("\nMedeola evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mede_evaluation(t,c,1896+(i*13),1885+(i*10),1869+(i*7),1858+(i*4),2022+(i%3));}
ps("\nMedeola accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mede_accessory(t,c,1888+(i*11),1879+(i*9),1865+(i*6),1855+(i*3),2023+(i%2));}
ps("\nMedeola marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mede_market(t,c,1882+(i*9),1873+(i*7),1860+(i*5),1852+(i*3),2024);}
ps("\n");mede_report();mede_state();ps("\n=== Demo Complete ===\n");return 0;}
