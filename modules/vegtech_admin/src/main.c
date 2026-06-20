/* vegtech_admin: Vegetable technology administration (v1.0)
 * Vegetable planting, vegetable processing, vegetable packaging, cold chain, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vgt_t;
typedef struct{int n_vp,n_vg,n_vpk,n_vc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vgt_state_t;
static vgt_t vps[N],vgs[N-2],vpks[N-4],vcs[N-6],mks[N-6]; static vgt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vgt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vgt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vgt_init(void){if(init)return -1;st.n_vp=0;st.n_vg=0;st.n_vpk=0;st.n_vc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vps[i].active=0;for(int i=0;i<N-2;i++)vgs[i].active=0;for(int i=0;i<N-4;i++)vpks[i].active=0;for(int i=0;i<N-6;i++)vcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[VGT] Vegtech initialized\n");return 0;}
int vgt_plant(int t,int c,int a,int b,int d,int e,int y){return add(vps,&st.n_vp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vgt_process(int t,int c,int a,int b,int d,int e,int y){return add(vgs,&st.n_vg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vgt_package(int t,int c,int a,int b,int d,int e,int y){return add(vpks,&st.n_vpk,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vgt_cold(int t,int c,int a,int b,int d,int e,int y){return add(vcs,&st.n_vc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vgt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vgt_report(void){ps("[VGT] Plant: ");pi(st.n_vp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_vg);ps(" kg=");pi(st.t_f2);ps("\nPkg: ");pi(st.n_vpk);ps(" PCS=");pi(st.t_f3);ps("\nCold: ");pi(st.n_vc);ps(" Ton=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vgt_state(void){ps("[VGT] Vp=");pi(st.n_vp);ps(" Vg=");pi(st.n_vg);ps(" Vpk=");pi(st.n_vpk);ps(" Vc=");pi(st.n_vc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Veg Tech Admin Demo ===\n\n");vgt_init();
ps("Vegetable planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vgt_plant(t,c,191+(i*17),176+(i*14),156+(i*10),138+(i*6),2020+(i%5));}
ps("\nVegetable processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vgt_process(t,c,180+(i*15),166+(i*12),148+(i*8),135+(i*5),2021+(i%4));}
ps("\nVegetable packaging...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vgt_package(t,c,172+(i*13),158+(i*10),142+(i*7),131+(i*4),2022+(i%3));}
ps("\nCold chain...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vgt_cold(t,c,164+(i*11),152+(i*9),138+(i*6),128+(i*3),2023+(i%2));}
ps("\nVeg marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vgt_market(t,c,158+(i*9),147+(i*7),134+(i*5),126+(i*3),2024);}
ps("\n");vgt_report();vgt_state();ps("\n=== Demo Complete ===\n");return 0;}
