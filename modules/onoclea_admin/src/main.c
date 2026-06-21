/* onoclea_admin: Onoclea management technology administration (v1.0)
 * Onoclea planning, onoclea execution, onoclea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ono_t;
typedef struct{int n_onop,n_onoe,n_ono2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ono_state_t;
static ono_t onops[N],onoss[N-2],onovss[N-4],onoas[N-6],onomks[N-6]; static ono_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ono_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ono_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ONO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ono_init(void){if(init)return -1;st.n_onop=0;st.n_onoe=0;st.n_ono2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)onops[i].active=0;for(int i=0;i<N-2;i++)onoss[i].active=0;for(int i=0;i<N-4;i++)onovss[i].active=0;for(int i=0;i<N-6;i++)onoas[i].active=0;for(int i=0;i<N-6;i++)onomks[i].active=0;init=1;ps("[ONO] Onoclea initialized\n");return 0;}
int ono_planning(int t,int c,int a,int b,int d,int e,int y){return add(onops,&st.n_onop,&st.t_f1,N,t,c,a,b,d,e,y);}
int ono_execution(int t,int c,int a,int b,int d,int e,int y){return add(onoss,&st.n_onoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ono_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(onovss,&st.n_ono2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ono_accessory(int t,int c,int a,int b,int d,int e,int y){return add(onoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ono_market(int t,int c,int a,int b,int d,int e,int y){return add(onomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ono_report(void){ps("[ONO] Onop: ");pi(st.n_onop);ps(" PCS=");pi(st.t_f1);ps("\nOnoe: ");pi(st.n_onoe);ps(" PCS=");pi(st.t_f2);ps("\nOnov: ");pi(st.n_ono2);ps(" PCS=");pi(st.t_f3);ps("\nOnoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ono_state(void){ps("[ONO] Onop=");pi(st.n_onop);ps(" Onoe=");pi(st.n_onoe);ps(" Onov=");pi(st.n_ono2);ps(" Onoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Onoclea Admin Demo ===\n\n");ono_init();
ps("Onoclea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ono_planning(t,c,1075+(i*17),1064+(i*14),1044+(i*10),1026+(i*6),2020+(i%5));}
ps("\nOnoclea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ono_execution(t,c,1064+(i*15),1053+(i*12),1035+(i*8),1022+(i*5),2021+(i%4));}
ps("\nOnoclea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ono_evaluation(t,c,1056+(i*13),1045+(i*10),1029+(i*7),1018+(i*4),2022+(i%3));}
ps("\nOnoclea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ono_accessory(t,c,1048+(i*11),1039+(i*9),1025+(i*6),1015+(i*3),2023+(i%2));}
ps("\nOnoclea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ono_market(t,c,1042+(i*9),1033+(i*7),1020+(i*5),1012+(i*3),2024);}
ps("\n");ono_report();ono_state();ps("\n=== Demo Complete ===\n");return 0;}
