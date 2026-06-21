/* tolmiea_admin: Tolmiea management technology administration (v1.0)
 * Tolmiea planning, tolmiea execution, tolmiea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tol_t;
typedef struct{int n_tolp,n_tole,n_tol2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tol_state_t;
static tol_t tolps[N],tolss[N-2],tolvss[N-4],tolas[N-6],tolmks[N-6]; static tol_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tol_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tol_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tol_init(void){if(init)return -1;st.n_tolp=0;st.n_tole=0;st.n_tol2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tolps[i].active=0;for(int i=0;i<N-2;i++)tolss[i].active=0;for(int i=0;i<N-4;i++)tolvss[i].active=0;for(int i=0;i<N-6;i++)tolas[i].active=0;for(int i=0;i<N-6;i++)tolmks[i].active=0;init=1;ps("[TOL] Tolmiea initialized\n");return 0;}
int tol_planning(int t,int c,int a,int b,int d,int e,int y){return add(tolps,&st.n_tolp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tol_execution(int t,int c,int a,int b,int d,int e,int y){return add(tolss,&st.n_tole,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tol_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tolvss,&st.n_tol2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tol_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tolas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tol_market(int t,int c,int a,int b,int d,int e,int y){return add(tolmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tol_report(void){ps("[TOL] Tolpp: ");pi(st.n_tolp);ps(" PCS=");pi(st.t_f1);ps("\nTole: ");pi(st.n_tole);ps(" PCS=");pi(st.t_f2);ps("\nTolv: ");pi(st.n_tol2);ps(" PCS=");pi(st.t_f3);ps("\nTolc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tol_state(void){ps("[TOL] Tolpp=");pi(st.n_tolp);ps(" Tole=");pi(st.n_tole);ps(" Tolv=");pi(st.n_tol2);ps(" Tolc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tolmiea Admin Demo ===\n\n");tol_init();
ps("Tolmiea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tol_planning(t,c,1104+(i*17),1093+(i*14),1073+(i*10),1055+(i*6),2020+(i%5));}
ps("\nTolmiea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tol_execution(t,c,1093+(i*15),1082+(i*12),1064+(i*8),1051+(i*5),2021+(i%4));}
ps("\nTolmiea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tol_evaluation(t,c,1085+(i*13),1074+(i*10),1058+(i*7),1047+(i*4),2022+(i%3));}
ps("\nTolmiea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tol_accessory(t,c,1077+(i*11),1068+(i*9),1054+(i*6),1044+(i*3),2023+(i%2));}
ps("\nTolmiea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tol_market(t,c,1071+(i*9),1062+(i*7),1049+(i*5),1041+(i*3),2024);}
ps("\n");tol_report();tol_state();ps("\n=== Demo Complete ===\n");return 0;}
