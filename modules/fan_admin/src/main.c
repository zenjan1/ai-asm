/* fan_admin: Fan technology administration (v1.0)
 * Axial fan, centrifugal fan, mixed flow fan, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fan_t;
typedef struct{int n_ax,n_cf,n_mx,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fan_state_t;
static fan_t axs[N],cfs[N-2],mxs[N-4],acs[N-6],mks[N-6]; static fan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fan_init(void){if(init)return -1;st.n_ax=0;st.n_cf=0;st.n_mx=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)axs[i].active=0;for(int i=0;i<N-2;i++)cfs[i].active=0;for(int i=0;i<N-4;i++)mxs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FAN] Fan initialized\n");return 0;}
int fan_axial(int t,int c,int a,int b,int d,int e,int y){return add(axs,&st.n_ax,&st.t_f1,N,t,c,a,b,d,e,y);}
int fan_centrifugal(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fan_mixed(int t,int c,int a,int b,int d,int e,int y){return add(mxs,&st.n_mx,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fan_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fan_report(void){ps("[FAN] Ax: ");pi(st.n_ax);ps(" PCS=");pi(st.t_f1);ps("\nCf: ");pi(st.n_cf);ps(" PCS=");pi(st.t_f2);ps("\nMx: ");pi(st.n_mx);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fan_state(void){ps("[FAN] Ax=");pi(st.n_ax);ps(" Cf=");pi(st.n_cf);ps(" Mx=");pi(st.n_mx);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fan Admin Demo ===\n\n");fan_init();
ps("Axial fans...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fan_axial(t,c,370+(i*17),355+(i*14),335+(i*10),317+(i*6),2020+(i%5));}
ps("\nCentrifugal fans...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fan_centrifugal(t,c,359+(i*15),345+(i*12),327+(i*8),314+(i*5),2021+(i%4));}
ps("\nMixed flow fans...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fan_mixed(t,c,351+(i*13),337+(i*10),321+(i*7),310+(i*4),2022+(i%3));}
ps("\nFan accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fan_accessory(t,c,343+(i*11),331+(i*9),317+(i*6),307+(i*3),2023+(i%2));}
ps("\nFan marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fan_market(t,c,337+(i*9),326+(i*7),313+(i*5),305+(i*3),2024);}
ps("\n");fan_report();fan_state();ps("\n=== Demo Complete ===\n");return 0;}
