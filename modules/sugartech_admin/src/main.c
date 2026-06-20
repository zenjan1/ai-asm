/* sugartech_admin: Sugar technology administration (v1.0)
 * Sugarcane planting, sugar beet planting, sugar extraction, sugar refining, sugar marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sgr_t;
typedef struct{int n_ca,n_be,n_ex,n_rf,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sgr_state_t;
static sgr_t cas[N],bes[N-2],exs[N-4],rfs[N-6],mks[N-6]; static sgr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sgr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sgr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SGR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sgr_init(void){if(init)return -1;st.n_ca=0;st.n_be=0;st.n_ex=0;st.n_rf=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cas[i].active=0;for(int i=0;i<N-2;i++)bes[i].active=0;for(int i=0;i<N-4;i++)exs[i].active=0;for(int i=0;i<N-6;i++)rfs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SGR] Sugartech initialized\n");return 0;}
int sgr_cane(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f1,N,t,c,a,b,d,e,y);}
int sgr_beet(int t,int c,int a,int b,int d,int e,int y){return add(bes,&st.n_be,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sgr_extract(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sgr_refine(int t,int c,int a,int b,int d,int e,int y){return add(rfs,&st.n_rf,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sgr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sgr_report(void){ps("[SGR] Cane: ");pi(st.n_ca);ps(" ha=");pi(st.t_f1);ps("\nBeet: ");pi(st.n_be);ps(" ha=");pi(st.t_f2);ps("\nExtract: ");pi(st.n_ex);ps(" Ton=");pi(st.t_f3);ps("\nRefine: ");pi(st.n_rf);ps(" Ton=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sgr_state(void){ps("[SGR] Ca=");pi(st.n_ca);ps(" Be=");pi(st.n_be);ps(" Ex=");pi(st.n_ex);ps(" Rf=");pi(st.n_rf);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sugar Tech Admin Demo ===\n\n");sgr_init();
ps("Sugarcane planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sgr_cane(t,c,181+(i*17),166+(i*14),146+(i*10),128+(i*6),2020+(i%5));}
ps("\nSugar beet planting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sgr_beet(t,c,170+(i*15),156+(i*12),138+(i*8),125+(i*5),2021+(i%4));}
ps("\nSugar extraction...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sgr_extract(t,c,162+(i*13),148+(i*10),132+(i*7),121+(i*4),2022+(i%3));}
ps("\nSugar refining...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sgr_refine(t,c,154+(i*11),142+(i*9),128+(i*6),118+(i*3),2023+(i%2));}
ps("\nSugar marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sgr_market(t,c,148+(i*9),137+(i*7),124+(i*5),116+(i*3),2024);}
ps("\n");sgr_report();sgr_state();ps("\n=== Demo Complete ===\n");return 0;}
