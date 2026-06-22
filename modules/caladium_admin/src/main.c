/* caladium_admin: Caladium management technology administration (v1.0)
 * Caladium planning, caladium execution, caladium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cala_t;
typedef struct{int n_calap,n_cala,n_cala2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cala_state_t;
static cala_t calas[N],calae[N-2],cala2[N-4],calaac[N-6],calam[N-6]; static cala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cala_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cala_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CALA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cala_init(void){if(init)return -1;st.n_calap=0;st.n_cala=0;st.n_cala2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)calas[i].active=0;for(int i=0;i<N-2;i++)calae[i].active=0;for(int i=0;i<N-4;i++)cala2[i].active=0;for(int i=0;i<N-6;i++)calaac[i].active=0;for(int i=0;i<N-6;i++)calam[i].active=0;init=1;ps("[CALA] Caladium initialized\n");return 0;}
int cala_planning(int t,int c,int a,int b,int d,int e,int y){return add(calas,&st.n_calap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cala_execution(int t,int c,int a,int b,int d,int e,int y){return add(calae,&st.n_cala,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cala_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cala2,&st.n_cala2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cala_accessory(int t,int c,int a,int b,int d,int e,int y){return add(calaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cala_market(int t,int c,int a,int b,int d,int e,int y){return add(calam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cala_report(void){ps("[CALA] Lap: ");pi(st.n_calap);ps(" PCS=");pi(st.t_f1);ps("\nLae: ");pi(st.n_cala);ps(" PCS=");pi(st.t_f2);ps("\nLa2: ");pi(st.n_cala2);ps(" PCS=");pi(st.t_f3);ps("\nLac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cala_state(void){ps("[CALA] Lap=");pi(st.n_calap);ps(" La=");pi(st.n_cala);ps(" La2=");pi(st.n_cala2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Caladium Admin Demo ===\n\n");cala_init();
ps("Caladium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cala_planning(t,c,1452+(i*17),1441+(i*14),1421+(i*10),1403+(i*6),2020+(i%5));}
ps("\nCaladium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cala_execution(t,c,1441+(i*15),1430+(i*12),1412+(i*8),1399+(i*5),2021+(i%4));}
ps("\nCaladium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cala_evaluation(t,c,1433+(i*13),1422+(i*10),1406+(i*7),1395+(i*4),2022+(i%3));}
ps("\nCaladium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cala_accessory(t,c,1425+(i*11),1416+(i*9),1402+(i*6),1392+(i*3),2023+(i%2));}
ps("\nCaladium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cala_market(t,c,1419+(i*9),1410+(i*7),1397+(i*5),1389+(i*3),2024);}
ps("\n");cala_report();cala_state();ps("\n=== Demo Complete ===\n");return 0;}
