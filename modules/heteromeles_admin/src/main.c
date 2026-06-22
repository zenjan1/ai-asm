/* heteromeles_admin: Heteromeles management technology administration (v1.0)
 * Heteromeles planning, heteromeles execution, heteromeles evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hete_t;
typedef struct{int n_hetep,n_hete,n_hete2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hete_state_t;
static hete_t hetes[N],hete_e[N-2],hete2[N-4],heteac[N-6],heteam[N-6]; static hete_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hete_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hete_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HETE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hete_init(void){if(init)return -1;st.n_hetep=0;st.n_hete=0;st.n_hete2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hetes[i].active=0;for(int i=0;i<N-2;i++)hete_e[i].active=0;for(int i=0;i<N-4;i++)hete2[i].active=0;for(int i=0;i<N-6;i++)heteac[i].active=0;for(int i=0;i<N-6;i++)heteam[i].active=0;init=1;ps("[HETE] Heteromeles initialized\n");return 0;}
int hete_planning(int t,int c,int a,int b,int d,int e,int y){return add(hetes,&st.n_hetep,&st.t_f1,N,t,c,a,b,d,e,y);}
int hete_execution(int t,int c,int a,int b,int d,int e,int y){return add(hete_e,&st.n_hete,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hete_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hete2,&st.n_hete2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hete_accessory(int t,int c,int a,int b,int d,int e,int y){return add(heteac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hete_market(int t,int c,int a,int b,int d,int e,int y){return add(heteam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hete_report(void){ps("[HETE] Hetep: ");pi(st.n_hetep);ps(" PCS=");pi(st.t_f1);ps("\nHete: ");pi(st.n_hete);ps(" PCS=");pi(st.t_f2);ps("\nHete2: ");pi(st.n_hete2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hete_state(void){ps("[HETE] Hetep=");pi(st.n_hetep);ps(" Hete=");pi(st.n_hete);ps(" Hete2=");pi(st.n_hete2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Heteromeles Admin Demo ===\n\n");hete_init();
ps("Heteromeles planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hete_planning(t,c,1532+(i*17),1521+(i*14),1501+(i*10),1483+(i*6),2020+(i%5));}
ps("\nHeteromeles execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hete_execution(t,c,1521+(i*15),1510+(i*12),1492+(i*8),1479+(i*5),2021+(i%4));}
ps("\nHeteromeles evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hete_evaluation(t,c,1513+(i*13),1502+(i*10),1486+(i*7),1475+(i*4),2022+(i%3));}
ps("\nHeteromeles accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hete_accessory(t,c,1505+(i*11),1496+(i*9),1482+(i*6),1472+(i*3),2023+(i%2));}
ps("\nHeteromeles marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hete_market(t,c,1499+(i*9),1490+(i*7),1477+(i*5),1469+(i*3),2024);}
ps("\n");hete_report();hete_state();ps("\n=== Demo Complete ===\n");return 0;}
