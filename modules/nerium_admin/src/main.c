/* nerium_admin: Nerium management technology administration (v1.0)
 * Nerium planning, nerium execution, nerium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nrm_t;
typedef struct{int n_nrp,n_nre,n_nrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nrm_state_t;
static nrm_t nrm_ps[N],nrm_ss[N-2],nrm_vs[N-4],nrm_as[N-6],nrm_ms[N-6]; static nrm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nrm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nrm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nrm_init(void){if(init)return -1;st.n_nrp=0;st.n_nre=0;st.n_nrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nrm_ps[i].active=0;for(int i=0;i<N-2;i++)nrm_ss[i].active=0;for(int i=0;i<N-4;i++)nrm_vs[i].active=0;for(int i=0;i<N-6;i++)nrm_as[i].active=0;for(int i=0;i<N-6;i++)nrm_ms[i].active=0;init=1;ps("[NRM] Nerium initialized\n");return 0;}
int nrm_planning(int t,int c,int a,int b,int d,int e,int y){return add(nrm_ps,&st.n_nrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nrm_execution(int t,int c,int a,int b,int d,int e,int y){return add(nrm_ss,&st.n_nre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nrm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nrm_vs,&st.n_nrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nrm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nrm_as,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nrm_market(int t,int c,int a,int b,int d,int e,int y){return add(nrm_ms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nrm_report(void){ps("[NRM] Nrp: ");pi(st.n_nrp);ps(" PCS=");pi(st.t_f1);ps("\nNre: ");pi(st.n_nre);ps(" PCS=");pi(st.t_f2);ps("\nNrv: ");pi(st.n_nrv);ps(" PCS=");pi(st.t_f3);ps("\nNrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nrm_state(void){ps("[NRM] Nrp=");pi(st.n_nrp);ps(" Nre=");pi(st.n_nre);ps(" Nrv=");pi(st.n_nrv);ps(" Nrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nerium Admin Demo ===\n\n");nrm_init();
ps("Nerium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nrm_planning(t,c,843+(i*17),832+(i*14),812+(i*10),794+(i*6),2020+(i%5));}
ps("\nNerium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nrm_execution(t,c,832+(i*15),821+(i*12),803+(i*8),790+(i*5),2021+(i%4));}
ps("\nNerium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nrm_evaluation(t,c,824+(i*13),813+(i*10),797+(i*7),786+(i*4),2022+(i%3));}
ps("\nNerium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrm_accessory(t,c,816+(i*11),807+(i*9),793+(i*6),783+(i*3),2023+(i%2));}
ps("\nNerium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nrm_market(t,c,810+(i*9),801+(i*7),788+(i*5),780+(i*3),2024);}
ps("\n");nrm_report();nrm_state();ps("\n=== Demo Complete ===\n");return 0;}
