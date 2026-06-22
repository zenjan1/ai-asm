/* huernia_admin: Huernia management technology administration (v1.0)
 * Huernia planning, huernia execution, huernia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hui_t;
typedef struct{int n_huip,n_huie,n_hui2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hui_state_t;
static hui_t huips[N],huiss[N-2],huivss[N-4],huias[N-6],huimks[N-6]; static hui_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hui_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hui_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hui_init(void){if(init)return -1;st.n_huip=0;st.n_huie=0;st.n_hui2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)huips[i].active=0;for(int i=0;i<N-2;i++)huiss[i].active=0;for(int i=0;i<N-4;i++)huivss[i].active=0;for(int i=0;i<N-6;i++)huias[i].active=0;for(int i=0;i<N-6;i++)huimks[i].active=0;init=1;ps("[HUI] Huernia initialized\n");return 0;}
int hui_planning(int t,int c,int a,int b,int d,int e,int y){return add(huips,&st.n_huip,&st.t_f1,N,t,c,a,b,d,e,y);}
int hui_execution(int t,int c,int a,int b,int d,int e,int y){return add(huiss,&st.n_huie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hui_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(huivss,&st.n_hui2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hui_accessory(int t,int c,int a,int b,int d,int e,int y){return add(huias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hui_market(int t,int c,int a,int b,int d,int e,int y){return add(huimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hui_report(void){ps("[HUI] Huep: ");pi(st.n_huip);ps(" PCS=");pi(st.t_f1);ps("\nHuee: ");pi(st.n_huie);ps(" PCS=");pi(st.t_f2);ps("\nHuev: ");pi(st.n_hui2);ps(" PCS=");pi(st.t_f3);ps("\nHuec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hui_state(void){ps("[HUI] Huep=");pi(st.n_huip);ps(" Huee=");pi(st.n_huie);ps(" Huev=");pi(st.n_hui2);ps(" Huec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Huernia Admin Demo ===\n\n");hui_init();
ps("Huernia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hui_planning(t,c,1294+(i*17),1283+(i*14),1263+(i*10),1245+(i*6),2020+(i%5));}
ps("\nHuernia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hui_execution(t,c,1283+(i*15),1272+(i*12),1254+(i*8),1241+(i*5),2021+(i%4));}
ps("\nHuernia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hui_evaluation(t,c,1275+(i*13),1264+(i*10),1248+(i*7),1237+(i*4),2022+(i%3));}
ps("\nHuernia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hui_accessory(t,c,1267+(i*11),1258+(i*9),1244+(i*6),1234+(i*3),2023+(i%2));}
ps("\nHuernia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hui_market(t,c,1261+(i*9),1252+(i*7),1239+(i*5),1231+(i*3),2024);}
ps("\n");hui_report();hui_state();ps("\n=== Demo Complete ===\n");return 0;}
