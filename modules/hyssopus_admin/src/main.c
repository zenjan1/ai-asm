/* hyssopus_admin: Hyssopus management technology administration (v1.0)
 * Hyssopus planning, hyssopus execution, hyssopus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hyss_t;
typedef struct{int n_hyssp,n_hyss,n_hyss2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hyss_state_t;
static hyss_t hysss[N],hysse[N-2],hyss2[N-4],hyssac[N-6],hyssam[N-6]; static hyss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hyss_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hyss_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYSS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hyss_init(void){if(init)return -1;st.n_hyssp=0;st.n_hyss=0;st.n_hyss2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hysss[i].active=0;for(int i=0;i<N-2;i++)hysse[i].active=0;for(int i=0;i<N-4;i++)hyss2[i].active=0;for(int i=0;i<N-6;i++)hyssac[i].active=0;for(int i=0;i<N-6;i++)hyssam[i].active=0;init=1;ps("[HYSS] Hyssopus initialized\n");return 0;}
int hyss_planning(int t,int c,int a,int b,int d,int e,int y){return add(hysss,&st.n_hyssp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hyss_execution(int t,int c,int a,int b,int d,int e,int y){return add(hysse,&st.n_hyss,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hyss_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hyss2,&st.n_hyss2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hyss_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hyssac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hyss_market(int t,int c,int a,int b,int d,int e,int y){return add(hyssam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hyss_report(void){ps("[HYSS] Hyssp: ");pi(st.n_hyssp);ps(" PCS=");pi(st.t_f1);ps("\nHysse: ");pi(st.n_hyss);ps(" PCS=");pi(st.t_f2);ps("\nHyss2: ");pi(st.n_hyss2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hyss_state(void){ps("[HYSS] Hyssp=");pi(st.n_hyssp);ps(" Hyss=");pi(st.n_hyss);ps(" Hyss2=");pi(st.n_hyss2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hyssopus Admin Demo ===\n\n");hyss_init();
ps("Hyssopus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hyss_planning(t,c,1541+(i*17),1530+(i*14),1510+(i*10),1492+(i*6),2020+(i%5));}
ps("\nHyssopus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hyss_execution(t,c,1530+(i*15),1519+(i*12),1501+(i*8),1488+(i*5),2021+(i%4));}
ps("\nHyssopus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hyss_evaluation(t,c,1522+(i*13),1511+(i*10),1495+(i*7),1484+(i*4),2022+(i%3));}
ps("\nHyssopus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyss_accessory(t,c,1514+(i*11),1505+(i*9),1491+(i*6),1481+(i*3),2023+(i%2));}
ps("\nHyssopus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyss_market(t,c,1508+(i*9),1499+(i*7),1486+(i*5),1478+(i*3),2024);}
ps("\n");hyss_report();hyss_state();ps("\n=== Demo Complete ===\n");return 0;}
