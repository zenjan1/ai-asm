/* japonica_admin: Japonica management technology administration (v1.0)
 * Japonica planning, japonica execution, japonica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} japo_t;
typedef struct{int n_japop,n_japo,n_japo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} japo_state_t;
static japo_t japos[N],japoe[N-2],japo2[N-4],japoac[N-6],japoam[N-6]; static japo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(japo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;japo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAPO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int japo_init(void){if(init)return -1;st.n_japop=0;st.n_japo=0;st.n_japo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)japos[i].active=0;for(int i=0;i<N-2;i++)japoe[i].active=0;for(int i=0;i<N-4;i++)japo2[i].active=0;for(int i=0;i<N-6;i++)japoac[i].active=0;for(int i=0;i<N-6;i++)japoam[i].active=0;init=1;ps("[JAPO] Japonica initialized\n");return 0;}
int japo_planning(int t,int c,int a,int b,int d,int e,int y){return add(japos,&st.n_japop,&st.t_f1,N,t,c,a,b,d,e,y);}
int japo_execution(int t,int c,int a,int b,int d,int e,int y){return add(japoe,&st.n_japo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int japo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(japo2,&st.n_japo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int japo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(japoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int japo_market(int t,int c,int a,int b,int d,int e,int y){return add(japoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void japo_report(void){ps("[JAPO] Japop: ");pi(st.n_japop);ps(" PCS=");pi(st.t_f1);ps("\nJapoe: ");pi(st.n_japo);ps(" PCS=");pi(st.t_f2);ps("\nJapo2: ");pi(st.n_japo2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void japo_state(void){ps("[JAPO] Japop=");pi(st.n_japop);ps(" Japo=");pi(st.n_japo);ps(" Japo2=");pi(st.n_japo2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Japonica Admin Demo ===\n\n");japo_init();
ps("Japonica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;japo_planning(t,c,1544+(i*17),1533+(i*14),1513+(i*10),1495+(i*6),2020+(i%5));}
ps("\nJaponica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;japo_execution(t,c,1533+(i*15),1522+(i*12),1504+(i*8),1491+(i*5),2021+(i%4));}
ps("\nJaponica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;japo_evaluation(t,c,1525+(i*13),1514+(i*10),1498+(i*7),1487+(i*4),2022+(i%3));}
ps("\nJaponica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;japo_accessory(t,c,1517+(i*11),1508+(i*9),1494+(i*6),1484+(i*3),2023+(i%2));}
ps("\nJaponica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;japo_market(t,c,1511+(i*9),1502+(i*7),1489+(i*5),1481+(i*3),2024);}
ps("\n");japo_report();japo_state();ps("\n=== Demo Complete ===\n");return 0;}
