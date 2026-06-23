/* basilicum_admin: Basilicum management technology administration (v1.0)
 * Basilicum planning, basilicum execution, basilicum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} basi_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} basi_state_t;
static basi_t basip[N],basix[N-2],basi2[N-4],basiac[N-6],basim[N-6]; static basi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(basi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;basi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BASI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int basi_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)basip[i].active=0;for(int i=0;i<N-2;i++)basix[i].active=0;for(int i=0;i<N-4;i++)basi2[i].active=0;for(int i=0;i<N-6;i++)basiac[i].active=0;for(int i=0;i<N-6;i++)basim[i].active=0;init=1;ps("[BASI] Basilicum initialized\n");return 0;}
int basi_planning(int t,int c,int a,int b,int d,int e,int y){return add(basip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int basi_execution(int t,int c,int a,int b,int d,int e,int y){return add(basix,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int basi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(basi2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int basi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(basiac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int basi_market(int t,int c,int a,int b,int d,int e,int y){return add(basim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void basi_report(void){ps("[BASI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void basi_state(void){ps("[BASI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Basilicum Admin Demo ===\n\n");basi_init();
ps("Basilicum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;basi_planning(t,c,1784+(i*17),1773+(i*14),1753+(i*10),1735+(i*6),2020+(i%5));}
ps("\nBasilicum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;basi_execution(t,c,1773+(i*15),1762+(i*12),1744+(i*8),1731+(i*5),2021+(i%4));}
ps("\nBasilicum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;basi_evaluation(t,c,1765+(i*13),1754+(i*10),1738+(i*7),1727+(i*4),2022+(i%3));}
ps("\nBasilicum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;basi_accessory(t,c,1757+(i*11),1748+(i*9),1734+(i*6),1724+(i*3),2023+(i%2));}
ps("\nBasilicum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;basi_market(t,c,1751+(i*9),1742+(i*7),1729+(i*5),1721+(i*3),2024);}
ps("\n");basi_report();basi_state();ps("\n=== Demo Complete ===\n");return 0;}
