/* comparative_economics_admin: Comparative economics administration (v1.0)
 * Economic systems, ownership comparison, resource allocation, performance comparison, transition comparison
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cmp_t;
typedef struct{int n_es,n_ow,n_ra,n_pc,n_tc,t_f1,t_f2,t_f3,t_f4,t_f5;} cmp_state_t;
static cmp_t ess[N],ows[N-2],ras[N-4],pcs[N-6],tcs[N-6]; static cmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cmp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cmp_init(void){if(init)return -1;st.n_es=0;st.n_ow=0;st.n_ra=0;st.n_pc=0;st.n_tc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ess[i].active=0;for(int i=0;i<N-2;i++)ows[i].active=0;for(int i=0;i<N-4;i++)ras[i].active=0;for(int i=0;i<N-6;i++)pcs[i].active=0;for(int i=0;i<N-6;i++)tcs[i].active=0;init=1;ps("[CMP] Comparative economics initialized\n");return 0;}
int cmp_system(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f1,N,t,c,a,b,d,e,y);}
int cmp_ownership(int t,int c,int a,int b,int d,int e,int y){return add(ows,&st.n_ow,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cmp_allocation(int t,int c,int a,int b,int d,int e,int y){return add(ras,&st.n_ra,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cmp_performance(int t,int c,int a,int b,int d,int e,int y){return add(pcs,&st.n_pc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cmp_transition(int t,int c,int a,int b,int d,int e,int y){return add(tcs,&st.n_tc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cmp_report(void){ps("[CMP] System: ");pi(st.n_es);ps(" Market=");pi(st.t_f1);ps("\nOwnership: ");pi(st.n_ow);ps(" Private=");pi(st.t_f2);ps("\nAllocation: ");pi(st.n_ra);ps(" Price=");pi(st.t_f3);ps("\nPerform: ");pi(st.n_pc);ps(" Efficien=");pi(st.t_f4);ps("\nTransit: ");pi(st.n_tc);ps(" Gradual=");pi(st.t_f5);ps("\n");}
void cmp_state(void){ps("[CMP] Es=");pi(st.n_es);ps(" Ow=");pi(st.n_ow);ps(" Ra=");pi(st.n_ra);ps(" Pc=");pi(st.n_pc);ps(" Tc=");pi(st.n_tc);ps("\n");}
int main(void){
ps("=== Comparative Economics Admin Demo ===\n\n");cmp_init();
ps("Economic systems...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cmp_system(t,c,86+(i*18),71+(i*15),51+(i*11),34+(i*7),2020+(i%5));}
ps("\nOwnership comparison...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cmp_ownership(t,c,75+(i*16),61+(i*13),43+(i*9),30+(i*6),2021+(i%4));}
ps("\nResource allocation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cmp_allocation(t,c,67+(i*14),53+(i*11),37+(i*7),26+(i*4),2022+(i%3));}
ps("\nPerformance comparison...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_performance(t,c,59+(i*12),47+(i*9),33+(i*6),23+(i*3),2023+(i%2));}
ps("\nTransition comparison...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_transition(t,c,53+(i*10),42+(i*8),29+(i*5),21+(i*3),2024);}
ps("\n");cmp_report();cmp_state();ps("\n=== Demo Complete ===\n");return 0;}
