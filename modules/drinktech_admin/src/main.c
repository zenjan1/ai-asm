/* drinktech_admin: Beverage technology administration (v1.0)
 * Carbonated drinks, juice drinks, bottled water, functional drinks, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drk_t;
typedef struct{int n_cb,n_jc,n_bw,n_fd,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drk_state_t;
static drk_t cbs[N],jcs[N-2],bws[N-4],fds[N-6],mks[N-6]; static drk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(drk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drk_init(void){if(init)return -1;st.n_cb=0;st.n_jc=0;st.n_bw=0;st.n_fd=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cbs[i].active=0;for(int i=0;i<N-2;i++)jcs[i].active=0;for(int i=0;i<N-4;i++)bws[i].active=0;for(int i=0;i<N-6;i++)fds[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DRK] Drinktech initialized\n");return 0;}
int drk_carbonated(int t,int c,int a,int b,int d,int e,int y){return add(cbs,&st.n_cb,&st.t_f1,N,t,c,a,b,d,e,y);}
int drk_juice(int t,int c,int a,int b,int d,int e,int y){return add(jcs,&st.n_jc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drk_water(int t,int c,int a,int b,int d,int e,int y){return add(bws,&st.n_bw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drk_functional(int t,int c,int a,int b,int d,int e,int y){return add(fds,&st.n_fd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drk_report(void){ps("[DRK] Carb: ");pi(st.n_cb);ps(" L=");pi(st.t_f1);ps("\nJuice: ");pi(st.n_jc);ps(" L=");pi(st.t_f2);ps("\nWater: ");pi(st.n_bw);ps(" L=");pi(st.t_f3);ps("\nFunc: ");pi(st.n_fd);ps(" L=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drk_state(void){ps("[DRK] Cb=");pi(st.n_cb);ps(" Jc=");pi(st.n_jc);ps(" Bw=");pi(st.n_bw);ps(" Fd=");pi(st.n_fd);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Drink Tech Admin Demo ===\n\n");drk_init();
ps("Carbonated drinks...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drk_carbonated(t,c,185+(i*17),170+(i*14),150+(i*10),132+(i*6),2020+(i%5));}
ps("\nJuice drinks...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drk_juice(t,c,174+(i*15),160+(i*12),142+(i*8),129+(i*5),2021+(i%4));}
ps("\nBottled water...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drk_water(t,c,166+(i*13),152+(i*10),136+(i*7),125+(i*4),2022+(i%3));}
ps("\nFunctional drinks...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drk_functional(t,c,158+(i*11),146+(i*9),132+(i*6),122+(i*3),2023+(i%2));}
ps("\nBeverage marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drk_market(t,c,152+(i*9),141+(i*7),128+(i*5),120+(i*3),2024);}
ps("\n");drk_report();drk_state();ps("\n=== Demo Complete ===\n");return 0;}
