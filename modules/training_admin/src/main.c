/* training_admin: Training management technology administration (v1.0)
 * Training planning, training implementation, training evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trx_t;
typedef struct{int n_tp,n_ti,n_te,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trx_state_t;
static trx_t tps[N],tis[N-2],tes[N-4],acs[N-6],mks[N-6]; static trx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trx_init(void){if(init)return -1;st.n_tp=0;st.n_ti=0;st.n_te=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tps[i].active=0;for(int i=0;i<N-2;i++)tis[i].active=0;for(int i=0;i<N-4;i++)tes[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TRX] Training initialized\n");return 0;}
int trx_planning(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f1,N,t,c,a,b,d,e,y);}
int trx_implementation(int t,int c,int a,int b,int d,int e,int y){return add(tis,&st.n_ti,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tes,&st.n_te,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trx_report(void){ps("[TRX] Tp: ");pi(st.n_tp);ps(" PCS=");pi(st.t_f1);ps("\nTi: ");pi(st.n_ti);ps(" PCS=");pi(st.t_f2);ps("\nTe: ");pi(st.n_te);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trx_state(void){ps("[TRX] Tp=");pi(st.n_tp);ps(" Ti=");pi(st.n_ti);ps(" Te=");pi(st.n_te);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Training Admin Demo ===\n\n");trx_init();
ps("Training planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trx_planning(t,c,444+(i*17),433+(i*14),413+(i*10),395+(i*6),2020+(i%5));}
ps("\nTraining implementation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trx_implementation(t,c,433+(i*15),422+(i*12),404+(i*8),391+(i*5),2021+(i%4));}
ps("\nTraining evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trx_evaluation(t,c,425+(i*13),414+(i*10),398+(i*7),387+(i*4),2022+(i%3));}
ps("\nTraining accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trx_accessory(t,c,417+(i*11),408+(i*9),394+(i*6),384+(i*3),2023+(i%2));}
ps("\nTraining marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trx_market(t,c,411+(i*9),402+(i*7),389+(i*5),381+(i*3),2024);}
ps("\n");trx_report();trx_state();ps("\n=== Demo Complete ===\n");return 0;}
