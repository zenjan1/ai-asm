/* scoparia_admin: Scoparia management technology administration (v1.0)
 * Scoparia planning, scoparia execution, scoparia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sco_t;
typedef struct{int n_scop,n_scoe,n_sco2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sco_state_t;
static sco_t scops[N],scoss[N-2],scovss[N-4],scoas[N-6],scomks[N-6]; static sco_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sco_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sco_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SCO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sco_init(void){if(init)return -1;st.n_scop=0;st.n_scoe=0;st.n_sco2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)scops[i].active=0;for(int i=0;i<N-2;i++)scoss[i].active=0;for(int i=0;i<N-4;i++)scovss[i].active=0;for(int i=0;i<N-6;i++)scoas[i].active=0;for(int i=0;i<N-6;i++)scomks[i].active=0;init=1;ps("[SCO] Scoparia initialized\n");return 0;}
int sco_planning(int t,int c,int a,int b,int d,int e,int y){return add(scops,&st.n_scop,&st.t_f1,N,t,c,a,b,d,e,y);}
int sco_execution(int t,int c,int a,int b,int d,int e,int y){return add(scoss,&st.n_scoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sco_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(scovss,&st.n_sco2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sco_accessory(int t,int c,int a,int b,int d,int e,int y){return add(scoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sco_market(int t,int c,int a,int b,int d,int e,int y){return add(scomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sco_report(void){ps("[SCO] Scopp: ");pi(st.n_scop);ps(" PCS=");pi(st.t_f1);ps("\nScoe: ");pi(st.n_scoe);ps(" PCS=");pi(st.t_f2);ps("\nScov: ");pi(st.n_sco2);ps(" PCS=");pi(st.t_f3);ps("\nScoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sco_state(void){ps("[SCO] Scopp=");pi(st.n_scop);ps(" Scoe=");pi(st.n_scoe);ps(" Scov=");pi(st.n_sco2);ps(" Scoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Scoparia Admin Demo ===\n\n");sco_init();
ps("Scoparia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sco_planning(t,c,1188+(i*17),1177+(i*14),1157+(i*10),1139+(i*6),2020+(i%5));}
ps("\nScoparia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sco_execution(t,c,1177+(i*15),1166+(i*12),1148+(i*8),1135+(i*5),2021+(i%4));}
ps("\nScoparia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sco_evaluation(t,c,1169+(i*13),1158+(i*10),1142+(i*7),1131+(i*4),2022+(i%3));}
ps("\nScoparia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sco_accessory(t,c,1161+(i*11),1152+(i*9),1138+(i*6),1128+(i*3),2023+(i%2));}
ps("\nScoparia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sco_market(t,c,1155+(i*9),1146+(i*7),1133+(i*5),1125+(i*3),2024);}
ps("\n");sco_report();sco_state();ps("\n=== Demo Complete ===\n");return 0;}
