/* nubigena_admin: Nubigena management technology administration (v1.0)
 * Nubigena planning, nubigena execution, nubigena evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nub_t;
typedef struct{int n_nubp,n_nube,n_nub2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nub_state_t;
static nub_t nubps[N],nubess[N-2],nubvss[N-4],nubas[N-6],nubmks[N-6]; static nub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(nub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NUB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nub_init(void){if(init)return -1;st.n_nubp=0;st.n_nube=0;st.n_nub2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nubps[i].active=0;for(int i=0;i<N-2;i++)nubess[i].active=0;for(int i=0;i<N-4;i++)nubvss[i].active=0;for(int i=0;i<N-6;i++)nubas[i].active=0;for(int i=0;i<N-6;i++)nubmks[i].active=0;init=1;ps("[NUB] Nubigena initialized\n");return 0;}
int nub_planning(int t,int c,int a,int b,int d,int e,int y){return add(nubps,&st.n_nubp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nub_execution(int t,int c,int a,int b,int d,int e,int y){return add(nubess,&st.n_nube,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nub_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nubvss,&st.n_nub2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nub_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nubas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nub_market(int t,int c,int a,int b,int d,int e,int y){return add(nubmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nub_report(void){ps("[NUB] Nubp: ");pi(st.n_nubp);ps(" PCS=");pi(st.t_f1);ps("\nNube: ");pi(st.n_nube);ps(" PCS=");pi(st.t_f2);ps("\nNubv: ");pi(st.n_nub2);ps(" PCS=");pi(st.t_f3);ps("\nNubc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nub_state(void){ps("[NUB] Nubp=");pi(st.n_nubp);ps(" Nube=");pi(st.n_nube);ps(" Nubv=");pi(st.n_nub2);ps(" Nubc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nubigena Admin Demo ===\n\n");nub_init();
ps("Nubigena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nub_planning(t,c,1319+(i*17),1308+(i*14),1288+(i*10),1270+(i*6),2020+(i%5));}
ps("\nNubigena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nub_execution(t,c,1308+(i*15),1297+(i*12),1279+(i*8),1266+(i*5),2021+(i%4));}
ps("\nNubigena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nub_evaluation(t,c,1300+(i*13),1289+(i*10),1273+(i*7),1262+(i*4),2022+(i%3));}
ps("\nNubigena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nub_accessory(t,c,1292+(i*11),1283+(i*9),1269+(i*6),1259+(i*3),2023+(i%2));}
ps("\nNubigena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nub_market(t,c,1286+(i*9),1277+(i*7),1264+(i*5),1256+(i*3),2024);}
ps("\n");nub_report();nub_state();ps("\n=== Demo Complete ===\n");return 0;}
