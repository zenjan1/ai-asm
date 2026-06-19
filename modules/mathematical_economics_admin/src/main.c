/* mathematical_economics_admin: Mathematical economics administration (v1.0)
 * Mathematical foundations, optimization theory, equilibrium theory, game mathematics, dynamic systems
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mat_t;
typedef struct{int n_mf,n_ot,n_eq,n_gm,n_ds,t_f1,t_f2,t_f3,t_f4,t_f5;} mat_state_t;
static mat_t mfs[N],ots[N-2],eqs[N-4],gms[N-6],dss[N-6]; static mat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mat_init(void){if(init)return -1;st.n_mf=0;st.n_ot=0;st.n_eq=0;st.n_gm=0;st.n_ds=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mfs[i].active=0;for(int i=0;i<N-2;i++)ots[i].active=0;for(int i=0;i<N-4;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)gms[i].active=0;for(int i=0;i<N-6;i++)dss[i].active=0;init=1;ps("[MAT] Mathematical economics initialized\n");return 0;}
int mat_foundation(int t,int c,int a,int b,int d,int e,int y){return add(mfs,&st.n_mf,&st.t_f1,N,t,c,a,b,d,e,y);}
int mat_optimize(int t,int c,int a,int b,int d,int e,int y){return add(ots,&st.n_ot,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mat_equilibrium(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mat_game(int t,int c,int a,int b,int d,int e,int y){return add(gms,&st.n_gm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mat_dynamic(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mat_report(void){ps("[MAT] Found: ");pi(st.n_mf);ps(" Topo=");pi(st.t_f1);ps("\nOptim: ");pi(st.n_ot);ps(" Convex=");pi(st.t_f2);ps("\nEquil: ");pi(st.n_eq);ps(" Exist=");pi(st.t_f3);ps("\nGame: ");pi(st.n_gm);ps(" Bayes=");pi(st.t_f4);ps("\nDynamic: ");pi(st.n_ds);ps(" Chaos=");pi(st.t_f5);ps("\n");}
void mat_state(void){ps("[MAT] Mf=");pi(st.n_mf);ps(" Ot=");pi(st.n_ot);ps(" Eq=");pi(st.n_eq);ps(" Gm=");pi(st.n_gm);ps(" Ds=");pi(st.n_ds);ps("\n");}
int main(void){
ps("=== Mathematical Economics Admin Demo ===\n\n");mat_init();
ps("Mathematical foundations...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mat_foundation(t,c,99+(i*17),84+(i*14),64+(i*10),47+(i*6),2020+(i%5));}
ps("\nOptimization theory...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mat_optimize(t,c,88+(i*15),74+(i*12),56+(i*8),43+(i*5),2021+(i%4));}
ps("\nEquilibrium theory...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mat_equilibrium(t,c,80+(i*13),66+(i*10),50+(i*7),39+(i*4),2022+(i%3));}
ps("\nGame mathematics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mat_game(t,c,72+(i*11),60+(i*9),46+(i*6),36+(i*3),2023+(i%2));}
ps("\nDynamic systems...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mat_dynamic(t,c,66+(i*9),55+(i*7),42+(i*5),34+(i*3),2024);}
ps("\n");mat_report();mat_state();ps("\n=== Demo Complete ===\n");return 0;}
