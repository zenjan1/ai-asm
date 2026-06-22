/* euryops_admin: Euryops management technology administration (v1.0)
 * Euryops planning, euryops execution, euryops evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eur_t;
typedef struct{int n_eurp,n_eure,n_eur2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eur_state_t;
static eur_t eurps[N],eurss[N-2],eurvss[N-4],euras[N-6],eurmks[N-6]; static eur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eur_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eur_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eur_init(void){if(init)return -1;st.n_eurp=0;st.n_eure=0;st.n_eur2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eurps[i].active=0;for(int i=0;i<N-2;i++)eurss[i].active=0;for(int i=0;i<N-4;i++)eurvss[i].active=0;for(int i=0;i<N-6;i++)euras[i].active=0;for(int i=0;i<N-6;i++)eurmks[i].active=0;init=1;ps("[EUR] Euryops initialized\n");return 0;}
int eur_planning(int t,int c,int a,int b,int d,int e,int y){return add(eurps,&st.n_eurp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eur_execution(int t,int c,int a,int b,int d,int e,int y){return add(eurss,&st.n_eure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eurvss,&st.n_eur2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(euras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eur_market(int t,int c,int a,int b,int d,int e,int y){return add(eurmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eur_report(void){ps("[EUR] Eurp: ");pi(st.n_eurp);ps(" PCS=");pi(st.t_f1);ps("\nEure: ");pi(st.n_eure);ps(" PCS=");pi(st.t_f2);ps("\nEurv: ");pi(st.n_eur2);ps(" PCS=");pi(st.t_f3);ps("\nEurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eur_state(void){ps("[EUR] Eurp=");pi(st.n_eurp);ps(" Eure=");pi(st.n_eure);ps(" Eurv=");pi(st.n_eur2);ps(" Eurc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Euryops Admin Demo ===\n\n");eur_init();
ps("Euryops planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eur_planning(t,c,1284+(i*17),1273+(i*14),1253+(i*10),1235+(i*6),2020+(i%5));}
ps("\nEuryops execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eur_execution(t,c,1273+(i*15),1262+(i*12),1244+(i*8),1231+(i*5),2021+(i%4));}
ps("\nEuryops evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eur_evaluation(t,c,1265+(i*13),1254+(i*10),1238+(i*7),1227+(i*4),2022+(i%3));}
ps("\nEuryops accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eur_accessory(t,c,1257+(i*11),1248+(i*9),1234+(i*6),1224+(i*3),2023+(i%2));}
ps("\nEuryops marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eur_market(t,c,1251+(i*9),1242+(i*7),1229+(i*5),1221+(i*3),2024);}
ps("\n");eur_report();eur_state();ps("\n=== Demo Complete ===\n");return 0;}
