/* funkia_admin: Funkia management technology administration (v1.0)
 * Funkia planning, funkia execution, funkia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fun_t;
typedef struct{int n_funp,n_fune,n_fun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fun_state_t;
static fun_t funps[N],funss[N-2],funvss[N-4],funas[N-6],funmks[N-6]; static fun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fun_init(void){if(init)return -1;st.n_funp=0;st.n_fune=0;st.n_fun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)funps[i].active=0;for(int i=0;i<N-2;i++)funss[i].active=0;for(int i=0;i<N-4;i++)funvss[i].active=0;for(int i=0;i<N-6;i++)funas[i].active=0;for(int i=0;i<N-6;i++)funmks[i].active=0;init=1;ps("[FUN] Funkia initialized\n");return 0;}
int fun_planning(int t,int c,int a,int b,int d,int e,int y){return add(funps,&st.n_funp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fun_execution(int t,int c,int a,int b,int d,int e,int y){return add(funss,&st.n_fune,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(funvss,&st.n_fun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(funas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fun_market(int t,int c,int a,int b,int d,int e,int y){return add(funmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fun_report(void){ps("[FUN] Funpp: ");pi(st.n_funp);ps(" PCS=");pi(st.t_f1);ps("\nFune: ");pi(st.n_fune);ps(" PCS=");pi(st.t_f2);ps("\nFunv: ");pi(st.n_fun2);ps(" PCS=");pi(st.t_f3);ps("\nFunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fun_state(void){ps("[FUN] Funpp=");pi(st.n_funp);ps(" Fune=");pi(st.n_fune);ps(" Funv=");pi(st.n_fun2);ps(" Func=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Funkia Admin Demo ===\n\n");fun_init();
ps("Funkia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fun_planning(t,c,1143+(i*17),1132+(i*14),1112+(i*10),1094+(i*6),2020+(i%5));}
ps("\nFunkia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fun_execution(t,c,1132+(i*15),1121+(i*12),1103+(i*8),1090+(i*5),2021+(i%4));}
ps("\nFunkia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fun_evaluation(t,c,1124+(i*13),1113+(i*10),1097+(i*7),1086+(i*4),2022+(i%3));}
ps("\nFunkia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fun_accessory(t,c,1116+(i*11),1107+(i*9),1093+(i*6),1083+(i*3),2023+(i%2));}
ps("\nFunkia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fun_market(t,c,1110+(i*9),1101+(i*7),1088+(i*5),1080+(i*3),2024);}
ps("\n");fun_report();fun_state();ps("\n=== Demo Complete ===\n");return 0;}
