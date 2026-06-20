/* rubbertech_admin: Rubber technology administration (v1.0)
 * Rubber planting, rubber processing, tire production, rubber products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rbr_t;
typedef struct{int n_rp,n_rpg,n_tr,n_rbp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rbr_state_t;
static rbr_t rps[N],rpgs[N-2],trs[N-4],rbps[N-6],mks[N-6]; static rbr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rbr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rbr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RBR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rbr_init(void){if(init)return -1;st.n_rp=0;st.n_rpg=0;st.n_tr=0;st.n_rbp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rps[i].active=0;for(int i=0;i<N-2;i++)rpgs[i].active=0;for(int i=0;i<N-4;i++)trs[i].active=0;for(int i=0;i<N-6;i++)rbps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RBR] Rubbertech initialized\n");return 0;}
int rbr_plant(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rbr_process(int t,int c,int a,int b,int d,int e,int y){return add(rpgs,&st.n_rpg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rbr_tire(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rbr_product(int t,int c,int a,int b,int d,int e,int y){return add(rbps,&st.n_rbp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rbr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rbr_report(void){ps("[RBR] Plant: ");pi(st.n_rp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_rpg);ps(" Ton=");pi(st.t_f2);ps("\nTire: ");pi(st.n_tr);ps(" PCS=");pi(st.t_f3);ps("\nProd: ");pi(st.n_rbp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rbr_state(void){ps("[RBR] Rp=");pi(st.n_rp);ps(" Rpg=");pi(st.n_rpg);ps(" Tr=");pi(st.n_tr);ps(" Rbp=");pi(st.n_rbp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rubber Tech Admin Demo ===\n\n");rbr_init();
ps("Rubber planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rbr_plant(t,c,207+(i*17),192+(i*14),172+(i*10),154+(i*6),2020+(i%5));}
ps("\nRubber processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rbr_process(t,c,196+(i*15),182+(i*12),164+(i*8),151+(i*5),2021+(i%4));}
ps("\nTire production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rbr_tire(t,c,188+(i*13),174+(i*10),158+(i*7),147+(i*4),2022+(i%3));}
ps("\nRubber products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbr_product(t,c,180+(i*11),168+(i*9),154+(i*6),144+(i*3),2023+(i%2));}
ps("\nRubber marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbr_market(t,c,174+(i*9),163+(i*7),150+(i*5),142+(i*3),2024);}
ps("\n");rbr_report();rbr_state();ps("\n=== Demo Complete ===\n");return 0;}
