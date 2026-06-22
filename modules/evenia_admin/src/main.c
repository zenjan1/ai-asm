/* evenia_admin: Evenia management technology administration (v1.0)
 * Evenia planning, evenia execution, evenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} even_t;
typedef struct{int n_evenp,n_even,n_even2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} even_state_t;
static even_t evens[N],evene[N-2],even2[N-4],evenac[N-6],evenam[N-6]; static even_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(even_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;even_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EVEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int even_init(void){if(init)return -1;st.n_evenp=0;st.n_even=0;st.n_even2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)evens[i].active=0;for(int i=0;i<N-2;i++)evene[i].active=0;for(int i=0;i<N-4;i++)even2[i].active=0;for(int i=0;i<N-6;i++)evenac[i].active=0;for(int i=0;i<N-6;i++)evenam[i].active=0;init=1;ps("[EVEN] Evenia initialized\n");return 0;}
int even_planning(int t,int c,int a,int b,int d,int e,int y){return add(evens,&st.n_evenp,&st.t_f1,N,t,c,a,b,d,e,y);}
int even_execution(int t,int c,int a,int b,int d,int e,int y){return add(evene,&st.n_even,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int even_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(even2,&st.n_even2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int even_accessory(int t,int c,int a,int b,int d,int e,int y){return add(evenac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int even_market(int t,int c,int a,int b,int d,int e,int y){return add(evenam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void even_report(void){ps("[EVEN] Enp: ");pi(st.n_evenp);ps(" PCS=");pi(st.t_f1);ps("\nEne: ");pi(st.n_even);ps(" PCS=");pi(st.t_f2);ps("\nEn2: ");pi(st.n_even2);ps(" PCS=");pi(st.t_f3);ps("\nEac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void even_state(void){ps("[EVEN] Enp=");pi(st.n_evenp);ps(" En=");pi(st.n_even);ps(" En2=");pi(st.n_even2);ps(" Eac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Evenia Admin Demo ===\n\n");even_init();
ps("Evenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;even_planning(t,c,1508+(i*17),1497+(i*14),1477+(i*10),1459+(i*6),2020+(i%5));}
ps("\nEvenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;even_execution(t,c,1497+(i*15),1486+(i*12),1468+(i*8),1455+(i*5),2021+(i%4));}
ps("\nEvenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;even_evaluation(t,c,1489+(i*13),1478+(i*10),1462+(i*7),1451+(i*4),2022+(i%3));}
ps("\nEvenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;even_accessory(t,c,1481+(i*11),1472+(i*9),1458+(i*6),1448+(i*3),2023+(i%2));}
ps("\nEvenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;even_market(t,c,1475+(i*9),1466+(i*7),1453+(i*5),1445+(i*3),2024);}
ps("\n");even_report();even_state();ps("\n=== Demo Complete ===\n");return 0;}
