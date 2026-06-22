/* eugenia_admin: Eugenia management technology administration (v1.0)
 * Eugenia planning, eugenia execution, eugenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eug_t;
typedef struct{int n_eugp,n_euge,n_eug2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eug_state_t;
static eug_t eugps[N],eugss[N-2],eugvss[N-4],eugas[N-6],eugmks[N-6]; static eug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eug_init(void){if(init)return -1;st.n_eugp=0;st.n_euge=0;st.n_eug2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eugps[i].active=0;for(int i=0;i<N-2;i++)eugss[i].active=0;for(int i=0;i<N-4;i++)eugvss[i].active=0;for(int i=0;i<N-6;i++)eugas[i].active=0;for(int i=0;i<N-6;i++)eugmks[i].active=0;init=1;ps("[EUG] Eugenia initialized\n");return 0;}
int eug_planning(int t,int c,int a,int b,int d,int e,int y){return add(eugps,&st.n_eugp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eug_execution(int t,int c,int a,int b,int d,int e,int y){return add(eugss,&st.n_euge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eugvss,&st.n_eug2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eugas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eug_market(int t,int c,int a,int b,int d,int e,int y){return add(eugmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eug_report(void){ps("[EUG] Eugp: ");pi(st.n_eugp);ps(" PCS=");pi(st.t_f1);ps("\nEuge: ");pi(st.n_euge);ps(" PCS=");pi(st.t_f2);ps("\nEugv: ");pi(st.n_eug2);ps(" PCS=");pi(st.t_f3);ps("\nEugc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eug_state(void){ps("[EUG] Eugp=");pi(st.n_eugp);ps(" Euge=");pi(st.n_euge);ps(" Eugv=");pi(st.n_eug2);ps(" Eugc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Eugenia Admin Demo ===\n\n");eug_init();
ps("Eugenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eug_planning(t,c,1281+(i*17),1270+(i*14),1250+(i*10),1232+(i*6),2020+(i%5));}
ps("\nEugenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eug_execution(t,c,1270+(i*15),1259+(i*12),1241+(i*8),1228+(i*5),2021+(i%4));}
ps("\nEugenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eug_evaluation(t,c,1262+(i*13),1251+(i*10),1235+(i*7),1224+(i*4),2022+(i%3));}
ps("\nEugenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eug_accessory(t,c,1254+(i*11),1245+(i*9),1231+(i*6),1221+(i*3),2023+(i%2));}
ps("\nEugenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eug_market(t,c,1248+(i*9),1239+(i*7),1226+(i*5),1218+(i*3),2024);}
ps("\n");eug_report();eug_state();ps("\n=== Demo Complete ===\n");return 0;}
