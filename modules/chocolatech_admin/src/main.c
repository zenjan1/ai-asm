/* chocolatech_admin: Chocolate technology administration (v1.0)
 * Cocoa planting, cocoa processing, chocolate making, chocolate design, chocolate marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clt_t;
typedef struct{int n_co,n_cp,n_cm,n_cd,n_cm2,t_f1,t_f2,t_f3,t_f4,t_f5;} clt_state_t;
static clt_t cos[N],cps[N-2],cms[N-4],cds[N-6],cm2[N-6]; static clt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clt_init(void){if(init)return -1;st.n_co=0;st.n_cp=0;st.n_cm=0;st.n_cd=0;st.n_cm2=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cos[i].active=0;for(int i=0;i<N-2;i++)cps[i].active=0;for(int i=0;i<N-4;i++)cms[i].active=0;for(int i=0;i<N-6;i++)cds[i].active=0;for(int i=0;i<N-6;i++)cm2[i].active=0;init=1;ps("[CLT] Chocolatech initialized\n");return 0;}
int clt_cocoa(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f1,N,t,c,a,b,d,e,y);}
int clt_process(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clt_make(int t,int c,int a,int b,int d,int e,int y){return add(cms,&st.n_cm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clt_design(int t,int c,int a,int b,int d,int e,int y){return add(cds,&st.n_cd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clt_market(int t,int c,int a,int b,int d,int e,int y){return add(cm2,&st.n_cm2,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clt_report(void){ps("[CLT] Cocoa: ");pi(st.n_co);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_cp);ps(" Ton=");pi(st.t_f2);ps("\nMake: ");pi(st.n_cm);ps(" kg=");pi(st.t_f3);ps("\nDesign: ");pi(st.n_cd);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_cm2);ps(" USD=");pi(st.t_f5);ps("\n");}
void clt_state(void){ps("[CLT] Co=");pi(st.n_co);ps(" Cp=");pi(st.n_cp);ps(" Cm=");pi(st.n_cm);ps(" Cd=");pi(st.n_cd);ps(" Cm2=");pi(st.n_cm2);ps("\n");}
int main(void){
ps("=== Chocolate Tech Admin Demo ===\n\n");clt_init();
ps("Cocoa planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clt_cocoa(t,c,176+(i*17),161+(i*14),141+(i*10),124+(i*6),2020+(i%5));}
ps("\nCocoa processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clt_process(t,c,165+(i*15),151+(i*12),133+(i*8),120+(i*5),2021+(i%4));}
ps("\nChocolate making...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clt_make(t,c,157+(i*13),143+(i*10),127+(i*7),116+(i*4),2022+(i%3));}
ps("\nChocolate design...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clt_design(t,c,149+(i*11),137+(i*9),123+(i*6),113+(i*3),2023+(i%2));}
ps("\nChocolate marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clt_market(t,c,143+(i*9),132+(i*7),119+(i*5),111+(i*3),2024);}
ps("\n");clt_report();clt_state();ps("\n=== Demo Complete ===\n");return 0;}
