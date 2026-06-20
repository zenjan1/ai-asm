/* couplingtech_admin: Coupling technology administration (v1.0)
 * Rigid coupling, flexible coupling, elastic coupling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cpl_t;
typedef struct{int n_rg,n_fx,n_el,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cpl_state_t;
static cpl_t rgs[N],fxs[N-2],els[N-4],acs[N-6],mks[N-6]; static cpl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cpl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cpl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpl_init(void){if(init)return -1;st.n_rg=0;st.n_fx=0;st.n_el=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rgs[i].active=0;for(int i=0;i<N-2;i++)fxs[i].active=0;for(int i=0;i<N-4;i++)els[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CPL] Couplingtech initialized\n");return 0;}
int cpl_rigid(int t,int c,int a,int b,int d,int e,int y){return add(rgs,&st.n_rg,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpl_flexible(int t,int c,int a,int b,int d,int e,int y){return add(fxs,&st.n_fx,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpl_elastic(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpl_report(void){ps("[CPL] Rg: ");pi(st.n_rg);ps(" PCS=");pi(st.t_f1);ps("\nFx: ");pi(st.n_fx);ps(" PCS=");pi(st.t_f2);ps("\nEl: ");pi(st.n_el);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cpl_state(void){ps("[CPL] Rg=");pi(st.n_rg);ps(" Fx=");pi(st.n_fx);ps(" El=");pi(st.n_el);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coupling Tech Admin Demo ===\n\n");cpl_init();
ps("Rigid couplings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpl_rigid(t,c,347+(i*17),332+(i*14),312+(i*10),294+(i*6),2020+(i%5));}
ps("\nFlexible couplings...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpl_flexible(t,c,336+(i*15),322+(i*12),304+(i*8),291+(i*5),2021+(i%4));}
ps("\nElastic couplings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpl_elastic(t,c,328+(i*13),314+(i*10),298+(i*7),287+(i*4),2022+(i%3));}
ps("\nCoupling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpl_accessory(t,c,320+(i*11),308+(i*9),294+(i*6),284+(i*3),2023+(i%2));}
ps("\nCoupling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpl_market(t,c,314+(i*9),303+(i*7),290+(i*5),282+(i*3),2024);}
ps("\n");cpl_report();cpl_state();ps("\n=== Demo Complete ===\n");return 0;}
