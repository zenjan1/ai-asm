/* caryopteris_admin: Caryopteris management technology administration (v1.0)
 * Caryopteris planning, caryopteris execution, caryopteris evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cary_t;
typedef struct{int n_caryp,n_carye,n_cary2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cary_state_t;
static cary_t caryps[N],caryss[N-2],caryvss[N-4],caryas[N-6],carymks[N-6]; static cary_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cary_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cary_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CARY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cary_init(void){if(init)return -1;st.n_caryp=0;st.n_carye=0;st.n_cary2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)caryps[i].active=0;for(int i=0;i<N-2;i++)caryss[i].active=0;for(int i=0;i<N-4;i++)caryvss[i].active=0;for(int i=0;i<N-6;i++)caryas[i].active=0;for(int i=0;i<N-6;i++)carymks[i].active=0;init=1;ps("[CARY] Caryopteris initialized\n");return 0;}
int cary_planning(int t,int c,int a,int b,int d,int e,int y){return add(caryps,&st.n_caryp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cary_execution(int t,int c,int a,int b,int d,int e,int y){return add(caryss,&st.n_carye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cary_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(caryvss,&st.n_cary2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cary_accessory(int t,int c,int a,int b,int d,int e,int y){return add(caryas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cary_market(int t,int c,int a,int b,int d,int e,int y){return add(carymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cary_report(void){ps("[CARY] Carpp: ");pi(st.n_caryp);ps(" PCS=");pi(st.t_f1);ps("\nCare: ");pi(st.n_carye);ps(" PCS=");pi(st.t_f2);ps("\nCarv: ");pi(st.n_cary2);ps(" PCS=");pi(st.t_f3);ps("\nCarc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cary_state(void){ps("[CARY] Carpp=");pi(st.n_caryp);ps(" Care=");pi(st.n_carye);ps(" Carv=");pi(st.n_cary2);ps(" Carc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Caryopteris Admin Demo ===\n\n");cary_init();
ps("Caryopteris planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cary_planning(t,c,1126+(i*17),1115+(i*14),1095+(i*10),1077+(i*6),2020+(i%5));}
ps("\nCaryopteris execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cary_execution(t,c,1115+(i*15),1104+(i*12),1086+(i*8),1073+(i*5),2021+(i%4));}
ps("\nCaryopteris evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cary_evaluation(t,c,1107+(i*13),1096+(i*10),1080+(i*7),1069+(i*4),2022+(i%3));}
ps("\nCaryopteris accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cary_accessory(t,c,1099+(i*11),1090+(i*9),1076+(i*6),1066+(i*3),2023+(i%2));}
ps("\nCaryopteris marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cary_market(t,c,1093+(i*9),1084+(i*7),1071+(i*5),1063+(i*3),2024);}
ps("\n");cary_report();cary_state();ps("\n=== Demo Complete ===\n");return 0;}
