/* hovenia_admin: Hovenia management technology administration (v1.0)
 * Hovenia planning, hovenia execution, hovenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hove_t;
typedef struct{int n_hovep,n_hove,n_hove2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hove_state_t;
static hove_t hoves[N],hove_e[N-2],hove2[N-4],hoveac[N-6],hoveam[N-6]; static hove_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hove_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hove_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HOVE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hove_init(void){if(init)return -1;st.n_hovep=0;st.n_hove=0;st.n_hove2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hoves[i].active=0;for(int i=0;i<N-2;i++)hove_e[i].active=0;for(int i=0;i<N-4;i++)hove2[i].active=0;for(int i=0;i<N-6;i++)hoveac[i].active=0;for(int i=0;i<N-6;i++)hoveam[i].active=0;init=1;ps("[HOVE] Hovenia initialized\n");return 0;}
int hove_planning(int t,int c,int a,int b,int d,int e,int y){return add(hoves,&st.n_hovep,&st.t_f1,N,t,c,a,b,d,e,y);}
int hove_execution(int t,int c,int a,int b,int d,int e,int y){return add(hove_e,&st.n_hove,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hove_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hove2,&st.n_hove2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hove_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hoveac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hove_market(int t,int c,int a,int b,int d,int e,int y){return add(hoveam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hove_report(void){ps("[HOVE] Hovep: ");pi(st.n_hovep);ps(" PCS=");pi(st.t_f1);ps("\nHove: ");pi(st.n_hove);ps(" PCS=");pi(st.t_f2);ps("\nHove2: ");pi(st.n_hove2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hove_state(void){ps("[HOVE] Hovep=");pi(st.n_hovep);ps(" Hove=");pi(st.n_hove);ps(" Hove2=");pi(st.n_hove2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hovenia Admin Demo ===\n\n");hove_init();
ps("Hovenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hove_planning(t,c,1535+(i*17),1524+(i*14),1504+(i*10),1486+(i*6),2020+(i%5));}
ps("\nHovenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hove_execution(t,c,1524+(i*15),1513+(i*12),1495+(i*8),1482+(i*5),2021+(i%4));}
ps("\nHovenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hove_evaluation(t,c,1516+(i*13),1505+(i*10),1489+(i*7),1478+(i*4),2022+(i%3));}
ps("\nHovenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hove_accessory(t,c,1508+(i*11),1499+(i*9),1485+(i*6),1475+(i*3),2023+(i%2));}
ps("\nHovenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hove_market(t,c,1502+(i*9),1493+(i*7),1480+(i*5),1472+(i*3),2024);}
ps("\n");hove_report();hove_state();ps("\n=== Demo Complete ===\n");return 0;}
