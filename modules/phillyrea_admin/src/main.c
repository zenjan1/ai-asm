/* phillyrea_admin: Phillyrea management technology administration (v1.0)
 * Phillyrea planning, phillyrea execution, phillyrea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} phil_t;
typedef struct{int n_philp,n_phile,n_phil2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} phil_state_t;
static phil_t philjs[N],philess[N-2],philvss[N-4],philas[N-6],philmks[N-6]; static phil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(phil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;phil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int phil_init(void){if(init)return -1;st.n_philp=0;st.n_phile=0;st.n_phil2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)philjs[i].active=0;for(int i=0;i<N-2;i++)philess[i].active=0;for(int i=0;i<N-4;i++)philvss[i].active=0;for(int i=0;i<N-6;i++)philas[i].active=0;for(int i=0;i<N-6;i++)philmks[i].active=0;init=1;ps("[PHI] Phillyrea initialized\n");return 0;}
int phil_planning(int t,int c,int a,int b,int d,int e,int y){return add(philjs,&st.n_philp,&st.t_f1,N,t,c,a,b,d,e,y);}
int phil_execution(int t,int c,int a,int b,int d,int e,int y){return add(philess,&st.n_phile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int phil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(philvss,&st.n_phil2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int phil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(philas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int phil_market(int t,int c,int a,int b,int d,int e,int y){return add(philmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void phil_report(void){ps("[PHI] Philp: ");pi(st.n_philp);ps(" PCS=");pi(st.t_f1);ps("\nPhile: ");pi(st.n_phile);ps(" PCS=");pi(st.t_f2);ps("\nPhilv: ");pi(st.n_phil2);ps(" PCS=");pi(st.t_f3);ps("\nPhilc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void phil_state(void){ps("[PHI] Philp=");pi(st.n_philp);ps(" Phile=");pi(st.n_phile);ps(" Philv=");pi(st.n_phil2);ps(" Philc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phillyrea Admin Demo ===\n\n");phil_init();
ps("Phillyrea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;phil_planning(t,c,1327+(i*17),1316+(i*14),1296+(i*10),1278+(i*6),2020+(i%5));}
ps("\nPhillyrea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;phil_execution(t,c,1316+(i*15),1305+(i*12),1287+(i*8),1274+(i*5),2021+(i%4));}
ps("\nPhillyrea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;phil_evaluation(t,c,1308+(i*13),1297+(i*10),1281+(i*7),1270+(i*4),2022+(i%3));}
ps("\nPhillyrea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phil_accessory(t,c,1300+(i*11),1291+(i*9),1277+(i*6),1267+(i*3),2023+(i%2));}
ps("\nPhillyrea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;phil_market(t,c,1294+(i*9),1285+(i*7),1272+(i*5),1264+(i*3),2024);}
ps("\n");phil_report();phil_state();ps("\n=== Demo Complete ===\n");return 0;}
