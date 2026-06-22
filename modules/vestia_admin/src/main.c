/* vestia_admin: Vestia management technology administration (v1.0)
 * Vestia planning, vestia execution, vestia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vest_t;
typedef struct{int n_vestp,n_vest,n_vest2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vest_state_t;
static vest_t vests[N],vestess[N-2],vestvss[N-4],vestas[N-6],vestmks[N-6]; static vest_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(vest_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vest_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VEST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vest_init(void){if(init)return -1;st.n_vestp=0;st.n_vest=0;st.n_vest2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vests[i].active=0;for(int i=0;i<N-2;i++)vestess[i].active=0;for(int i=0;i<N-4;i++)vestvss[i].active=0;for(int i=0;i<N-6;i++)vestas[i].active=0;for(int i=0;i<N-6;i++)vestmks[i].active=0;init=1;ps("[VEST] Vestia initialized\n");return 0;}
int vest_planning(int t,int c,int a,int b,int d,int e,int y){return add(vests,&st.n_vestp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vest_execution(int t,int c,int a,int b,int d,int e,int y){return add(vestess,&st.n_vest,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vest_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vestvss,&st.n_vest2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vest_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vestas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vest_market(int t,int c,int a,int b,int d,int e,int y){return add(vestmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vest_report(void){ps("[VEST] Vestp: ");pi(st.n_vestp);ps(" PCS=");pi(st.t_f1);ps("\nVeste: ");pi(st.n_vest);ps(" PCS=");pi(st.t_f2);ps("\nVestv: ");pi(st.n_vest2);ps(" PCS=");pi(st.t_f3);ps("\nVestc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vest_state(void){ps("[VEST] Vestp=");pi(st.n_vestp);ps(" Veste=");pi(st.n_vest);ps(" Vestv=");pi(st.n_vest2);ps(" Vestc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vestia Admin Demo ===\n\n");vest_init();
ps("Vestia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vest_planning(t,c,1358+(i*17),1347+(i*14),1327+(i*10),1309+(i*6),2020+(i%5));}
ps("\nVestia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vest_execution(t,c,1347+(i*15),1336+(i*12),1318+(i*8),1305+(i*5),2021+(i%4));}
ps("\nVestia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vest_evaluation(t,c,1339+(i*13),1328+(i*10),1312+(i*7),1301+(i*4),2022+(i%3));}
ps("\nVestia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vest_accessory(t,c,1331+(i*11),1322+(i*9),1308+(i*6),1298+(i*3),2023+(i%2));}
ps("\nVestia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vest_market(t,c,1325+(i*9),1316+(i*7),1303+(i*5),1295+(i*3),2024);}
ps("\n");vest_report();vest_state();ps("\n=== Demo Complete ===\n");return 0;}
