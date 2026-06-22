/* euryale_admin: Euryale management technology administration (v1.0)
 * Euryale planning, euryale execution, euryale evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eury_t;
typedef struct{int n_euryp,n_eury,n_eury2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eury_state_t;
static eury_t eurys[N],eurye[N-2],eury2[N-4],euryac[N-6],euryam[N-6]; static eury_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eury_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eury_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EURY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eury_init(void){if(init)return -1;st.n_euryp=0;st.n_eury=0;st.n_eury2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eurys[i].active=0;for(int i=0;i<N-2;i++)eurye[i].active=0;for(int i=0;i<N-4;i++)eury2[i].active=0;for(int i=0;i<N-6;i++)euryac[i].active=0;for(int i=0;i<N-6;i++)euryam[i].active=0;init=1;ps("[EURY] Euryale initialized\n");return 0;}
int eury_planning(int t,int c,int a,int b,int d,int e,int y){return add(eurys,&st.n_euryp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eury_execution(int t,int c,int a,int b,int d,int e,int y){return add(eurye,&st.n_eury,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eury_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eury2,&st.n_eury2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eury_accessory(int t,int c,int a,int b,int d,int e,int y){return add(euryac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eury_market(int t,int c,int a,int b,int d,int e,int y){return add(euryam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eury_report(void){ps("[EURY] Ryp: ");pi(st.n_euryp);ps(" PCS=");pi(st.t_f1);ps("\nRye: ");pi(st.n_eury);ps(" PCS=");pi(st.t_f2);ps("\nRy2: ");pi(st.n_eury2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eury_state(void){ps("[EURY] Ryp=");pi(st.n_euryp);ps(" Ry=");pi(st.n_eury);ps(" Ry2=");pi(st.n_eury2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Euryale Admin Demo ===\n\n");eury_init();
ps("Euryale planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eury_planning(t,c,1507+(i*17),1496+(i*14),1476+(i*10),1458+(i*6),2020+(i%5));}
ps("\nEuryale execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eury_execution(t,c,1496+(i*15),1485+(i*12),1467+(i*8),1454+(i*5),2021+(i%4));}
ps("\nEuryale evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eury_evaluation(t,c,1488+(i*13),1477+(i*10),1461+(i*7),1450+(i*4),2022+(i%3));}
ps("\nEuryale accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eury_accessory(t,c,1480+(i*11),1471+(i*9),1457+(i*6),1447+(i*3),2023+(i%2));}
ps("\nEuryale marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eury_market(t,c,1474+(i*9),1465+(i*7),1452+(i*5),1444+(i*3),2024);}
ps("\n");eury_report();eury_state();ps("\n=== Demo Complete ===\n");return 0;}
