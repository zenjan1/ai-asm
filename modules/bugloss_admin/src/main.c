/* bugloss_admin: Bugloss management technology administration (v1.0)
 * Bugloss planning, bugloss execution, bugloss evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bug_t;
typedef struct{int n_bugp,n_buge,n_bug2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bug_state_t;
static bug_t bugps[N],bugss[N-2],bugvss[N-4],bugas[N-6],bugmks[N-6]; static bug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bug_init(void){if(init)return -1;st.n_bugp=0;st.n_buge=0;st.n_bug2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bugps[i].active=0;for(int i=0;i<N-2;i++)bugss[i].active=0;for(int i=0;i<N-4;i++)bugvss[i].active=0;for(int i=0;i<N-6;i++)bugas[i].active=0;for(int i=0;i<N-6;i++)bugmks[i].active=0;init=1;ps("[BUG] Bugloss initialized\n");return 0;}
int bug_planning(int t,int c,int a,int b,int d,int e,int y){return add(bugps,&st.n_bugp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bug_execution(int t,int c,int a,int b,int d,int e,int y){return add(bugss,&st.n_buge,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bugvss,&st.n_bug2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bugas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bug_market(int t,int c,int a,int b,int d,int e,int y){return add(bugmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bug_report(void){ps("[BUG] Bugpp: ");pi(st.n_bugp);ps(" PCS=");pi(st.t_f1);ps("\nBugpe: ");pi(st.n_buge);ps(" PCS=");pi(st.t_f2);ps("\nBugv: ");pi(st.n_bug2);ps(" PCS=");pi(st.t_f3);ps("\nBugc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bug_state(void){ps("[BUG] Bugpp=");pi(st.n_bugp);ps(" Bugpe=");pi(st.n_buge);ps(" Bugv=");pi(st.n_bug2);ps(" Bugc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bugloss Admin Demo ===\n\n");bug_init();
ps("Bugloss planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bug_planning(t,c,1223+(i*17),1212+(i*14),1192+(i*10),1174+(i*6),2020+(i%5));}
ps("\nBugloss execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bug_execution(t,c,1212+(i*15),1201+(i*12),1183+(i*8),1170+(i*5),2021+(i%4));}
ps("\nBugloss evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bug_evaluation(t,c,1204+(i*13),1193+(i*10),1177+(i*7),1166+(i*4),2022+(i%3));}
ps("\nBugloss accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bug_accessory(t,c,1196+(i*11),1187+(i*9),1173+(i*6),1163+(i*3),2023+(i%2));}
ps("\nBugloss marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bug_market(t,c,1190+(i*9),1181+(i*7),1168+(i*5),1160+(i*3),2024);}
ps("\n");bug_report();bug_state();ps("\n=== Demo Complete ===\n");return 0;}
