/* triticum_admin: Triticum management technology administration (v1.0)
 * Triticum planning, triticum execution, triticum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trit_t;
typedef struct{int n_tritp,n_trite,n_trit2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trit_state_t;
static trit_t trits[N],tritess[N-2],trivss[N-4],tritas[N-6],tritmks[N-6]; static trit_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(trit_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trit_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRIT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trit_init(void){if(init)return -1;st.n_tritp=0;st.n_trite=0;st.n_trit2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trits[i].active=0;for(int i=0;i<N-2;i++)tritess[i].active=0;for(int i=0;i<N-4;i++)trivss[i].active=0;for(int i=0;i<N-6;i++)tritas[i].active=0;for(int i=0;i<N-6;i++)tritmks[i].active=0;init=1;ps("[TRIT] Triticum initialized\n");return 0;}
int trit_planning(int t,int c,int a,int b,int d,int e,int y){return add(trits,&st.n_tritp,&st.t_f1,N,t,c,a,b,d,e,y);}
int trit_execution(int t,int c,int a,int b,int d,int e,int y){return add(tritess,&st.n_trite,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trit_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(trivss,&st.n_trit2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trit_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tritas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trit_market(int t,int c,int a,int b,int d,int e,int y){return add(tritmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trit_report(void){ps("[TRIT] Tritp: ");pi(st.n_tritp);ps(" PCS=");pi(st.t_f1);ps("\nTrite: ");pi(st.n_trite);ps(" PCS=");pi(st.t_f2);ps("\nTriv: ");pi(st.n_trit2);ps(" PCS=");pi(st.t_f3);ps("\nTritc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trit_state(void){ps("[TRIT] Tritp=");pi(st.n_tritp);ps(" Trite=");pi(st.n_trite);ps(" Triv=");pi(st.n_trit2);ps(" Tritc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Triticum Admin Demo ===\n\n");trit_init();
ps("Triticum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trit_planning(t,c,1350+(i*17),1339+(i*14),1319+(i*10),1301+(i*6),2020+(i%5));}
ps("\nTriticum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trit_execution(t,c,1339+(i*15),1328+(i*12),1310+(i*8),1297+(i*5),2021+(i%4));}
ps("\nTriticum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trit_evaluation(t,c,1331+(i*13),1320+(i*10),1304+(i*7),1293+(i*4),2022+(i%3));}
ps("\nTriticum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trit_accessory(t,c,1323+(i*11),1314+(i*9),1300+(i*6),1290+(i*3),2023+(i%2));}
ps("\nTriticum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trit_market(t,c,1317+(i*9),1308+(i*7),1295+(i*5),1287+(i*3),2024);}
ps("\n");trit_report();trit_state();ps("\n=== Demo Complete ===\n");return 0;}
