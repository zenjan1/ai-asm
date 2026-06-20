/* woodtech_admin: Wood technology administration (v1.0)
 * Wood processing, board production, furniture making, wood products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wdt_t;
typedef struct{int n_wp,n_bd,n_fn,n_wpr,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wdt_state_t;
static wdt_t wps[N],bds[N-2],fns[N-4],wprs[N-6],mks[N-6]; static wdt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wdt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wdt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wdt_init(void){if(init)return -1;st.n_wp=0;st.n_bd=0;st.n_fn=0;st.n_wpr=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wps[i].active=0;for(int i=0;i<N-2;i++)bds[i].active=0;for(int i=0;i<N-4;i++)fns[i].active=0;for(int i=0;i<N-6;i++)wprs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WDT] Woodtech initialized\n");return 0;}
int wdt_process(int t,int c,int a,int b,int d,int e,int y){return add(wps,&st.n_wp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wdt_board(int t,int c,int a,int b,int d,int e,int y){return add(bds,&st.n_bd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wdt_furniture(int t,int c,int a,int b,int d,int e,int y){return add(fns,&st.n_fn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wdt_product(int t,int c,int a,int b,int d,int e,int y){return add(wprs,&st.n_wpr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wdt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wdt_report(void){ps("[WDT] Proc: ");pi(st.n_wp);ps(" m3=");pi(st.t_f1);ps("\nBoard: ");pi(st.n_bd);ps(" PCS=");pi(st.t_f2);ps("\nFurn: ");pi(st.n_fn);ps(" PCS=");pi(st.t_f3);ps("\nProd: ");pi(st.n_wpr);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wdt_state(void){ps("[WDT] Wp=");pi(st.n_wp);ps(" Bd=");pi(st.n_bd);ps(" Fn=");pi(st.n_fn);ps(" Wpr=");pi(st.n_wpr);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wood Tech Admin Demo ===\n\n");wdt_init();
ps("Wood processing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wdt_process(t,c,214+(i*17),199+(i*14),179+(i*10),161+(i*6),2020+(i%5));}
ps("\nBoard production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wdt_board(t,c,203+(i*15),189+(i*12),171+(i*8),158+(i*5),2021+(i%4));}
ps("\nFurniture making...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wdt_furniture(t,c,195+(i*13),181+(i*10),165+(i*7),154+(i*4),2022+(i%3));}
ps("\nWood products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wdt_product(t,c,187+(i*11),175+(i*9),161+(i*6),151+(i*3),2023+(i%2));}
ps("\nWood marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wdt_market(t,c,181+(i*9),170+(i*7),157+(i*5),149+(i*3),2024);}
ps("\n");wdt_report();wdt_state();ps("\n=== Demo Complete ===\n");return 0;}
