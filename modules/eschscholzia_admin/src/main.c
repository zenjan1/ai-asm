/* eschscholzia_admin: Eschscholzia management technology administration (v1.0)
 * Eschscholzia planning, eschscholzia execution, eschscholzia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} esch_t;
typedef struct{int n_eschp,n_esch,n_esch2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} esch_state_t;
static esch_t eschs[N],esche[N-2],esch2[N-4],eschac[N-6],escham[N-6]; static esch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(esch_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;esch_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ESCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int esch_init(void){if(init)return -1;st.n_eschp=0;st.n_esch=0;st.n_esch2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eschs[i].active=0;for(int i=0;i<N-2;i++)esche[i].active=0;for(int i=0;i<N-4;i++)esch2[i].active=0;for(int i=0;i<N-6;i++)eschac[i].active=0;for(int i=0;i<N-6;i++)escham[i].active=0;init=1;ps("[ESCH] Eschscholzia initialized\n");return 0;}
int esch_planning(int t,int c,int a,int b,int d,int e,int y){return add(eschs,&st.n_eschp,&st.t_f1,N,t,c,a,b,d,e,y);}
int esch_execution(int t,int c,int a,int b,int d,int e,int y){return add(esche,&st.n_esch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int esch_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(esch2,&st.n_esch2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int esch_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eschac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int esch_market(int t,int c,int a,int b,int d,int e,int y){return add(escham,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void esch_report(void){ps("[ESCH] Chp: ");pi(st.n_eschp);ps(" PCS=");pi(st.t_f1);ps("\nChe: ");pi(st.n_esch);ps(" PCS=");pi(st.t_f2);ps("\nCh2: ");pi(st.n_esch2);ps(" PCS=");pi(st.t_f3);ps("\nCac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void esch_state(void){ps("[ESCH] Chp=");pi(st.n_eschp);ps(" Ch=");pi(st.n_esch);ps(" Ch2=");pi(st.n_esch2);ps(" Cac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eschscholzia Admin Demo ===\n\n");esch_init();
ps("Eschscholzia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;esch_planning(t,c,1506+(i*17),1495+(i*14),1475+(i*10),1457+(i*6),2020+(i%5));}
ps("\nEschscholzia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;esch_execution(t,c,1495+(i*15),1484+(i*12),1466+(i*8),1453+(i*5),2021+(i%4));}
ps("\nEschscholzia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;esch_evaluation(t,c,1487+(i*13),1476+(i*10),1460+(i*7),1449+(i*4),2022+(i%3));}
ps("\nEschscholzia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;esch_accessory(t,c,1479+(i*11),1470+(i*9),1456+(i*6),1446+(i*3),2023+(i%2));}
ps("\nEschscholzia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;esch_market(t,c,1473+(i*9),1464+(i*7),1451+(i*5),1443+(i*3),2024);}
ps("\n");esch_report();esch_state();ps("\n=== Demo Complete ===\n");return 0;}
