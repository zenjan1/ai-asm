/* pneumatech_admin: Pneumatic technology administration (v1.0)
 * Air source, actuators, control valves, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pnu_t;
typedef struct{int n_as,n_ac,n_cv,n_ax,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pnu_state_t;
static pnu_t ass[N],acs[N-2],cvs[N-4],axs[N-6],mks[N-6]; static pnu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pnu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pnu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PNU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pnu_init(void){if(init)return -1;st.n_as=0;st.n_ac=0;st.n_cv=0;st.n_ax=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ass[i].active=0;for(int i=0;i<N-2;i++)acs[i].active=0;for(int i=0;i<N-4;i++)cvs[i].active=0;for(int i=0;i<N-6;i++)axs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PNU] Pneumatech initialized\n");return 0;}
int pnu_airsource(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f1,N,t,c,a,b,d,e,y);}
int pnu_actuator(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pnu_valve(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pnu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(axs,&st.n_ax,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pnu_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pnu_report(void){ps("[PNU] As: ");pi(st.n_as);ps(" PCS=");pi(st.t_f1);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f2);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f3);ps("\nAx: ");pi(st.n_ax);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pnu_state(void){ps("[PNU] As=");pi(st.n_as);ps(" Ac=");pi(st.n_ac);ps(" Cv=");pi(st.n_cv);ps(" Ax=");pi(st.n_ax);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pneumatic Tech Admin Demo ===\n\n");pnu_init();
ps("Air source...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pnu_airsource(t,c,298+(i*17),283+(i*14),263+(i*10),245+(i*6),2020+(i%5));}
ps("\nActuators...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pnu_actuator(t,c,287+(i*15),273+(i*12),255+(i*8),242+(i*5),2021+(i%4));}
ps("\nControl valves...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pnu_valve(t,c,279+(i*13),265+(i*10),249+(i*7),238+(i*4),2022+(i%3));}
ps("\nPneumatic accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnu_accessory(t,c,271+(i*11),259+(i*9),245+(i*6),235+(i*3),2023+(i%2));}
ps("\nPneumatic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pnu_market(t,c,265+(i*9),254+(i*7),241+(i*5),233+(i*3),2024);}
ps("\n");pnu_report();pnu_state();ps("\n=== Demo Complete ===\n");return 0;}
