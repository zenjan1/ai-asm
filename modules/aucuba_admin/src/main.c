/* aucuba_admin: Aucuba management technology administration (v1.0)
 * Aucuba planning, aucuba execution, aucuba evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aucu_t;
typedef struct{int n_aucup,n_aucu,n_aucu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aucu_state_t;
static aucu_t aucus[N],auce[N-2],aucu2[N-4],aucuac[N-6],aucuam[N-6]; static aucu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(aucu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aucu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AUCU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aucu_init(void){if(init)return -1;st.n_aucup=0;st.n_aucu=0;st.n_aucu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aucus[i].active=0;for(int i=0;i<N-2;i++)auce[i].active=0;for(int i=0;i<N-4;i++)aucu2[i].active=0;for(int i=0;i<N-6;i++)aucuac[i].active=0;for(int i=0;i<N-6;i++)aucuam[i].active=0;init=1;ps("[AUCU] Aucuba initialized\n");return 0;}
int aucu_planning(int t,int c,int a,int b,int d,int e,int y){return add(aucus,&st.n_aucup,&st.t_f1,N,t,c,a,b,d,e,y);}
int aucu_execution(int t,int c,int a,int b,int d,int e,int y){return add(auce,&st.n_aucu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aucu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aucu2,&st.n_aucu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aucu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aucuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aucu_market(int t,int c,int a,int b,int d,int e,int y){return add(aucuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aucu_report(void){ps("[AUCU] Cup: ");pi(st.n_aucup);ps(" PCS=");pi(st.t_f1);ps("\nCuces: ");pi(st.n_aucu);ps(" PCS=");pi(st.t_f2);ps("\nCu2: ");pi(st.n_aucu2);ps(" PCS=");pi(st.t_f3);ps("\nCuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aucu_state(void){ps("[AUCU] Cup=");pi(st.n_aucup);ps(" Cuce=");pi(st.n_aucu);ps(" Cu2=");pi(st.n_aucu2);ps(" Cuac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aucuba Admin Demo ===\n\n");aucu_init();
ps("Aucuba planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aucu_planning(t,c,1421+(i*17),1410+(i*14),1390+(i*10),1372+(i*6),2020+(i%5));}
ps("\nAucuba execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aucu_execution(t,c,1410+(i*15),1399+(i*12),1381+(i*8),1368+(i*5),2021+(i%4));}
ps("\nAucuba evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aucu_evaluation(t,c,1402+(i*13),1391+(i*10),1375+(i*7),1364+(i*4),2022+(i%3));}
ps("\nAucuba accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aucu_accessory(t,c,1394+(i*11),1385+(i*9),1371+(i*6),1361+(i*3),2023+(i%2));}
ps("\nAucuba marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aucu_market(t,c,1388+(i*9),1379+(i*7),1366+(i*5),1358+(i*3),2024);}
ps("\n");aucu_report();aucu_state();ps("\n=== Demo Complete ===\n");return 0;}
