/* loasa_admin: Loasa management technology administration (v1.0)
 * Loasa planning, loasa execution, loasa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} loa_t;
typedef struct{int n_loap,n_loae,n_loa2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} loa_state_t;
static loa_t loaps[N],loaess[N-2],loavss[N-4],loaas[N-6],loamks[N-6]; static loa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(loa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;loa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int loa_init(void){if(init)return -1;st.n_loap=0;st.n_loae=0;st.n_loa2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)loaps[i].active=0;for(int i=0;i<N-2;i++)loaess[i].active=0;for(int i=0;i<N-4;i++)loavss[i].active=0;for(int i=0;i<N-6;i++)loaas[i].active=0;for(int i=0;i<N-6;i++)loamks[i].active=0;init=1;ps("[LOA] Loasa initialized\n");return 0;}
int loa_planning(int t,int c,int a,int b,int d,int e,int y){return add(loaps,&st.n_loap,&st.t_f1,N,t,c,a,b,d,e,y);}
int loa_execution(int t,int c,int a,int b,int d,int e,int y){return add(loaess,&st.n_loae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int loa_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(loavss,&st.n_loa2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int loa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(loaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int loa_market(int t,int c,int a,int b,int d,int e,int y){return add(loamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void loa_report(void){ps("[LOA] Loap: ");pi(st.n_loap);ps(" PCS=");pi(st.t_f1);ps("\nLoae: ");pi(st.n_loae);ps(" PCS=");pi(st.t_f2);ps("\nLoav: ");pi(st.n_loa2);ps(" PCS=");pi(st.t_f3);ps("\nLoac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void loa_state(void){ps("[LOA] Loap=");pi(st.n_loap);ps(" Loae=");pi(st.n_loae);ps(" Loav=");pi(st.n_loa2);ps(" Loac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Loasa Admin Demo ===\n\n");loa_init();
ps("Loasa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;loa_planning(t,c,1309+(i*17),1298+(i*14),1278+(i*10),1260+(i*6),2020+(i%5));}
ps("\nLoasa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;loa_execution(t,c,1298+(i*15),1287+(i*12),1269+(i*8),1256+(i*5),2021+(i%4));}
ps("\nLoasa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;loa_evaluation(t,c,1290+(i*13),1279+(i*10),1263+(i*7),1252+(i*4),2022+(i%3));}
ps("\nLoasa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;loa_accessory(t,c,1282+(i*11),1273+(i*9),1259+(i*6),1249+(i*3),2023+(i%2));}
ps("\nLoasa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;loa_market(t,c,1276+(i*9),1267+(i*7),1254+(i*5),1246+(i*3),2024);}
ps("\n");loa_report();loa_state();ps("\n=== Demo Complete ===\n");return 0;}
