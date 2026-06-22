/* dasymaschalon_admin: Dasymaschalon management technology administration (v1.0)
 * Dasymaschalon planning, dasymaschalon execution, dasymaschalon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} das_t;
typedef struct{int n_dasp,n_dase,n_das2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} das_state_t;
static das_t dasps[N],dasss[N-2],dasvss[N-4],dasas[N-6],dasmks[N-6]; static das_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(das_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;das_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int das_init(void){if(init)return -1;st.n_dasp=0;st.n_dase=0;st.n_das2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dasps[i].active=0;for(int i=0;i<N-2;i++)dasss[i].active=0;for(int i=0;i<N-4;i++)dasvss[i].active=0;for(int i=0;i<N-6;i++)dasas[i].active=0;for(int i=0;i<N-6;i++)dasmks[i].active=0;init=1;ps("[DAS] Dasymaschalon initialized\n");return 0;}
int das_planning(int t,int c,int a,int b,int d,int e,int y){return add(dasps,&st.n_dasp,&st.t_f1,N,t,c,a,b,d,e,y);}
int das_execution(int t,int c,int a,int b,int d,int e,int y){return add(dasss,&st.n_dase,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int das_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dasvss,&st.n_das2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int das_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dasas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int das_market(int t,int c,int a,int b,int d,int e,int y){return add(dasmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void das_report(void){ps("[DAS] Daspp: ");pi(st.n_dasp);ps(" PCS=");pi(st.t_f1);ps("\nDaspe: ");pi(st.n_dase);ps(" PCS=");pi(st.t_f2);ps("\nDasv: ");pi(st.n_das2);ps(" PCS=");pi(st.t_f3);ps("\nDasc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void das_state(void){ps("[DAS] Daspp=");pi(st.n_dasp);ps(" Daspe=");pi(st.n_dase);ps(" Dasv=");pi(st.n_das2);ps(" Dasc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dasymaschalon Admin Demo ===\n\n");das_init();
ps("Dasymaschalon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;das_planning(t,c,1256+(i*17),1245+(i*14),1225+(i*10),1207+(i*6),2020+(i%5));}
ps("\nDasymaschalon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;das_execution(t,c,1245+(i*15),1234+(i*12),1216+(i*8),1203+(i*5),2021+(i%4));}
ps("\nDasymaschalon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;das_evaluation(t,c,1237+(i*13),1226+(i*10),1210+(i*7),1199+(i*4),2022+(i%3));}
ps("\nDasymaschalon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;das_accessory(t,c,1229+(i*11),1220+(i*9),1206+(i*6),1196+(i*3),2023+(i%2));}
ps("\nDasymaschalon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;das_market(t,c,1223+(i*9),1214+(i*7),1201+(i*5),1193+(i*3),2024);}
ps("\n");das_report();das_state();ps("\n=== Demo Complete ===\n");return 0;}
