/* jacaranda_admin: Jacaranda management technology administration (v1.0)
 * Jacaranda planning, jacaranda execution, jacaranda evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jac_t;
typedef struct{int n_jacp,n_jace,n_jac2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jac_state_t;
static jac_t jacps[N],jacess[N-2],jacvss[N-4],jacas[N-6],jacmks[N-6]; static jac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jac_init(void){if(init)return -1;st.n_jacp=0;st.n_jace=0;st.n_jac2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jacps[i].active=0;for(int i=0;i<N-2;i++)jacess[i].active=0;for(int i=0;i<N-4;i++)jacvss[i].active=0;for(int i=0;i<N-6;i++)jacas[i].active=0;for(int i=0;i<N-6;i++)jacmks[i].active=0;init=1;ps("[JAC] Jacaranda initialized\n");return 0;}
int jac_planning(int t,int c,int a,int b,int d,int e,int y){return add(jacps,&st.n_jacp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jac_execution(int t,int c,int a,int b,int d,int e,int y){return add(jacess,&st.n_jace,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jacvss,&st.n_jac2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jacas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jac_market(int t,int c,int a,int b,int d,int e,int y){return add(jacmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jac_report(void){ps("[JAC] Jacp: ");pi(st.n_jacp);ps(" PCS=");pi(st.t_f1);ps("\nJace: ");pi(st.n_jace);ps(" PCS=");pi(st.t_f2);ps("\nJacv: ");pi(st.n_jac2);ps(" PCS=");pi(st.t_f3);ps("\nJacc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jac_state(void){ps("[JAC] Jacp=");pi(st.n_jacp);ps(" Jace=");pi(st.n_jace);ps(" Jacv=");pi(st.n_jac2);ps(" Jacc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jacaranda Admin Demo ===\n\n");jac_init();
ps("Jacaranda planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jac_planning(t,c,1303+(i*17),1292+(i*14),1272+(i*10),1254+(i*6),2020+(i%5));}
ps("\nJacaranda execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jac_execution(t,c,1292+(i*15),1281+(i*12),1263+(i*8),1250+(i*5),2021+(i%4));}
ps("\nJacaranda evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jac_evaluation(t,c,1284+(i*13),1273+(i*10),1257+(i*7),1246+(i*4),2022+(i%3));}
ps("\nJacaranda accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jac_accessory(t,c,1276+(i*11),1267+(i*9),1253+(i*6),1243+(i*3),2023+(i%2));}
ps("\nJacaranda marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jac_market(t,c,1270+(i*9),1261+(i*7),1248+(i*5),1240+(i*3),2024);}
ps("\n");jac_report();jac_state();ps("\n=== Demo Complete ===\n");return 0;}
