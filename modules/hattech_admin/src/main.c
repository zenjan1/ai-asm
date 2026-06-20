/* hattech_admin: Hat technology administration (v1.0)
 * Baseball caps, sun hats, winter hats, fashion hats, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hat_t;
typedef struct{int n_bc,n_sh,n_wh,n_fh,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hat_state_t;
static hat_t bcs[N],shs[N-2],whs[N-4],fhs[N-6],mks[N-6]; static hat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hat_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hat_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HAT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hat_init(void){if(init)return -1;st.n_bc=0;st.n_sh=0;st.n_wh=0;st.n_fh=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bcs[i].active=0;for(int i=0;i<N-2;i++)shs[i].active=0;for(int i=0;i<N-4;i++)whs[i].active=0;for(int i=0;i<N-6;i++)fhs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HAT] Hattech initialized\n");return 0;}
int hat_cap(int t,int c,int a,int b,int d,int e,int y){return add(bcs,&st.n_bc,&st.t_f1,N,t,c,a,b,d,e,y);}
int hat_sun(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hat_winter(int t,int c,int a,int b,int d,int e,int y){return add(whs,&st.n_wh,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hat_fashion(int t,int c,int a,int b,int d,int e,int y){return add(fhs,&st.n_fh,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hat_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hat_report(void){ps("[HAT] Cap: ");pi(st.n_bc);ps(" PCS=");pi(st.t_f1);ps("\nSun: ");pi(st.n_sh);ps(" PCS=");pi(st.t_f2);ps("\nWinter: ");pi(st.n_wh);ps(" PCS=");pi(st.t_f3);ps("\nFashion: ");pi(st.n_fh);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hat_state(void){ps("[HAT] Bc=");pi(st.n_bc);ps(" Sh=");pi(st.n_sh);ps(" Wh=");pi(st.n_wh);ps(" Fh=");pi(st.n_fh);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hat Tech Admin Demo ===\n\n");hat_init();
ps("Baseball caps...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hat_cap(t,c,200+(i*17),185+(i*14),165+(i*10),147+(i*6),2020+(i%5));}
ps("\nSun hats...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hat_sun(t,c,189+(i*15),175+(i*12),157+(i*8),144+(i*5),2021+(i%4));}
ps("\nWinter hats...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hat_winter(t,c,181+(i*13),167+(i*10),151+(i*7),140+(i*4),2022+(i%3));}
ps("\nFashion hats...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hat_fashion(t,c,173+(i*11),161+(i*9),147+(i*6),137+(i*3),2023+(i%2));}
ps("\nHat marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hat_market(t,c,167+(i*9),156+(i*7),143+(i*5),135+(i*3),2024);}
ps("\n");hat_report();hat_state();ps("\n=== Demo Complete ===\n");return 0;}
