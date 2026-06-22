/* cyclamen_admin: Cyclamen management technology administration (v1.0)
 * Cyclamen planning, cyclamen execution, cyclamen evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ycl_t;
typedef struct{int n_yclp,n_ycle,n_ycl2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ycl_state_t;
static ycl_t yclps[N],yclss[N-2],yclvss[N-4],yclas[N-6],yclmks[N-6]; static ycl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ycl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ycl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[YCL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ycl_init(void){if(init)return -1;st.n_yclp=0;st.n_ycle=0;st.n_ycl2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)yclps[i].active=0;for(int i=0;i<N-2;i++)yclss[i].active=0;for(int i=0;i<N-4;i++)yclvss[i].active=0;for(int i=0;i<N-6;i++)yclas[i].active=0;for(int i=0;i<N-6;i++)yclmks[i].active=0;init=1;ps("[YCL] Cyclamen initialized\n");return 0;}
int ycl_planning(int t,int c,int a,int b,int d,int e,int y){return add(yclps,&st.n_yclp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ycl_execution(int t,int c,int a,int b,int d,int e,int y){return add(yclss,&st.n_ycle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ycl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(yclvss,&st.n_ycl2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ycl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(yclas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ycl_market(int t,int c,int a,int b,int d,int e,int y){return add(yclmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ycl_report(void){ps("[YCL] Cycpp: ");pi(st.n_yclp);ps(" PCS=");pi(st.t_f1);ps("\nCycpe: ");pi(st.n_ycle);ps(" PCS=");pi(st.t_f2);ps("\nCycv: ");pi(st.n_ycl2);ps(" PCS=");pi(st.t_f3);ps("\nCycc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ycl_state(void){ps("[YCL] Cycpp=");pi(st.n_yclp);ps(" Cycpe=");pi(st.n_ycle);ps(" Cycv=");pi(st.n_ycl2);ps(" Cycc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyclamen Admin Demo ===\n\n");ycl_init();
ps("Cyclamen planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ycl_planning(t,c,1250+(i*17),1239+(i*14),1219+(i*10),1201+(i*6),2020+(i%5));}
ps("\nCyclamen execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ycl_execution(t,c,1239+(i*15),1228+(i*12),1210+(i*8),1197+(i*5),2021+(i%4));}
ps("\nCyclamen evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ycl_evaluation(t,c,1231+(i*13),1220+(i*10),1204+(i*7),1193+(i*4),2022+(i%3));}
ps("\nCyclamen accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ycl_accessory(t,c,1223+(i*11),1214+(i*9),1200+(i*6),1190+(i*3),2023+(i%2));}
ps("\nCyclamen marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ycl_market(t,c,1217+(i*9),1208+(i*7),1195+(i*5),1187+(i*3),2024);}
ps("\n");ycl_report();ycl_state();ps("\n=== Demo Complete ===\n");return 0;}
