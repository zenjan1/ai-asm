/* hura_admin: Hura management technology administration (v1.0)
 * Hura planning, hura execution, hura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hur_t;
typedef struct{int n_hurp,n_hure,n_hur2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hur_state_t;
static hur_t hurps[N],hurss[N-2],hurvss[N-4],huras[N-6],hurmks[N-6]; static hur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hur_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hur_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hur_init(void){if(init)return -1;st.n_hurp=0;st.n_hure=0;st.n_hur2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hurps[i].active=0;for(int i=0;i<N-2;i++)hurss[i].active=0;for(int i=0;i<N-4;i++)hurvss[i].active=0;for(int i=0;i<N-6;i++)huras[i].active=0;for(int i=0;i<N-6;i++)hurmks[i].active=0;init=1;ps("[HUR] Hura initialized\n");return 0;}
int hur_planning(int t,int c,int a,int b,int d,int e,int y){return add(hurps,&st.n_hurp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hur_execution(int t,int c,int a,int b,int d,int e,int y){return add(hurss,&st.n_hure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hurvss,&st.n_hur2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(huras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hur_market(int t,int c,int a,int b,int d,int e,int y){return add(hurmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hur_report(void){ps("[HUR] Hurp: ");pi(st.n_hurp);ps(" PCS=");pi(st.t_f1);ps("\nHure: ");pi(st.n_hure);ps(" PCS=");pi(st.t_f2);ps("\nHurv: ");pi(st.n_hur2);ps(" PCS=");pi(st.t_f3);ps("\nHurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hur_state(void){ps("[HUR] Hurp=");pi(st.n_hurp);ps(" Hure=");pi(st.n_hure);ps(" Hurv=");pi(st.n_hur2);ps(" Hurc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hura Admin Demo ===\n\n");hur_init();
ps("Hura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hur_planning(t,c,1297+(i*17),1286+(i*14),1266+(i*10),1248+(i*6),2020+(i%5));}
ps("\nHura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hur_execution(t,c,1286+(i*15),1275+(i*12),1257+(i*8),1244+(i*5),2021+(i%4));}
ps("\nHura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hur_evaluation(t,c,1278+(i*13),1267+(i*10),1251+(i*7),1240+(i*4),2022+(i%3));}
ps("\nHura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hur_accessory(t,c,1270+(i*11),1261+(i*9),1247+(i*6),1237+(i*3),2023+(i%2));}
ps("\nHura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hur_market(t,c,1264+(i*9),1255+(i*7),1242+(i*5),1234+(i*3),2024);}
ps("\n");hur_report();hur_state();ps("\n=== Demo Complete ===\n");return 0;}
