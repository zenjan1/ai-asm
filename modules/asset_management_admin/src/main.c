/* asset_management_admin: Asset management administration (v1.0)
 * Portfolio theory, asset allocation, security analysis, performance evaluation, investment advisory
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ama_t;
typedef struct{int n_pt,n_aa,n_sa,n_pe,n_ia,t_f1,t_f2,t_f3,t_f4,t_f5;} ama_state_t;
static ama_t pts[N],aas[N-2],sas[N-4],pes[N-6],ias[N-6]; static ama_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ama_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ama_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AMA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ama_init(void){if(init)return -1;st.n_pt=0;st.n_aa=0;st.n_sa=0;st.n_pe=0;st.n_ia=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pts[i].active=0;for(int i=0;i<N-2;i++)aas[i].active=0;for(int i=0;i<N-4;i++)sas[i].active=0;for(int i=0;i<N-6;i++)pes[i].active=0;for(int i=0;i<N-6;i++)ias[i].active=0;init=1;ps("[AMA] Asset management initialized\n");return 0;}
int ama_portfolio(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f1,N,t,c,a,b,d,e,y);}
int ama_alloc(int t,int c,int a,int b,int d,int e,int y){return add(aas,&st.n_aa,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ama_security(int t,int c,int a,int b,int d,int e,int y){return add(sas,&st.n_sa,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ama_perf(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ama_advisory(int t,int c,int a,int b,int d,int e,int y){return add(ias,&st.n_ia,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ama_report(void){ps("[AMA] Port: ");pi(st.n_pt);ps(" MV=");pi(st.t_f1);ps("\nAlloc: ");pi(st.n_aa);ps(" SRP=");pi(st.t_f2);ps("\nSec: ");pi(st.n_sa);ps(" PE=");pi(st.t_f3);ps("\nPerf: ");pi(st.n_pe);ps(" IR=");pi(st.t_f4);ps("\nAdv: ");pi(st.n_ia);ps(" AUM=");pi(st.t_f5);ps("\n");}
void ama_state(void){ps("[AMA] Pt=");pi(st.n_pt);ps(" Aa=");pi(st.n_aa);ps(" Sa=");pi(st.n_sa);ps(" Pe=");pi(st.n_pe);ps(" Ia=");pi(st.n_ia);ps("\n");}
int main(void){
ps("=== Asset Management Admin Demo ===\n\n");ama_init();
ps("Portfolio theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ama_portfolio(t,c,118+(i*17),103+(i*14),83+(i*10),66+(i*6),2020+(i%5));}
ps("\nAsset allocation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ama_alloc(t,c,107+(i*15),93+(i*12),75+(i*8),62+(i*5),2021+(i%4));}
ps("\nSecurity analysis...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ama_security(t,c,99+(i*13),85+(i*10),69+(i*7),58+(i*4),2022+(i%3));}
ps("\nPerformance evaluation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ama_perf(t,c,91+(i*11),79+(i*9),65+(i*6),55+(i*3),2023+(i%2));}
ps("\nInvestment advisory...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ama_advisory(t,c,85+(i*9),74+(i*7),61+(i*5),53+(i*3),2024);}
ps("\n");ama_report();ama_state();ps("\n=== Demo Complete ===\n");return 0;}
