/* onagra_admin: Onagra management technology administration (v1.0)
 * Onagra planning, onagra execution, onagra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ona_t;
typedef struct{int n_onap,n_onae,n_ona2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ona_state_t;
static ona_t onaps[N],onaess[N-2],onavss[N-4],onas[N-6],onamks[N-6]; static ona_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ona_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ona_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ONA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ona_init(void){if(init)return -1;st.n_onap=0;st.n_onae=0;st.n_ona2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)onaps[i].active=0;for(int i=0;i<N-2;i++)onaess[i].active=0;for(int i=0;i<N-4;i++)onavss[i].active=0;for(int i=0;i<N-6;i++)onas[i].active=0;for(int i=0;i<N-6;i++)onamks[i].active=0;init=1;ps("[ONA] Onagra initialized\n");return 0;}
int ona_planning(int t,int c,int a,int b,int d,int e,int y){return add(onaps,&st.n_onap,&st.t_f1,N,t,c,a,b,d,e,y);}
int ona_execution(int t,int c,int a,int b,int d,int e,int y){return add(onaess,&st.n_onae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ona_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(onavss,&st.n_ona2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ona_accessory(int t,int c,int a,int b,int d,int e,int y){return add(onas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ona_market(int t,int c,int a,int b,int d,int e,int y){return add(onamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ona_report(void){ps("[ONA] Onap: ");pi(st.n_onap);ps(" PCS=");pi(st.t_f1);ps("\nOnae: ");pi(st.n_onae);ps(" PCS=");pi(st.t_f2);ps("\nOnav: ");pi(st.n_ona2);ps(" PCS=");pi(st.t_f3);ps("\nOnac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ona_state(void){ps("[ONA] Onap=");pi(st.n_onap);ps(" Onae=");pi(st.n_onae);ps(" Onav=");pi(st.n_ona2);ps(" Onac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Onagra Admin Demo ===\n\n");ona_init();
ps("Onagra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ona_planning(t,c,1323+(i*17),1312+(i*14),1292+(i*10),1274+(i*6),2020+(i%5));}
ps("\nOnagra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ona_execution(t,c,1312+(i*15),1301+(i*12),1283+(i*8),1270+(i*5),2021+(i%4));}
ps("\nOnagra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ona_evaluation(t,c,1304+(i*13),1293+(i*10),1277+(i*7),1266+(i*4),2022+(i%3));}
ps("\nOnagra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ona_accessory(t,c,1296+(i*11),1287+(i*9),1273+(i*6),1263+(i*3),2023+(i%2));}
ps("\nOnagra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ona_market(t,c,1290+(i*9),1281+(i*7),1268+(i*5),1260+(i*3),2024);}
ps("\n");ona_report();ona_state();ps("\n=== Demo Complete ===\n");return 0;}
