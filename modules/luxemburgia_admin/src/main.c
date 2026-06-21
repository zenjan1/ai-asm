/* luxemburgia_admin: Luxemburgia management technology administration (v1.0)
 * Luxemburgia planning, luxemburgia execution, luxemburgia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lux_t;
typedef struct{int n_luxp,n_luxe,n_lux2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lux_state_t;
static lux_t luxps[N],luxss[N-2],luxvss[N-4],luxas[N-6],luxmks[N-6]; static lux_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lux_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lux_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LUX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lux_init(void){if(init)return -1;st.n_luxp=0;st.n_luxe=0;st.n_lux2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)luxps[i].active=0;for(int i=0;i<N-2;i++)luxss[i].active=0;for(int i=0;i<N-4;i++)luxvss[i].active=0;for(int i=0;i<N-6;i++)luxas[i].active=0;for(int i=0;i<N-6;i++)luxmks[i].active=0;init=1;ps("[LUX] Luxemburgia initialized\n");return 0;}
int lux_planning(int t,int c,int a,int b,int d,int e,int y){return add(luxps,&st.n_luxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lux_execution(int t,int c,int a,int b,int d,int e,int y){return add(luxss,&st.n_luxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lux_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(luxvss,&st.n_lux2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lux_accessory(int t,int c,int a,int b,int d,int e,int y){return add(luxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lux_market(int t,int c,int a,int b,int d,int e,int y){return add(luxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lux_report(void){ps("[LUX] Luxp: ");pi(st.n_luxp);ps(" PCS=");pi(st.t_f1);ps("\nLuxe: ");pi(st.n_luxe);ps(" PCS=");pi(st.t_f2);ps("\nLuxv: ");pi(st.n_lux2);ps(" PCS=");pi(st.t_f3);ps("\nLuxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lux_state(void){ps("[LUX] Luxp=");pi(st.n_luxp);ps(" Luxe=");pi(st.n_luxe);ps(" Luxv=");pi(st.n_lux2);ps(" Luxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Luxemburgia Admin Demo ===\n\n");lux_init();
ps("Luxemburgia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lux_planning(t,c,1065+(i*17),1054+(i*14),1034+(i*10),1016+(i*6),2020+(i%5));}
ps("\nLuxemburgia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lux_execution(t,c,1054+(i*15),1043+(i*12),1025+(i*8),1012+(i*5),2021+(i%4));}
ps("\nLuxemburgia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lux_evaluation(t,c,1046+(i*13),1035+(i*10),1019+(i*7),1008+(i*4),2022+(i%3));}
ps("\nLuxemburgia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lux_accessory(t,c,1038+(i*11),1029+(i*9),1015+(i*6),1005+(i*3),2023+(i%2));}
ps("\nLuxemburgia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lux_market(t,c,1032+(i*9),1023+(i*7),1010+(i*5),1002+(i*3),2024);}
ps("\n");lux_report();lux_state();ps("\n=== Demo Complete ===\n");return 0;}
