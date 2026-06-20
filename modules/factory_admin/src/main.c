/* factory_admin: Factory technology administration (v1.0)
 * Smart factory, automated production lines, factory management systems, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fcx_t;
typedef struct{int n_sf,n_al,n_ms,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fcx_state_t;
static fcx_t sfs[N],als[N-2],mss[N-4],acs[N-6],mks[N-6]; static fcx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fcx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fcx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FCX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fcx_init(void){if(init)return -1;st.n_sf=0;st.n_al=0;st.n_ms=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sfs[i].active=0;for(int i=0;i<N-2;i++)als[i].active=0;for(int i=0;i<N-4;i++)mss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FCX] Factory initialized\n");return 0;}
int fcx_smart(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f1,N,t,c,a,b,d,e,y);}
int fcx_automation(int t,int c,int a,int b,int d,int e,int y){return add(als,&st.n_al,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fcx_management(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fcx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fcx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fcx_report(void){ps("[FCX] Sf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f1);ps("\nAl: ");pi(st.n_al);ps(" PCS=");pi(st.t_f2);ps("\nMs: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fcx_state(void){ps("[FCX] Sf=");pi(st.n_sf);ps(" Al=");pi(st.n_al);ps(" Ms=");pi(st.n_ms);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Factory Admin Demo ===\n\n");fcx_init();
ps("Smart factory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fcx_smart(t,c,428+(i*17),417+(i*14),397+(i*10),379+(i*6),2020+(i%5));}
ps("\nAutomated production lines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fcx_automation(t,c,417+(i*15),406+(i*12),388+(i*8),375+(i*5),2021+(i%4));}
ps("\nFactory management systems...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fcx_management(t,c,409+(i*13),398+(i*10),382+(i*7),371+(i*4),2022+(i%3));}
ps("\nFactory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fcx_accessory(t,c,401+(i*11),392+(i*9),378+(i*6),368+(i*3),2023+(i%2));}
ps("\nFactory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fcx_market(t,c,395+(i*9),386+(i*7),373+(i*5),365+(i*3),2024);}
ps("\n");fcx_report();fcx_state();ps("\n=== Demo Complete ===\n");return 0;}
