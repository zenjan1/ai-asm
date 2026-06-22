/* onopordium_admin: Onopordium management technology administration (v1.0)
 * Onopordium planning, onopordium execution, onopordium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oni_t;
typedef struct{int n_onip,n_onie,n_oni2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oni_state_t;
static oni_t onips[N],oniss[N-2],onivss[N-4],onias[N-6],onimks[N-6]; static oni_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oni_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oni_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ONI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oni_init(void){if(init)return -1;st.n_onip=0;st.n_onie=0;st.n_oni2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)onips[i].active=0;for(int i=0;i<N-2;i++)oniss[i].active=0;for(int i=0;i<N-4;i++)onivss[i].active=0;for(int i=0;i<N-6;i++)onias[i].active=0;for(int i=0;i<N-6;i++)onimks[i].active=0;init=1;ps("[ONI] Onopordium initialized\n");return 0;}
int oni_planning(int t,int c,int a,int b,int d,int e,int y){return add(onips,&st.n_onip,&st.t_f1,N,t,c,a,b,d,e,y);}
int oni_execution(int t,int c,int a,int b,int d,int e,int y){return add(oniss,&st.n_onie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oni_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(onivss,&st.n_oni2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oni_accessory(int t,int c,int a,int b,int d,int e,int y){return add(onias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oni_market(int t,int c,int a,int b,int d,int e,int y){return add(onimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oni_report(void){ps("[ONI] Onopp: ");pi(st.n_onip);ps(" PCS=");pi(st.t_f1);ps("\nOnoe: ");pi(st.n_onie);ps(" PCS=");pi(st.t_f2);ps("\nOnov: ");pi(st.n_oni2);ps(" PCS=");pi(st.t_f3);ps("\nOnoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oni_state(void){ps("[ONI] Onopp=");pi(st.n_onip);ps(" Onoe=");pi(st.n_onie);ps(" Onov=");pi(st.n_oni2);ps(" Onoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Onopordium Admin Demo ===\n\n");oni_init();
ps("Onopordium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oni_planning(t,c,1164+(i*17),1153+(i*14),1133+(i*10),1115+(i*6),2020+(i%5));}
ps("\nOnopordium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oni_execution(t,c,1153+(i*15),1142+(i*12),1124+(i*8),1111+(i*5),2021+(i%4));}
ps("\nOnopordium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oni_evaluation(t,c,1145+(i*13),1134+(i*10),1118+(i*7),1107+(i*4),2022+(i%3));}
ps("\nOnopordium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oni_accessory(t,c,1137+(i*11),1128+(i*9),1114+(i*6),1104+(i*3),2023+(i%2));}
ps("\nOnopordium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oni_market(t,c,1131+(i*9),1122+(i*7),1109+(i*5),1101+(i*3),2024);}
ps("\n");oni_report();oni_state();ps("\n=== Demo Complete ===\n");return 0;}
