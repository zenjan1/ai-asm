/* cannabis_admin: Cannabis management technology administration (v1.0)
 * Cannabis planning, cannabis execution, cannabis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cann_t;
typedef struct{int n_cannp,n_cann,n_cann2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cann_state_t;
static cann_t canns[N],canne[N-2],cann2[N-4],cannac[N-6],cannam[N-6]; static cann_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cann_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cann_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CANN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cann_init(void){if(init)return -1;st.n_cannp=0;st.n_cann=0;st.n_cann2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)canns[i].active=0;for(int i=0;i<N-2;i++)canne[i].active=0;for(int i=0;i<N-4;i++)cann2[i].active=0;for(int i=0;i<N-6;i++)cannac[i].active=0;for(int i=0;i<N-6;i++)cannam[i].active=0;init=1;ps("[CANN] Cannabis initialized\n");return 0;}
int cann_planning(int t,int c,int a,int b,int d,int e,int y){return add(canns,&st.n_cannp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cann_execution(int t,int c,int a,int b,int d,int e,int y){return add(canne,&st.n_cann,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cann_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cann2,&st.n_cann2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cann_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cannac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cann_market(int t,int c,int a,int b,int d,int e,int y){return add(cannam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cann_report(void){ps("[CANN] Np: ");pi(st.n_cannp);ps(" PCS=");pi(st.t_f1);ps("\nNe: ");pi(st.n_cann);ps(" PCS=");pi(st.t_f2);ps("\nNn2: ");pi(st.n_cann2);ps(" PCS=");pi(st.t_f3);ps("\nNac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cann_state(void){ps("[CANN] Np=");pi(st.n_cannp);ps(" Nn=");pi(st.n_cann);ps(" Nn2=");pi(st.n_cann2);ps(" Nac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cannabis Admin Demo ===\n\n");cann_init();
ps("Cannabis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cann_planning(t,c,1457+(i*17),1446+(i*14),1426+(i*10),1408+(i*6),2020+(i%5));}
ps("\nCannabis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cann_execution(t,c,1446+(i*15),1435+(i*12),1417+(i*8),1404+(i*5),2021+(i%4));}
ps("\nCannabis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cann_evaluation(t,c,1438+(i*13),1427+(i*10),1411+(i*7),1400+(i*4),2022+(i%3));}
ps("\nCannabis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cann_accessory(t,c,1430+(i*11),1421+(i*9),1407+(i*6),1397+(i*3),2023+(i%2));}
ps("\nCannabis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cann_market(t,c,1424+(i*9),1415+(i*7),1402+(i*5),1394+(i*3),2024);}
ps("\n");cann_report();cann_state();ps("\n=== Demo Complete ===\n");return 0;}
