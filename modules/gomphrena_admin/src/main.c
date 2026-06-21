/* gomphrena_admin: Gomphrena management technology administration (v1.0)
 * Gomphrena planning, gomphrena execution, gomphrena evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gom_t;
typedef struct{int n_gop,n_goe,n_gov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gom_state_t;
static gom_t gops[N],goes[N-2],govs[N-4],acs[N-6],mks[N-6]; static gom_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gom_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gom_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GOM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gom_init(void){if(init)return -1;st.n_gop=0;st.n_goe=0;st.n_gov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gops[i].active=0;for(int i=0;i<N-2;i++)goes[i].active=0;for(int i=0;i<N-4;i++)govs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GOM] Gomphrena initialized\n");return 0;}
int gom_planning(int t,int c,int a,int b,int d,int e,int y){return add(gops,&st.n_gop,&st.t_f1,N,t,c,a,b,d,e,y);}
int gom_execution(int t,int c,int a,int b,int d,int e,int y){return add(goes,&st.n_goe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gom_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(govs,&st.n_gov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gom_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gom_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gom_report(void){ps("[GOM] Gop: ");pi(st.n_gop);ps(" PCS=");pi(st.t_f1);ps("\nGoe: ");pi(st.n_goe);ps(" PCS=");pi(st.t_f2);ps("\nGov: ");pi(st.n_gov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gom_state(void){ps("[GOM] Gop=");pi(st.n_gop);ps(" Goe=");pi(st.n_goe);ps(" Gov=");pi(st.n_gov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gomphrena Admin Demo ===\n\n");gom_init();
ps("Gomphrena planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gom_planning(t,c,596+(i*17),585+(i*14),565+(i*10),547+(i*6),2020+(i%5));}
ps("\nGomphrena execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gom_execution(t,c,585+(i*15),574+(i*12),556+(i*8),543+(i*5),2021+(i%4));}
ps("\nGomphrena evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gom_evaluation(t,c,577+(i*13),566+(i*10),550+(i*7),539+(i*4),2022+(i%3));}
ps("\nGomphrena accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gom_accessory(t,c,569+(i*11),560+(i*9),546+(i*6),536+(i*3),2023+(i%2));}
ps("\nGomphrena marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gom_market(t,c,563+(i*9),554+(i*7),541+(i*5),533+(i*3),2024);}
ps("\n");gom_report();gom_state();ps("\n=== Demo Complete ===\n");return 0;}
