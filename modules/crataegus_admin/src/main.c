/* crataegus_admin: Crataegus management technology administration (v1.0)
 * Crataegus planning, crataegus execution, crataegus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} crt_t;
typedef struct{int n_crtp,n_crte,n_crt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} crt_state_t;
static crt_t crtps[N],crtss[N-2],crtvss[N-4],crtas[N-6],crtmks[N-6]; static crt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(crt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;crt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int crt_init(void){if(init)return -1;st.n_crtp=0;st.n_crte=0;st.n_crt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crtps[i].active=0;for(int i=0;i<N-2;i++)crtss[i].active=0;for(int i=0;i<N-4;i++)crtvss[i].active=0;for(int i=0;i<N-6;i++)crtas[i].active=0;for(int i=0;i<N-6;i++)crtmks[i].active=0;init=1;ps("[CRT] Crataegus initialized\n");return 0;}
int crt_planning(int t,int c,int a,int b,int d,int e,int y){return add(crtps,&st.n_crtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int crt_execution(int t,int c,int a,int b,int d,int e,int y){return add(crtss,&st.n_crte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int crt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(crtvss,&st.n_crt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int crt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(crtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int crt_market(int t,int c,int a,int b,int d,int e,int y){return add(crtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void crt_report(void){ps("[CRT] Crapp: ");pi(st.n_crtp);ps(" PCS=");pi(st.t_f1);ps("\nCrae: ");pi(st.n_crte);ps(" PCS=");pi(st.t_f2);ps("\nCrav: ");pi(st.n_crt2);ps(" PCS=");pi(st.t_f3);ps("\nCrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void crt_state(void){ps("[CRT] Crapp=");pi(st.n_crtp);ps(" Crae=");pi(st.n_crte);ps(" Crav=");pi(st.n_crt2);ps(" Crac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crataegus Admin Demo ===\n\n");crt_init();
ps("Crataegus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;crt_planning(t,c,1133+(i*17),1122+(i*14),1102+(i*10),1084+(i*6),2020+(i%5));}
ps("\nCrataegus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;crt_execution(t,c,1122+(i*15),1111+(i*12),1093+(i*8),1080+(i*5),2021+(i%4));}
ps("\nCrataegus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;crt_evaluation(t,c,1114+(i*13),1103+(i*10),1087+(i*7),1076+(i*4),2022+(i%3));}
ps("\nCrataegus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crt_accessory(t,c,1106+(i*11),1097+(i*9),1083+(i*6),1073+(i*3),2023+(i%2));}
ps("\nCrataegus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;crt_market(t,c,1100+(i*9),1091+(i*7),1078+(i*5),1070+(i*3),2024);}
ps("\n");crt_report();crt_state();ps("\n=== Demo Complete ===\n");return 0;}
