/* hunnemania_admin: Hunnemania management technology administration (v1.0)
 * Hunnemania planning, hunnemania execution, hunnemania evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hunn_t;
typedef struct{int n_hunnp,n_hunn,n_hunn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hunn_state_t;
static hunn_t hunns[N],hunne[N-2],hunn2[N-4],hunnac[N-6],hunnam[N-6]; static hunn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hunn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hunn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUNN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hunn_init(void){if(init)return -1;st.n_hunnp=0;st.n_hunn=0;st.n_hunn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hunns[i].active=0;for(int i=0;i<N-2;i++)hunne[i].active=0;for(int i=0;i<N-4;i++)hunn2[i].active=0;for(int i=0;i<N-6;i++)hunnac[i].active=0;for(int i=0;i<N-6;i++)hunnam[i].active=0;init=1;ps("[HUNN] Hunnemania initialized\n");return 0;}
int hunn_planning(int t,int c,int a,int b,int d,int e,int y){return add(hunns,&st.n_hunnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hunn_execution(int t,int c,int a,int b,int d,int e,int y){return add(hunne,&st.n_hunn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hunn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hunn2,&st.n_hunn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hunn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hunnac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hunn_market(int t,int c,int a,int b,int d,int e,int y){return add(hunnam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hunn_report(void){ps("[HUNN] Hunnp: ");pi(st.n_hunnp);ps(" PCS=");pi(st.t_f1);ps("\nHunne: ");pi(st.n_hunn);ps(" PCS=");pi(st.t_f2);ps("\nHunn2: ");pi(st.n_hunn2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hunn_state(void){ps("[HUNN] Hunnp=");pi(st.n_hunnp);ps(" Hunn=");pi(st.n_hunn);ps(" Hunn2=");pi(st.n_hunn2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hunnemania Admin Demo ===\n\n");hunn_init();
ps("Hunnemania planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hunn_planning(t,c,1537+(i*17),1526+(i*14),1506+(i*10),1488+(i*6),2020+(i%5));}
ps("\nHunnemania execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hunn_execution(t,c,1526+(i*15),1515+(i*12),1497+(i*8),1484+(i*5),2021+(i%4));}
ps("\nHunnemania evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hunn_evaluation(t,c,1518+(i*13),1507+(i*10),1491+(i*7),1480+(i*4),2022+(i%3));}
ps("\nHunnemania accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hunn_accessory(t,c,1510+(i*11),1501+(i*9),1487+(i*6),1477+(i*3),2023+(i%2));}
ps("\nHunnemania marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hunn_market(t,c,1504+(i*9),1495+(i*7),1482+(i*5),1474+(i*3),2024);}
ps("\n");hunn_report();hunn_state();ps("\n=== Demo Complete ===\n");return 0;}
