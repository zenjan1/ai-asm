/* wulffia_admin: Wulffia management technology administration (v1.0)
 * Wulffia planning, wulffia execution, wulffia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wulf_t;
typedef struct{int n_wulfp,n_wulf,n_wulf2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wulf_state_t;
static wulf_t wulfs[N],wulfe[N-2],wulfv[N-4],wulfac[N-6],wulm[N-6]; static wulf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(wulf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wulf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WULF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wulf_init(void){if(init)return -1;st.n_wulfp=0;st.n_wulf=0;st.n_wulf2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wulfs[i].active=0;for(int i=0;i<N-2;i++)wulfe[i].active=0;for(int i=0;i<N-4;i++)wulfv[i].active=0;for(int i=0;i<N-6;i++)wulfac[i].active=0;for(int i=0;i<N-6;i++)wulm[i].active=0;init=1;ps("[WULF] Wulffia initialized\n");return 0;}
int wulf_planning(int t,int c,int a,int b,int d,int e,int y){return add(wulfs,&st.n_wulfp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wulf_execution(int t,int c,int a,int b,int d,int e,int y){return add(wulfe,&st.n_wulf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wulf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wulfv,&st.n_wulf2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wulf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wulfac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wulf_market(int t,int c,int a,int b,int d,int e,int y){return add(wulm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wulf_report(void){ps("[WULF] Wulfp: ");pi(st.n_wulfp);ps(" PCS=");pi(st.t_f1);ps("\nWulfe: ");pi(st.n_wulf);ps(" PCS=");pi(st.t_f2);ps("\nWulfv: ");pi(st.n_wulf2);ps(" PCS=");pi(st.t_f3);ps("\nWulac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wulf_state(void){ps("[WULF] Wulfp=");pi(st.n_wulfp);ps(" Wulfe=");pi(st.n_wulf);ps(" Wulfv=");pi(st.n_wulf2);ps(" Wulac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wulffia Admin Demo ===\n\n");wulf_init();
ps("Wulffia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wulf_planning(t,c,1374+(i*17),1363+(i*14),1343+(i*10),1325+(i*6),2020+(i%5));}
ps("\nWulffia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wulf_execution(t,c,1363+(i*15),1352+(i*12),1334+(i*8),1321+(i*5),2021+(i%4));}
ps("\nWulffia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wulf_evaluation(t,c,1355+(i*13),1344+(i*10),1328+(i*7),1317+(i*4),2022+(i%3));}
ps("\nWulffia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wulf_accessory(t,c,1347+(i*11),1338+(i*9),1324+(i*6),1314+(i*3),2023+(i%2));}
ps("\nWulffia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wulf_market(t,c,1341+(i*9),1332+(i*7),1319+(i*5),1311+(i*3),2024);}
ps("\n");wulf_report();wulf_state();ps("\n=== Demo Complete ===\n");return 0;}
