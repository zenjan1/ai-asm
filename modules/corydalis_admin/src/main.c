/* corydalis_admin: Corydalis management technology administration (v1.0)
 * Corydalis planning, corydalis execution, corydalis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cor_t;
typedef struct{int n_corp,n_core,n_cor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cor_state_t;
static cor_t corps[N],corss[N-2],corvss[N-4],coras[N-6],cormks[N-6]; static cor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[COR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cor_init(void){if(init)return -1;st.n_corp=0;st.n_core=0;st.n_cor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)corps[i].active=0;for(int i=0;i<N-2;i++)corss[i].active=0;for(int i=0;i<N-4;i++)corvss[i].active=0;for(int i=0;i<N-6;i++)coras[i].active=0;for(int i=0;i<N-6;i++)cormks[i].active=0;init=1;ps("[COR] Corydalis initialized\n");return 0;}
int cor_planning(int t,int c,int a,int b,int d,int e,int y){return add(corps,&st.n_corp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cor_execution(int t,int c,int a,int b,int d,int e,int y){return add(corss,&st.n_core,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(corvss,&st.n_cor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(coras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cor_market(int t,int c,int a,int b,int d,int e,int y){return add(cormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cor_report(void){ps("[COR] Corp: ");pi(st.n_corp);ps(" PCS=");pi(st.t_f1);ps("\nCore: ");pi(st.n_core);ps(" PCS=");pi(st.t_f2);ps("\nCorv: ");pi(st.n_cor2);ps(" PCS=");pi(st.t_f3);ps("\nCorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cor_state(void){ps("[COR] Corp=");pi(st.n_corp);ps(" Core=");pi(st.n_core);ps(" Corv=");pi(st.n_cor2);ps(" Corc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Corydalis Admin Demo ===\n\n");cor_init();
ps("Corydalis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cor_planning(t,c,913+(i*17),902+(i*14),882+(i*10),864+(i*6),2020+(i%5));}
ps("\nCorydalis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cor_execution(t,c,902+(i*15),891+(i*12),873+(i*8),860+(i*5),2021+(i%4));}
ps("\nCorydalis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cor_evaluation(t,c,894+(i*13),883+(i*10),867+(i*7),856+(i*4),2022+(i%3));}
ps("\nCorydalis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cor_accessory(t,c,886+(i*11),877+(i*9),863+(i*6),853+(i*3),2023+(i%2));}
ps("\nCorydalis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cor_market(t,c,880+(i*9),871+(i*7),858+(i*5),850+(i*3),2024);}
ps("\n");cor_report();cor_state();ps("\n=== Demo Complete ===\n");return 0;}
