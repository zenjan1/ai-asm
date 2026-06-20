/* collaboration_admin: Collaboration management technology administration (v1.0)
 * Collaboration planning, collaboration execution, collaboration optimization, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clx_t;
typedef struct{int n_cpl,n_ce,n_cop,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} clx_state_t;
static clx_t cpls[N],ces[N-2],cops[N-4],acs[N-6],mks[N-6]; static clx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clx_init(void){if(init)return -1;st.n_cpl=0;st.n_ce=0;st.n_cop=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cpls[i].active=0;for(int i=0;i<N-2;i++)ces[i].active=0;for(int i=0;i<N-4;i++)cops[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CLX] Collaboration initialized\n");return 0;}
int clx_planning(int t,int c,int a,int b,int d,int e,int y){return add(cpls,&st.n_cpl,&st.t_f1,N,t,c,a,b,d,e,y);}
int clx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clx_optimization(int t,int c,int a,int b,int d,int e,int y){return add(cops,&st.n_cop,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clx_report(void){ps("[CLX] Cpl: ");pi(st.n_cpl);ps(" PCS=");pi(st.t_f1);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f2);ps("\nCop: ");pi(st.n_cop);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clx_state(void){ps("[CLX] Cpl=");pi(st.n_cpl);ps(" Ce=");pi(st.n_ce);ps(" Cop=");pi(st.n_cop);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Collaboration Admin Demo ===\n\n");clx_init();
ps("Collaboration planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clx_planning(t,c,474+(i*17),463+(i*14),443+(i*10),425+(i*6),2020+(i%5));}
ps("\nCollaboration execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clx_execution(t,c,463+(i*15),452+(i*12),434+(i*8),421+(i*5),2021+(i%4));}
ps("\nCollaboration optimization...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clx_optimization(t,c,455+(i*13),444+(i*10),428+(i*7),417+(i*4),2022+(i%3));}
ps("\nCollaboration accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clx_accessory(t,c,447+(i*11),438+(i*9),424+(i*6),414+(i*3),2023+(i%2));}
ps("\nCollaboration marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clx_market(t,c,441+(i*9),432+(i*7),419+(i*5),411+(i*3),2024);}
ps("\n");clx_report();clx_state();ps("\n=== Demo Complete ===\n");return 0;}
