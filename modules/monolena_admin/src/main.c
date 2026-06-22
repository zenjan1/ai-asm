/* monolena_admin: Monolena management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} monl_t;
typedef struct{int n_monlop,n_monl,n_monl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} monl_state_t;
static monl_t monlos[N],monle[N-2],monl2[N-4],monlac[N-6],monlam[N-6]; static monl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(monl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;monl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MONL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int monl_init(void){if(init)return -1;st.n_monlop=0;st.n_monl=0;st.n_monl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)monlos[i].active=0;for(int i=0;i<N-2;i++)monle[i].active=0;for(int i=0;i<N-4;i++)monl2[i].active=0;for(int i=0;i<N-6;i++)monlac[i].active=0;for(int i=0;i<N-6;i++)monlam[i].active=0;init=1;ps("[MONL] Monolena initialized\n");return 0;}
int monl_planning(int t,int c,int a,int b,int d,int e,int y){return add(monlos,&st.n_monlop,&st.t_f1,N,t,c,a,b,d,e,y);}
int monl_execution(int t,int c,int a,int b,int d,int e,int y){return add(monle,&st.n_monl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int monl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(monl2,&st.n_monl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int monl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(monlac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int monl_market(int t,int c,int a,int b,int d,int e,int y){return add(monlam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void monl_report(void){ps("[MONL] Monlp: ");pi(st.n_monlop);ps(" PCS=");pi(st.t_f1);ps("\nMonle: ");pi(st.n_monl);ps(" PCS=");pi(st.t_f2);ps("\nMonl2: ");pi(st.n_monl2);ps(" PCS=");pi(st.t_f3);ps("\nMnl: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void monl_state(void){ps("[MONL] Monlp=");pi(st.n_monlop);ps(" Monl=");pi(st.n_monl);ps(" Nl2=");pi(st.n_monl2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Monolena Admin Demo ===\n\n");monl_init();
ps("Monolena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;monl_planning(t,c,1578-(i*17),1567-(i*14),1547-(i*10),1529-(i*6),2020+(i%5));}
ps("\nMonolena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;monl_execution(t,c,1567-(i*15),1556-(i*12),1538-(i*8),1525-(i*5),2021+(i%4));}
ps("\nMonolena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;monl_evaluation(t,c,1559-(i*13),1548-(i*10),1532-(i*7),1521-(i*4),2022+(i%3));}
ps("\nMonolena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;monl_accessory(t,c,1551-(i*11),1542-(i*9),1528-(i*6),1518-(i*3),2023+(i%2));}
ps("\nMonolena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;monl_market(t,c,1545-(i*9),1536-(i*7),1523-(i*5),1515-(i*3),2024);}
ps("\n");monl_report();monl_state();ps("\n=== Demo Complete ===\n");return 0;}
