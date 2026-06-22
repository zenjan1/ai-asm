/* malope_admin: Malope management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} malo_t;
typedef struct{int n_maloop,n_malo,n_malo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} malo_state_t;
static malo_t mallos[N],maloe[N-2],malo2[N-4],maloac[N-6],maloam[N-6]; static malo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(malo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;malo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MALO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int malo_init(void){if(init)return -1;st.n_maloop=0;st.n_malo=0;st.n_malo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mallos[i].active=0;for(int i=0;i<N-2;i++)maloe[i].active=0;for(int i=0;i<N-4;i++)malo2[i].active=0;for(int i=0;i<N-6;i++)maloac[i].active=0;for(int i=0;i<N-6;i++)maloam[i].active=0;init=1;ps("[MALO] Malope initialized\n");return 0;}
int malo_planning(int t,int c,int a,int b,int d,int e,int y){return add(mallos,&st.n_maloop,&st.t_f1,N,t,c,a,b,d,e,y);}
int malo_execution(int t,int c,int a,int b,int d,int e,int y){return add(maloe,&st.n_malo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int malo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(malo2,&st.n_malo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int malo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(maloac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int malo_market(int t,int c,int a,int b,int d,int e,int y){return add(maloam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void malo_report(void){ps("[MALO] Malop: ");pi(st.n_maloop);ps(" PCS=");pi(st.t_f1);ps("\nMale: ");pi(st.n_malo);ps(" PCS=");pi(st.t_f2);ps("\nMal2: ");pi(st.n_malo2);ps(" PCS=");pi(st.t_f3);ps("\nMlo: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void malo_state(void){ps("[MALO] Malop=");pi(st.n_maloop);ps(" Malo=");pi(st.n_malo);ps(" Mlo2=");pi(st.n_malo2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Malope Admin Demo ===\n\n");malo_init();
ps("Malope planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;malo_planning(t,c,1560-(i*17),1549-(i*14),1529-(i*10),1511-(i*6),2020+(i%5));}
ps("\nMalope execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;malo_execution(t,c,1549-(i*15),1538-(i*12),1520-(i*8),1507-(i*5),2021+(i%4));}
ps("\nMalope evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;malo_evaluation(t,c,1541-(i*13),1530-(i*10),1514-(i*7),1503-(i*4),2022+(i%3));}
ps("\nMalope accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;malo_accessory(t,c,1533-(i*11),1524-(i*9),1510-(i*6),1500-(i*3),2023+(i%2));}
ps("\nMalope marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;malo_market(t,c,1527-(i*9),1518-(i*7),1505-(i*5),1497-(i*3),2024);}
ps("\n");malo_report();malo_state();ps("\n=== Demo Complete ===\n");return 0;}
