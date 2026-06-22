/* davidia_admin: Davidia management technology administration (v1.0)
 * Davidia planning, davidia execution, davidia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} davi_t;
typedef struct{int n_davip,n_davi,n_davi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} davi_state_t;
static davi_t davis[N],davie[N-2],davi2[N-4],daviac[N-6],daviam[N-6]; static davi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(davi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;davi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAVI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int davi_init(void){if(init)return -1;st.n_davip=0;st.n_davi=0;st.n_davi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)davis[i].active=0;for(int i=0;i<N-2;i++)davie[i].active=0;for(int i=0;i<N-4;i++)davi2[i].active=0;for(int i=0;i<N-6;i++)daviac[i].active=0;for(int i=0;i<N-6;i++)daviam[i].active=0;init=1;ps("[DAVI] Davidia initialized\n");return 0;}
int davi_planning(int t,int c,int a,int b,int d,int e,int y){return add(davis,&st.n_davip,&st.t_f1,N,t,c,a,b,d,e,y);}
int davi_execution(int t,int c,int a,int b,int d,int e,int y){return add(davie,&st.n_davi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int davi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(davi2,&st.n_davi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int davi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(daviac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int davi_market(int t,int c,int a,int b,int d,int e,int y){return add(daviam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void davi_report(void){ps("[DAVI] Vip: ");pi(st.n_davip);ps(" PCS=");pi(st.t_f1);ps("\nVie: ");pi(st.n_davi);ps(" PCS=");pi(st.t_f2);ps("\nVi2: ");pi(st.n_davi2);ps(" PCS=");pi(st.t_f3);ps("\nVac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void davi_state(void){ps("[DAVI] Vip=");pi(st.n_davip);ps(" Vi=");pi(st.n_davi);ps(" Vi2=");pi(st.n_davi2);ps(" Vac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Davidia Admin Demo ===\n\n");davi_init();
ps("Davidia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;davi_planning(t,c,1491+(i*17),1480+(i*14),1460+(i*10),1442+(i*6),2020+(i%5));}
ps("\nDavidia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;davi_execution(t,c,1480+(i*15),1469+(i*12),1451+(i*8),1438+(i*5),2021+(i%4));}
ps("\nDavidia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;davi_evaluation(t,c,1472+(i*13),1461+(i*10),1445+(i*7),1434+(i*4),2022+(i%3));}
ps("\nDavidia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;davi_accessory(t,c,1464+(i*11),1455+(i*9),1441+(i*6),1431+(i*3),2023+(i%2));}
ps("\nDavidia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;davi_market(t,c,1458+(i*9),1449+(i*7),1436+(i*5),1428+(i*3),2024);}
ps("\n");davi_report();davi_state();ps("\n=== Demo Complete ===\n");return 0;}
