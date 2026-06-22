/* delonix_admin: Delonix management technology administration (v1.0)
 * Delonix planning, delonix execution, delonix evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} delo_t;
typedef struct{int n_delop,n_delo,n_delo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} delo_state_t;
static delo_t delos[N],deloe[N-2],delo2[N-4],deloac[N-6],deloam[N-6]; static delo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(delo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;delo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DELO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int delo_init(void){if(init)return -1;st.n_delop=0;st.n_delo=0;st.n_delo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)delos[i].active=0;for(int i=0;i<N-2;i++)deloe[i].active=0;for(int i=0;i<N-4;i++)delo2[i].active=0;for(int i=0;i<N-6;i++)deloac[i].active=0;for(int i=0;i<N-6;i++)deloam[i].active=0;init=1;ps("[DELO] Delonix initialized\n");return 0;}
int delo_planning(int t,int c,int a,int b,int d,int e,int y){return add(delos,&st.n_delop,&st.t_f1,N,t,c,a,b,d,e,y);}
int delo_execution(int t,int c,int a,int b,int d,int e,int y){return add(deloe,&st.n_delo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int delo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(delo2,&st.n_delo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int delo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(deloac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int delo_market(int t,int c,int a,int b,int d,int e,int y){return add(deloam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void delo_report(void){ps("[DELO] Lop: ");pi(st.n_delop);ps(" PCS=");pi(st.t_f1);ps("\nLoe: ");pi(st.n_delo);ps(" PCS=");pi(st.t_f2);ps("\nLo2: ");pi(st.n_delo2);ps(" PCS=");pi(st.t_f3);ps("\nLac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void delo_state(void){ps("[DELO] Lop=");pi(st.n_delop);ps(" Lo=");pi(st.n_delo);ps(" Lo2=");pi(st.n_delo2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Delonix Admin Demo ===\n\n");delo_init();
ps("Delonix planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;delo_planning(t,c,1493+(i*17),1482+(i*14),1462+(i*10),1444+(i*6),2020+(i%5));}
ps("\nDelonix execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;delo_execution(t,c,1482+(i*15),1471+(i*12),1453+(i*8),1440+(i*5),2021+(i%4));}
ps("\nDelonix evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;delo_evaluation(t,c,1474+(i*13),1463+(i*10),1447+(i*7),1436+(i*4),2022+(i%3));}
ps("\nDelonix accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;delo_accessory(t,c,1466+(i*11),1457+(i*9),1443+(i*6),1433+(i*3),2023+(i%2));}
ps("\nDelonix marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;delo_market(t,c,1460+(i*9),1451+(i*7),1438+(i*5),1430+(i*3),2024);}
ps("\n");delo_report();delo_state();ps("\n=== Demo Complete ===\n");return 0;}
