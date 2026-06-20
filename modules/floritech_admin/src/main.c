/* floritech_admin: Flori technology administration (v1.0)
 * Flower planting, flower processing, floral design, flower packaging, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flr_t;
typedef struct{int n_fl,n_fg,n_fa,n_fp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flr_state_t;
static flr_t fls[N],fgs[N-2],fas[N-4],fps[N-6],mks[N-6]; static flr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flr_init(void){if(init)return -1;st.n_fl=0;st.n_fg=0;st.n_fa=0;st.n_fp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fls[i].active=0;for(int i=0;i<N-2;i++)fgs[i].active=0;for(int i=0;i<N-4;i++)fas[i].active=0;for(int i=0;i<N-6;i++)fps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FLR] Floritech initialized\n");return 0;}
int flr_plant(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f1,N,t,c,a,b,d,e,y);}
int flr_process(int t,int c,int a,int b,int d,int e,int y){return add(fgs,&st.n_fg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flr_design(int t,int c,int a,int b,int d,int e,int y){return add(fas,&st.n_fa,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flr_package(int t,int c,int a,int b,int d,int e,int y){return add(fps,&st.n_fp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flr_report(void){ps("[FLR] Plant: ");pi(st.n_fl);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_fg);ps(" kg=");pi(st.t_f2);ps("\nDesign: ");pi(st.n_fa);ps(" PCS=");pi(st.t_f3);ps("\nPkg: ");pi(st.n_fp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flr_state(void){ps("[FLR] Fl=");pi(st.n_fl);ps(" Fg=");pi(st.n_fg);ps(" Fa=");pi(st.n_fa);ps(" Fp=");pi(st.n_fp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flori Tech Admin Demo ===\n\n");flr_init();
ps("Flower planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flr_plant(t,c,194+(i*17),179+(i*14),159+(i*10),141+(i*6),2020+(i%5));}
ps("\nFlower processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flr_process(t,c,183+(i*15),169+(i*12),151+(i*8),138+(i*5),2021+(i%4));}
ps("\nFloral design...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flr_design(t,c,175+(i*13),161+(i*10),145+(i*7),134+(i*4),2022+(i%3));}
ps("\nFlower packaging...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flr_package(t,c,167+(i*11),155+(i*9),141+(i*6),131+(i*3),2023+(i%2));}
ps("\nFlori marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flr_market(t,c,161+(i*9),150+(i*7),137+(i*5),129+(i*3),2024);}
ps("\n");flr_report();flr_state();ps("\n=== Demo Complete ===\n");return 0;}
