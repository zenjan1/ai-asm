/* odontonema_admin: Odontonema management technology administration (v1.0)
 * Odontonema planning, odontonema execution, odontonema evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} odo_t;
typedef struct{int n_odop,n_odoe,n_odo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} odo_state_t;
static odo_t odops[N],odoss[N-2],odovss[N-4],odoas[N-6],odomks[N-6]; static odo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(odo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;odo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ODO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int odo_init(void){if(init)return -1;st.n_odop=0;st.n_odoe=0;st.n_odo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)odops[i].active=0;for(int i=0;i<N-2;i++)odoss[i].active=0;for(int i=0;i<N-4;i++)odovss[i].active=0;for(int i=0;i<N-6;i++)odoas[i].active=0;for(int i=0;i<N-6;i++)odomks[i].active=0;init=1;ps("[ODO] Odontonema initialized\n");return 0;}
int odo_planning(int t,int c,int a,int b,int d,int e,int y){return add(odops,&st.n_odop,&st.t_f1,N,t,c,a,b,d,e,y);}
int odo_execution(int t,int c,int a,int b,int d,int e,int y){return add(odoss,&st.n_odoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int odo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(odovss,&st.n_odo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int odo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(odoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int odo_market(int t,int c,int a,int b,int d,int e,int y){return add(odomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void odo_report(void){ps("[ODO] Odopp: ");pi(st.n_odop);ps(" PCS=");pi(st.t_f1);ps("\nOdoe: ");pi(st.n_odoe);ps(" PCS=");pi(st.t_f2);ps("\nOdov: ");pi(st.n_odo2);ps(" PCS=");pi(st.t_f3);ps("\nOdoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void odo_state(void){ps("[ODO] Odopp=");pi(st.n_odop);ps(" Odoe=");pi(st.n_odoe);ps(" Odov=");pi(st.n_odo2);ps(" Odoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Odontonema Admin Demo ===\n\n");odo_init();
ps("Odontonema planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;odo_planning(t,c,1163+(i*17),1152+(i*14),1132+(i*10),1114+(i*6),2020+(i%5));}
ps("\nOdontonema execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;odo_execution(t,c,1152+(i*15),1141+(i*12),1123+(i*8),1110+(i*5),2021+(i%4));}
ps("\nOdontonema evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;odo_evaluation(t,c,1144+(i*13),1133+(i*10),1117+(i*7),1106+(i*4),2022+(i%3));}
ps("\nOdontonema accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;odo_accessory(t,c,1136+(i*11),1127+(i*9),1113+(i*6),1103+(i*3),2023+(i%2));}
ps("\nOdontonema marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;odo_market(t,c,1130+(i*9),1121+(i*7),1108+(i*5),1100+(i*3),2024);}
ps("\n");odo_report();odo_state();ps("\n=== Demo Complete ===\n");return 0;}
