/* lungwort_admin: Lungwort management technology administration (v1.0)
 * Lungwort planning, lungwort execution, lungwort evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lw_t;
typedef struct{int n_lgp,n_lge,n_lgv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lw_state_t;
static lw_t lgps[N],lgss[N-2],lgvss[N-4],lgas[N-6],lgmks[N-6]; static lw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lw_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lw_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LGW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lg_init(void){if(init)return -1;st.n_lgp=0;st.n_lge=0;st.n_lgv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lgps[i].active=0;for(int i=0;i<N-2;i++)lgss[i].active=0;for(int i=0;i<N-4;i++)lgvss[i].active=0;for(int i=0;i<N-6;i++)lgas[i].active=0;for(int i=0;i<N-6;i++)lgmks[i].active=0;init=1;ps("[LGW] Lungwort initialized\n");return 0;}
int lg_planning(int t,int c,int a,int b,int d,int e,int y){return add(lgps,&st.n_lgp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lg_execution(int t,int c,int a,int b,int d,int e,int y){return add(lgss,&st.n_lge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lg_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lgvss,&st.n_lgv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lgas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lg_market(int t,int c,int a,int b,int d,int e,int y){return add(lgmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lg_report(void){ps("[LGW] Lgp: ");pi(st.n_lgp);ps(" PCS=");pi(st.t_f1);ps("\nLge: ");pi(st.n_lge);ps(" PCS=");pi(st.t_f2);ps("\nLgv: ");pi(st.n_lgv);ps(" PCS=");pi(st.t_f3);ps("\nLgc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lg_state(void){ps("[LGW] Lgp=");pi(st.n_lgp);ps(" Lge=");pi(st.n_lge);ps(" Lgv=");pi(st.n_lgv);ps(" Lgc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lungwort Admin Demo ===\n\n");lg_init();
ps("Lungwort planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lg_planning(t,c,700+(i*17),689+(i*14),669+(i*10),651+(i*6),2020+(i%5));}
ps("\nLungwort execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lg_execution(t,c,689+(i*15),678+(i*12),660+(i*8),647+(i*5),2021+(i%4));}
ps("\nLungwort evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lg_evaluation(t,c,681+(i*13),670+(i*10),654+(i*7),643+(i*4),2022+(i%3));}
ps("\nLungwort accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lg_accessory(t,c,673+(i*11),664+(i*9),650+(i*6),640+(i*3),2023+(i%2));}
ps("\nLungwort marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lg_market(t,c,667+(i*9),658+(i*7),645+(i*5),637+(i*3),2024);}
ps("\n");lg_report();lg_state();ps("\n=== Demo Complete ===\n");return 0;}
