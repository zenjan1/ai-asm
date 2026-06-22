/* cypella_admin: Cypella management technology administration (v1.0)
 * Cypella planning, cypella execution, cypella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cype_t;
typedef struct{int n_cypep,n_cype,n_cype2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cype_state_t;
static cype_t cypes[N],cypee[N-2],cype2[N-4],cypeac[N-6],cypeam[N-6]; static cype_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cype_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cype_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYPE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cype_init(void){if(init)return -1;st.n_cypep=0;st.n_cype=0;st.n_cype2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cypes[i].active=0;for(int i=0;i<N-2;i++)cypee[i].active=0;for(int i=0;i<N-4;i++)cype2[i].active=0;for(int i=0;i<N-6;i++)cypeac[i].active=0;for(int i=0;i<N-6;i++)cypeam[i].active=0;init=1;ps("[CYPE] Cypella initialized\n");return 0;}
int cype_planning(int t,int c,int a,int b,int d,int e,int y){return add(cypes,&st.n_cypep,&st.t_f1,N,t,c,a,b,d,e,y);}
int cype_execution(int t,int c,int a,int b,int d,int e,int y){return add(cypee,&st.n_cype,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cype_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cype2,&st.n_cype2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cype_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cypeac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cype_market(int t,int c,int a,int b,int d,int e,int y){return add(cypeam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cype_report(void){ps("[CYPE] Pep: ");pi(st.n_cypep);ps(" PCS=");pi(st.t_f1);ps("\nPee: ");pi(st.n_cype);ps(" PCS=");pi(st.t_f2);ps("\nPe2: ");pi(st.n_cype2);ps(" PCS=");pi(st.t_f3);ps("\nPac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cype_state(void){ps("[CYPE] Pep=");pi(st.n_cypep);ps(" Pe=");pi(st.n_cype);ps(" Pe2=");pi(st.n_cype2);ps(" Pac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cypella Admin Demo ===\n\n");cype_init();
ps("Cypella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cype_planning(t,c,1485+(i*17),1474+(i*14),1454+(i*10),1436+(i*6),2020+(i%5));}
ps("\nCypella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cype_execution(t,c,1474+(i*15),1463+(i*12),1445+(i*8),1432+(i*5),2021+(i%4));}
ps("\nCypella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cype_evaluation(t,c,1466+(i*13),1455+(i*10),1439+(i*7),1428+(i*4),2022+(i%3));}
ps("\nCypella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cype_accessory(t,c,1458+(i*11),1449+(i*9),1435+(i*6),1425+(i*3),2023+(i%2));}
ps("\nCypella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cype_market(t,c,1452+(i*9),1443+(i*7),1430+(i*5),1422+(i*3),2024);}
ps("\n");cype_report();cype_state();ps("\n=== Demo Complete ===\n");return 0;}
