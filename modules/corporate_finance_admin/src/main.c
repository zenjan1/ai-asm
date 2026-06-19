/* corporate_finance_admin: Corporate finance administration (v1.0)
 * Corporate investment, capital structure, dividend policy, M&A, corporate governance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cpf_t;
typedef struct{int n_ci,n_cs,n_dp,n_ma,n_cg,t_f1,t_f2,t_f3,t_f4,t_f5;} cpf_state_t;
static cpf_t cis[N],css[N-2],dps[N-4],mas[N-6],cgs[N-6]; static cpf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cpf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cpf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpf_init(void){if(init)return -1;st.n_ci=0;st.n_cs=0;st.n_dp=0;st.n_ma=0;st.n_cg=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cis[i].active=0;for(int i=0;i<N-2;i++)css[i].active=0;for(int i=0;i<N-4;i++)dps[i].active=0;for(int i=0;i<N-6;i++)mas[i].active=0;for(int i=0;i<N-6;i++)cgs[i].active=0;init=1;ps("[CPF] Corporate finance initialized\n");return 0;}
int cpf_invest(int t,int c,int a,int b,int d,int e,int y){return add(cis,&st.n_ci,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpf_structure(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpf_dividend(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpf_ma(int t,int c,int a,int b,int d,int e,int y){return add(mas,&st.n_ma,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpf_govern(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpf_report(void){ps("[CPF] Invest: ");pi(st.n_ci);ps(" NPV=");pi(st.t_f1);ps("\nStruct: ");pi(st.n_cs);ps(" MM=");pi(st.t_f2);ps("\nDiv: ");pi(st.n_dp);ps(" Pay=");pi(st.t_f3);ps("\nMA: ");pi(st.n_ma);ps(" Syn=");pi(st.t_f4);ps("\nGov: ");pi(st.n_cg);ps(" Agcy=");pi(st.t_f5);ps("\n");}
void cpf_state(void){ps("[CPF] Ci=");pi(st.n_ci);ps(" Cs=");pi(st.n_cs);ps(" Dp=");pi(st.n_dp);ps(" Ma=");pi(st.n_ma);ps(" Cg=");pi(st.n_cg);ps("\n");}
int main(void){
ps("=== Corporate Finance Admin Demo ===\n\n");cpf_init();
ps("Corporate investment...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpf_invest(t,c,115+(i*17),100+(i*14),80+(i*10),63+(i*6),2020+(i%5));}
ps("\nCapital structure...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpf_structure(t,c,104+(i*15),90+(i*12),72+(i*8),59+(i*5),2021+(i%4));}
ps("\nDividend policy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpf_dividend(t,c,96+(i*13),82+(i*10),66+(i*7),55+(i*4),2022+(i%3));}
ps("\nM&A restructuring...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpf_ma(t,c,88+(i*11),76+(i*9),62+(i*6),52+(i*3),2023+(i%2));}
ps("\nCorporate governance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpf_govern(t,c,82+(i*9),71+(i*7),58+(i*5),50+(i*3),2024);}
ps("\n");cpf_report();cpf_state();ps("\n=== Demo Complete ===\n");return 0;}
