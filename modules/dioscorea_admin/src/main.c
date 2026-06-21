/* dioscorea_admin: Dioscorea management technology administration (v1.0)
 * Dioscorea planning, dioscorea execution, dioscorea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dio_t;
typedef struct{int n_diop,n_dioe,n_dio2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dio_state_t;
static dio_t diops[N],dioss[N-2],diovss[N-4],dioas[N-6],diomks[N-6]; static dio_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dio_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dio_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DIO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dio_init(void){if(init)return -1;st.n_diop=0;st.n_dioe=0;st.n_dio2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)diops[i].active=0;for(int i=0;i<N-2;i++)dioss[i].active=0;for(int i=0;i<N-4;i++)diovss[i].active=0;for(int i=0;i<N-6;i++)dioas[i].active=0;for(int i=0;i<N-6;i++)diomks[i].active=0;init=1;ps("[DIO] Dioscorea initialized\n");return 0;}
int dio_planning(int t,int c,int a,int b,int d,int e,int y){return add(diops,&st.n_diop,&st.t_f1,N,t,c,a,b,d,e,y);}
int dio_execution(int t,int c,int a,int b,int d,int e,int y){return add(dioss,&st.n_dioe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dio_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(diovss,&st.n_dio2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dio_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dioas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dio_market(int t,int c,int a,int b,int d,int e,int y){return add(diomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dio_report(void){ps("[DIO] Diop: ");pi(st.n_diop);ps(" PCS=");pi(st.t_f1);ps("\nDioe: ");pi(st.n_dioe);ps(" PCS=");pi(st.t_f2);ps("\nDiov: ");pi(st.n_dio2);ps(" PCS=");pi(st.t_f3);ps("\nDioc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dio_state(void){ps("[DIO] Diop=");pi(st.n_diop);ps(" Dioe=");pi(st.n_dioe);ps(" Diov=");pi(st.n_dio2);ps(" Dioc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dioscorea Admin Demo ===\n\n");dio_init();
ps("Dioscorea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dio_planning(t,c,994+(i*17),983+(i*14),963+(i*10),945+(i*6),2020+(i%5));}
ps("\nDioscorea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dio_execution(t,c,983+(i*15),972+(i*12),954+(i*8),941+(i*5),2021+(i%4));}
ps("\nDioscorea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dio_evaluation(t,c,975+(i*13),964+(i*10),948+(i*7),937+(i*4),2022+(i%3));}
ps("\nDioscorea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dio_accessory(t,c,967+(i*11),958+(i*9),944+(i*6),934+(i*3),2023+(i%2));}
ps("\nDioscorea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dio_market(t,c,961+(i*9),952+(i*7),939+(i*5),931+(i*3),2024);}
ps("\n");dio_report();dio_state();ps("\n=== Demo Complete ===\n");return 0;}
