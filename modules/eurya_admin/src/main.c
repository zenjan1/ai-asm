/* eurya_admin: Eurya management technology administration (v1.0)
 * Eurya planning, eurya execution, eurya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eua_t;
typedef struct{int n_euap,n_euae,n_eua2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eua_state_t;
static eua_t euaps[N],euass[N-2],euavss[N-4],euaas[N-6],euamks[N-6]; static eua_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eua_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eua_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eua_init(void){if(init)return -1;st.n_euap=0;st.n_euae=0;st.n_eua2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)euaps[i].active=0;for(int i=0;i<N-2;i++)euass[i].active=0;for(int i=0;i<N-4;i++)euavss[i].active=0;for(int i=0;i<N-6;i++)euaas[i].active=0;for(int i=0;i<N-6;i++)euamks[i].active=0;init=1;ps("[EUA] Eurya initialized\n");return 0;}
int eua_planning(int t,int c,int a,int b,int d,int e,int y){return add(euaps,&st.n_euap,&st.t_f1,N,t,c,a,b,d,e,y);}
int eua_execution(int t,int c,int a,int b,int d,int e,int y){return add(euass,&st.n_euae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eua_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(euavss,&st.n_eua2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eua_accessory(int t,int c,int a,int b,int d,int e,int y){return add(euaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eua_market(int t,int c,int a,int b,int d,int e,int y){return add(euamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eua_report(void){ps("[EUA] Eurp: ");pi(st.n_euap);ps(" PCS=");pi(st.t_f1);ps("\nEure: ");pi(st.n_euae);ps(" PCS=");pi(st.t_f2);ps("\nEurv: ");pi(st.n_eua2);ps(" PCS=");pi(st.t_f3);ps("\nEurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eua_state(void){ps("[EUA] Eurp=");pi(st.n_euap);ps(" Eure=");pi(st.n_euae);ps(" Eurv=");pi(st.n_eua2);ps(" Eurc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eurya Admin Demo ===\n\n");eua_init();
ps("Eurya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eua_planning(t,c,1283+(i*17),1272+(i*14),1252+(i*10),1234+(i*6),2020+(i%5));}
ps("\nEurya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eua_execution(t,c,1272+(i*15),1261+(i*12),1243+(i*8),1230+(i*5),2021+(i%4));}
ps("\nEurya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eua_evaluation(t,c,1264+(i*13),1253+(i*10),1237+(i*7),1226+(i*4),2022+(i%3));}
ps("\nEurya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eua_accessory(t,c,1256+(i*11),1247+(i*9),1233+(i*6),1223+(i*3),2023+(i%2));}
ps("\nEurya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eua_market(t,c,1250+(i*9),1241+(i*7),1228+(i*5),1220+(i*3),2024);}
ps("\n");eua_report();eua_state();ps("\n=== Demo Complete ===\n");return 0;}
