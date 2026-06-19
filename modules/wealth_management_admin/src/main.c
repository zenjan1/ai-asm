/* wealth_management_admin: Wealth management administration (v1.0)
 * HNW clients, estate planning, tax planning, retirement planning, comprehensive advisory
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wma_t;
typedef struct{int n_hn,n_ep,n_tp,n_rp,n_ca,t_f1,t_f2,t_f3,t_f4,t_f5;} wma_state_t;
static wma_t hns[N],eps[N-2],tps[N-4],rps[N-6],cas[N-6]; static wma_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wma_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wma_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WMA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wma_init(void){if(init)return -1;st.n_hn=0;st.n_ep=0;st.n_tp=0;st.n_rp=0;st.n_ca=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hns[i].active=0;for(int i=0;i<N-2;i++)eps[i].active=0;for(int i=0;i<N-4;i++)tps[i].active=0;for(int i=0;i<N-6;i++)rps[i].active=0;for(int i=0;i<N-6;i++)cas[i].active=0;init=1;ps("[WMA] Wealth management initialized\n");return 0;}
int wma_hnw(int t,int c,int a,int b,int d,int e,int y){return add(hns,&st.n_hn,&st.t_f1,N,t,c,a,b,d,e,y);}
int wma_estate(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wma_tax(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wma_retire(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wma_consult(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wma_report(void){ps("[WMA] HNW: ");pi(st.n_hn);ps(" UHNW=");pi(st.t_f1);ps("\nEstate: ");pi(st.n_ep);ps(" Trust=");pi(st.t_f2);ps("\nTax: ");pi(st.n_tp);ps(" Opt=");pi(st.t_f3);ps("\nRetire: ");pi(st.n_rp);ps(" Ann=");pi(st.t_f4);ps("\nConsult: ");pi(st.n_ca);ps(" AUM=");pi(st.t_f5);ps("\n");}
void wma_state(void){ps("[WMA] Hn=");pi(st.n_hn);ps(" Ep=");pi(st.n_ep);ps(" Tp=");pi(st.n_tp);ps(" Rp=");pi(st.n_rp);ps(" Ca=");pi(st.n_ca);ps("\n");}
int main(void){
ps("=== Wealth Management Admin Demo ===\n\n");wma_init();
ps("HNW clients...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wma_hnw(t,c,119+(i*17),104+(i*14),84+(i*10),67+(i*6),2020+(i%5));}
ps("\nEstate planning...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wma_estate(t,c,108+(i*15),94+(i*12),76+(i*8),63+(i*5),2021+(i%4));}
ps("\nTax planning...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wma_tax(t,c,100+(i*13),86+(i*10),70+(i*7),59+(i*4),2022+(i%3));}
ps("\nRetirement planning...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wma_retire(t,c,92+(i*11),80+(i*9),66+(i*6),56+(i*3),2023+(i%2));}
ps("\nComprehensive advisory...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wma_consult(t,c,86+(i*9),75+(i*7),62+(i*5),54+(i*3),2024);}
ps("\n");wma_report();wma_state();ps("\n=== Demo Complete ===\n");return 0;}
