/* cycas_admin: Cycas management technology administration (v1.0)
 * Cycas planning, cycas execution, cycas evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyca_t;
typedef struct{int n_cycap,n_cyca,n_cyca2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyca_state_t;
static cyca_t cycas[N],cycae[N-2],cyca2[N-4],cycaac[N-6],cycaam[N-6]; static cyca_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cyca_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyca_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYCA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyca_init(void){if(init)return -1;st.n_cycap=0;st.n_cyca=0;st.n_cyca2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cycas[i].active=0;for(int i=0;i<N-2;i++)cycae[i].active=0;for(int i=0;i<N-4;i++)cyca2[i].active=0;for(int i=0;i<N-6;i++)cycaac[i].active=0;for(int i=0;i<N-6;i++)cycaam[i].active=0;init=1;ps("[CYCA] Cycas initialized\n");return 0;}
int cyca_planning(int t,int c,int a,int b,int d,int e,int y){return add(cycas,&st.n_cycap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyca_execution(int t,int c,int a,int b,int d,int e,int y){return add(cycae,&st.n_cyca,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyca_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cyca2,&st.n_cyca2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyca_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cycaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyca_market(int t,int c,int a,int b,int d,int e,int y){return add(cycaam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyca_report(void){ps("[CYCA] Cap: ");pi(st.n_cycap);ps(" PCS=");pi(st.t_f1);ps("\nCae: ");pi(st.n_cyca);ps(" PCS=");pi(st.t_f2);ps("\nCa2: ");pi(st.n_cyca2);ps(" PCS=");pi(st.t_f3);ps("\nCac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyca_state(void){ps("[CYCA] Cap=");pi(st.n_cycap);ps(" Ca=");pi(st.n_cyca);ps(" Ca2=");pi(st.n_cyca2);ps(" Cac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cycas Admin Demo ===\n\n");cyca_init();
ps("Cycas planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyca_planning(t,c,1484+(i*17),1473+(i*14),1453+(i*10),1435+(i*6),2020+(i%5));}
ps("\nCycas execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyca_execution(t,c,1473+(i*15),1462+(i*12),1444+(i*8),1431+(i*5),2021+(i%4));}
ps("\nCycas evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyca_evaluation(t,c,1465+(i*13),1454+(i*10),1438+(i*7),1427+(i*4),2022+(i%3));}
ps("\nCycas accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyca_accessory(t,c,1457+(i*11),1448+(i*9),1434+(i*6),1424+(i*3),2023+(i%2));}
ps("\nCycas marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyca_market(t,c,1451+(i*9),1442+(i*7),1429+(i*5),1421+(i*3),2024);}
ps("\n");cyca_report();cyca_state();ps("\n=== Demo Complete ===\n");return 0;}
