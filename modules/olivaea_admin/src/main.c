/* olivaea_admin: Olivaea management technology administration (v1.0)
 * Olivaea planning, olivaea execution, olivaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oli_t;
typedef struct{int n_olip,n_olie,n_oli2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oli_state_t;
static oli_t olips[N],oliess[N-2],olivss[N-4],olias[N-6],olimks[N-6]; static oli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(oli_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oli_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OLI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oli_init(void){if(init)return -1;st.n_olip=0;st.n_olie=0;st.n_oli2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)olips[i].active=0;for(int i=0;i<N-2;i++)oliess[i].active=0;for(int i=0;i<N-4;i++)olivss[i].active=0;for(int i=0;i<N-6;i++)olias[i].active=0;for(int i=0;i<N-6;i++)olimks[i].active=0;init=1;ps("[OLI] Olivaea initialized\n");return 0;}
int oli_planning(int t,int c,int a,int b,int d,int e,int y){return add(olips,&st.n_olip,&st.t_f1,N,t,c,a,b,d,e,y);}
int oli_execution(int t,int c,int a,int b,int d,int e,int y){return add(oliess,&st.n_olie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(olivss,&st.n_oli2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(olias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oli_market(int t,int c,int a,int b,int d,int e,int y){return add(olimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oli_report(void){ps("[OLI] Olip: ");pi(st.n_olip);ps(" PCS=");pi(st.t_f1);ps("\nOlie: ");pi(st.n_olie);ps(" PCS=");pi(st.t_f2);ps("\nOliv: ");pi(st.n_oli2);ps(" PCS=");pi(st.t_f3);ps("\nOlic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oli_state(void){ps("[OLI] Olip=");pi(st.n_olip);ps(" Olie=");pi(st.n_olie);ps(" Oliv=");pi(st.n_oli2);ps(" Olic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Olivaea Admin Demo ===\n\n");oli_init();
ps("Olivaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oli_planning(t,c,1322+(i*17),1311+(i*14),1291+(i*10),1273+(i*6),2020+(i%5));}
ps("\nOlivaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oli_execution(t,c,1311+(i*15),1300+(i*12),1282+(i*8),1269+(i*5),2021+(i%4));}
ps("\nOlivaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oli_evaluation(t,c,1303+(i*13),1292+(i*10),1276+(i*7),1265+(i*4),2022+(i%3));}
ps("\nOlivaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oli_accessory(t,c,1295+(i*11),1286+(i*9),1272+(i*6),1262+(i*3),2023+(i%2));}
ps("\nOlivaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oli_market(t,c,1289+(i*9),1280+(i*7),1267+(i*5),1259+(i*3),2024);}
ps("\n");oli_report();oli_state();ps("\n=== Demo Complete ===\n");return 0;}
