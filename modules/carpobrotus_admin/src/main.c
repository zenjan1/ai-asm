/* carpobrotus_admin: Carpobrotus management technology administration (v1.0)
 * Carpobrotus planning, carpobrotus execution, carpobrotus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} carp_t;
typedef struct{int n_carpp,n_carp,n_carp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} carp_state_t;
static carp_t carps[N],carpe[N-2],carp2[N-4],carpac[N-6],carpam[N-6]; static carp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(carp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;carp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CARP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int carp_init(void){if(init)return -1;st.n_carpp=0;st.n_carp=0;st.n_carp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)carps[i].active=0;for(int i=0;i<N-2;i++)carpe[i].active=0;for(int i=0;i<N-4;i++)carp2[i].active=0;for(int i=0;i<N-6;i++)carpac[i].active=0;for(int i=0;i<N-6;i++)carpam[i].active=0;init=1;ps("[CARP] Carpobrotus initialized\n");return 0;}
int carp_planning(int t,int c,int a,int b,int d,int e,int y){return add(carps,&st.n_carpp,&st.t_f1,N,t,c,a,b,d,e,y);}
int carp_execution(int t,int c,int a,int b,int d,int e,int y){return add(carpe,&st.n_carp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int carp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(carp2,&st.n_carp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int carp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(carpac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int carp_market(int t,int c,int a,int b,int d,int e,int y){return add(carpam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void carp_report(void){ps("[CARP] Arpp: ");pi(st.n_carpp);ps(" PCS=");pi(st.t_f1);ps("\nArpe: ");pi(st.n_carp);ps(" PCS=");pi(st.t_f2);ps("\nArp2: ");pi(st.n_carp2);ps(" PCS=");pi(st.t_f3);ps("\nArac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void carp_state(void){ps("[CARP] Arpp=");pi(st.n_carpp);ps(" Arp=");pi(st.n_carp);ps(" Arp2=");pi(st.n_carp2);ps(" Arac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Carpobrotus Admin Demo ===\n\n");carp_init();
ps("Carpobrotus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;carp_planning(t,c,1462+(i*17),1451+(i*14),1431+(i*10),1413+(i*6),2020+(i%5));}
ps("\nCarpobrotus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;carp_execution(t,c,1451+(i*15),1440+(i*12),1422+(i*8),1409+(i*5),2021+(i%4));}
ps("\nCarpobrotus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;carp_evaluation(t,c,1443+(i*13),1432+(i*10),1416+(i*7),1405+(i*4),2022+(i%3));}
ps("\nCarpobrotus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;carp_accessory(t,c,1435+(i*11),1426+(i*9),1412+(i*6),1402+(i*3),2023+(i%2));}
ps("\nCarpobrotus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;carp_market(t,c,1429+(i*9),1420+(i*7),1407+(i*5),1399+(i*3),2024);}
ps("\n");carp_report();carp_state();ps("\n=== Demo Complete ===\n");return 0;}
