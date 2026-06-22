/* acori_admin: Acori management technology administration (v1.0)
 * Acori planning, acori execution, acori evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acor_t;
typedef struct{int n_acorp,n_acor,n_acor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acor_state_t;
static acor_t acors[N],acore[N-2],acorv[N-4],acorac[N-6],acorm[N-6]; static acor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acor_init(void){if(init)return -1;st.n_acorp=0;st.n_acor=0;st.n_acor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acors[i].active=0;for(int i=0;i<N-2;i++)acore[i].active=0;for(int i=0;i<N-4;i++)acorv[i].active=0;for(int i=0;i<N-6;i++)acorac[i].active=0;for(int i=0;i<N-6;i++)acorm[i].active=0;init=1;ps("[ACOR] Acori initialized\n");return 0;}
int acor_planning(int t,int c,int a,int b,int d,int e,int y){return add(acors,&st.n_acorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acor_execution(int t,int c,int a,int b,int d,int e,int y){return add(acore,&st.n_acor,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acorv,&st.n_acor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acorac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acor_market(int t,int c,int a,int b,int d,int e,int y){return add(acorm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acor_report(void){ps("[ACOR] Corp: ");pi(st.n_acorp);ps(" PCS=");pi(st.t_f1);ps("\nCore: ");pi(st.n_acor);ps(" PCS=");pi(st.t_f2);ps("\nCorv: ");pi(st.n_acor2);ps(" PCS=");pi(st.t_f3);ps("\nCorac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acor_state(void){ps("[ACOR] Corp=");pi(st.n_acorp);ps(" Core=");pi(st.n_acor);ps(" Corv=");pi(st.n_acor2);ps(" Corac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acori Admin Demo ===\n\n");acor_init();
ps("Acori planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acor_planning(t,c,1391+(i*17),1380+(i*14),1360+(i*10),1342+(i*6),2020+(i%5));}
ps("\nAcori execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acor_execution(t,c,1380+(i*15),1369+(i*12),1351+(i*8),1338+(i*5),2021+(i%4));}
ps("\nAcori evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acor_evaluation(t,c,1372+(i*13),1361+(i*10),1345+(i*7),1334+(i*4),2022+(i%3));}
ps("\nAcori accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acor_accessory(t,c,1364+(i*11),1355+(i*9),1341+(i*6),1331+(i*3),2023+(i%2));}
ps("\nAcori marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acor_market(t,c,1358+(i*9),1349+(i*7),1336+(i*5),1328+(i*3),2024);}
ps("\n");acor_report();acor_state();ps("\n=== Demo Complete ===\n");return 0;}
