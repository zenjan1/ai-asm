/* calycanthus_admin: Calycanthus management technology administration (v1.0)
 * Calycanthus planning, calycanthus execution, calycanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} caly_t;
typedef struct{int n_calyp,n_caly,n_caly2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} caly_state_t;
static caly_t calys[N],calye[N-2],caly2[N-4],calyac[N-6],calyam[N-6]; static caly_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(caly_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;caly_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CALY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int caly_init(void){if(init)return -1;st.n_calyp=0;st.n_caly=0;st.n_caly2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)calys[i].active=0;for(int i=0;i<N-2;i++)calye[i].active=0;for(int i=0;i<N-4;i++)caly2[i].active=0;for(int i=0;i<N-6;i++)calyac[i].active=0;for(int i=0;i<N-6;i++)calyam[i].active=0;init=1;ps("[CALY] Calycanthus initialized\n");return 0;}
int caly_planning(int t,int c,int a,int b,int d,int e,int y){return add(calys,&st.n_calyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int caly_execution(int t,int c,int a,int b,int d,int e,int y){return add(calye,&st.n_caly,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int caly_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(caly2,&st.n_caly2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int caly_accessory(int t,int c,int a,int b,int d,int e,int y){return add(calyac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int caly_market(int t,int c,int a,int b,int d,int e,int y){return add(calyam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void caly_report(void){ps("[CALY] Lyp: ");pi(st.n_calyp);ps(" PCS=");pi(st.t_f1);ps("\nLye: ");pi(st.n_caly);ps(" PCS=");pi(st.t_f2);ps("\nLy2: ");pi(st.n_caly2);ps(" PCS=");pi(st.t_f3);ps("\nLac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void caly_state(void){ps("[CALY] Lyp=");pi(st.n_calyp);ps(" Ly=");pi(st.n_caly);ps(" Ly2=");pi(st.n_caly2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calycanthus Admin Demo ===\n\n");caly_init();
ps("Calycanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;caly_planning(t,c,1454+(i*17),1443+(i*14),1423+(i*10),1405+(i*6),2020+(i%5));}
ps("\nCalycanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;caly_execution(t,c,1443+(i*15),1432+(i*12),1414+(i*8),1401+(i*5),2021+(i%4));}
ps("\nCalycanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;caly_evaluation(t,c,1435+(i*13),1424+(i*10),1408+(i*7),1397+(i*4),2022+(i%3));}
ps("\nCalycanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;caly_accessory(t,c,1427+(i*11),1418+(i*9),1404+(i*6),1394+(i*3),2023+(i%2));}
ps("\nCalycanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;caly_market(t,c,1421+(i*9),1412+(i*7),1399+(i*5),1391+(i*3),2024);}
ps("\n");caly_report();caly_state();ps("\n=== Demo Complete ===\n");return 0;}
