/* casuarina_admin: Casuarina management technology administration (v1.0)
 * Casuarina planning, casuarina execution, casuarina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} casu_t;
typedef struct{int n_casup,n_casu,n_casu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} casu_state_t;
static casu_t casus[N],casue[N-2],casu2[N-4],casuac[N-6],casuam[N-6]; static casu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(casu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;casu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CASU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int casu_init(void){if(init)return -1;st.n_casup=0;st.n_casu=0;st.n_casu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)casus[i].active=0;for(int i=0;i<N-2;i++)casue[i].active=0;for(int i=0;i<N-4;i++)casu2[i].active=0;for(int i=0;i<N-6;i++)casuac[i].active=0;for(int i=0;i<N-6;i++)casuam[i].active=0;init=1;ps("[CASU] Casuarina initialized\n");return 0;}
int casu_planning(int t,int c,int a,int b,int d,int e,int y){return add(casus,&st.n_casup,&st.t_f1,N,t,c,a,b,d,e,y);}
int casu_execution(int t,int c,int a,int b,int d,int e,int y){return add(casue,&st.n_casu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int casu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(casu2,&st.n_casu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int casu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(casuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int casu_market(int t,int c,int a,int b,int d,int e,int y){return add(casuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void casu_report(void){ps("[CASU] Asup: ");pi(st.n_casup);ps(" PCS=");pi(st.t_f1);ps("\nAsue: ");pi(st.n_casu);ps(" PCS=");pi(st.t_f2);ps("\nAsu2: ");pi(st.n_casu2);ps(" PCS=");pi(st.t_f3);ps("\nAsac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void casu_state(void){ps("[CASU] Asup=");pi(st.n_casup);ps(" Asu=");pi(st.n_casu);ps(" Asu2=");pi(st.n_casu2);ps(" Asac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Casuarina Admin Demo ===\n\n");casu_init();
ps("Casuarina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;casu_planning(t,c,1465+(i*17),1454+(i*14),1434+(i*10),1416+(i*6),2020+(i%5));}
ps("\nCasuarina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;casu_execution(t,c,1454+(i*15),1443+(i*12),1425+(i*8),1412+(i*5),2021+(i%4));}
ps("\nCasuarina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;casu_evaluation(t,c,1446+(i*13),1435+(i*10),1419+(i*7),1408+(i*4),2022+(i%3));}
ps("\nCasuarina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;casu_accessory(t,c,1438+(i*11),1429+(i*9),1415+(i*6),1405+(i*3),2023+(i%2));}
ps("\nCasuarina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;casu_market(t,c,1432+(i*9),1423+(i*7),1410+(i*5),1402+(i*3),2024);}
ps("\n");casu_report();casu_state();ps("\n=== Demo Complete ===\n");return 0;}
