/* savory_admin: Savory management technology administration (v1.0)
 * Savory planning, savory execution, savory evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sy_t;
typedef struct{int n_syp,n_sye,n_syv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sy_state_t;
static sy_t syps[N],syss[N-2],syvss[N-4],syas[N-6],symks[N-6]; static sy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SVY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sy_init(void){if(init)return -1;st.n_syp=0;st.n_sye=0;st.n_syv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)syps[i].active=0;for(int i=0;i<N-2;i++)syss[i].active=0;for(int i=0;i<N-4;i++)syvss[i].active=0;for(int i=0;i<N-6;i++)syas[i].active=0;for(int i=0;i<N-6;i++)symks[i].active=0;init=1;ps("[SVY] Savory initialized\n");return 0;}
int sy_planning(int t,int c,int a,int b,int d,int e,int y){return add(syps,&st.n_syp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sy_execution(int t,int c,int a,int b,int d,int e,int y){return add(syss,&st.n_sye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sy_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(syvss,&st.n_syv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sy_accessory(int t,int c,int a,int b,int d,int e,int y){return add(syas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sy_market(int t,int c,int a,int b,int d,int e,int y){return add(symks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sy_report(void){ps("[SVY] Syp: ");pi(st.n_syp);ps(" PCS=");pi(st.t_f1);ps("\nSye: ");pi(st.n_sye);ps(" PCS=");pi(st.t_f2);ps("\nSyv: ");pi(st.n_syv);ps(" PCS=");pi(st.t_f3);ps("\nSyc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sy_state(void){ps("[SVY] Syp=");pi(st.n_syp);ps(" Sye=");pi(st.n_sye);ps(" Syv=");pi(st.n_syv);ps(" Syc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Savory Admin Demo ===\n\n");sy_init();
ps("Savory planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sy_planning(t,c,662+(i*17),651+(i*14),631+(i*10),613+(i*6),2020+(i%5));}
ps("\nSavory execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sy_execution(t,c,651+(i*15),640+(i*12),622+(i*8),609+(i*5),2021+(i%4));}
ps("\nSavory evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sy_evaluation(t,c,643+(i*13),632+(i*10),616+(i*7),605+(i*4),2022+(i%3));}
ps("\nSavory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sy_accessory(t,c,635+(i*11),626+(i*9),612+(i*6),602+(i*3),2023+(i%2));}
ps("\nSavory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sy_market(t,c,629+(i*9),620+(i*7),607+(i*5),599+(i*3),2024);}
ps("\n");sy_report();sy_state();ps("\n=== Demo Complete ===\n");return 0;}
