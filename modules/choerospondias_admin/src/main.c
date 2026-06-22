/* choerospondias_admin: Choerospondias management technology administration (v1.0)
 * Choerospondias planning, choerospondias execution, choerospondias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} roe_t;
typedef struct{int n_roep,n_roee,n_roe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} roe_state_t;
static roe_t roeps[N],roess[N-2],roevss[N-4],roeas[N-6],roemks[N-6]; static roe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(roe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;roe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ROE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int roe_init(void){if(init)return -1;st.n_roep=0;st.n_roee=0;st.n_roe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)roeps[i].active=0;for(int i=0;i<N-2;i++)roess[i].active=0;for(int i=0;i<N-4;i++)roevss[i].active=0;for(int i=0;i<N-6;i++)roeas[i].active=0;for(int i=0;i<N-6;i++)roemks[i].active=0;init=1;ps("[ROE] Choerospondias initialized\n");return 0;}
int roe_planning(int t,int c,int a,int b,int d,int e,int y){return add(roeps,&st.n_roep,&st.t_f1,N,t,c,a,b,d,e,y);}
int roe_execution(int t,int c,int a,int b,int d,int e,int y){return add(roess,&st.n_roee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int roe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(roevss,&st.n_roe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int roe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(roeas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int roe_market(int t,int c,int a,int b,int d,int e,int y){return add(roemks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void roe_report(void){ps("[ROE] Chopp: ");pi(st.n_roep);ps(" PCS=");pi(st.t_f1);ps("\nChope: ");pi(st.n_roee);ps(" PCS=");pi(st.t_f2);ps("\nChov: ");pi(st.n_roe2);ps(" PCS=");pi(st.t_f3);ps("\nChoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void roe_state(void){ps("[ROE] Chopp=");pi(st.n_roep);ps(" Chope=");pi(st.n_roee);ps(" Chov=");pi(st.n_roe2);ps(" Choc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Choerospondias Admin Demo ===\n\n");roe_init();
ps("Choerospondias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;roe_planning(t,c,1235+(i*17),1224+(i*14),1204+(i*10),1186+(i*6),2020+(i%5));}
ps("\nChoerospondias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;roe_execution(t,c,1224+(i*15),1213+(i*12),1195+(i*8),1182+(i*5),2021+(i%4));}
ps("\nChoerospondias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;roe_evaluation(t,c,1216+(i*13),1205+(i*10),1189+(i*7),1178+(i*4),2022+(i%3));}
ps("\nChoerospondias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;roe_accessory(t,c,1208+(i*11),1199+(i*9),1185+(i*6),1175+(i*3),2023+(i%2));}
ps("\nChoerospondias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;roe_market(t,c,1202+(i*9),1193+(i*7),1180+(i*5),1172+(i*3),2024);}
ps("\n");roe_report();roe_state();ps("\n=== Demo Complete ===\n");return 0;}
