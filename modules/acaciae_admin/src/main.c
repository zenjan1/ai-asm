/* acaciae_admin: Acaciae management technology administration (v1.0)
 * Acaciae planning, acaciae execution, acaciae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acac_t;
typedef struct{int n_acacp,n_acac,n_acac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acac_state_t;
static acac_t acacs[N],acace[N-2],acacv[N-4],acacac[N-6],acacm[N-6]; static acac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acac_init(void){if(init)return -1;st.n_acacp=0;st.n_acac=0;st.n_acac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acacs[i].active=0;for(int i=0;i<N-2;i++)acace[i].active=0;for(int i=0;i<N-4;i++)acacv[i].active=0;for(int i=0;i<N-6;i++)acacac[i].active=0;for(int i=0;i<N-6;i++)acacm[i].active=0;init=1;ps("[ACAC] Acaciae initialized\n");return 0;}
int acac_planning(int t,int c,int a,int b,int d,int e,int y){return add(acacs,&st.n_acacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acac_execution(int t,int c,int a,int b,int d,int e,int y){return add(acace,&st.n_acac,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acacv,&st.n_acac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acacac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acac_market(int t,int c,int a,int b,int d,int e,int y){return add(acacm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acac_report(void){ps("[ACAC] Acp: ");pi(st.n_acacp);ps(" PCS=");pi(st.t_f1);ps("\nAce: ");pi(st.n_acac);ps(" PCS=");pi(st.t_f2);ps("\nAcv: ");pi(st.n_acac2);ps(" PCS=");pi(st.t_f3);ps("\nAcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acac_state(void){ps("[ACAC] Acp=");pi(st.n_acacp);ps(" Ace=");pi(st.n_acac);ps(" Acv=");pi(st.n_acac2);ps(" Acac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acaciae Admin Demo ===\n\n");acac_init();
ps("Acaciae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acac_planning(t,c,1386+(i*17),1375+(i*14),1355+(i*10),1337+(i*6),2020+(i%5));}
ps("\nAcaciae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acac_execution(t,c,1375+(i*15),1364+(i*12),1346+(i*8),1333+(i*5),2021+(i%4));}
ps("\nAcaciae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acac_evaluation(t,c,1367+(i*13),1356+(i*10),1340+(i*7),1329+(i*4),2022+(i%3));}
ps("\nAcaciae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acac_accessory(t,c,1359+(i*11),1350+(i*9),1336+(i*6),1326+(i*3),2023+(i%2));}
ps("\nAcaciae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acac_market(t,c,1353+(i*9),1344+(i*7),1331+(i*5),1323+(i*3),2024);}
ps("\n");acac_report();acac_state();ps("\n=== Demo Complete ===\n");return 0;}
