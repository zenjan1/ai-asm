/* milktech_admin: Dairy technology administration (v1.0)
 * Dairy cow breeding, milk collection, dairy processing, dairy products, dairy marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mlk_t;
typedef struct{int n_co,n_cl,n_dp,n_pd,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mlk_state_t;
static mlk_t cos[N],cls[N-2],dps[N-4],pds[N-6],mks[N-6]; static mlk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mlk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mlk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mlk_init(void){if(init)return -1;st.n_co=0;st.n_cl=0;st.n_dp=0;st.n_pd=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cos[i].active=0;for(int i=0;i<N-2;i++)cls[i].active=0;for(int i=0;i<N-4;i++)dps[i].active=0;for(int i=0;i<N-6;i++)pds[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MLK] Milktech initialized\n");return 0;}
int mlk_cow(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f1,N,t,c,a,b,d,e,y);}
int mlk_collect(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mlk_process(int t,int c,int a,int b,int d,int e,int y){return add(dps,&st.n_dp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mlk_product(int t,int c,int a,int b,int d,int e,int y){return add(pds,&st.n_pd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mlk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mlk_report(void){ps("[MLK] Cow: ");pi(st.n_co);ps(" head=");pi(st.t_f1);ps("\nCollect: ");pi(st.n_cl);ps(" L=");pi(st.t_f2);ps("\nProc: ");pi(st.n_dp);ps(" Ton=");pi(st.t_f3);ps("\nProd: ");pi(st.n_pd);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mlk_state(void){ps("[MLK] Co=");pi(st.n_co);ps(" Cl=");pi(st.n_cl);ps(" Dp=");pi(st.n_dp);ps(" Pd=");pi(st.n_pd);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Milk Tech Admin Demo ===\n\n");mlk_init();
ps("Dairy cow breeding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mlk_cow(t,c,178+(i*17),163+(i*14),143+(i*10),125+(i*6),2020+(i%5));}
ps("\nMilk collection...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mlk_collect(t,c,167+(i*15),153+(i*12),135+(i*8),122+(i*5),2021+(i%4));}
ps("\nDairy processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mlk_process(t,c,159+(i*13),145+(i*10),129+(i*7),118+(i*4),2022+(i%3));}
ps("\nDairy products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlk_product(t,c,151+(i*11),139+(i*9),125+(i*6),115+(i*3),2023+(i%2));}
ps("\nDairy marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mlk_market(t,c,145+(i*9),134+(i*7),121+(i*5),113+(i*3),2024);}
ps("\n");mlk_report();mlk_state();ps("\n=== Demo Complete ===\n");return 0;}
