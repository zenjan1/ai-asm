/* network_admin: Network management technology administration (v1.0)
 * Network planning, network execution, network evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ntx_t;
typedef struct{int n_np,n_ne,n_nv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ntx_state_t;
static ntx_t nps[N],nes[N-2],nvs[N-4],acs[N-6],mks[N-6]; static ntx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ntx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ntx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NTX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ntx_init(void){if(init)return -1;st.n_np=0;st.n_ne=0;st.n_nv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nps[i].active=0;for(int i=0;i<N-2;i++)nes[i].active=0;for(int i=0;i<N-4;i++)nvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NTX] Network initialized\n");return 0;}
int ntx_planning(int t,int c,int a,int b,int d,int e,int y){return add(nps,&st.n_np,&st.t_f1,N,t,c,a,b,d,e,y);}
int ntx_execution(int t,int c,int a,int b,int d,int e,int y){return add(nes,&st.n_ne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ntx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nvs,&st.n_nv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ntx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ntx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ntx_report(void){ps("[NTX] Np: ");pi(st.n_np);ps(" PCS=");pi(st.t_f1);ps("\nNe: ");pi(st.n_ne);ps(" PCS=");pi(st.t_f2);ps("\nNv: ");pi(st.n_nv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ntx_state(void){ps("[NTX] Np=");pi(st.n_np);ps(" Ne=");pi(st.n_ne);ps(" Nv=");pi(st.n_nv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Network Admin Demo ===\n\n");ntx_init();
ps("Network planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ntx_planning(t,c,483+(i*17),472+(i*14),452+(i*10),434+(i*6),2020+(i%5));}
ps("\nNetwork execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ntx_execution(t,c,472+(i*15),461+(i*12),443+(i*8),430+(i*5),2021+(i%4));}
ps("\nNetwork evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ntx_evaluation(t,c,464+(i*13),453+(i*10),437+(i*7),426+(i*4),2022+(i%3));}
ps("\nNetwork accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntx_accessory(t,c,456+(i*11),447+(i*9),433+(i*6),423+(i*3),2023+(i%2));}
ps("\nNetwork marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ntx_market(t,c,450+(i*9),441+(i*7),428+(i*5),420+(i*3),2024);}
ps("\n");ntx_report();ntx_state();ps("\n=== Demo Complete ===\n");return 0;}
