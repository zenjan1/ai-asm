/* saucetech_admin: Sauce technology administration (v1.0)
 * Soy sauce brewing, vinegar brewing, hot sauce production, compound seasoning, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sce_t;
typedef struct{int n_sy,n_vn,n_hs,n_cp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sce_state_t;
static sce_t sys[N],vns[N-2],hss[N-4],cps[N-6],mks[N-6]; static sce_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sce_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sce_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SCE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sce_init(void){if(init)return -1;st.n_sy=0;st.n_vn=0;st.n_hs=0;st.n_cp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sys[i].active=0;for(int i=0;i<N-2;i++)vns[i].active=0;for(int i=0;i<N-4;i++)hss[i].active=0;for(int i=0;i<N-6;i++)cps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SCE] Saucetech initialized\n");return 0;}
int sce_soy(int t,int c,int a,int b,int d,int e,int y){return add(sys,&st.n_sy,&st.t_f1,N,t,c,a,b,d,e,y);}
int sce_vinegar(int t,int c,int a,int b,int d,int e,int y){return add(vns,&st.n_vn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sce_hot(int t,int c,int a,int b,int d,int e,int y){return add(hss,&st.n_hs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sce_compound(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sce_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sce_report(void){ps("[SCE] Soy: ");pi(st.n_sy);ps(" L=");pi(st.t_f1);ps("\nVinegar: ");pi(st.n_vn);ps(" L=");pi(st.t_f2);ps("\nHot: ");pi(st.n_hs);ps(" kg=");pi(st.t_f3);ps("\nCompound: ");pi(st.n_cp);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sce_state(void){ps("[SCE] Sy=");pi(st.n_sy);ps(" Vn=");pi(st.n_vn);ps(" Hs=");pi(st.n_hs);ps(" Cp=");pi(st.n_cp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sauce Tech Admin Demo ===\n\n");sce_init();
ps("Soy sauce brewing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sce_soy(t,c,182+(i*17),167+(i*14),147+(i*10),129+(i*6),2020+(i%5));}
ps("\nVinegar brewing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sce_vinegar(t,c,171+(i*15),157+(i*12),139+(i*8),126+(i*5),2021+(i%4));}
ps("\nHot sauce production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sce_hot(t,c,163+(i*13),149+(i*10),133+(i*7),122+(i*4),2022+(i%3));}
ps("\nCompound seasoning...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sce_compound(t,c,155+(i*11),143+(i*9),129+(i*6),119+(i*3),2023+(i%2));}
ps("\nSauce marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sce_market(t,c,149+(i*9),138+(i*7),125+(i*5),117+(i*3),2024);}
ps("\n");sce_report();sce_state();ps("\n=== Demo Complete ===\n");return 0;}
