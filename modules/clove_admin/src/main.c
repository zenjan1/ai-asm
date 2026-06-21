/* clove_admin: Clove management technology administration (v1.0)
 * Clove planning, clove execution, clove evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clv_t;
typedef struct{int n_clvp,n_clve,n_clvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} clv_state_t;
static clv_t clvps[N],clvss[N-2],clvvss[N-4],clvas[N-6],clvmks[N-6]; static clv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clv_init(void){if(init)return -1;st.n_clvp=0;st.n_clve=0;st.n_clvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)clvps[i].active=0;for(int i=0;i<N-2;i++)clvss[i].active=0;for(int i=0;i<N-4;i++)clvvss[i].active=0;for(int i=0;i<N-6;i++)clvas[i].active=0;for(int i=0;i<N-6;i++)clvmks[i].active=0;init=1;ps("[CLV] Clove initialized\n");return 0;}
int clv_planning(int t,int c,int a,int b,int d,int e,int y){return add(clvps,&st.n_clvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int clv_execution(int t,int c,int a,int b,int d,int e,int y){return add(clvss,&st.n_clve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(clvvss,&st.n_clvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(clvas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clv_market(int t,int c,int a,int b,int d,int e,int y){return add(clvmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clv_report(void){ps("[CLV] Clvp: ");pi(st.n_clvp);ps(" PCS=");pi(st.t_f1);ps("\nClve: ");pi(st.n_clve);ps(" PCS=");pi(st.t_f2);ps("\nClvv: ");pi(st.n_clvv);ps(" PCS=");pi(st.t_f3);ps("\nClvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clv_state(void){ps("[CLV] Clvp=");pi(st.n_clvp);ps(" Clve=");pi(st.n_clve);ps(" Clvv=");pi(st.n_clvv);ps(" Clvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clove Admin Demo ===\n\n");clv_init();
ps("Clove planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clv_planning(t,c,671+(i*17),660+(i*14),640+(i*10),622+(i*6),2020+(i%5));}
ps("\nClove execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clv_execution(t,c,660+(i*15),649+(i*12),631+(i*8),618+(i*5),2021+(i%4));}
ps("\nClove evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clv_evaluation(t,c,652+(i*13),641+(i*10),625+(i*7),614+(i*4),2022+(i%3));}
ps("\nClove accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clv_accessory(t,c,644+(i*11),635+(i*9),621+(i*6),611+(i*3),2023+(i%2));}
ps("\nClove marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clv_market(t,c,638+(i*9),629+(i*7),616+(i*5),608+(i*3),2024);}
ps("\n");clv_report();clv_state();ps("\n=== Demo Complete ===\n");return 0;}
