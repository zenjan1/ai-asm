/* coffeetech_admin: Coffee technology administration (v1.0)
 * Coffee planting, coffee processing, coffee roasting, coffee brewing, coffee marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cft_t;
typedef struct{int n_gp,n_pr,n_ro,n_br,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cft_state_t;
static cft_t gps[N],prs[N-2],ros[N-4],brs[N-6],mks[N-6]; static cft_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cft_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cft_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CFT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cft_init(void){if(init)return -1;st.n_gp=0;st.n_pr=0;st.n_ro=0;st.n_br=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gps[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)ros[i].active=0;for(int i=0;i<N-6;i++)brs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CFT] Coffeetech initialized\n");return 0;}
int cft_grow(int t,int c,int a,int b,int d,int e,int y){return add(gps,&st.n_gp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cft_process(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cft_roast(int t,int c,int a,int b,int d,int e,int y){return add(ros,&st.n_ro,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cft_brew(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cft_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cft_report(void){ps("[CFT] Grow: ");pi(st.n_gp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_pr);ps(" Ton=");pi(st.t_f2);ps("\nRoast: ");pi(st.n_ro);ps(" C=");pi(st.t_f3);ps("\nBrew: ");pi(st.n_br);ps(" Cup=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cft_state(void){ps("[CFT] Gp=");pi(st.n_gp);ps(" Pr=");pi(st.n_pr);ps(" Ro=");pi(st.n_ro);ps(" Br=");pi(st.n_br);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coffee Tech Admin Demo ===\n\n");cft_init();
ps("Coffee planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cft_grow(t,c,175+(i*17),160+(i*14),140+(i*10),123+(i*6),2020+(i%5));}
ps("\nCoffee processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cft_process(t,c,164+(i*15),150+(i*12),132+(i*8),119+(i*5),2021+(i%4));}
ps("\nCoffee roasting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cft_roast(t,c,156+(i*13),142+(i*10),126+(i*7),115+(i*4),2022+(i%3));}
ps("\nCoffee brewing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cft_brew(t,c,148+(i*11),136+(i*9),122+(i*6),112+(i*3),2023+(i%2));}
ps("\nCoffee marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cft_market(t,c,142+(i*9),131+(i*7),118+(i*5),110+(i*3),2024);}
ps("\n");cft_report();cft_state();ps("\n=== Demo Complete ===\n");return 0;}
