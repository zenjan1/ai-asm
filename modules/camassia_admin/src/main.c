/* camassia_admin: Camassia management technology administration (v1.0)
 * Camassia planning, camassia execution, camassia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cama_t;
typedef struct{int n_camaap,n_cama,n_cama2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cama_state_t;
static cama_t camas[N],camae[N-2],cama2[N-4],camaac[N-6],camam[N-6]; static cama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cama_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cama_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAMA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cama_init(void){if(init)return -1;st.n_camaap=0;st.n_cama=0;st.n_cama2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)camas[i].active=0;for(int i=0;i<N-2;i++)camae[i].active=0;for(int i=0;i<N-4;i++)cama2[i].active=0;for(int i=0;i<N-6;i++)camaac[i].active=0;for(int i=0;i<N-6;i++)camam[i].active=0;init=1;ps("[CAMA] Camassia initialized\n");return 0;}
int cama_planning(int t,int c,int a,int b,int d,int e,int y){return add(camas,&st.n_camaap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cama_execution(int t,int c,int a,int b,int d,int e,int y){return add(camae,&st.n_cama,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cama_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cama2,&st.n_cama2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cama_accessory(int t,int c,int a,int b,int d,int e,int y){return add(camaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cama_market(int t,int c,int a,int b,int d,int e,int y){return add(camam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cama_report(void){ps("[CAMA] Map: ");pi(st.n_camaap);ps(" PCS=");pi(st.t_f1);ps("\nMae: ");pi(st.n_cama);ps(" PCS=");pi(st.t_f2);ps("\nMa2: ");pi(st.n_cama2);ps(" PCS=");pi(st.t_f3);ps("\nMac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cama_state(void){ps("[CAMA] Map=");pi(st.n_camaap);ps(" Ma=");pi(st.n_cama);ps(" Ma2=");pi(st.n_cama2);ps(" Mac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Camassia Admin Demo ===\n\n");cama_init();
ps("Camassia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cama_planning(t,c,1455+(i*17),1444+(i*14),1424+(i*10),1406+(i*6),2020+(i%5));}
ps("\nCamassia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cama_execution(t,c,1444+(i*15),1433+(i*12),1415+(i*8),1402+(i*5),2021+(i%4));}
ps("\nCamassia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cama_evaluation(t,c,1436+(i*13),1425+(i*10),1409+(i*7),1398+(i*4),2022+(i%3));}
ps("\nCamassia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cama_accessory(t,c,1428+(i*11),1419+(i*9),1405+(i*6),1395+(i*3),2023+(i%2));}
ps("\nCamassia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cama_market(t,c,1422+(i*9),1413+(i*7),1400+(i*5),1392+(i*3),2024);}
ps("\n");cama_report();cama_state();ps("\n=== Demo Complete ===\n");return 0;}
