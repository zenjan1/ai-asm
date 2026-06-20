/* capability_admin: Capability management technology administration (v1.0)
 * Capability assessment, capability development, capability application, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cpb_t;
typedef struct{int n_ca,n_cd,n_cap,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cpb_state_t;
static cpb_t cas[N],cds[N-2],caps[N-4],acs[N-6],mks[N-6]; static cpb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cpb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cpb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpb_init(void){if(init)return -1;st.n_ca=0;st.n_cd=0;st.n_cap=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cas[i].active=0;for(int i=0;i<N-2;i++)cds[i].active=0;for(int i=0;i<N-4;i++)caps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CPB] Capability initialized\n");return 0;}
int cpb_assessment(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpb_development(int t,int c,int a,int b,int d,int e,int y){return add(cds,&st.n_cd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpb_application(int t,int c,int a,int b,int d,int e,int y){return add(caps,&st.n_cap,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpb_report(void){ps("[CPB] Ca: ");pi(st.n_ca);ps(" PCS=");pi(st.t_f1);ps("\nCd: ");pi(st.n_cd);ps(" PCS=");pi(st.t_f2);ps("\nCap: ");pi(st.n_cap);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cpb_state(void){ps("[CPB] Ca=");pi(st.n_ca);ps(" Cd=");pi(st.n_cd);ps(" Cap=");pi(st.n_cap);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Capability Admin Demo ===\n\n");cpb_init();
ps("Capability assessment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpb_assessment(t,c,457+(i*17),446+(i*14),426+(i*10),408+(i*6),2020+(i%5));}
ps("\nCapability development...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpb_development(t,c,446+(i*15),435+(i*12),417+(i*8),404+(i*5),2021+(i%4));}
ps("\nCapability application...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpb_application(t,c,438+(i*13),427+(i*10),411+(i*7),400+(i*4),2022+(i%3));}
ps("\nCapability accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpb_accessory(t,c,430+(i*11),421+(i*9),407+(i*6),397+(i*3),2023+(i%2));}
ps("\nCapability marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpb_market(t,c,424+(i*9),415+(i*7),402+(i*5),394+(i*3),2024);}
ps("\n");cpb_report();cpb_state();ps("\n=== Demo Complete ===\n");return 0;}
