/* ephedra_admin: Ephedra management technology administration (v1.0)
 * Ephedra planning, ephedra execution, ephedra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eph_t;
typedef struct{int n_ephp,n_ephe,n_eph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eph_state_t;
static eph_t ephps[N],ephss[N-2],ephvss[N-4],ephas[N-6],ephmks[N-6]; static eph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eph_init(void){if(init)return -1;st.n_ephp=0;st.n_ephe=0;st.n_eph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ephps[i].active=0;for(int i=0;i<N-2;i++)ephss[i].active=0;for(int i=0;i<N-4;i++)ephvss[i].active=0;for(int i=0;i<N-6;i++)ephas[i].active=0;for(int i=0;i<N-6;i++)ephmks[i].active=0;init=1;ps("[EPH] Ephedra initialized\n");return 0;}
int eph_planning(int t,int c,int a,int b,int d,int e,int y){return add(ephps,&st.n_ephp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eph_execution(int t,int c,int a,int b,int d,int e,int y){return add(ephss,&st.n_ephe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ephvss,&st.n_eph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ephas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eph_market(int t,int c,int a,int b,int d,int e,int y){return add(ephmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eph_report(void){ps("[EPH] Ephp: ");pi(st.n_ephp);ps(" PCS=");pi(st.t_f1);ps("\nEphe: ");pi(st.n_ephe);ps(" PCS=");pi(st.t_f2);ps("\nEphv: ");pi(st.n_eph2);ps(" PCS=");pi(st.t_f3);ps("\nEphc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eph_state(void){ps("[EPH] Ephp=");pi(st.n_ephp);ps(" Ephe=");pi(st.n_ephe);ps(" Ephv=");pi(st.n_eph2);ps(" Ephc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ephedra Admin Demo ===\n\n");eph_init();
ps("Ephedra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eph_planning(t,c,1277+(i*17),1266+(i*14),1246+(i*10),1228+(i*6),2020+(i%5));}
ps("\nEphedra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eph_execution(t,c,1266+(i*15),1255+(i*12),1237+(i*8),1224+(i*5),2021+(i%4));}
ps("\nEphedra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eph_evaluation(t,c,1258+(i*13),1247+(i*10),1231+(i*7),1220+(i*4),2022+(i%3));}
ps("\nEphedra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eph_accessory(t,c,1250+(i*11),1241+(i*9),1227+(i*6),1217+(i*3),2023+(i%2));}
ps("\nEphedra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eph_market(t,c,1244+(i*9),1235+(i*7),1222+(i*5),1214+(i*3),2024);}
ps("\n");eph_report();eph_state();ps("\n=== Demo Complete ===\n");return 0;}
