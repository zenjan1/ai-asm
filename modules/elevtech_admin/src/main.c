/* elevtech_admin: Elevator technology administration (v1.0)
 * Passenger elevators, cargo elevators, special elevators, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} elv_t;
typedef struct{int n_ps,n_cg,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} elv_state_t;
static elv_t pss[N],cgs[N-2],sps[N-4],acs[N-6],mks[N-6]; static elv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;elv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int elv_init(void){if(init)return -1;st.n_ps=0;st.n_cg=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pss[i].active=0;for(int i=0;i<N-2;i++)cgs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ELV] Elevtech initialized\n");return 0;}
int elv_passenger(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f1,N,t,c,a,b,d,e,y);}
int elv_cargo(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int elv_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int elv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int elv_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void elv_report(void){ps("[ELV] Ps: ");pi(st.n_ps);ps(" PCS=");pi(st.t_f1);ps("\nCg: ");pi(st.n_cg);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void elv_state(void){ps("[ELV] Ps=");pi(st.n_ps);ps(" Cg=");pi(st.n_cg);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Elevator Tech Admin Demo ===\n\n");elv_init();
ps("Passenger elevators...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;elv_passenger(t,c,250+(i*17),235+(i*14),215+(i*10),197+(i*6),2020+(i%5));}
ps("\nCargo elevators...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;elv_cargo(t,c,239+(i*15),225+(i*12),207+(i*8),194+(i*5),2021+(i%4));}
ps("\nSpecial elevators...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;elv_special(t,c,231+(i*13),217+(i*10),201+(i*7),190+(i*4),2022+(i%3));}
ps("\nElevator accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elv_accessory(t,c,223+(i*11),211+(i*9),197+(i*6),187+(i*3),2023+(i%2));}
ps("\nElevator marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elv_market(t,c,217+(i*9),206+(i*7),193+(i*5),185+(i*3),2024);}
ps("\n");elv_report();elv_state();ps("\n=== Demo Complete ===\n");return 0;}
