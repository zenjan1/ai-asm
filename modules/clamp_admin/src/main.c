/* clamp_admin: Clamp technology administration (v1.0)
 * Machine clamp, welding clamp, assembly clamp, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clp_t;
typedef struct{int n_mc,n_wd,n_as,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} clp_state_t;
static clp_t mcs[N],wds[N-2],ass[N-4],acs[N-6],mks[N-6]; static clp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clp_init(void){if(init)return -1;st.n_mc=0;st.n_wd=0;st.n_as=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)wds[i].active=0;for(int i=0;i<N-4;i++)ass[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CLP] Clamp initialized\n");return 0;}
int clp_machine(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int clp_welding(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clp_assembly(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clp_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clp_report(void){ps("[CLP] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f2);ps("\nAs: ");pi(st.n_as);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clp_state(void){ps("[CLP] Mc=");pi(st.n_mc);ps(" Wd=");pi(st.n_wd);ps(" As=");pi(st.n_as);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clamp Admin Demo ===\n\n");clp_init();
ps("Machine clamps...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clp_machine(t,c,384+(i*17),369+(i*14),349+(i*10),331+(i*6),2020+(i%5));}
ps("\nWelding clamps...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clp_welding(t,c,373+(i*15),359+(i*12),341+(i*8),328+(i*5),2021+(i%4));}
ps("\nAssembly clamps...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clp_assembly(t,c,365+(i*13),351+(i*10),335+(i*7),324+(i*4),2022+(i%3));}
ps("\nClamp accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clp_accessory(t,c,357+(i*11),345+(i*9),331+(i*6),321+(i*3),2023+(i%2));}
ps("\nClamp marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clp_market(t,c,351+(i*9),340+(i*7),327+(i*5),319+(i*3),2024);}
ps("\n");clp_report();clp_state();ps("\n=== Demo Complete ===\n");return 0;}
