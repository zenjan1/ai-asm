/* berberis_admin: Berberis management technology administration (v1.0)
 * Berberis planning, berberis execution, berberis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} berb_t;
typedef struct{int n_berbp,n_berb,n_berb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} berb_state_t;
static berb_t berbs[N],berbe[N-2],berb2[N-4],berbac[N-6],berbam[N-6]; static berb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(berb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;berb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BERB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int berb_init(void){if(init)return -1;st.n_berbp=0;st.n_berb=0;st.n_berb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)berbs[i].active=0;for(int i=0;i<N-2;i++)berbe[i].active=0;for(int i=0;i<N-4;i++)berb2[i].active=0;for(int i=0;i<N-6;i++)berbac[i].active=0;for(int i=0;i<N-6;i++)berbam[i].active=0;init=1;ps("[BERB] Berberis initialized\n");return 0;}
int berb_planning(int t,int c,int a,int b,int d,int e,int y){return add(berbs,&st.n_berbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int berb_execution(int t,int c,int a,int b,int d,int e,int y){return add(berbe,&st.n_berb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int berb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(berb2,&st.n_berb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int berb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(berbac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int berb_market(int t,int c,int a,int b,int d,int e,int y){return add(berbam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void berb_report(void){ps("[BERB] Erbp: ");pi(st.n_berbp);ps(" PCS=");pi(st.t_f1);ps("\nErbe: ");pi(st.n_berb);ps(" PCS=");pi(st.t_f2);ps("\nErb2: ");pi(st.n_berb2);ps(" PCS=");pi(st.t_f3);ps("\nErbac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void berb_state(void){ps("[BERB] Erbp=");pi(st.n_berbp);ps(" Erbe=");pi(st.n_berb);ps(" Erb2=");pi(st.n_berb2);ps(" Erbac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Berberis Admin Demo ===\n\n");berb_init();
ps("Berberis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;berb_planning(t,c,1434+(i*17),1423+(i*14),1403+(i*10),1385+(i*6),2020+(i%5));}
ps("\nBerberis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;berb_execution(t,c,1423+(i*15),1412+(i*12),1394+(i*8),1381+(i*5),2021+(i%4));}
ps("\nBerberis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;berb_evaluation(t,c,1415+(i*13),1404+(i*10),1388+(i*7),1377+(i*4),2022+(i%3));}
ps("\nBerberis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;berb_accessory(t,c,1407+(i*11),1398+(i*9),1384+(i*6),1374+(i*3),2023+(i%2));}
ps("\nBerberis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;berb_market(t,c,1401+(i*9),1392+(i*7),1379+(i*5),1371+(i*3),2024);}
ps("\n");berb_report();berb_state();ps("\n=== Demo Complete ===\n");return 0;}
