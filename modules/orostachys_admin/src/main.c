/* orostachys_admin: Orostachys management technology administration (v1.0)
 * Orostachys planning, orostachys execution, orostachys evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oro_t;
typedef struct{int n_orop,n_oroe,n_oro2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oro_state_t;
static oro_t orops[N],oross[N-2],orovss[N-4],oroas[N-6],oromks[N-6]; static oro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oro_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oro_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ORO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oro_init(void){if(init)return -1;st.n_orop=0;st.n_oroe=0;st.n_oro2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)orops[i].active=0;for(int i=0;i<N-2;i++)oross[i].active=0;for(int i=0;i<N-4;i++)orovss[i].active=0;for(int i=0;i<N-6;i++)oroas[i].active=0;for(int i=0;i<N-6;i++)oromks[i].active=0;init=1;ps("[ORO] Orostachys initialized\n");return 0;}
int oro_planning(int t,int c,int a,int b,int d,int e,int y){return add(orops,&st.n_orop,&st.t_f1,N,t,c,a,b,d,e,y);}
int oro_execution(int t,int c,int a,int b,int d,int e,int y){return add(oross,&st.n_oroe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oro_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(orovss,&st.n_oro2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oro_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oroas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oro_market(int t,int c,int a,int b,int d,int e,int y){return add(oromks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oro_report(void){ps("[ORO] orop: ");pi(st.n_orop);ps(" PCS=");pi(st.t_f1);ps("\noroe: ");pi(st.n_oroe);ps(" PCS=");pi(st.t_f2);ps("\norov: ");pi(st.n_oro2);ps(" PCS=");pi(st.t_f3);ps("\noroc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oro_state(void){ps("[ORO] orop=");pi(st.n_orop);ps(" oroe=");pi(st.n_oroe);ps(" orov=");pi(st.n_oro2);ps(" oroc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Orostachys Admin Demo ===\n\n");oro_init();
ps("Orostachys planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oro_planning(t,c,1079+(i*17),1068+(i*14),1048+(i*10),1030+(i*6),2020+(i%5));}
ps("\nOrostachys execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oro_execution(t,c,1068+(i*15),1057+(i*12),1039+(i*8),1026+(i*5),2021+(i%4));}
ps("\nOrostachys evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oro_evaluation(t,c,1060+(i*13),1049+(i*10),1033+(i*7),1022+(i*4),2022+(i%3));}
ps("\nOrostachys accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oro_accessory(t,c,1052+(i*11),1043+(i*9),1029+(i*6),1019+(i*3),2023+(i%2));}
ps("\nOrostachys marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oro_market(t,c,1046+(i*9),1037+(i*7),1024+(i*5),1016+(i*3),2024);}
ps("\n");oro_report();oro_state();ps("\n=== Demo Complete ===\n");return 0;}
