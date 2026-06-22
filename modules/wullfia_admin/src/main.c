/* wullfia_admin: Wullfia management technology administration (v1.0)
 * Wullfia planning, wullfia execution, wullfia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wull_t;
typedef struct{int n_wullp,n_wull,n_wull2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wull_state_t;
static wull_t wulls[N],wulle[N-2],wullv[N-4],wullac[N-6],wullm[N-6]; static wull_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(wull_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wull_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WULL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wull_init(void){if(init)return -1;st.n_wullp=0;st.n_wull=0;st.n_wull2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wulls[i].active=0;for(int i=0;i<N-2;i++)wulle[i].active=0;for(int i=0;i<N-4;i++)wullv[i].active=0;for(int i=0;i<N-6;i++)wullac[i].active=0;for(int i=0;i<N-6;i++)wullm[i].active=0;init=1;ps("[WULL] Wullfia initialized\n");return 0;}
int wull_planning(int t,int c,int a,int b,int d,int e,int y){return add(wulls,&st.n_wullp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wull_execution(int t,int c,int a,int b,int d,int e,int y){return add(wulle,&st.n_wull,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wull_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wullv,&st.n_wull2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wull_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wullac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wull_market(int t,int c,int a,int b,int d,int e,int y){return add(wullm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wull_report(void){ps("[WULL] Wullp: ");pi(st.n_wullp);ps(" PCS=");pi(st.t_f1);ps("\nWulle: ");pi(st.n_wull);ps(" PCS=");pi(st.t_f2);ps("\nWullv: ");pi(st.n_wull2);ps(" PCS=");pi(st.t_f3);ps("\nWullc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wull_state(void){ps("[WULL] Wullp=");pi(st.n_wullp);ps(" Wulle=");pi(st.n_wull);ps(" Wullv=");pi(st.n_wull2);ps(" Wullc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wullfia Admin Demo ===\n\n");wull_init();
ps("Wullfia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wull_planning(t,c,1375+(i*17),1364+(i*14),1344+(i*10),1326+(i*6),2020+(i%5));}
ps("\nWullfia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wull_execution(t,c,1364+(i*15),1353+(i*12),1335+(i*8),1322+(i*5),2021+(i%4));}
ps("\nWullfia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wull_evaluation(t,c,1356+(i*13),1345+(i*10),1329+(i*7),1318+(i*4),2022+(i%3));}
ps("\nWullfia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wull_accessory(t,c,1348+(i*11),1339+(i*9),1325+(i*6),1315+(i*3),2023+(i%2));}
ps("\nWullfia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wull_market(t,c,1342+(i*9),1333+(i*7),1320+(i*5),1312+(i*3),2024);}
ps("\n");wull_report();wull_state();ps("\n=== Demo Complete ===\n");return 0;}
