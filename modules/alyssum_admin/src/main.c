/* alyssum_admin: Alyssum management technology administration (v1.0)
 * Alyssum planning, alyssum execution, alyssum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aly_t;
typedef struct{int n_alyp,n_alye,n_aly2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aly_state_t;
static aly_t alyps[N],alyss[N-2],alyvss[N-4],alyas[N-6],alymks[N-6]; static aly_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aly_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aly_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aly_init(void){if(init)return -1;st.n_alyp=0;st.n_alye=0;st.n_aly2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)alyps[i].active=0;for(int i=0;i<N-2;i++)alyss[i].active=0;for(int i=0;i<N-4;i++)alyvss[i].active=0;for(int i=0;i<N-6;i++)alyas[i].active=0;for(int i=0;i<N-6;i++)alymks[i].active=0;init=1;ps("[ALY] Alyssum initialized\n");return 0;}
int aly_planning(int t,int c,int a,int b,int d,int e,int y){return add(alyps,&st.n_alyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aly_execution(int t,int c,int a,int b,int d,int e,int y){return add(alyss,&st.n_alye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aly_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alyvss,&st.n_aly2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aly_accessory(int t,int c,int a,int b,int d,int e,int y){return add(alyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aly_market(int t,int c,int a,int b,int d,int e,int y){return add(alymks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aly_report(void){ps("[ALY] Alyp: ");pi(st.n_alyp);ps(" PCS=");pi(st.t_f1);ps("\nAlye: ");pi(st.n_alye);ps(" PCS=");pi(st.t_f2);ps("\nAlyv: ");pi(st.n_aly2);ps(" PCS=");pi(st.t_f3);ps("\nAlyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aly_state(void){ps("[ALY] Alyp=");pi(st.n_alyp);ps(" Alye=");pi(st.n_alye);ps(" Alyv=");pi(st.n_aly2);ps(" Alyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Alyssum Admin Demo ===\n\n");aly_init();
ps("Alyssum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aly_planning(t,c,894+(i*17),883+(i*14),863+(i*10),845+(i*6),2020+(i%5));}
ps("\nAlyssum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aly_execution(t,c,883+(i*15),872+(i*12),854+(i*8),841+(i*5),2021+(i%4));}
ps("\nAlyssum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aly_evaluation(t,c,875+(i*13),864+(i*10),848+(i*7),837+(i*4),2022+(i%3));}
ps("\nAlyssum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aly_accessory(t,c,867+(i*11),858+(i*9),844+(i*6),834+(i*3),2023+(i%2));}
ps("\nAlyssum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aly_market(t,c,861+(i*9),852+(i*7),839+(i*5),831+(i*3),2024);}
ps("\n");aly_report();aly_state();ps("\n=== Demo Complete ===\n");return 0;}
