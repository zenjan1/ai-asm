/* malvaviscus_admin: Malvaviscus management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} malv_t;
typedef struct{int n_malvop,n_malv,n_malv2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} malv_state_t;
static malv_t malvos[N],malve[N-2],malv2[N-4],malvac[N-6],malvam[N-6]; static malv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(malv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;malv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MALV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int malv_init(void){if(init)return -1;st.n_malvop=0;st.n_malv=0;st.n_malv2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)malvos[i].active=0;for(int i=0;i<N-2;i++)malve[i].active=0;for(int i=0;i<N-4;i++)malv2[i].active=0;for(int i=0;i<N-6;i++)malvac[i].active=0;for(int i=0;i<N-6;i++)malvam[i].active=0;init=1;ps("[MALV] Malvaviscus initialized\n");return 0;}
int malv_planning(int t,int c,int a,int b,int d,int e,int y){return add(malvos,&st.n_malvop,&st.t_f1,N,t,c,a,b,d,e,y);}
int malv_execution(int t,int c,int a,int b,int d,int e,int y){return add(malve,&st.n_malv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int malv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(malv2,&st.n_malv2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int malv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(malvac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int malv_market(int t,int c,int a,int b,int d,int e,int y){return add(malvam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void malv_report(void){ps("[MALV] Malvp: ");pi(st.n_malvop);ps(" PCS=");pi(st.t_f1);ps("\nMalve: ");pi(st.n_malv);ps(" PCS=");pi(st.t_f2);ps("\nMalv2: ");pi(st.n_malv2);ps(" PCS=");pi(st.t_f3);ps("\nMlv: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void malv_state(void){ps("[MALV] Malvp=");pi(st.n_malvop);ps(" Malv=");pi(st.n_malv);ps(" Mlv2=");pi(st.n_malv2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Malvaviscus Admin Demo ===\n\n");malv_init();
ps("Malvaviscus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;malv_planning(t,c,1561-(i*17),1550-(i*14),1530-(i*10),1512-(i*6),2020+(i%5));}
ps("\nMalvaviscus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;malv_execution(t,c,1550-(i*15),1539-(i*12),1521-(i*8),1508-(i*5),2021+(i%4));}
ps("\nMalvaviscus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;malv_evaluation(t,c,1542-(i*13),1531-(i*10),1515-(i*7),1504-(i*4),2022+(i%3));}
ps("\nMalvaviscus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;malv_accessory(t,c,1534-(i*11),1525-(i*9),1511-(i*6),1501-(i*3),2023+(i%2));}
ps("\nMalvaviscus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;malv_market(t,c,1528-(i*9),1519-(i*7),1506-(i*5),1498-(i*3),2024);}
ps("\n");malv_report();malv_state();ps("\n=== Demo Complete ===\n");return 0;}
