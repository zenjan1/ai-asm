/* energy_economics_admin: Energy economics administration (v1.0)
 * Energy markets, energy pricing, energy security, energy transition, energy policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ene_t;
typedef struct{int n_em,n_ep,n_es,n_et,n_eo,t_f1,t_f2,t_f3,t_f4,t_f5;} ene_state_t;
static ene_t ems[N],eps[N-2],ess[N-4],ets[N-6],eos[N-6]; static ene_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ene_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ene_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ENE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ene_init(void){if(init)return -1;st.n_em=0;st.n_ep=0;st.n_es=0;st.n_et=0;st.n_eo=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ems[i].active=0;for(int i=0;i<N-2;i++)eps[i].active=0;for(int i=0;i<N-4;i++)ess[i].active=0;for(int i=0;i<N-6;i++)ets[i].active=0;for(int i=0;i<N-6;i++)eos[i].active=0;init=1;ps("[ENE] Energy economics initialized\n");return 0;}
int ene_market(int t,int c,int a,int b,int d,int e,int y){return add(ems,&st.n_em,&st.t_f1,N,t,c,a,b,d,e,y);}
int ene_pricing(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ene_security(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ene_transition(int t,int c,int a,int b,int d,int e,int y){return add(ets,&st.n_et,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ene_policy(int t,int c,int a,int b,int d,int e,int y){return add(eos,&st.n_eo,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ene_report(void){ps("[ENE] Market: ");pi(st.n_em);ps(" Oil=");pi(st.t_f1);ps("\nPricing: ");pi(st.n_ep);ps(" PeakV=");pi(st.t_f2);ps("\nSecur: ");pi(st.n_es);ps(" Reserve=");pi(st.t_f3);ps("\nTransit: ");pi(st.n_et);ps(" Renew=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_eo);ps(" Reg=");pi(st.t_f5);ps("\n");}
void ene_state(void){ps("[ENE] Em=");pi(st.n_em);ps(" Ep=");pi(st.n_ep);ps(" Es=");pi(st.n_es);ps(" Et=");pi(st.n_et);ps(" Eo=");pi(st.n_eo);ps("\n");}
int main(void){
ps("=== Energy Economics Admin Demo ===\n\n");ene_init();
ps("Energy markets...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ene_market(t,c,101+(i*17),86+(i*14),66+(i*10),49+(i*6),2020+(i%5));}
ps("\nEnergy pricing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ene_pricing(t,c,90+(i*15),76+(i*12),58+(i*8),45+(i*5),2021+(i%4));}
ps("\nEnergy security...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ene_security(t,c,82+(i*13),68+(i*10),52+(i*7),41+(i*4),2022+(i%3));}
ps("\nEnergy transition...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ene_transition(t,c,74+(i*11),62+(i*9),48+(i*6),38+(i*3),2023+(i%2));}
ps("\nEnergy policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ene_policy(t,c,68+(i*9),57+(i*7),44+(i*5),36+(i*3),2024);}
ps("\n");ene_report();ene_state();ps("\n=== Demo Complete ===\n");return 0;}
