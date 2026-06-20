/* cam_admin: CAM technology administration (v1.0)
 * CNC programming CAM, CAPP process planning, manufacturing simulation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cmx_t;
typedef struct{int n_np,n_cp,n_sm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cmx_state_t;
static cmx_t nps[N],cps[N-2],sms[N-4],acs[N-6],mks[N-6]; static cmx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cmx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cmx_init(void){if(init)return -1;st.n_np=0;st.n_cp=0;st.n_sm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nps[i].active=0;for(int i=0;i<N-2;i++)cps[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CMX] CAM initialized\n");return 0;}
int cmx_programming(int t,int c,int a,int b,int d,int e,int y){return add(nps,&st.n_np,&st.t_f1,N,t,c,a,b,d,e,y);}
int cmx_process(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cmx_simulation(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cmx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cmx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cmx_report(void){ps("[CMX] Np: ");pi(st.n_np);ps(" PCS=");pi(st.t_f1);ps("\nCp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f2);ps("\nSm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cmx_state(void){ps("[CMX] Np=");pi(st.n_np);ps(" Cp=");pi(st.n_cp);ps(" Sm=");pi(st.n_sm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== CAM Admin Demo ===\n\n");cmx_init();
ps("CNC programming...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cmx_programming(t,c,426+(i*17),415+(i*14),395+(i*10),377+(i*6),2020+(i%5));}
ps("\nProcess planning...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cmx_process(t,c,415+(i*15),404+(i*12),386+(i*8),373+(i*5),2021+(i%4));}
ps("\nManufacturing simulation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cmx_simulation(t,c,407+(i*13),396+(i*10),380+(i*7),369+(i*4),2022+(i%3));}
ps("\nCAM accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmx_accessory(t,c,399+(i*11),390+(i*9),376+(i*6),366+(i*3),2023+(i%2));}
ps("\nCAM marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmx_market(t,c,393+(i*9),384+(i*7),371+(i*5),363+(i*3),2024);}
ps("\n");cmx_report();cmx_state();ps("\n=== Demo Complete ===\n");return 0;}
