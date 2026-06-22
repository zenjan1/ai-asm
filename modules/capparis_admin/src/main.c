/* capparis_admin: Capparis management technology administration (v1.0)
 * Capparis planning, capparis execution, capparis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} capp_t;
typedef struct{int n_capp_p,n_capp,n_capp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} capp_state_t;
static capp_t capps[N],cappe[N-2],capp2[N-4],cappac[N-6],cappam[N-6]; static capp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(capp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;capp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAPP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int capp_init(void){if(init)return -1;st.n_capp_p=0;st.n_capp=0;st.n_capp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)capps[i].active=0;for(int i=0;i<N-2;i++)cappe[i].active=0;for(int i=0;i<N-4;i++)capp2[i].active=0;for(int i=0;i<N-6;i++)cappac[i].active=0;for(int i=0;i<N-6;i++)cappam[i].active=0;init=1;ps("[CAPP] Capparis initialized\n");return 0;}
int capp_planning(int t,int c,int a,int b,int d,int e,int y){return add(capps,&st.n_capp_p,&st.t_f1,N,t,c,a,b,d,e,y);}
int capp_execution(int t,int c,int a,int b,int d,int e,int y){return add(cappe,&st.n_capp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int capp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(capp2,&st.n_capp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int capp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cappac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int capp_market(int t,int c,int a,int b,int d,int e,int y){return add(cappam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void capp_report(void){ps("[CAPP] Pp: ");pi(st.n_capp_p);ps(" PCS=");pi(st.t_f1);ps("\nPe: ");pi(st.n_capp);ps(" PCS=");pi(st.t_f2);ps("\nPp2: ");pi(st.n_capp2);ps(" PCS=");pi(st.t_f3);ps("\nPac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void capp_state(void){ps("[CAPP] Pp=");pi(st.n_capp_p);ps(" Cp=");pi(st.n_capp);ps(" Cp2=");pi(st.n_capp2);ps(" Pac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Capparis Admin Demo ===\n\n");capp_init();
ps("Capparis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;capp_planning(t,c,1458+(i*17),1447+(i*14),1427+(i*10),1409+(i*6),2020+(i%5));}
ps("\nCapparis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;capp_execution(t,c,1447+(i*15),1436+(i*12),1418+(i*8),1405+(i*5),2021+(i%4));}
ps("\nCapparis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;capp_evaluation(t,c,1439+(i*13),1428+(i*10),1412+(i*7),1401+(i*4),2022+(i%3));}
ps("\nCapparis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;capp_accessory(t,c,1431+(i*11),1422+(i*9),1408+(i*6),1398+(i*3),2023+(i%2));}
ps("\nCapparis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;capp_market(t,c,1425+(i*9),1416+(i*7),1403+(i*5),1395+(i*3),2024);}
ps("\n");capp_report();capp_state();ps("\n=== Demo Complete ===\n");return 0;}
