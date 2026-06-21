/* aloe_admin: Aloe management technology administration (v1.0)
 * Aloe planning, aloe execution, aloe evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alo_t;
typedef struct{int n_alop,n_aloe,n_alo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alo_state_t;
static alo_t alops[N],aloss[N-2],alovss[N-4],aloas[N-6],alomks[N-6]; static alo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alo_init(void){if(init)return -1;st.n_alop=0;st.n_aloe=0;st.n_alo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)alops[i].active=0;for(int i=0;i<N-2;i++)aloss[i].active=0;for(int i=0;i<N-4;i++)alovss[i].active=0;for(int i=0;i<N-6;i++)aloas[i].active=0;for(int i=0;i<N-6;i++)alomks[i].active=0;init=1;ps("[ALO] Aloe initialized\n");return 0;}
int alo_planning(int t,int c,int a,int b,int d,int e,int y){return add(alops,&st.n_alop,&st.t_f1,N,t,c,a,b,d,e,y);}
int alo_execution(int t,int c,int a,int b,int d,int e,int y){return add(aloss,&st.n_aloe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alovss,&st.n_alo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aloas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alo_market(int t,int c,int a,int b,int d,int e,int y){return add(alomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alo_report(void){ps("[ALO] Alop: ");pi(st.n_alop);ps(" PCS=");pi(st.t_f1);ps("\nAloe: ");pi(st.n_aloe);ps(" PCS=");pi(st.t_f2);ps("\nAlov: ");pi(st.n_alo2);ps(" PCS=");pi(st.t_f3);ps("\nAloc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alo_state(void){ps("[ALO] Alop=");pi(st.n_alop);ps(" Aloe=");pi(st.n_aloe);ps(" Alov=");pi(st.n_alo2);ps(" Aloc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aloe Admin Demo ===\n\n");alo_init();
ps("Aloe planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alo_planning(t,c,971+(i*17),960+(i*14),940+(i*10),922+(i*6),2020+(i%5));}
ps("\nAloe execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alo_execution(t,c,960+(i*15),949+(i*12),931+(i*8),918+(i*5),2021+(i%4));}
ps("\nAloe evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alo_evaluation(t,c,952+(i*13),941+(i*10),925+(i*7),914+(i*4),2022+(i%3));}
ps("\nAloe accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alo_accessory(t,c,944+(i*11),935+(i*9),921+(i*6),911+(i*3),2023+(i%2));}
ps("\nAloe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alo_market(t,c,938+(i*9),929+(i*7),916+(i*5),908+(i*3),2024);}
ps("\n");alo_report();alo_state();ps("\n=== Demo Complete ===\n");return 0;}
