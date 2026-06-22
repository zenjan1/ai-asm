/* darlingtonia_admin: Darlingtonia management technology administration (v1.0)
 * Darlingtonia planning, darlingtonia execution, darlingtonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} darl_t;
typedef struct{int n_darlp,n_darl,n_darl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} darl_state_t;
static darl_t darls[N],darle[N-2],darl2[N-4],darlac[N-6],darlam[N-6]; static darl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(darl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;darl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DARL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int darl_init(void){if(init)return -1;st.n_darlp=0;st.n_darl=0;st.n_darl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)darls[i].active=0;for(int i=0;i<N-2;i++)darle[i].active=0;for(int i=0;i<N-4;i++)darl2[i].active=0;for(int i=0;i<N-6;i++)darlac[i].active=0;for(int i=0;i<N-6;i++)darlam[i].active=0;init=1;ps("[DARL] Darlingtonia initialized\n");return 0;}
int darl_planning(int t,int c,int a,int b,int d,int e,int y){return add(darls,&st.n_darlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int darl_execution(int t,int c,int a,int b,int d,int e,int y){return add(darle,&st.n_darl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int darl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(darl2,&st.n_darl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int darl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(darlac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int darl_market(int t,int c,int a,int b,int d,int e,int y){return add(darlam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void darl_report(void){ps("[DARL] Rlp: ");pi(st.n_darlp);ps(" PCS=");pi(st.t_f1);ps("\nRle: ");pi(st.n_darl);ps(" PCS=");pi(st.t_f2);ps("\nRl2: ");pi(st.n_darl2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void darl_state(void){ps("[DARL] Rlp=");pi(st.n_darlp);ps(" Rl=");pi(st.n_darl);ps(" Rl2=");pi(st.n_darl2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Darlingtonia Admin Demo ===\n\n");darl_init();
ps("Darlingtonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;darl_planning(t,c,1489+(i*17),1478+(i*14),1458+(i*10),1440+(i*6),2020+(i%5));}
ps("\nDarlingtonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;darl_execution(t,c,1478+(i*15),1467+(i*12),1449+(i*8),1436+(i*5),2021+(i%4));}
ps("\nDarlingtonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;darl_evaluation(t,c,1470+(i*13),1459+(i*10),1443+(i*7),1432+(i*4),2022+(i%3));}
ps("\nDarlingtonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;darl_accessory(t,c,1462+(i*11),1453+(i*9),1439+(i*6),1429+(i*3),2023+(i%2));}
ps("\nDarlingtonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;darl_market(t,c,1456+(i*9),1447+(i*7),1434+(i*5),1426+(i*3),2024);}
ps("\n");darl_report();darl_state();ps("\n=== Demo Complete ===\n");return 0;}
