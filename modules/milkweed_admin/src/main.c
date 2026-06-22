/* milkweed_admin: Milkweed management technology administration (v1.0)
 * Milkweed planning, milkweed execution, milkweed evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mil_t;
typedef struct{int n_milp,n_mile,n_mil2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mil_state_t;
static mil_t milps[N],miless[N-2],milvss[N-4],mils[N-6],milms[N-6]; static mil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(mil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mil_init(void){if(init)return -1;st.n_milp=0;st.n_mile=0;st.n_mil2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)milps[i].active=0;for(int i=0;i<N-2;i++)miless[i].active=0;for(int i=0;i<N-4;i++)milvss[i].active=0;for(int i=0;i<N-6;i++)mils[i].active=0;for(int i=0;i<N-6;i++)milms[i].active=0;init=1;ps("[MIL] Milkweed initialized\n");return 0;}
int mil_planning(int t,int c,int a,int b,int d,int e,int y){return add(milps,&st.n_milp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mil_execution(int t,int c,int a,int b,int d,int e,int y){return add(miless,&st.n_mile,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(milvss,&st.n_mil2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mils,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mil_market(int t,int c,int a,int b,int d,int e,int y){return add(milms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mil_report(void){ps("[MIL] Milp: ");pi(st.n_milp);ps(" PCS=");pi(st.t_f1);ps("\nMile: ");pi(st.n_mile);ps(" PCS=");pi(st.t_f2);ps("\nMilv: ");pi(st.n_mil2);ps(" PCS=");pi(st.t_f3);ps("\nMilc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mil_state(void){ps("[MIL] Milp=");pi(st.n_milp);ps(" Mile=");pi(st.n_mile);ps(" Milv=");pi(st.n_mil2);ps(" Milc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Milkweed Admin Demo ===\n\n");mil_init();
ps("Milkweed planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mil_planning(t,c,1316+(i*17),1305+(i*14),1285+(i*10),1267+(i*6),2020+(i%5));}
ps("\nMilkweed execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mil_execution(t,c,1305+(i*15),1294+(i*12),1276+(i*8),1263+(i*5),2021+(i%4));}
ps("\nMilkweed evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mil_evaluation(t,c,1297+(i*13),1286+(i*10),1270+(i*7),1259+(i*4),2022+(i%3));}
ps("\nMilkweed accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mil_accessory(t,c,1289+(i*11),1280+(i*9),1266+(i*6),1256+(i*3),2023+(i%2));}
ps("\nMilkweed marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mil_market(t,c,1283+(i*9),1274+(i*7),1261+(i*5),1253+(i*3),2024);}
ps("\n");mil_report();mil_state();ps("\n=== Demo Complete ===\n");return 0;}
