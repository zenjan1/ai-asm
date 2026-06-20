/* egtech_admin: Egg technology administration (v1.0)
 * Layer breeding, egg collection, egg processing, egg products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} egg_t;
typedef struct{int n_ly,n_eg,n_ep,n_epr,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} egg_state_t;
static egg_t lys[N],egs[N-2],eps[N-4],eprs[N-6],mks[N-6]; static egg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(egg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;egg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EGG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int egg_init(void){if(init)return -1;st.n_ly=0;st.n_eg=0;st.n_ep=0;st.n_epr=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lys[i].active=0;for(int i=0;i<N-2;i++)egs[i].active=0;for(int i=0;i<N-4;i++)eps[i].active=0;for(int i=0;i<N-6;i++)eprs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[EGG] Egtech initialized\n");return 0;}
int egg_layer(int t,int c,int a,int b,int d,int e,int y){return add(lys,&st.n_ly,&st.t_f1,N,t,c,a,b,d,e,y);}
int egg_collect(int t,int c,int a,int b,int d,int e,int y){return add(egs,&st.n_eg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int egg_process(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int egg_product(int t,int c,int a,int b,int d,int e,int y){return add(eprs,&st.n_epr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int egg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void egg_report(void){ps("[EGG] Layer: ");pi(st.n_ly);ps(" head=");pi(st.t_f1);ps("\nCollect: ");pi(st.n_eg);ps(" PCS=");pi(st.t_f2);ps("\nProc: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f3);ps("\nProd: ");pi(st.n_epr);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void egg_state(void){ps("[EGG] Ly=");pi(st.n_ly);ps(" Eg=");pi(st.n_eg);ps(" Ep=");pi(st.n_ep);ps(" Epr=");pi(st.n_epr);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Egg Tech Admin Demo ===\n\n");egg_init();
ps("Layer breeding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;egg_layer(t,c,189+(i*17),174+(i*14),154+(i*10),136+(i*6),2020+(i%5));}
ps("\nEgg collection...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;egg_collect(t,c,178+(i*15),164+(i*12),146+(i*8),133+(i*5),2021+(i%4));}
ps("\nEgg processing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;egg_process(t,c,170+(i*13),156+(i*10),140+(i*7),129+(i*4),2022+(i%3));}
ps("\nEgg products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;egg_product(t,c,162+(i*11),150+(i*9),136+(i*6),126+(i*3),2023+(i%2));}
ps("\nEgg marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;egg_market(t,c,156+(i*9),145+(i*7),132+(i*5),124+(i*3),2024);}
ps("\n");egg_report();egg_state();ps("\n=== Demo Complete ===\n");return 0;}
