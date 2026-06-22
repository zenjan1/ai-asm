/* juniperus_admin: Juniperus management technology administration (v1.0)
 * Juniperus planning, juniperus execution, juniperus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jun_t;
typedef struct{int n_junp,n_june,n_jun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jun_state_t;
static jun_t junps[N],juness[N-2],junvss[N-4],junas[N-6],junmks[N-6]; static jun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(jun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jun_init(void){if(init)return -1;st.n_junp=0;st.n_june=0;st.n_jun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)junps[i].active=0;for(int i=0;i<N-2;i++)juness[i].active=0;for(int i=0;i<N-4;i++)junvss[i].active=0;for(int i=0;i<N-6;i++)junas[i].active=0;for(int i=0;i<N-6;i++)junmks[i].active=0;init=1;ps("[JUN] Juniperus initialized\n");return 0;}
int jun_planning(int t,int c,int a,int b,int d,int e,int y){return add(junps,&st.n_junp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jun_execution(int t,int c,int a,int b,int d,int e,int y){return add(juness,&st.n_june,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(junvss,&st.n_jun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(junas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jun_market(int t,int c,int a,int b,int d,int e,int y){return add(junmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jun_report(void){ps("[JUN] Junp: ");pi(st.n_junp);ps(" PCS=");pi(st.t_f1);ps("\nJune: ");pi(st.n_june);ps(" PCS=");pi(st.t_f2);ps("\nJunv: ");pi(st.n_jun2);ps(" PCS=");pi(st.t_f3);ps("\nJunc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jun_state(void){ps("[JUN] Junp=");pi(st.n_junp);ps(" June=");pi(st.n_june);ps(" Junv=");pi(st.n_jun2);ps(" Junc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Juniperus Admin Demo ===\n\n");jun_init();
ps("Juniperus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jun_planning(t,c,1304+(i*17),1293+(i*14),1273+(i*10),1255+(i*6),2020+(i%5));}
ps("\nJuniperus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jun_execution(t,c,1293+(i*15),1282+(i*12),1264+(i*8),1251+(i*5),2021+(i%4));}
ps("\nJuniperus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jun_evaluation(t,c,1285+(i*13),1274+(i*10),1258+(i*7),1247+(i*4),2022+(i%3));}
ps("\nJuniperus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jun_accessory(t,c,1277+(i*11),1268+(i*9),1254+(i*6),1244+(i*3),2023+(i%2));}
ps("\nJuniperus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jun_market(t,c,1271+(i*9),1262+(i*7),1249+(i*5),1241+(i*3),2024);}
ps("\n");jun_report();jun_state();ps("\n=== Demo Complete ===\n");return 0;}
