/* cultural_economics_admin: Cultural economics administration (v1.0)
 * Cultural products, cultural heritage, cultural policy, creative industries, cultural capital
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cul_t;
typedef struct{int n_cp,n_ch,n_co,n_ci,n_cc,t_f1,t_f2,t_f3,t_f4,t_f5;} cul_state_t;
static cul_t cps[N],chs[N-2],cos[N-4],cis[N-6],ccs[N-6]; static cul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cul_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cul_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CUL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cul_init(void){if(init)return -1;st.n_cp=0;st.n_ch=0;st.n_co=0;st.n_ci=0;st.n_cc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cps[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)cos[i].active=0;for(int i=0;i<N-6;i++)cis[i].active=0;for(int i=0;i<N-6;i++)ccs[i].active=0;init=1;ps("[CUL] Cultural economics initialized\n");return 0;}
int cul_product(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cul_heritage(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cul_policy(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cul_creative(int t,int c,int a,int b,int d,int e,int y){return add(cis,&st.n_ci,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cul_capital(int t,int c,int a,int b,int d,int e,int y){return add(ccs,&st.n_cc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cul_report(void){ps("[CUL] Product: ");pi(st.n_cp);ps(" Industry=");pi(st.t_f1);ps("\nHeritage: ");pi(st.n_ch);ps(" Protect=");pi(st.t_f2);ps("\nPolicy: ");pi(st.n_co);ps(" Fund=");pi(st.t_f3);ps("\nCreative: ");pi(st.n_ci);ps(" Design=");pi(st.t_f4);ps("\nCapital: ");pi(st.n_cc);ps(" Soft=");pi(st.t_f5);ps("\n");}
void cul_state(void){ps("[CUL] Cp=");pi(st.n_cp);ps(" Ch=");pi(st.n_ch);ps(" Co=");pi(st.n_co);ps(" Ci=");pi(st.n_ci);ps(" Cc=");pi(st.n_cc);ps("\n");}
int main(void){
ps("=== Cultural Economics Admin Demo ===\n\n");cul_init();
ps("Cultural products...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cul_product(t,c,105+(i*17),90+(i*14),70+(i*10),53+(i*6),2020+(i%5));}
ps("\nCultural heritage...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cul_heritage(t,c,94+(i*15),80+(i*12),62+(i*8),49+(i*5),2021+(i%4));}
ps("\nCultural policy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cul_policy(t,c,86+(i*13),72+(i*10),56+(i*7),45+(i*4),2022+(i%3));}
ps("\nCreative industries...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cul_creative(t,c,78+(i*11),66+(i*9),52+(i*6),42+(i*3),2023+(i%2));}
ps("\nCultural capital...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cul_capital(t,c,72+(i*9),61+(i*7),48+(i*5),40+(i*3),2024);}
ps("\n");cul_report();cul_state();ps("\n=== Demo Complete ===\n");return 0;}
