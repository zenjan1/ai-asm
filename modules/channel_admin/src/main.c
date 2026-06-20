/* channel_admin: Channel management technology administration (v1.0)
 * Channel planning, channel operations, channel evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chx_t;
typedef struct{int n_cp,n_co,n_ce,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chx_state_t;
static chx_t cps[N],cos[N-2],ces[N-4],acs[N-6],mks[N-6]; static chx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(chx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chx_init(void){if(init)return -1;st.n_cp=0;st.n_co=0;st.n_ce=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cps[i].active=0;for(int i=0;i<N-2;i++)cos[i].active=0;for(int i=0;i<N-4;i++)ces[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CHX] Channel initialized\n");return 0;}
int chx_planning(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f1,N,t,c,a,b,d,e,y);}
int chx_operations(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chx_report(void){ps("[CHX] Cp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f1);ps("\nCo: ");pi(st.n_co);ps(" PCS=");pi(st.t_f2);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chx_state(void){ps("[CHX] Cp=");pi(st.n_cp);ps(" Co=");pi(st.n_co);ps(" Ce=");pi(st.n_ce);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Channel Admin Demo ===\n\n");chx_init();
ps("Channel planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chx_planning(t,c,462+(i*17),451+(i*14),431+(i*10),413+(i*6),2020+(i%5));}
ps("\nChannel operations...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chx_operations(t,c,451+(i*15),440+(i*12),422+(i*8),409+(i*5),2021+(i%4));}
ps("\nChannel evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chx_evaluation(t,c,443+(i*13),432+(i*10),416+(i*7),405+(i*4),2022+(i%3));}
ps("\nChannel accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chx_accessory(t,c,435+(i*11),426+(i*9),412+(i*6),402+(i*3),2023+(i%2));}
ps("\nChannel marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chx_market(t,c,429+(i*9),420+(i*7),407+(i*5),399+(i*3),2024);}
ps("\n");chx_report();chx_state();ps("\n=== Demo Complete ===\n");return 0;}
