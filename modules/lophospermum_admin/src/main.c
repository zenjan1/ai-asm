/* lophospermum_admin: Lophospermum management technology administration (v1.0)
 * Lophospermum planning, lophospermum execution, lophospermum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lop_t;
typedef struct{int n_lopp,n_lope,n_lop2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lop_state_t;
static lop_t lopps[N],lopess[N-2],lopvss[N-4],lopas[N-6],lopmks[N-6]; static lop_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lop_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lop_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lop_init(void){if(init)return -1;st.n_lopp=0;st.n_lope=0;st.n_lop2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lopps[i].active=0;for(int i=0;i<N-2;i++)lopess[i].active=0;for(int i=0;i<N-4;i++)lopvss[i].active=0;for(int i=0;i<N-6;i++)lopas[i].active=0;for(int i=0;i<N-6;i++)lopmks[i].active=0;init=1;ps("[LOP] Lophospermum initialized\n");return 0;}
int lop_planning(int t,int c,int a,int b,int d,int e,int y){return add(lopps,&st.n_lopp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lop_execution(int t,int c,int a,int b,int d,int e,int y){return add(lopess,&st.n_lope,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lop_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lopvss,&st.n_lop2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lop_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lopas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lop_market(int t,int c,int a,int b,int d,int e,int y){return add(lopmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lop_report(void){ps("[LOP] Lopp: ");pi(st.n_lopp);ps(" PCS=");pi(st.t_f1);ps("\nLope: ");pi(st.n_lope);ps(" PCS=");pi(st.t_f2);ps("\nLopv: ");pi(st.n_lop2);ps(" PCS=");pi(st.t_f3);ps("\nLopc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lop_state(void){ps("[LOP] Lopp=");pi(st.n_lopp);ps(" Lope=");pi(st.n_lope);ps(" Lopv=");pi(st.n_lop2);ps(" Lopc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lophospermum Admin Demo ===\n\n");lop_init();
ps("Lophospermum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lop_planning(t,c,1310+(i*17),1299+(i*14),1279+(i*10),1261+(i*6),2020+(i%5));}
ps("\nLophospermum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lop_execution(t,c,1299+(i*15),1288+(i*12),1270+(i*8),1257+(i*5),2021+(i%4));}
ps("\nLophospermum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lop_evaluation(t,c,1291+(i*13),1280+(i*10),1264+(i*7),1253+(i*4),2022+(i%3));}
ps("\nLophospermum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lop_accessory(t,c,1283+(i*11),1274+(i*9),1260+(i*6),1250+(i*3),2023+(i%2));}
ps("\nLophospermum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lop_market(t,c,1277+(i*9),1268+(i*7),1255+(i*5),1247+(i*3),2024);}
ps("\n");lop_report();lop_state();ps("\n=== Demo Complete ===\n");return 0;}
