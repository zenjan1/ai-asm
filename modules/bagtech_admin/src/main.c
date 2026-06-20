/* bagtech_admin: Bag technology administration (v1.0)
 * Handbags, backpacks, luggage, wallets, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bag_t;
typedef struct{int n_hd,n_bp,n_lg,n_wl,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bag_state_t;
static bag_t hds[N],bps[N-2],lgs[N-4],wls[N-6],mks[N-6]; static bag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bag_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bag_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bag_init(void){if(init)return -1;st.n_hd=0;st.n_bp=0;st.n_lg=0;st.n_wl=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hds[i].active=0;for(int i=0;i<N-2;i++)bps[i].active=0;for(int i=0;i<N-4;i++)lgs[i].active=0;for(int i=0;i<N-6;i++)wls[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BAG] Bagtech initialized\n");return 0;}
int bag_handbag(int t,int c,int a,int b,int d,int e,int y){return add(hds,&st.n_hd,&st.t_f1,N,t,c,a,b,d,e,y);}
int bag_backpack(int t,int c,int a,int b,int d,int e,int y){return add(bps,&st.n_bp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bag_luggage(int t,int c,int a,int b,int d,int e,int y){return add(lgs,&st.n_lg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bag_wallet(int t,int c,int a,int b,int d,int e,int y){return add(wls,&st.n_wl,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bag_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bag_report(void){ps("[BAG] Hand: ");pi(st.n_hd);ps(" PCS=");pi(st.t_f1);ps("\nBack: ");pi(st.n_bp);ps(" PCS=");pi(st.t_f2);ps("\nLug: ");pi(st.n_lg);ps(" PCS=");pi(st.t_f3);ps("\nWallet: ");pi(st.n_wl);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bag_state(void){ps("[BAG] Hd=");pi(st.n_hd);ps(" Bp=");pi(st.n_bp);ps(" Lg=");pi(st.n_lg);ps(" Wl=");pi(st.n_wl);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bag Tech Admin Demo ===\n\n");bag_init();
ps("Handbags...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bag_handbag(t,c,198+(i*17),183+(i*14),163+(i*10),145+(i*6),2020+(i%5));}
ps("\nBackpacks...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bag_backpack(t,c,187+(i*15),173+(i*12),155+(i*8),142+(i*5),2021+(i%4));}
ps("\nLuggage...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bag_luggage(t,c,179+(i*13),165+(i*10),149+(i*7),138+(i*4),2022+(i%3));}
ps("\nWallets...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bag_wallet(t,c,171+(i*11),159+(i*9),145+(i*6),135+(i*3),2023+(i%2));}
ps("\nBag marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bag_market(t,c,165+(i*9),154+(i*7),141+(i*5),133+(i*3),2024);}
ps("\n");bag_report();bag_state();ps("\n=== Demo Complete ===\n");return 0;}
