/* monochoria_admin: Monochoria management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mono_t;
typedef struct{int n_monoop,n_mono,n_mono2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mono_state_t;
static mono_t monos[N],monoe[N-2],mono2[N-4],monoac[N-6],monoam[N-6]; static mono_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mono_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mono_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MONO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mono_init(void){if(init)return -1;st.n_monoop=0;st.n_mono=0;st.n_mono2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)monos[i].active=0;for(int i=0;i<N-2;i++)monoe[i].active=0;for(int i=0;i<N-4;i++)mono2[i].active=0;for(int i=0;i<N-6;i++)monoac[i].active=0;for(int i=0;i<N-6;i++)monoam[i].active=0;init=1;ps("[MONO] Monochoria initialized\n");return 0;}
int mono_planning(int t,int c,int a,int b,int d,int e,int y){return add(monos,&st.n_monoop,&st.t_f1,N,t,c,a,b,d,e,y);}
int mono_execution(int t,int c,int a,int b,int d,int e,int y){return add(monoe,&st.n_mono,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mono_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mono2,&st.n_mono2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mono_accessory(int t,int c,int a,int b,int d,int e,int y){return add(monoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mono_market(int t,int c,int a,int b,int d,int e,int y){return add(monoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mono_report(void){ps("[MONO] Monop: ");pi(st.n_monoop);ps(" PCS=");pi(st.t_f1);ps("\nMonoe: ");pi(st.n_mono);ps(" PCS=");pi(st.t_f2);ps("\nMono2: ");pi(st.n_mono2);ps(" PCS=");pi(st.t_f3);ps("\nMno: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mono_state(void){ps("[MONO] Monop=");pi(st.n_monoop);ps(" Mono=");pi(st.n_mono);ps(" No2=");pi(st.n_mono2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Monochoria Admin Demo ===\n\n");mono_init();
ps("Monochoria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mono_planning(t,c,1577-(i*17),1566-(i*14),1546-(i*10),1528-(i*6),2020+(i%5));}
ps("\nMonochoria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mono_execution(t,c,1566-(i*15),1555-(i*12),1537-(i*8),1524-(i*5),2021+(i%4));}
ps("\nMonochoria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mono_evaluation(t,c,1558-(i*13),1547-(i*10),1531-(i*7),1520-(i*4),2022+(i%3));}
ps("\nMonochoria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mono_accessory(t,c,1550-(i*11),1541-(i*9),1527-(i*6),1517-(i*3),2023+(i%2));}
ps("\nMonochoria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mono_market(t,c,1544-(i*9),1535-(i*7),1522-(i*5),1514-(i*3),2024);}
ps("\n");mono_report();mono_state();ps("\n=== Demo Complete ===\n");return 0;}
