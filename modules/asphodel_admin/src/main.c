/* asphodel_admin: Asphodel management technology administration (v1.0)
 * Asphodel planning, asphodel execution, asphodel evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asp_t;
typedef struct{int n_app,n_ape,n_apv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asp_state_t;
static asp_t aspps[N],aspss[N-2],aspvss[N-4],aspas[N-6],aspmks[N-6]; static asp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asp_init(void){if(init)return -1;st.n_app=0;st.n_ape=0;st.n_apv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aspps[i].active=0;for(int i=0;i<N-2;i++)aspss[i].active=0;for(int i=0;i<N-4;i++)aspvss[i].active=0;for(int i=0;i<N-6;i++)aspas[i].active=0;for(int i=0;i<N-6;i++)aspmks[i].active=0;init=1;ps("[ASP] Asphodel initialized\n");return 0;}
int asp_planning(int t,int c,int a,int b,int d,int e,int y){return add(aspps,&st.n_app,&st.t_f1,N,t,c,a,b,d,e,y);}
int asp_execution(int t,int c,int a,int b,int d,int e,int y){return add(aspss,&st.n_ape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aspvss,&st.n_apv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aspas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asp_market(int t,int c,int a,int b,int d,int e,int y){return add(aspmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asp_report(void){ps("[ASP] App: ");pi(st.n_app);ps(" PCS=");pi(st.t_f1);ps("\nApe: ");pi(st.n_ape);ps(" PCS=");pi(st.t_f2);ps("\nApv: ");pi(st.n_apv);ps(" PCS=");pi(st.t_f3);ps("\nApc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asp_state(void){ps("[ASP] App=");pi(st.n_app);ps(" Ape=");pi(st.n_ape);ps(" Apv=");pi(st.n_apv);ps(" Apc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Asphodel Admin Demo ===\n\n");asp_init();
ps("Asphodel planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asp_planning(t,c,887+(i*17),876+(i*14),856+(i*10),838+(i*6),2020+(i%5));}
ps("\nAsphodel execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asp_execution(t,c,876+(i*15),865+(i*12),847+(i*8),834+(i*5),2021+(i%4));}
ps("\nAsphodel evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asp_evaluation(t,c,868+(i*13),857+(i*10),841+(i*7),830+(i*4),2022+(i%3));}
ps("\nAsphodel accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asp_accessory(t,c,860+(i*11),851+(i*9),837+(i*6),827+(i*3),2023+(i%2));}
ps("\nAsphodel marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asp_market(t,c,854+(i*9),845+(i*7),832+(i*5),824+(i*3),2024);}
ps("\n");asp_report();asp_state();ps("\n=== Demo Complete ===\n");return 0;}
