/* cyananthus_admin: Cyananthus management technology administration (v1.0)
 * Cyananthus planning, cyananthus execution, cyananthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyan_t;
typedef struct{int n_cyanp,n_cyan,n_cyan2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyan_state_t;
static cyan_t cyans[N],cyane[N-2],cyan2[N-4],cyanac[N-6],cyanam[N-6]; static cyan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cyan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyan_init(void){if(init)return -1;st.n_cyanp=0;st.n_cyan=0;st.n_cyan2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cyans[i].active=0;for(int i=0;i<N-2;i++)cyane[i].active=0;for(int i=0;i<N-4;i++)cyan2[i].active=0;for(int i=0;i<N-6;i++)cyanac[i].active=0;for(int i=0;i<N-6;i++)cyanam[i].active=0;init=1;ps("[CYAN] Cyananthus initialized\n");return 0;}
int cyan_planning(int t,int c,int a,int b,int d,int e,int y){return add(cyans,&st.n_cyanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyan_execution(int t,int c,int a,int b,int d,int e,int y){return add(cyane,&st.n_cyan,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cyan2,&st.n_cyan2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cyanac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyan_market(int t,int c,int a,int b,int d,int e,int y){return add(cyanam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyan_report(void){ps("[CYAN] Anp: ");pi(st.n_cyanp);ps(" PCS=");pi(st.t_f1);ps("\nAne: ");pi(st.n_cyan);ps(" PCS=");pi(st.t_f2);ps("\nAn2: ");pi(st.n_cyan2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyan_state(void){ps("[CYAN] Anp=");pi(st.n_cyanp);ps(" An=");pi(st.n_cyan);ps(" An2=");pi(st.n_cyan2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyananthus Admin Demo ===\n\n");cyan_init();
ps("Cyananthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyan_planning(t,c,1483+(i*17),1472+(i*14),1452+(i*10),1434+(i*6),2020+(i%5));}
ps("\nCyananthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyan_execution(t,c,1472+(i*15),1461+(i*12),1443+(i*8),1430+(i*5),2021+(i%4));}
ps("\nCyananthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyan_evaluation(t,c,1464+(i*13),1453+(i*10),1437+(i*7),1426+(i*4),2022+(i%3));}
ps("\nCyananthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyan_accessory(t,c,1456+(i*11),1447+(i*9),1433+(i*6),1423+(i*3),2023+(i%2));}
ps("\nCyananthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyan_market(t,c,1450+(i*9),1441+(i*7),1428+(i*5),1420+(i*3),2024);}
ps("\n");cyan_report();cyan_state();ps("\n=== Demo Complete ===\n");return 0;}
