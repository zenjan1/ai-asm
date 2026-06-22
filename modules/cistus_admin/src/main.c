/* cistus_admin: Cistus management technology administration (v1.0)
 * Cistus planning, cistus execution, cistus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cist_t;
typedef struct{int n_cistp,n_cist,n_cist2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cist_state_t;
static cist_t cists[N],ciste[N-2],cist2[N-4],cistac[N-6],cistam[N-6]; static cist_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cist_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cist_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CIST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cist_init(void){if(init)return -1;st.n_cistp=0;st.n_cist=0;st.n_cist2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cists[i].active=0;for(int i=0;i<N-2;i++)ciste[i].active=0;for(int i=0;i<N-4;i++)cist2[i].active=0;for(int i=0;i<N-6;i++)cistac[i].active=0;for(int i=0;i<N-6;i++)cistam[i].active=0;init=1;ps("[CIST] Cistus initialized\n");return 0;}
int cist_planning(int t,int c,int a,int b,int d,int e,int y){return add(cists,&st.n_cistp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cist_execution(int t,int c,int a,int b,int d,int e,int y){return add(ciste,&st.n_cist,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cist_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cist2,&st.n_cist2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cist_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cistac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cist_market(int t,int c,int a,int b,int d,int e,int y){return add(cistam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cist_report(void){ps("[CIST] Stp: ");pi(st.n_cistp);ps(" PCS=");pi(st.t_f1);ps("\nSte: ");pi(st.n_cist);ps(" PCS=");pi(st.t_f2);ps("\nSt2: ");pi(st.n_cist2);ps(" PCS=");pi(st.t_f3);ps("\nSac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cist_state(void){ps("[CIST] Stp=");pi(st.n_cistp);ps(" St=");pi(st.n_cist);ps(" St2=");pi(st.n_cist2);ps(" Sac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cistus Admin Demo ===\n\n");cist_init();
ps("Cistus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cist_planning(t,c,1476+(i*17),1465+(i*14),1445+(i*10),1427+(i*6),2020+(i%5));}
ps("\nCistus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cist_execution(t,c,1465+(i*15),1454+(i*12),1436+(i*8),1423+(i*5),2021+(i%4));}
ps("\nCistus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cist_evaluation(t,c,1457+(i*13),1446+(i*10),1430+(i*7),1419+(i*4),2022+(i%3));}
ps("\nCistus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cist_accessory(t,c,1449+(i*11),1440+(i*9),1426+(i*6),1416+(i*3),2023+(i%2));}
ps("\nCistus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cist_market(t,c,1443+(i*9),1434+(i*7),1421+(i*5),1413+(i*3),2024);}
ps("\n");cist_report();cist_state();ps("\n=== Demo Complete ===\n");return 0;}
