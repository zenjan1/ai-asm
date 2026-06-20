/* instrumentech_admin: Instrument technology administration (v1.0)
 * Measuring instruments, control instruments, actuators, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ist_t;
typedef struct{int n_ms,n_ct,n_ac,n_ax,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ist_state_t;
static ist_t mss[N],cts[N-2],acs[N-4],axs[N-6],mks[N-6]; static ist_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ist_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ist_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ist_init(void){if(init)return -1;st.n_ms=0;st.n_ct=0;st.n_ac=0;st.n_ax=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mss[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)acs[i].active=0;for(int i=0;i<N-6;i++)axs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IST] Instrumentech initialized\n");return 0;}
int ist_measuring(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f1,N,t,c,a,b,d,e,y);}
int ist_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ist_actuator(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ist_accessory(int t,int c,int a,int b,int d,int e,int y){return add(axs,&st.n_ax,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ist_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ist_report(void){ps("[IST] Ms: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f3);ps("\nAx: ");pi(st.n_ax);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ist_state(void){ps("[IST] Ms=");pi(st.n_ms);ps(" Ct=");pi(st.n_ct);ps(" Ac=");pi(st.n_ac);ps(" Ax=");pi(st.n_ax);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Instrument Tech Admin Demo ===\n\n");ist_init();
ps("Measuring instruments...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ist_measuring(t,c,306+(i*17),291+(i*14),271+(i*10),253+(i*6),2020+(i%5));}
ps("\nControl instruments...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ist_control(t,c,295+(i*15),281+(i*12),263+(i*8),250+(i*5),2021+(i%4));}
ps("\nActuators...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ist_actuator(t,c,287+(i*13),273+(i*10),257+(i*7),246+(i*4),2022+(i%3));}
ps("\nInstrument accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ist_accessory(t,c,279+(i*11),267+(i*9),253+(i*6),243+(i*3),2023+(i%2));}
ps("\nInstrument marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ist_market(t,c,273+(i*9),262+(i*7),249+(i*5),241+(i*3),2024);}
ps("\n");ist_report();ist_state();ps("\n=== Demo Complete ===\n");return 0;}
