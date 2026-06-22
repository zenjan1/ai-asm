/* centaurea_admin: Centaurea management technology administration (v1.0)
 * Centaurea planning, centaurea execution, centaurea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cen_t;
typedef struct{int n_cenp,n_cene,n_cen2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cen_state_t;
static cen_t cenps[N],censs[N-2],cenvss[N-4],cenas[N-6],cenmks[N-6]; static cen_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cen_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cen_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CEN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cen_init(void){if(init)return -1;st.n_cenp=0;st.n_cene=0;st.n_cen2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cenps[i].active=0;for(int i=0;i<N-2;i++)censs[i].active=0;for(int i=0;i<N-4;i++)cenvss[i].active=0;for(int i=0;i<N-6;i++)cenas[i].active=0;for(int i=0;i<N-6;i++)cenmks[i].active=0;init=1;ps("[CEN] Centaurea initialized\n");return 0;}
int cen_planning(int t,int c,int a,int b,int d,int e,int y){return add(cenps,&st.n_cenp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cen_execution(int t,int c,int a,int b,int d,int e,int y){return add(censs,&st.n_cene,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cen_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cenvss,&st.n_cen2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cen_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cenas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cen_market(int t,int c,int a,int b,int d,int e,int y){return add(cenmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cen_report(void){ps("[CEN] Cenpp: ");pi(st.n_cenp);ps(" PCS=");pi(st.t_f1);ps("\nCene: ");pi(st.n_cene);ps(" PCS=");pi(st.t_f2);ps("\nCenv: ");pi(st.n_cen2);ps(" PCS=");pi(st.t_f3);ps("\nCenc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cen_state(void){ps("[CEN] Cenpp=");pi(st.n_cenp);ps(" Cene=");pi(st.n_cene);ps(" Cenv=");pi(st.n_cen2);ps(" Cenc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Centaurea Admin Demo ===\n\n");cen_init();
ps("Centaurea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cen_planning(t,c,1127+(i*17),1116+(i*14),1096+(i*10),1078+(i*6),2020+(i%5));}
ps("\nCentaurea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cen_execution(t,c,1116+(i*15),1105+(i*12),1087+(i*8),1074+(i*5),2021+(i%4));}
ps("\nCentaurea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cen_evaluation(t,c,1108+(i*13),1097+(i*10),1081+(i*7),1070+(i*4),2022+(i%3));}
ps("\nCentaurea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cen_accessory(t,c,1100+(i*11),1091+(i*9),1077+(i*6),1067+(i*3),2023+(i%2));}
ps("\nCentaurea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cen_market(t,c,1094+(i*9),1085+(i*7),1072+(i*5),1064+(i*3),2024);}
ps("\n");cen_report();cen_state();ps("\n=== Demo Complete ===\n");return 0;}
