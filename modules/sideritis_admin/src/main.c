/* sideritis_admin: Sideritis management technology administration (v1.0)
 * Sideritis planning, sideritis execution, sideritis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} side_t;
typedef struct{int n_sidep,n_sidee,n_side2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} side_state_t;
static side_t sides[N],sidess[N-2],sidevss[N-4],sideas[N-6],sidemarks[N-6]; static side_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(side_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;side_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SIDE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int side_init(void){if(init)return -1;st.n_sidep=0;st.n_sidee=0;st.n_side2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sides[i].active=0;for(int i=0;i<N-2;i++)sidess[i].active=0;for(int i=0;i<N-4;i++)sidevss[i].active=0;for(int i=0;i<N-6;i++)sideas[i].active=0;for(int i=0;i<N-6;i++)sidemarks[i].active=0;init=1;ps("[SIDE] Sideritis initialized\n");return 0;}
int side_planning(int t,int c,int a,int b,int d,int e,int y){return add(sides,&st.n_sidep,&st.t_f1,N,t,c,a,b,d,e,y);}
int side_execution(int t,int c,int a,int b,int d,int e,int y){return add(sidess,&st.n_sidee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int side_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sidevss,&st.n_side2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int side_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sideas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int side_market(int t,int c,int a,int b,int d,int e,int y){return add(sidemarks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void side_report(void){ps("[SIDE] Sidep: ");pi(st.n_sidep);ps(" PCS=");pi(st.t_f1);ps("\nSidee: ");pi(st.n_sidee);ps(" PCS=");pi(st.t_f2);ps("\nSidev: ");pi(st.n_side2);ps(" PCS=");pi(st.t_f3);ps("\nSidec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void side_state(void){ps("[SIDE] Sidep=");pi(st.n_sidep);ps(" Sidee=");pi(st.n_sidee);ps(" Sidev=");pi(st.n_side2);ps(" Sidec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sideritis Admin Demo ===\n\n");side_init();
ps("Sideritis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;side_planning(t,c,1336+(i*17),1325+(i*14),1305+(i*10),1287+(i*6),2020+(i%5));}
ps("\nSideritis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;side_execution(t,c,1325+(i*15),1314+(i*12),1296+(i*8),1283+(i*5),2021+(i%4));}
ps("\nSideritis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;side_evaluation(t,c,1317+(i*13),1306+(i*10),1290+(i*7),1279+(i*4),2022+(i%3));}
ps("\nSideritis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;side_accessory(t,c,1309+(i*11),1300+(i*9),1286+(i*6),1276+(i*3),2023+(i%2));}
ps("\nSideritis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;side_market(t,c,1303+(i*9),1294+(i*7),1281+(i*5),1273+(i*3),2024);}
ps("\n");side_report();side_state();ps("\n=== Demo Complete ===\n");return 0;}
