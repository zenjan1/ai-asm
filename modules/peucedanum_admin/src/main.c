/* peucedanum_admin: Peucedanum management technology administration (v1.0)
 * Peucedanum planning, peucedanum execution, peucedanum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} peu_t;
typedef struct{int n_peup,n_peue,n_peu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} peu_state_t;
static peu_t peups[N],peuess[N-2],peuvss[N-4],peuas[N-6],peumks[N-6]; static peu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(peu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;peu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PEU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int peu_init(void){if(init)return -1;st.n_peup=0;st.n_peue=0;st.n_peu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)peups[i].active=0;for(int i=0;i<N-2;i++)peuess[i].active=0;for(int i=0;i<N-4;i++)peuvss[i].active=0;for(int i=0;i<N-6;i++)peuas[i].active=0;for(int i=0;i<N-6;i++)peumks[i].active=0;init=1;ps("[PEU] Peucedanum initialized\n");return 0;}
int peu_planning(int t,int c,int a,int b,int d,int e,int y){return add(peups,&st.n_peup,&st.t_f1,N,t,c,a,b,d,e,y);}
int peu_execution(int t,int c,int a,int b,int d,int e,int y){return add(peuess,&st.n_peue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int peu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(peuvss,&st.n_peu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int peu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(peuas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int peu_market(int t,int c,int a,int b,int d,int e,int y){return add(peumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void peu_report(void){ps("[PEU] Peup: ");pi(st.n_peup);ps(" PCS=");pi(st.t_f1);ps("\nPeue: ");pi(st.n_peue);ps(" PCS=");pi(st.t_f2);ps("\nPeuv: ");pi(st.n_peu2);ps(" PCS=");pi(st.t_f3);ps("\nPeuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void peu_state(void){ps("[PEU] Peup=");pi(st.n_peup);ps(" Peue=");pi(st.n_peue);ps(" Peuv=");pi(st.n_peu2);ps(" Peuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Peucedanum Admin Demo ===\n\n");peu_init();
ps("Peucedanum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;peu_planning(t,c,1325+(i*17),1314+(i*14),1294+(i*10),1276+(i*6),2020+(i%5));}
ps("\nPeucedanum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;peu_execution(t,c,1314+(i*15),1303+(i*12),1285+(i*8),1272+(i*5),2021+(i%4));}
ps("\nPeucedanum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;peu_evaluation(t,c,1306+(i*13),1295+(i*10),1279+(i*7),1268+(i*4),2022+(i%3));}
ps("\nPeucedanum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;peu_accessory(t,c,1298+(i*11),1289+(i*9),1275+(i*6),1265+(i*3),2023+(i%2));}
ps("\nPeucedanum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;peu_market(t,c,1292+(i*9),1283+(i*7),1270+(i*5),1262+(i*3),2024);}
ps("\n");peu_report();peu_state();ps("\n=== Demo Complete ===\n");return 0;}
