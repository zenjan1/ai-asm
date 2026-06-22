/* wollemia_admin: Wollemia management technology administration (v1.0)
 * Wollemia planning, wollemia execution, wollemia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} woll_t;
typedef struct{int n_wollp,n_woll,n_woll2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} woll_state_t;
static woll_t wolls[N],wolle[N-2],wolv2[N-4],wollac[N-6],wollm[N-6]; static woll_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(woll_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;woll_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WOLL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int woll_init(void){if(init)return -1;st.n_wollp=0;st.n_woll=0;st.n_woll2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wolls[i].active=0;for(int i=0;i<N-2;i++)wolle[i].active=0;for(int i=0;i<N-4;i++)wolv2[i].active=0;for(int i=0;i<N-6;i++)wollac[i].active=0;for(int i=0;i<N-6;i++)wollm[i].active=0;init=1;ps("[WOLL] Wollemia initialized\n");return 0;}
int woll_planning(int t,int c,int a,int b,int d,int e,int y){return add(wolls,&st.n_wollp,&st.t_f1,N,t,c,a,b,d,e,y);}
int woll_execution(int t,int c,int a,int b,int d,int e,int y){return add(wolle,&st.n_woll,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int woll_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wolv2,&st.n_woll2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int woll_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wollac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int woll_market(int t,int c,int a,int b,int d,int e,int y){return add(wollm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void woll_report(void){ps("[WOLL] Wollp: ");pi(st.n_wollp);ps(" PCS=");pi(st.t_f1);ps("\nWolle: ");pi(st.n_woll);ps(" PCS=");pi(st.t_f2);ps("\nWollv: ");pi(st.n_woll2);ps(" PCS=");pi(st.t_f3);ps("\nWollc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void woll_state(void){ps("[WOLL] Wollp=");pi(st.n_wollp);ps(" Wolle=");pi(st.n_woll);ps(" Wollv=");pi(st.n_woll2);ps(" Wollc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wollemia Admin Demo ===\n\n");woll_init();
ps("Wollemia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;woll_planning(t,c,1373+(i*17),1362+(i*14),1342+(i*10),1324+(i*6),2020+(i%5));}
ps("\nWollemia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;woll_execution(t,c,1362+(i*15),1351+(i*12),1333+(i*8),1320+(i*5),2021+(i%4));}
ps("\nWollemia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;woll_evaluation(t,c,1354+(i*13),1343+(i*10),1327+(i*7),1316+(i*4),2022+(i%3));}
ps("\nWollemia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;woll_accessory(t,c,1346+(i*11),1337+(i*9),1323+(i*6),1313+(i*3),2023+(i%2));}
ps("\nWollemia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;woll_market(t,c,1340+(i*9),1331+(i*7),1318+(i*5),1310+(i*3),2024);}
ps("\n");woll_report();woll_state();ps("\n=== Demo Complete ===\n");return 0;}
