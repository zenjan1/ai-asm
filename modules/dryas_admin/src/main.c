/* dryas_admin: Dryas management technology administration (v1.0)
 * Dryas planning, dryas execution, dryas evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drya_t;
typedef struct{int n_dryap,n_drya,n_drya2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drya_state_t;
static drya_t dryas[N],dryae[N-2],drya2[N-4],dryaac[N-6],dryaam[N-6]; static drya_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(drya_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drya_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRYA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drya_init(void){if(init)return -1;st.n_dryap=0;st.n_drya=0;st.n_drya2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dryas[i].active=0;for(int i=0;i<N-2;i++)dryae[i].active=0;for(int i=0;i<N-4;i++)drya2[i].active=0;for(int i=0;i<N-6;i++)dryaac[i].active=0;for(int i=0;i<N-6;i++)dryaam[i].active=0;init=1;ps("[DRYA] Dryas initialized\n");return 0;}
int drya_planning(int t,int c,int a,int b,int d,int e,int y){return add(dryas,&st.n_dryap,&st.t_f1,N,t,c,a,b,d,e,y);}
int drya_execution(int t,int c,int a,int b,int d,int e,int y){return add(dryae,&st.n_drya,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drya_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(drya2,&st.n_drya2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drya_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dryaac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drya_market(int t,int c,int a,int b,int d,int e,int y){return add(dryaam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drya_report(void){ps("[DRYA] Yap: ");pi(st.n_dryap);ps(" PCS=");pi(st.t_f1);ps("\nYae: ");pi(st.n_drya);ps(" PCS=");pi(st.t_f2);ps("\nYa2: ");pi(st.n_drya2);ps(" PCS=");pi(st.t_f3);ps("\nYac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drya_state(void){ps("[DRYA] Yap=");pi(st.n_dryap);ps(" Ya=");pi(st.n_drya);ps(" Ya2=");pi(st.n_drya2);ps(" Yac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dryas Admin Demo ===\n\n");drya_init();
ps("Dryas planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drya_planning(t,c,1500+(i*17),1489+(i*14),1469+(i*10),1451+(i*6),2020+(i%5));}
ps("\nDryas execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drya_execution(t,c,1489+(i*15),1478+(i*12),1460+(i*8),1447+(i*5),2021+(i%4));}
ps("\nDryas evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drya_evaluation(t,c,1481+(i*13),1470+(i*10),1454+(i*7),1443+(i*4),2022+(i%3));}
ps("\nDryas accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drya_accessory(t,c,1473+(i*11),1464+(i*9),1450+(i*6),1440+(i*3),2023+(i%2));}
ps("\nDryas marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drya_market(t,c,1467+(i*9),1458+(i*7),1445+(i*5),1437+(i*3),2024);}
ps("\n");drya_report();drya_state();ps("\n=== Demo Complete ===\n");return 0;}
