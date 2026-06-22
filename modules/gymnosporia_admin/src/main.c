/* gymnosporia_admin: Gymnosporia management technology administration (v1.0)
 * Gymnosporia planning, gymnosporia execution, gymnosporia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gymn_t;
typedef struct{int n_gymnp,n_gymn,n_gymn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gymn_state_t;
static gymn_t gymnss[N],gymne[N-2],gymn2[N-4],gymnac[N-6],gymnam[N-6]; static gymn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gymn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gymn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GYMN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gymn_init(void){if(init)return -1;st.n_gymnp=0;st.n_gymn=0;st.n_gymn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gymnss[i].active=0;for(int i=0;i<N-2;i++)gymne[i].active=0;for(int i=0;i<N-4;i++)gymn2[i].active=0;for(int i=0;i<N-6;i++)gymnac[i].active=0;for(int i=0;i<N-6;i++)gymnam[i].active=0;init=1;ps("[GYMN] Gymnosporia initialized\n");return 0;}
int gymn_planning(int t,int c,int a,int b,int d,int e,int y){return add(gymnss,&st.n_gymnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gymn_execution(int t,int c,int a,int b,int d,int e,int y){return add(gymne,&st.n_gymn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gymn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gymn2,&st.n_gymn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gymn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(gymnac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gymn_market(int t,int c,int a,int b,int d,int e,int y){return add(gymnam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gymn_report(void){ps("[GYMN] Gymnp: ");pi(st.n_gymnp);ps(" PCS=");pi(st.t_f1);ps("\nGymne: ");pi(st.n_gymn);ps(" PCS=");pi(st.t_f2);ps("\nGymn2: ");pi(st.n_gymn2);ps(" PCS=");pi(st.t_f3);ps("\nGymac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gymn_state(void){ps("[GYMN] Gymnp=");pi(st.n_gymnp);ps(" Gymn=");pi(st.n_gymn);ps(" Gymn2=");pi(st.n_gymn2);ps(" Gymac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gymnosporia Admin Demo ===\n\n");gymn_init();
ps("Gymnosporia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gymn_planning(t,c,1523+(i*17),1512+(i*14),1492+(i*10),1474+(i*6),2020+(i%5));}
ps("\nGymnosporia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gymn_execution(t,c,1512+(i*15),1501+(i*12),1483+(i*8),1470+(i*5),2021+(i%4));}
ps("\nGymnosporia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gymn_evaluation(t,c,1504+(i*13),1493+(i*10),1477+(i*7),1466+(i*4),2022+(i%3));}
ps("\nGymnosporia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gymn_accessory(t,c,1496+(i*11),1487+(i*9),1473+(i*6),1463+(i*3),2023+(i%2));}
ps("\nGymnosporia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gymn_market(t,c,1490+(i*9),1481+(i*7),1468+(i*5),1460+(i*3),2024);}
ps("\n");gymn_report();gymn_state();ps("\n=== Demo Complete ===\n");return 0;}
