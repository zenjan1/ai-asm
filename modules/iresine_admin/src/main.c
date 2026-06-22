/* iresine_admin: Iresine management technology administration (v1.0)
 * Iresine planning, iresine execution, iresine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ire_t;
typedef struct{int n_irep,n_iree,n_ire2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ire_state_t;
static ire_t ireps[N],ireess[N-2],irevss[N-4],ireas[N-6],iremks[N-6]; static ire_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ire_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ire_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IRE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ire_init(void){if(init)return -1;st.n_irep=0;st.n_iree=0;st.n_ire2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ireps[i].active=0;for(int i=0;i<N-2;i++)ireess[i].active=0;for(int i=0;i<N-4;i++)irevss[i].active=0;for(int i=0;i<N-6;i++)ireas[i].active=0;for(int i=0;i<N-6;i++)iremks[i].active=0;init=1;ps("[IRE] Iresine initialized\n");return 0;}
int ire_planning(int t,int c,int a,int b,int d,int e,int y){return add(ireps,&st.n_irep,&st.t_f1,N,t,c,a,b,d,e,y);}
int ire_execution(int t,int c,int a,int b,int d,int e,int y){return add(ireess,&st.n_iree,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ire_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(irevss,&st.n_ire2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ire_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ireas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ire_market(int t,int c,int a,int b,int d,int e,int y){return add(iremks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ire_report(void){ps("[IRE] Irep: ");pi(st.n_irep);ps(" PCS=");pi(st.t_f1);ps("\nIree: ");pi(st.n_iree);ps(" PCS=");pi(st.t_f2);ps("\nIrev: ");pi(st.n_ire2);ps(" PCS=");pi(st.t_f3);ps("\nIrec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ire_state(void){ps("[IRE] Irep=");pi(st.n_irep);ps(" Iree=");pi(st.n_iree);ps(" Irev=");pi(st.n_ire2);ps(" Irec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Iresine Admin Demo ===\n\n");ire_init();
ps("Iresine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ire_planning(t,c,1301+(i*17),1290+(i*14),1270+(i*10),1252+(i*6),2020+(i%5));}
ps("\nIresine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ire_execution(t,c,1290+(i*15),1279+(i*12),1261+(i*8),1248+(i*5),2021+(i%4));}
ps("\nIresine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ire_evaluation(t,c,1282+(i*13),1271+(i*10),1255+(i*7),1244+(i*4),2022+(i%3));}
ps("\nIresine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ire_accessory(t,c,1274+(i*11),1265+(i*9),1251+(i*6),1241+(i*3),2023+(i%2));}
ps("\nIresine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ire_market(t,c,1268+(i*9),1259+(i*7),1246+(i*5),1238+(i*3),2024);}
ps("\n");ire_report();ire_state();ps("\n=== Demo Complete ===\n");return 0;}
