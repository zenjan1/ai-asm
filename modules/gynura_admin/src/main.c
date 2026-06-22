/* gynura_admin: Gynura management technology administration (v1.0)
 * Gynura planning, gynura execution, gynura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gynu_t;
typedef struct{int n_gynup,n_gynu,n_gynu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gynu_state_t;
static gynu_t gynus[N],gynue[N-2],gynu2[N-4],gynuac[N-6],gynam[N-6]; static gynu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gynu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gynu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GYNU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gynu_init(void){if(init)return -1;st.n_gynup=0;st.n_gynu=0;st.n_gynu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gynus[i].active=0;for(int i=0;i<N-2;i++)gynue[i].active=0;for(int i=0;i<N-4;i++)gynu2[i].active=0;for(int i=0;i<N-6;i++)gynuac[i].active=0;for(int i=0;i<N-6;i++)gynam[i].active=0;init=1;ps("[GYNU] Gynura initialized\n");return 0;}
int gynu_planning(int t,int c,int a,int b,int d,int e,int y){return add(gynus,&st.n_gynup,&st.t_f1,N,t,c,a,b,d,e,y);}
int gynu_execution(int t,int c,int a,int b,int d,int e,int y){return add(gynue,&st.n_gynu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gynu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gynu2,&st.n_gynu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gynu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gynuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gynu_market(int t,int c,int a,int b,int d,int e,int y){return add(gynam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gynu_report(void){ps("[GYNU] Gynup: ");pi(st.n_gynup);ps(" PCS=");pi(st.t_f1);ps("\nGynue: ");pi(st.n_gynu);ps(" PCS=");pi(st.t_f2);ps("\nGynu2: ");pi(st.n_gynu2);ps(" PCS=");pi(st.t_f3);ps("\nGynuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gynu_state(void){ps("[GYNU] Gynup=");pi(st.n_gynup);ps(" Gynu=");pi(st.n_gynu);ps(" Gynu2=");pi(st.n_gynu2);ps(" Gynuac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gynura Admin Demo ===\n\n");gynu_init();
ps("Gynura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gynu_planning(t,c,1524+(i*17),1513+(i*14),1493+(i*10),1475+(i*6),2020+(i%5));}
ps("\nGynura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gynu_execution(t,c,1513+(i*15),1502+(i*12),1484+(i*8),1471+(i*5),2021+(i%4));}
ps("\nGynura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gynu_evaluation(t,c,1505+(i*13),1494+(i*10),1478+(i*7),1467+(i*4),2022+(i%3));}
ps("\nGynura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gynu_accessory(t,c,1497+(i*11),1488+(i*9),1474+(i*6),1464+(i*3),2023+(i%2));}
ps("\nGynura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gynu_market(t,c,1491+(i*9),1482+(i*7),1469+(i*5),1461+(i*3),2024);}
ps("\n");gynu_report();gynu_state();ps("\n=== Demo Complete ===\n");return 0;}
