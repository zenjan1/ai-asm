/* croton_admin: Croton management technology administration (v1.0)
 * Croton planning, croton execution, croton evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ton_t;
typedef struct{int n_tonp,n_tone,n_ton2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ton_state_t;
static ton_t tonps[N],tonss[N-2],tonvss[N-4],tonas[N-6],tonmks[N-6]; static ton_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ton_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ton_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ton_init(void){if(init)return -1;st.n_tonp=0;st.n_tone=0;st.n_ton2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tonps[i].active=0;for(int i=0;i<N-2;i++)tonss[i].active=0;for(int i=0;i<N-4;i++)tonvss[i].active=0;for(int i=0;i<N-6;i++)tonas[i].active=0;for(int i=0;i<N-6;i++)tonmks[i].active=0;init=1;ps("[TON] Croton initialized\n");return 0;}
int ton_planning(int t,int c,int a,int b,int d,int e,int y){return add(tonps,&st.n_tonp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ton_execution(int t,int c,int a,int b,int d,int e,int y){return add(tonss,&st.n_tone,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ton_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tonvss,&st.n_ton2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ton_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tonas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ton_market(int t,int c,int a,int b,int d,int e,int y){return add(tonmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ton_report(void){ps("[TON] Cropp: ");pi(st.n_tonp);ps(" PCS=");pi(st.t_f1);ps("\nCrope: ");pi(st.n_tone);ps(" PCS=");pi(st.t_f2);ps("\nCrov: ");pi(st.n_ton2);ps(" PCS=");pi(st.t_f3);ps("\nCroc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ton_state(void){ps("[TON] Cropp=");pi(st.n_tonp);ps(" Crope=");pi(st.n_tone);ps(" Crov=");pi(st.n_ton2);ps(" Croc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Croton Admin Demo ===\n\n");ton_init();
ps("Croton planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ton_planning(t,c,1247+(i*17),1236+(i*14),1216+(i*10),1198+(i*6),2020+(i%5));}
ps("\nCroton execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ton_execution(t,c,1236+(i*15),1225+(i*12),1207+(i*8),1194+(i*5),2021+(i%4));}
ps("\nCroton evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ton_evaluation(t,c,1228+(i*13),1217+(i*10),1201+(i*7),1190+(i*4),2022+(i%3));}
ps("\nCroton accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ton_accessory(t,c,1220+(i*11),1211+(i*9),1197+(i*6),1187+(i*3),2023+(i%2));}
ps("\nCroton marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ton_market(t,c,1214+(i*9),1205+(i*7),1192+(i*5),1184+(i*3),2024);}
ps("\n");ton_report();ton_state();ps("\n=== Demo Complete ===\n");return 0;}
