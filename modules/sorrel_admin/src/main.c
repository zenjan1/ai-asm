/* sorrel_admin: Sorrel management technology administration (v1.0)
 * Sorrel planning, sorrel execution, sorrel evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sr_t;
typedef struct{int n_srp,n_sre,n_srv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sr_state_t;
static sr_t srps[N],srss[N-2],srvss[N-4],sras[N-6],srmks[N-6]; static sr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SRL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sr_init(void){if(init)return -1;st.n_srp=0;st.n_sre=0;st.n_srv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)srps[i].active=0;for(int i=0;i<N-2;i++)srss[i].active=0;for(int i=0;i<N-4;i++)srvss[i].active=0;for(int i=0;i<N-6;i++)sras[i].active=0;for(int i=0;i<N-6;i++)srmks[i].active=0;init=1;ps("[SRL] Sorrel initialized\n");return 0;}
int sr_planning(int t,int c,int a,int b,int d,int e,int y){return add(srps,&st.n_srp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sr_execution(int t,int c,int a,int b,int d,int e,int y){return add(srss,&st.n_sre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(srvss,&st.n_srv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sr_market(int t,int c,int a,int b,int d,int e,int y){return add(srmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sr_report(void){ps("[SRL] Srp: ");pi(st.n_srp);ps(" PCS=");pi(st.t_f1);ps("\nSre: ");pi(st.n_sre);ps(" PCS=");pi(st.t_f2);ps("\nSrv: ");pi(st.n_srv);ps(" PCS=");pi(st.t_f3);ps("\nSrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sr_state(void){ps("[SRL] Srp=");pi(st.n_srp);ps(" Sre=");pi(st.n_sre);ps(" Srv=");pi(st.n_srv);ps(" Src=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sorrel Admin Demo ===\n\n");sr_init();
ps("Sorrel planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sr_planning(t,c,686+(i*17),675+(i*14),655+(i*10),637+(i*6),2020+(i%5));}
ps("\nSorrel execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sr_execution(t,c,675+(i*15),664+(i*12),646+(i*8),633+(i*5),2021+(i%4));}
ps("\nSorrel evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sr_evaluation(t,c,667+(i*13),656+(i*10),640+(i*7),629+(i*4),2022+(i%3));}
ps("\nSorrel accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sr_accessory(t,c,659+(i*11),650+(i*9),636+(i*6),626+(i*3),2023+(i%2));}
ps("\nSorrel marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sr_market(t,c,653+(i*9),644+(i*7),631+(i*5),623+(i*3),2024);}
ps("\n");sr_report();sr_state();ps("\n=== Demo Complete ===\n");return 0;}
