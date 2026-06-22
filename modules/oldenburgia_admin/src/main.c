/* oldenburgia_admin: Oldenburgia management technology administration (v1.0)
 * Oldenburgia planning, oldenburgia execution, oldenburgia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} old_t;
typedef struct{int n_oldp,n_olde,n_old2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} old_state_t;
static old_t oldps[N],oldess[N-2],oldvss[N-4],oldas[N-6],oldmks[N-6]; static old_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(old_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;old_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int old_init(void){if(init)return -1;st.n_oldp=0;st.n_olde=0;st.n_old2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oldps[i].active=0;for(int i=0;i<N-2;i++)oldess[i].active=0;for(int i=0;i<N-4;i++)oldvss[i].active=0;for(int i=0;i<N-6;i++)oldas[i].active=0;for(int i=0;i<N-6;i++)oldmks[i].active=0;init=1;ps("[OLD] Oldenburgia initialized\n");return 0;}
int old_planning(int t,int c,int a,int b,int d,int e,int y){return add(oldps,&st.n_oldp,&st.t_f1,N,t,c,a,b,d,e,y);}
int old_execution(int t,int c,int a,int b,int d,int e,int y){return add(oldess,&st.n_olde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int old_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oldvss,&st.n_old2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int old_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oldas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int old_market(int t,int c,int a,int b,int d,int e,int y){return add(oldmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void old_report(void){ps("[OLD] Oldp: ");pi(st.n_oldp);ps(" PCS=");pi(st.t_f1);ps("\nOlde: ");pi(st.n_olde);ps(" PCS=");pi(st.t_f2);ps("\nOldv: ");pi(st.n_old2);ps(" PCS=");pi(st.t_f3);ps("\nOldc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void old_state(void){ps("[OLD] Oldp=");pi(st.n_oldp);ps(" Olde=");pi(st.n_olde);ps(" Oldv=");pi(st.n_old2);ps(" Oldc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oldenburgia Admin Demo ===\n\n");old_init();
ps("Oldenburgia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;old_planning(t,c,1321+(i*17),1310+(i*14),1290+(i*10),1272+(i*6),2020+(i%5));}
ps("\nOldenburgia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;old_execution(t,c,1310+(i*15),1299+(i*12),1281+(i*8),1268+(i*5),2021+(i%4));}
ps("\nOldenburgia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;old_evaluation(t,c,1302+(i*13),1291+(i*10),1275+(i*7),1264+(i*4),2022+(i%3));}
ps("\nOldenburgia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;old_accessory(t,c,1294+(i*11),1285+(i*9),1271+(i*6),1261+(i*3),2023+(i%2));}
ps("\nOldenburgia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;old_market(t,c,1288+(i*9),1279+(i*7),1266+(i*5),1258+(i*3),2024);}
ps("\n");old_report();old_state();ps("\n=== Demo Complete ===\n");return 0;}
