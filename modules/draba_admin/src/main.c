/* draba_admin: Draba management technology administration (v1.0)
 * Draba planning, draba execution, draba evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drab_t;
typedef struct{int n_drabp,n_drab,n_drab2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drab_state_t;
static drab_t drabs[N],drabe[N-2],drab2[N-4],drabac[N-6],drabam[N-6]; static drab_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(drab_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drab_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRAB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drab_init(void){if(init)return -1;st.n_drabp=0;st.n_drab=0;st.n_drab2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drabs[i].active=0;for(int i=0;i<N-2;i++)drabe[i].active=0;for(int i=0;i<N-4;i++)drab2[i].active=0;for(int i=0;i<N-6;i++)drabac[i].active=0;for(int i=0;i<N-6;i++)drabam[i].active=0;init=1;ps("[DRAB] Draba initialized\n");return 0;}
int drab_planning(int t,int c,int a,int b,int d,int e,int y){return add(drabs,&st.n_drabp,&st.t_f1,N,t,c,a,b,d,e,y);}
int drab_execution(int t,int c,int a,int b,int d,int e,int y){return add(drabe,&st.n_drab,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drab_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(drab2,&st.n_drab2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drab_accessory(int t,int c,int a,int b,int d,int e,int y){return add(drabac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drab_market(int t,int c,int a,int b,int d,int e,int y){return add(drabam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drab_report(void){ps("[DRAB] Abp: ");pi(st.n_drabp);ps(" PCS=");pi(st.t_f1);ps("\nAbe: ");pi(st.n_drab);ps(" PCS=");pi(st.t_f2);ps("\nAb2: ");pi(st.n_drab2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drab_state(void){ps("[DRAB] Abp=");pi(st.n_drabp);ps(" Ab=");pi(st.n_drab);ps(" Ab2=");pi(st.n_drab2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Draba Admin Demo ===\n\n");drab_init();
ps("Draba planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drab_planning(t,c,1498+(i*17),1487+(i*14),1467+(i*10),1449+(i*6),2020+(i%5));}
ps("\nDraba execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drab_execution(t,c,1487+(i*15),1476+(i*12),1458+(i*8),1445+(i*5),2021+(i%4));}
ps("\nDraba evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drab_evaluation(t,c,1479+(i*13),1468+(i*10),1452+(i*7),1441+(i*4),2022+(i%3));}
ps("\nDraba accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drab_accessory(t,c,1471+(i*11),1462+(i*9),1448+(i*6),1438+(i*3),2023+(i%2));}
ps("\nDraba marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drab_market(t,c,1465+(i*9),1456+(i*7),1443+(i*5),1435+(i*3),2024);}
ps("\n");drab_report();drab_state();ps("\n=== Demo Complete ===\n");return 0;}
