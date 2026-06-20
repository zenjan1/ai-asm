/* conference_admin: Conference management technology administration (v1.0)
 * Conference planning, conference execution, conference evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cfx_t;
typedef struct{int n_cpl,n_ce,n_cvl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cfx_state_t;
static cfx_t cpls[N],ces[N-2],cvls[N-4],acs[N-6],mks[N-6]; static cfx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cfx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cfx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CFX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cfx_init(void){if(init)return -1;st.n_cpl=0;st.n_ce=0;st.n_cvl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cpls[i].active=0;for(int i=0;i<N-2;i++)ces[i].active=0;for(int i=0;i<N-4;i++)cvls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CFX] Conference initialized\n");return 0;}
int cfx_planning(int t,int c,int a,int b,int d,int e,int y){return add(cpls,&st.n_cpl,&st.t_f1,N,t,c,a,b,d,e,y);}
int cfx_execution(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cfx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvls,&st.n_cvl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cfx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cfx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cfx_report(void){ps("[CFX] Cpl: ");pi(st.n_cpl);ps(" PCS=");pi(st.t_f1);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f2);ps("\nCvl: ");pi(st.n_cvl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cfx_state(void){ps("[CFX] Cpl=");pi(st.n_cpl);ps(" Ce=");pi(st.n_ce);ps(" Cvl=");pi(st.n_cvl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Conference Admin Demo ===\n\n");cfx_init();
ps("Conference planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cfx_planning(t,c,468+(i*17),457+(i*14),437+(i*10),419+(i*6),2020+(i%5));}
ps("\nConference execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cfx_execution(t,c,457+(i*15),446+(i*12),428+(i*8),415+(i*5),2021+(i%4));}
ps("\nConference evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cfx_evaluation(t,c,449+(i*13),438+(i*10),422+(i*7),411+(i*4),2022+(i%3));}
ps("\nConference accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cfx_accessory(t,c,441+(i*11),432+(i*9),418+(i*6),408+(i*3),2023+(i%2));}
ps("\nConference marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cfx_market(t,c,435+(i*9),426+(i*7),413+(i*5),405+(i*3),2024);}
ps("\n");cfx_report();cfx_state();ps("\n=== Demo Complete ===\n");return 0;}
