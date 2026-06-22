/* spiraea_admin: Spiraea management technology administration (v1.0)
 * Spiraea planning, spiraea execution, spiraea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} spir_t;
typedef struct{int n_spirp,n_spire,n_spir2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} spir_state_t;
static spir_t spirs[N],spiress[N-2],spivss[N-4],spias[N-6],spimks[N-6]; static spir_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(spir_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;spir_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SPIR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int spir_init(void){if(init)return -1;st.n_spirp=0;st.n_spire=0;st.n_spir2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)spirs[i].active=0;for(int i=0;i<N-2;i++)spiress[i].active=0;for(int i=0;i<N-4;i++)spivss[i].active=0;for(int i=0;i<N-6;i++)spias[i].active=0;for(int i=0;i<N-6;i++)spimks[i].active=0;init=1;ps("[SPIR] Spiraea initialized\n");return 0;}
int spir_planning(int t,int c,int a,int b,int d,int e,int y){return add(spirs,&st.n_spirp,&st.t_f1,N,t,c,a,b,d,e,y);}
int spir_execution(int t,int c,int a,int b,int d,int e,int y){return add(spiress,&st.n_spire,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int spir_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(spivss,&st.n_spir2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int spir_accessory(int t,int c,int a,int b,int d,int e,int y){return add(spias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int spir_market(int t,int c,int a,int b,int d,int e,int y){return add(spimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void spir_report(void){ps("[SPIR] Sprip: ");pi(st.n_spirp);ps(" PCS=");pi(st.t_f1);ps("\nSpire: ");pi(st.n_spire);ps(" PCS=");pi(st.t_f2);ps("\nSpirv: ");pi(st.n_spir2);ps(" PCS=");pi(st.t_f3);ps("\nSperc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void spir_state(void){ps("[SPIR] Sprip=");pi(st.n_spirp);ps(" Spire=");pi(st.n_spire);ps(" Spirv=");pi(st.n_spir2);ps(" Sperc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Spiraea Admin Demo ===\n\n");spir_init();
ps("Spiraea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;spir_planning(t,c,1341+(i*17),1330+(i*14),1310+(i*10),1292+(i*6),2020+(i%5));}
ps("\nSpiraea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;spir_execution(t,c,1330+(i*15),1319+(i*12),1301+(i*8),1288+(i*5),2021+(i%4));}
ps("\nSpiraea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;spir_evaluation(t,c,1322+(i*13),1311+(i*10),1295+(i*7),1284+(i*4),2022+(i%3));}
ps("\nSpiraea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spir_accessory(t,c,1314+(i*11),1305+(i*9),1291+(i*6),1281+(i*3),2023+(i%2));}
ps("\nSpiraea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;spir_market(t,c,1308+(i*9),1299+(i*7),1286+(i*5),1278+(i*3),2024);}
ps("\n");spir_report();spir_state();ps("\n=== Demo Complete ===\n");return 0;}
