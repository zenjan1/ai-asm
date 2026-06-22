/* bacopa_admin: Bacopa management technology administration (v1.0)
 * Bacopa planning, bacopa execution, bacopa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} baco_t;
typedef struct{int n_bacop,n_baco,n_baco2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} baco_state_t;
static baco_t bacos[N],bace[N-2],baco2[N-4],bacoac[N-6],bacoam[N-6]; static baco_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(baco_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;baco_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BACO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int baco_init(void){if(init)return -1;st.n_bacop=0;st.n_baco=0;st.n_baco2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bacos[i].active=0;for(int i=0;i<N-2;i++)bace[i].active=0;for(int i=0;i<N-4;i++)baco2[i].active=0;for(int i=0;i<N-6;i++)bacoac[i].active=0;for(int i=0;i<N-6;i++)bacoam[i].active=0;init=1;ps("[BACO] Bacopa initialized\n");return 0;}
int baco_planning(int t,int c,int a,int b,int d,int e,int y){return add(bacos,&st.n_bacop,&st.t_f1,N,t,c,a,b,d,e,y);}
int baco_execution(int t,int c,int a,int b,int d,int e,int y){return add(bace,&st.n_baco,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int baco_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(baco2,&st.n_baco2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int baco_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bacoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int baco_market(int t,int c,int a,int b,int d,int e,int y){return add(bacoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void baco_report(void){ps("[BACO] Cop: ");pi(st.n_bacop);ps(" PCS=");pi(st.t_f1);ps("\nCoes: ");pi(st.n_baco);ps(" PCS=");pi(st.t_f2);ps("\nCo2: ");pi(st.n_baco2);ps(" PCS=");pi(st.t_f3);ps("\nCoac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void baco_state(void){ps("[BACO] Cop=");pi(st.n_bacop);ps(" Coe=");pi(st.n_baco);ps(" Co2=");pi(st.n_baco2);ps(" Coac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bacopa Admin Demo ===\n\n");baco_init();
ps("Bacopa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;baco_planning(t,c,1428+(i*17),1417+(i*14),1397+(i*10),1379+(i*6),2020+(i%5));}
ps("\nBacopa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;baco_execution(t,c,1417+(i*15),1406+(i*12),1388+(i*8),1375+(i*5),2021+(i%4));}
ps("\nBacopa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;baco_evaluation(t,c,1409+(i*13),1398+(i*10),1382+(i*7),1371+(i*4),2022+(i%3));}
ps("\nBacopa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;baco_accessory(t,c,1401+(i*11),1392+(i*9),1378+(i*6),1368+(i*3),2023+(i%2));}
ps("\nBacopa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;baco_market(t,c,1395+(i*9),1386+(i*7),1373+(i*5),1365+(i*3),2024);}
ps("\n");baco_report();baco_state();ps("\n=== Demo Complete ===\n");return 0;}
