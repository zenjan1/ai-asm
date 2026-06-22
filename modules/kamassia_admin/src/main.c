/* kamassia_admin: Kamassia management technology administration (v1.0)
 * Kamassia planning, kamassia execution, kamassia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kam_t;
typedef struct{int n_kamp,n_kame,n_kam2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kam_state_t;
static kam_t kamms[N],kamess[N-2],kamvss[N-4],kamas[N-6],kamks[N-6]; static kam_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(kam_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kam_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KAM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kam_init(void){if(init)return -1;st.n_kamp=0;st.n_kame=0;st.n_kam2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kamms[i].active=0;for(int i=0;i<N-2;i++)kamess[i].active=0;for(int i=0;i<N-4;i++)kamvss[i].active=0;for(int i=0;i<N-6;i++)kamas[i].active=0;for(int i=0;i<N-6;i++)kamks[i].active=0;init=1;ps("[KAM] Kamassia initialized\n");return 0;}
int kam_planning(int t,int c,int a,int b,int d,int e,int y){return add(kamms,&st.n_kamp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kam_execution(int t,int c,int a,int b,int d,int e,int y){return add(kamess,&st.n_kame,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kam_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kamvss,&st.n_kam2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kam_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kamas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kam_market(int t,int c,int a,int b,int d,int e,int y){return add(kamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kam_report(void){ps("[KAM] Kamp: ");pi(st.n_kamp);ps(" PCS=");pi(st.t_f1);ps("\nKame: ");pi(st.n_kame);ps(" PCS=");pi(st.t_f2);ps("\nKamv: ");pi(st.n_kam2);ps(" PCS=");pi(st.t_f3);ps("\nKamc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kam_state(void){ps("[KAM] Kamp=");pi(st.n_kamp);ps(" Kame=");pi(st.n_kame);ps(" Kamv=");pi(st.n_kam2);ps(" Kamc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kamassia Admin Demo ===\n\n");kam_init();
ps("Kamassia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kam_planning(t,c,1306+(i*17),1295+(i*14),1275+(i*10),1257+(i*6),2020+(i%5));}
ps("\nKamassia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kam_execution(t,c,1295+(i*15),1284+(i*12),1266+(i*8),1253+(i*5),2021+(i%4));}
ps("\nKamassia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kam_evaluation(t,c,1287+(i*13),1276+(i*10),1260+(i*7),1249+(i*4),2022+(i%3));}
ps("\nKamassia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kam_accessory(t,c,1279+(i*11),1270+(i*9),1256+(i*6),1246+(i*3),2023+(i%2));}
ps("\nKamassia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kam_market(t,c,1273+(i*9),1264+(i*7),1251+(i*5),1243+(i*3),2024);}
ps("\n");kam_report();kam_state();ps("\n=== Demo Complete ===\n");return 0;}
