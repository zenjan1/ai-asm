/* daphne_admin: Daphne management technology administration (v1.0)
 * Daphne planning, daphne execution, daphne evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dap_t;
typedef struct{int n_dapp,n_dape,n_dap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dap_state_t;
static dap_t dapps[N],dapss[N-2],dapvss[N-4],dapas[N-6],dapmks[N-6]; static dap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dap_init(void){if(init)return -1;st.n_dapp=0;st.n_dape=0;st.n_dap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dapps[i].active=0;for(int i=0;i<N-2;i++)dapss[i].active=0;for(int i=0;i<N-4;i++)dapvss[i].active=0;for(int i=0;i<N-6;i++)dapas[i].active=0;for(int i=0;i<N-6;i++)dapmks[i].active=0;init=1;ps("[DAP] Daphne initialized\n");return 0;}
int dap_planning(int t,int c,int a,int b,int d,int e,int y){return add(dapps,&st.n_dapp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dap_execution(int t,int c,int a,int b,int d,int e,int y){return add(dapss,&st.n_dape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dapvss,&st.n_dap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dapas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dap_market(int t,int c,int a,int b,int d,int e,int y){return add(dapmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dap_report(void){ps("[DAP] Dapp: ");pi(st.n_dapp);ps(" PCS=");pi(st.t_f1);ps("\nDape: ");pi(st.n_dape);ps(" PCS=");pi(st.t_f2);ps("\nDapv: ");pi(st.n_dap2);ps(" PCS=");pi(st.t_f3);ps("\nDapc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dap_state(void){ps("[DAP] Dapp=");pi(st.n_dapp);ps(" Dape=");pi(st.n_dape);ps(" Dapv=");pi(st.n_dap2);ps(" Dapc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Daphne Admin Demo ===\n\n");dap_init();
ps("Daphne planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dap_planning(t,c,915+(i*17),904+(i*14),884+(i*10),866+(i*6),2020+(i%5));}
ps("\nDaphne execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dap_execution(t,c,904+(i*15),893+(i*12),875+(i*8),862+(i*5),2021+(i%4));}
ps("\nDaphne evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dap_evaluation(t,c,896+(i*13),885+(i*10),869+(i*7),858+(i*4),2022+(i%3));}
ps("\nDaphne accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dap_accessory(t,c,888+(i*11),879+(i*9),865+(i*6),855+(i*3),2023+(i%2));}
ps("\nDaphne marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dap_market(t,c,882+(i*9),873+(i*7),860+(i*5),852+(i*3),2024);}
ps("\n");dap_report();dap_state();ps("\n=== Demo Complete ===\n");return 0;}
