/* innovation_admin: Innovation management technology administration (v1.0)
 * Innovation planning, innovation R&D, innovation management, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ivx_t;
typedef struct{int n_ip,n_ir,n_im,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ivx_state_t;
static ivx_t ips[N],irs[N-2],ims[N-4],acs[N-6],mks[N-6]; static ivx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ivx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ivx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IVX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ivx_init(void){if(init)return -1;st.n_ip=0;st.n_ir=0;st.n_im=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ips[i].active=0;for(int i=0;i<N-2;i++)irs[i].active=0;for(int i=0;i<N-4;i++)ims[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IVX] Innovation initialized\n");return 0;}
int ivx_planning(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ivx_research(int t,int c,int a,int b,int d,int e,int y){return add(irs,&st.n_ir,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ivx_management(int t,int c,int a,int b,int d,int e,int y){return add(ims,&st.n_im,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ivx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ivx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ivx_report(void){ps("[IVX] Ip: ");pi(st.n_ip);ps(" PCS=");pi(st.t_f1);ps("\nIr: ");pi(st.n_ir);ps(" PCS=");pi(st.t_f2);ps("\nIm: ");pi(st.n_im);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ivx_state(void){ps("[IVX] Ip=");pi(st.n_ip);ps(" Ir=");pi(st.n_ir);ps(" Im=");pi(st.n_im);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Innovation Admin Demo ===\n\n");ivx_init();
ps("Innovation planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ivx_planning(t,c,447+(i*17),436+(i*14),416+(i*10),398+(i*6),2020+(i%5));}
ps("\nInnovation R&D...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ivx_research(t,c,436+(i*15),425+(i*12),407+(i*8),394+(i*5),2021+(i%4));}
ps("\nInnovation management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ivx_management(t,c,428+(i*13),417+(i*10),401+(i*7),390+(i*4),2022+(i%3));}
ps("\nInnovation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ivx_accessory(t,c,420+(i*11),411+(i*9),397+(i*6),387+(i*3),2023+(i%2));}
ps("\nInnovation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ivx_market(t,c,414+(i*9),405+(i*7),392+(i*5),384+(i*3),2024);}
ps("\n");ivx_report();ivx_state();ps("\n=== Demo Complete ===\n");return 0;}
