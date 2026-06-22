/* baeckea_admin: Baeckea management technology administration (v1.0)
 * Baeckea planning, baeckea execution, baeckea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} baec_t;
typedef struct{int n_baecp,n_baec,n_baec2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} baec_state_t;
static baec_t baecs[N],baece[N-2],baec2[N-4],baecac[N-6],baecam[N-6]; static baec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(baec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;baec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int baec_init(void){if(init)return -1;st.n_baecp=0;st.n_baec=0;st.n_baec2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)baecs[i].active=0;for(int i=0;i<N-2;i++)baece[i].active=0;for(int i=0;i<N-4;i++)baec2[i].active=0;for(int i=0;i<N-6;i++)baecac[i].active=0;for(int i=0;i<N-6;i++)baecam[i].active=0;init=1;ps("[BAEC] Baeckea initialized\n");return 0;}
int baec_planning(int t,int c,int a,int b,int d,int e,int y){return add(baecs,&st.n_baecp,&st.t_f1,N,t,c,a,b,d,e,y);}
int baec_execution(int t,int c,int a,int b,int d,int e,int y){return add(baece,&st.n_baec,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int baec_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(baec2,&st.n_baec2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int baec_accessory(int t,int c,int a,int b,int d,int e,int y){return add(baecac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int baec_market(int t,int c,int a,int b,int d,int e,int y){return add(baecam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void baec_report(void){ps("[BAEC] Ecp: ");pi(st.n_baecp);ps(" PCS=");pi(st.t_f1);ps("\nEce: ");pi(st.n_baec);ps(" PCS=");pi(st.t_f2);ps("\nEc2: ");pi(st.n_baec2);ps(" PCS=");pi(st.t_f3);ps("\nEcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void baec_state(void){ps("[BAEC] Ecp=");pi(st.n_baecp);ps(" Ece=");pi(st.n_baec);ps(" Ec2=");pi(st.n_baec2);ps(" Ecac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Baeckea Admin Demo ===\n\n");baec_init();
ps("Baeckea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;baec_planning(t,c,1429+(i*17),1418+(i*14),1398+(i*10),1380+(i*6),2020+(i%5));}
ps("\nBaeckea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;baec_execution(t,c,1418+(i*15),1407+(i*12),1389+(i*8),1376+(i*5),2021+(i%4));}
ps("\nBaeckea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;baec_evaluation(t,c,1410+(i*13),1399+(i*10),1383+(i*7),1372+(i*4),2022+(i%3));}
ps("\nBaeckea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;baec_accessory(t,c,1402+(i*11),1393+(i*9),1379+(i*6),1369+(i*3),2023+(i%2));}
ps("\nBaeckea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;baec_market(t,c,1396+(i*9),1387+(i*7),1374+(i*5),1366+(i*3),2024);}
ps("\n");baec_report();baec_state();ps("\n=== Demo Complete ===\n");return 0;}
