/* datura_admin: Datura management technology administration (v1.0)
 * Datura planning, datura execution, datura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dau_t;
typedef struct{int n_daup,n_dauE,n_dau2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dau_state_t;
static dau_t daups[N],dauss[N-2],dauvss[N-4],dauas[N-6],daumks[N-6]; static dau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dau_init(void){if(init)return -1;st.n_daup=0;st.n_dauE=0;st.n_dau2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)daups[i].active=0;for(int i=0;i<N-2;i++)dauss[i].active=0;for(int i=0;i<N-4;i++)dauvss[i].active=0;for(int i=0;i<N-6;i++)dauas[i].active=0;for(int i=0;i<N-6;i++)daumks[i].active=0;init=1;ps("[DAU] Datura initialized\n");return 0;}
int dau_planning(int t,int c,int a,int b,int d,int e,int y){return add(daups,&st.n_daup,&st.t_f1,N,t,c,a,b,d,e,y);}
int dau_execution(int t,int c,int a,int b,int d,int e,int y){return add(dauss,&st.n_dauE,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dauvss,&st.n_dau2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dauas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dau_market(int t,int c,int a,int b,int d,int e,int y){return add(daumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dau_report(void){ps("[DAU] Daup: ");pi(st.n_daup);ps(" PCS=");pi(st.t_f1);ps("\nDaue: ");pi(st.n_dauE);ps(" PCS=");pi(st.t_f2);ps("\nDauv: ");pi(st.n_dau2);ps(" PCS=");pi(st.t_f3);ps("\nDauc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dau_state(void){ps("[DAU] Daup=");pi(st.n_daup);ps(" Daue=");pi(st.n_dauE);ps(" Dauv=");pi(st.n_dau2);ps(" Dauc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Datura Admin Demo ===\n\n");dau_init();
ps("Datura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dau_planning(t,c,991+(i*17),980+(i*14),960+(i*10),942+(i*6),2020+(i%5));}
ps("\nDatura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dau_execution(t,c,980+(i*15),969+(i*12),951+(i*8),938+(i*5),2021+(i%4));}
ps("\nDatura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dau_evaluation(t,c,972+(i*13),961+(i*10),945+(i*7),934+(i*4),2022+(i%3));}
ps("\nDatura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dau_accessory(t,c,964+(i*11),955+(i*9),941+(i*6),931+(i*3),2023+(i%2));}
ps("\nDatura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dau_market(t,c,958+(i*9),949+(i*7),936+(i*5),928+(i*3),2024);}
ps("\n");dau_report();dau_state();ps("\n=== Demo Complete ===\n");return 0;}
