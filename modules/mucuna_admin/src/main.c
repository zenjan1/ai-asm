/* mucuna_admin: Mucuna management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mucu_t;
typedef struct{int n_mucuop,n_mucu,n_mucu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mucu_state_t;
static mucu_t mucuos[N],mucue[N-2],mucu2[N-4],mucuac[N-6],mucuam[N-6]; static mucu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mucu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mucu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MUCU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mucu_init(void){if(init)return -1;st.n_mucuop=0;st.n_mucu=0;st.n_mucu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mucuos[i].active=0;for(int i=0;i<N-2;i++)mucue[i].active=0;for(int i=0;i<N-4;i++)mucu2[i].active=0;for(int i=0;i<N-6;i++)mucuac[i].active=0;for(int i=0;i<N-6;i++)mucuam[i].active=0;init=1;ps("[MUCU] Mucuna initialized\n");return 0;}
int mucu_planning(int t,int c,int a,int b,int d,int e,int y){return add(mucuos,&st.n_mucuop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mucu_execution(int t,int c,int a,int b,int d,int e,int y){return add(mucue,&st.n_mucu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mucu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mucu2,&st.n_mucu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mucu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mucuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mucu_market(int t,int c,int a,int b,int d,int e,int y){return add(mucuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mucu_report(void){ps("[MUCU] Mucp: ");pi(st.n_mucuop);ps(" PCS=");pi(st.t_f1);ps("\nMuce: ");pi(st.n_mucu);ps(" PCS=");pi(st.t_f2);ps("\nMucu2: ");pi(st.n_mucu2);ps(" PCS=");pi(st.t_f3);ps("\nMcu: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mucu_state(void){ps("[MUCU] Mucp=");pi(st.n_mucuop);ps(" Mucu=");pi(st.n_mucu);ps(" Cu2=");pi(st.n_mucu2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mucuna Admin Demo ===\n\n");mucu_init();
ps("Mucuna planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mucu_planning(t,c,1582-(i*17),1571-(i*14),1551-(i*10),1533-(i*6),2020+(i%5));}
ps("\nMucuna execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mucu_execution(t,c,1571-(i*15),1560-(i*12),1542-(i*8),1529-(i*5),2021+(i%4));}
ps("\nMucuna evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mucu_evaluation(t,c,1563-(i*13),1552-(i*10),1536-(i*7),1525-(i*4),2022+(i%3));}
ps("\nMucuna accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mucu_accessory(t,c,1555-(i*11),1546-(i*9),1532-(i*6),1522-(i*3),2023+(i%2));}
ps("\nMucuna marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mucu_market(t,c,1549-(i*9),1540-(i*7),1527-(i*5),1519-(i*3),2024);}
ps("\n");mucu_report();mucu_state();ps("\n=== Demo Complete ===\n");return 0;}
