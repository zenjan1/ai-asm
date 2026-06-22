/* collomia_admin: Collomia management technology administration (v1.0)
 * Collomia planning, collomia execution, collomia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} llo_t;
typedef struct{int n_llop,n_lloe,n_llo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} llo_state_t;
static llo_t llops[N],lloss[N-2],llovss[N-4],lloas[N-6],llomks[N-6]; static llo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(llo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;llo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LLO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int llo_init(void){if(init)return -1;st.n_llop=0;st.n_lloe=0;st.n_llo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)llops[i].active=0;for(int i=0;i<N-2;i++)lloss[i].active=0;for(int i=0;i<N-4;i++)llovss[i].active=0;for(int i=0;i<N-6;i++)lloas[i].active=0;for(int i=0;i<N-6;i++)llomks[i].active=0;init=1;ps("[LLO] Collomia initialized\n");return 0;}
int llo_planning(int t,int c,int a,int b,int d,int e,int y){return add(llops,&st.n_llop,&st.t_f1,N,t,c,a,b,d,e,y);}
int llo_execution(int t,int c,int a,int b,int d,int e,int y){return add(lloss,&st.n_lloe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int llo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(llovss,&st.n_llo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int llo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lloas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int llo_market(int t,int c,int a,int b,int d,int e,int y){return add(llomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void llo_report(void){ps("[LLO] Colpp: ");pi(st.n_llop);ps(" PCS=");pi(st.t_f1);ps("\nColpe: ");pi(st.n_lloe);ps(" PCS=");pi(st.t_f2);ps("\nColv: ");pi(st.n_llo2);ps(" PCS=");pi(st.t_f3);ps("\nColc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void llo_state(void){ps("[LLO] Colpp=");pi(st.n_llop);ps(" Colpe=");pi(st.n_lloe);ps(" Colv=");pi(st.n_llo2);ps(" Colc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Collomia Admin Demo ===\n\n");llo_init();
ps("Collomia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;llo_planning(t,c,1244+(i*17),1233+(i*14),1213+(i*10),1195+(i*6),2020+(i%5));}
ps("\nCollomia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;llo_execution(t,c,1233+(i*15),1222+(i*12),1204+(i*8),1191+(i*5),2021+(i%4));}
ps("\nCollomia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;llo_evaluation(t,c,1225+(i*13),1214+(i*10),1198+(i*7),1187+(i*4),2022+(i%3));}
ps("\nCollomia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;llo_accessory(t,c,1217+(i*11),1208+(i*9),1194+(i*6),1184+(i*3),2023+(i%2));}
ps("\nCollomia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;llo_market(t,c,1211+(i*9),1202+(i*7),1189+(i*5),1181+(i*3),2024);}
ps("\n");llo_report();llo_state();ps("\n=== Demo Complete ===\n");return 0;}
