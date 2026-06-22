/* atherosperma_admin: Atherosperma management technology administration (v1.0)
 * Atherosperma planning, atherosperma execution, atherosperma evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} athe_t;
typedef struct{int n_athep,n_athe,n_athe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} athe_state_t;
static athe_t athes[N],athee[N-2],athe2[N-4],atheac[N-6],atheam[N-6]; static athe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(athe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;athe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ATHE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int athe_init(void){if(init)return -1;st.n_athep=0;st.n_athe=0;st.n_athe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)athes[i].active=0;for(int i=0;i<N-2;i++)athee[i].active=0;for(int i=0;i<N-4;i++)athe2[i].active=0;for(int i=0;i<N-6;i++)atheac[i].active=0;for(int i=0;i<N-6;i++)atheam[i].active=0;init=1;ps("[ATHE] Atherosperma initialized\n");return 0;}
int athe_planning(int t,int c,int a,int b,int d,int e,int y){return add(athes,&st.n_athep,&st.t_f1,N,t,c,a,b,d,e,y);}
int athe_execution(int t,int c,int a,int b,int d,int e,int y){return add(athee,&st.n_athe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int athe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(athe2,&st.n_athe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int athe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(atheac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int athe_market(int t,int c,int a,int b,int d,int e,int y){return add(atheam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void athe_report(void){ps("[ATHE] Thep: ");pi(st.n_athep);ps(" PCS=");pi(st.t_f1);ps("\nThee: ");pi(st.n_athe);ps(" PCS=");pi(st.t_f2);ps("\nThe2: ");pi(st.n_athe2);ps(" PCS=");pi(st.t_f3);ps("\nTheac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void athe_state(void){ps("[ATHE] Thep=");pi(st.n_athep);ps(" Thee=");pi(st.n_athe);ps(" The2=");pi(st.n_athe2);ps(" Theac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Atherosperma Admin Demo ===\n\n");athe_init();
ps("Atherosperma planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;athe_planning(t,c,1419+(i*17),1408+(i*14),1388+(i*10),1370+(i*6),2020+(i%5));}
ps("\nAtherosperma execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;athe_execution(t,c,1408+(i*15),1397+(i*12),1379+(i*8),1366+(i*5),2021+(i%4));}
ps("\nAtherosperma evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;athe_evaluation(t,c,1400+(i*13),1389+(i*10),1373+(i*7),1362+(i*4),2022+(i%3));}
ps("\nAtherosperma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;athe_accessory(t,c,1392+(i*11),1383+(i*9),1369+(i*6),1359+(i*3),2023+(i%2));}
ps("\nAtherosperma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;athe_market(t,c,1386+(i*9),1377+(i*7),1364+(i*5),1356+(i*3),2024);}
ps("\n");athe_report();athe_state();ps("\n=== Demo Complete ===\n");return 0;}
