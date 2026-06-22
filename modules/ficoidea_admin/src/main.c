/* ficoidea_admin: Ficoidea management technology administration (v1.0)
 * Ficoidea planning, ficoidea execution, ficoidea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fico_t;
typedef struct{int n_ficop,n_fico,n_fico2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fico_state_t;
static fico_t ficos[N],ficoe[N-2],fico2[N-4],ficoac[N-6],ficoam[N-6]; static fico_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(fico_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fico_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FICO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fico_init(void){if(init)return -1;st.n_ficop=0;st.n_fico=0;st.n_fico2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ficos[i].active=0;for(int i=0;i<N-2;i++)ficoe[i].active=0;for(int i=0;i<N-4;i++)fico2[i].active=0;for(int i=0;i<N-6;i++)ficoac[i].active=0;for(int i=0;i<N-6;i++)ficoam[i].active=0;init=1;ps("[FICO] Ficoidea initialized\n");return 0;}
int fico_planning(int t,int c,int a,int b,int d,int e,int y){return add(ficos,&st.n_ficop,&st.t_f1,N,t,c,a,b,d,e,y);}
int fico_execution(int t,int c,int a,int b,int d,int e,int y){return add(ficoe,&st.n_fico,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fico_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fico2,&st.n_fico2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fico_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ficoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fico_market(int t,int c,int a,int b,int d,int e,int y){return add(ficoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fico_report(void){ps("[FICO] Cop: ");pi(st.n_ficop);ps(" PCS=");pi(st.t_f1);ps("\nCoe: ");pi(st.n_fico);ps(" PCS=");pi(st.t_f2);ps("\nCo2: ");pi(st.n_fico2);ps(" PCS=");pi(st.t_f3);ps("\nCac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fico_state(void){ps("[FICO] Cop=");pi(st.n_ficop);ps(" Co=");pi(st.n_fico);ps(" Co2=");pi(st.n_fico2);ps(" Cac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ficoidea Admin Demo ===\n\n");fico_init();
ps("Ficoidea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fico_planning(t,c,1513+(i*17),1502+(i*14),1482+(i*10),1464+(i*6),2020+(i%5));}
ps("\nFicoidea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fico_execution(t,c,1502+(i*15),1491+(i*12),1473+(i*8),1460+(i*5),2021+(i%4));}
ps("\nFicoidea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fico_evaluation(t,c,1494+(i*13),1483+(i*10),1467+(i*7),1456+(i*4),2022+(i%3));}
ps("\nFicoidea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fico_accessory(t,c,1486+(i*11),1477+(i*9),1463+(i*6),1453+(i*3),2023+(i%2));}
ps("\nFicoidea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fico_market(t,c,1480+(i*9),1471+(i*7),1458+(i*5),1450+(i*3),2024);}
ps("\n");fico_report();fico_state();ps("\n=== Demo Complete ===\n");return 0;}
