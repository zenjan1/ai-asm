/* hemerocallis_admin: Hemerocallis management technology administration (v1.0)
 * Hemerocallis planning, hemerocallis execution, hemerocallis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hmc_t;
typedef struct{int n_hmp,n_hme,n_hmv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hmc_state_t;
static hmc_t hmcs[N],hmess[N-2],hmvss[N-4],hmas[N-6],hmmks[N-6]; static hmc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hmc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hmc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HMC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hmc_init(void){if(init)return -1;st.n_hmp=0;st.n_hme=0;st.n_hmv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hmcs[i].active=0;for(int i=0;i<N-2;i++)hmess[i].active=0;for(int i=0;i<N-4;i++)hmvss[i].active=0;for(int i=0;i<N-6;i++)hmas[i].active=0;for(int i=0;i<N-6;i++)hmmks[i].active=0;init=1;ps("[HMC] Hemerocallis initialized\n");return 0;}
int hmc_planning(int t,int c,int a,int b,int d,int e,int y){return add(hmcs,&st.n_hmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hmc_execution(int t,int c,int a,int b,int d,int e,int y){return add(hmess,&st.n_hme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hmc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hmvss,&st.n_hmv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hmc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hmc_market(int t,int c,int a,int b,int d,int e,int y){return add(hmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hmc_report(void){ps("[HMC] Hmp: ");pi(st.n_hmp);ps(" PCS=");pi(st.t_f1);ps("\nHme: ");pi(st.n_hme);ps(" PCS=");pi(st.t_f2);ps("\nHmv: ");pi(st.n_hmv);ps(" PCS=");pi(st.t_f3);ps("\nHmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hmc_state(void){ps("[HMC] Hmp=");pi(st.n_hmp);ps(" Hme=");pi(st.n_hme);ps(" Hmv=");pi(st.n_hmv);ps(" Hmc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hemerocallis Admin Demo ===\n\n");hmc_init();
ps("Hemerocallis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hmc_planning(t,c,826+(i*17),815+(i*14),795+(i*10),777+(i*6),2020+(i%5));}
ps("\nHemerocallis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hmc_execution(t,c,815+(i*15),804+(i*12),786+(i*8),773+(i*5),2021+(i%4));}
ps("\nHemerocallis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hmc_evaluation(t,c,807+(i*13),796+(i*10),780+(i*7),769+(i*4),2022+(i%3));}
ps("\nHemerocallis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hmc_accessory(t,c,799+(i*11),790+(i*9),776+(i*6),766+(i*3),2023+(i%2));}
ps("\nHemerocallis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hmc_market(t,c,793+(i*9),784+(i*7),771+(i*5),763+(i*3),2024);}
ps("\n");hmc_report();hmc_state();ps("\n=== Demo Complete ===\n");return 0;}
