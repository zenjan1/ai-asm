/* coleus_admin: Coleus management technology administration (v1.0)
 * Coleus planning, coleus execution, coleus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} co_t;
typedef struct{int n_cop,n_coe,n_cov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} co_state_t;
static co_t cops[N],coss[N-2],covss[N-4],coas[N-6],comks[N-6]; static co_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(co_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;co_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cls_init(void){if(init)return -1;st.n_cop=0;st.n_coe=0;st.n_cov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cops[i].active=0;for(int i=0;i<N-2;i++)coss[i].active=0;for(int i=0;i<N-4;i++)covss[i].active=0;for(int i=0;i<N-6;i++)coas[i].active=0;for(int i=0;i<N-6;i++)comks[i].active=0;init=1;ps("[CLS] Coleus initialized\n");return 0;}
int cls_planning(int t,int c,int a,int b,int d,int e,int y){return add(cops,&st.n_cop,&st.t_f1,N,t,c,a,b,d,e,y);}
int cls_execution(int t,int c,int a,int b,int d,int e,int y){return add(coss,&st.n_coe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cls_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(covss,&st.n_cov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cls_accessory(int t,int c,int a,int b,int d,int e,int y){return add(coas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cls_market(int t,int c,int a,int b,int d,int e,int y){return add(comks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cls_report(void){ps("[CLS] Cop: ");pi(st.n_cop);ps(" PCS=");pi(st.t_f1);ps("\nCoe: ");pi(st.n_coe);ps(" PCS=");pi(st.t_f2);ps("\nCov: ");pi(st.n_cov);ps(" PCS=");pi(st.t_f3);ps("\nCoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cls_state(void){ps("[CLS] Cop=");pi(st.n_cop);ps(" Coe=");pi(st.n_coe);ps(" Cov=");pi(st.n_cov);ps(" Coc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coleus Admin Demo ===\n\n");cls_init();
ps("Coleus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cls_planning(t,c,764+(i*17),753+(i*14),733+(i*10),715+(i*6),2020+(i%5));}
ps("\nColeus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cls_execution(t,c,753+(i*15),742+(i*12),724+(i*8),711+(i*5),2021+(i%4));}
ps("\nColeus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cls_evaluation(t,c,745+(i*13),734+(i*10),720+(i*7),709+(i*4),2022+(i%3));}
ps("\nColeus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cls_accessory(t,c,737+(i*11),728+(i*9),714+(i*6),704+(i*3),2023+(i%2));}
ps("\nColeus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cls_market(t,c,731+(i*9),722+(i*7),709+(i*5),701+(i*3),2024);}
ps("\n");cls_report();cls_state();ps("\n=== Demo Complete ===\n");return 0;}
