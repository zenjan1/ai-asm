/* chionanthus_admin: Chionanthus management technology administration (v1.0)
 * Chionanthus planning, chionanthus execution, chionanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chio_t;
typedef struct{int n_chiop,n_chio,n_chio2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chio_state_t;
static chio_t chios[N],chioe[N-2],chio2[N-4],chioac[N-6],chioam[N-6]; static chio_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(chio_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chio_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHIO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chio_init(void){if(init)return -1;st.n_chiop=0;st.n_chio=0;st.n_chio2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chios[i].active=0;for(int i=0;i<N-2;i++)chioe[i].active=0;for(int i=0;i<N-4;i++)chio2[i].active=0;for(int i=0;i<N-6;i++)chioac[i].active=0;for(int i=0;i<N-6;i++)chioam[i].active=0;init=1;ps("[CHIO] Chionanthus initialized\n");return 0;}
int chio_planning(int t,int c,int a,int b,int d,int e,int y){return add(chios,&st.n_chiop,&st.t_f1,N,t,c,a,b,d,e,y);}
int chio_execution(int t,int c,int a,int b,int d,int e,int y){return add(chioe,&st.n_chio,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chio_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chio2,&st.n_chio2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chio_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chioac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chio_market(int t,int c,int a,int b,int d,int e,int y){return add(chioam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chio_report(void){ps("[CHIO] Iop: ");pi(st.n_chiop);ps(" PCS=");pi(st.t_f1);ps("\nIoe: ");pi(st.n_chio);ps(" PCS=");pi(st.t_f2);ps("\nIo2: ");pi(st.n_chio2);ps(" PCS=");pi(st.t_f3);ps("\nIac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chio_state(void){ps("[CHIO] Iop=");pi(st.n_chiop);ps(" Io=");pi(st.n_chio);ps(" Io2=");pi(st.n_chio2);ps(" Iac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chionanthus Admin Demo ===\n\n");chio_init();
ps("Chionanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chio_planning(t,c,1474+(i*17),1463+(i*14),1443+(i*10),1425+(i*6),2020+(i%5));}
ps("\nChionanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chio_execution(t,c,1463+(i*15),1452+(i*12),1434+(i*8),1421+(i*5),2021+(i%4));}
ps("\nChionanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chio_evaluation(t,c,1455+(i*13),1444+(i*10),1428+(i*7),1417+(i*4),2022+(i%3));}
ps("\nChionanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chio_accessory(t,c,1447+(i*11),1438+(i*9),1424+(i*6),1414+(i*3),2023+(i%2));}
ps("\nChionanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chio_market(t,c,1441+(i*9),1432+(i*7),1419+(i*5),1411+(i*3),2024);}
ps("\n");chio_report();chio_state();ps("\n=== Demo Complete ===\n");return 0;}
