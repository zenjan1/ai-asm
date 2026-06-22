/* cephalanthus_admin: Cephalanthus management technology administration (v1.0)
 * Cephalanthus planning, cephalanthus execution, cephalanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ceph_t;
typedef struct{int n_cephp,n_ceph,n_ceph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ceph_state_t;
static ceph_t cephps[N],cephe[N-2],ceph2[N-4],cephac[N-6],cepham[N-6]; static ceph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ceph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ceph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CEPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ceph_init(void){if(init)return -1;st.n_cephp=0;st.n_ceph=0;st.n_ceph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cephps[i].active=0;for(int i=0;i<N-2;i++)cephe[i].active=0;for(int i=0;i<N-4;i++)ceph2[i].active=0;for(int i=0;i<N-6;i++)cephac[i].active=0;for(int i=0;i<N-6;i++)cepham[i].active=0;init=1;ps("[CEPH] Cephalanthus initialized\n");return 0;}
int ceph_planning(int t,int c,int a,int b,int d,int e,int y){return add(cephps,&st.n_cephp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ceph_execution(int t,int c,int a,int b,int d,int e,int y){return add(cephe,&st.n_ceph,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ceph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ceph2,&st.n_ceph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ceph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cephac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ceph_market(int t,int c,int a,int b,int d,int e,int y){return add(cepham,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ceph_report(void){ps("[CEPH] Ephp: ");pi(st.n_cephp);ps(" PCS=");pi(st.t_f1);ps("\nEphe: ");pi(st.n_ceph);ps(" PCS=");pi(st.t_f2);ps("\nEph2: ");pi(st.n_ceph2);ps(" PCS=");pi(st.t_f3);ps("\nEphac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ceph_state(void){ps("[CEPH] Ephp=");pi(st.n_cephp);ps(" Eph=");pi(st.n_ceph);ps(" Eph2=");pi(st.n_ceph2);ps(" Ephac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cephalanthus Admin Demo ===\n\n");ceph_init();
ps("Cephalanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ceph_planning(t,c,1468+(i*17),1457+(i*14),1437+(i*10),1419+(i*6),2020+(i%5));}
ps("\nCephalanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ceph_execution(t,c,1457+(i*15),1446+(i*12),1428+(i*8),1415+(i*5),2021+(i%4));}
ps("\nCephalanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ceph_evaluation(t,c,1449+(i*13),1438+(i*10),1422+(i*7),1411+(i*4),2022+(i%3));}
ps("\nCephalanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ceph_accessory(t,c,1441+(i*11),1432+(i*9),1418+(i*6),1408+(i*3),2023+(i%2));}
ps("\nCephalanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ceph_market(t,c,1435+(i*9),1426+(i*7),1413+(i*5),1405+(i*3),2024);}
ps("\n");ceph_report();ceph_state();ps("\n=== Demo Complete ===\n");return 0;}
