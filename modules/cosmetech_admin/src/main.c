/* cosmetech_admin: Cosmetics technology administration (v1.0)
 * Skincare, makeup, perfume, haircare, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} csm_t;
typedef struct{int n_sk,n_mu,n_pf,n_hc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} csm_state_t;
static csm_t sks[N],mus[N-2],pfs[N-4],hcs[N-6],mks[N-6]; static csm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(csm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;csm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CSM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int csm_init(void){if(init)return -1;st.n_sk=0;st.n_mu=0;st.n_pf=0;st.n_hc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sks[i].active=0;for(int i=0;i<N-2;i++)mus[i].active=0;for(int i=0;i<N-4;i++)pfs[i].active=0;for(int i=0;i<N-6;i++)hcs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CSM] Cosmetech initialized\n");return 0;}
int csm_skincare(int t,int c,int a,int b,int d,int e,int y){return add(sks,&st.n_sk,&st.t_f1,N,t,c,a,b,d,e,y);}
int csm_makeup(int t,int c,int a,int b,int d,int e,int y){return add(mus,&st.n_mu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int csm_perfume(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int csm_haircare(int t,int c,int a,int b,int d,int e,int y){return add(hcs,&st.n_hc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int csm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void csm_report(void){ps("[CSM] Skin: ");pi(st.n_sk);ps(" PCS=");pi(st.t_f1);ps("\nMake: ");pi(st.n_mu);ps(" PCS=");pi(st.t_f2);ps("\nPerf: ");pi(st.n_pf);ps(" PCS=");pi(st.t_f3);ps("\nHair: ");pi(st.n_hc);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void csm_state(void){ps("[CSM] Sk=");pi(st.n_sk);ps(" Mu=");pi(st.n_mu);ps(" Pf=");pi(st.n_pf);ps(" Hc=");pi(st.n_hc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cosmetics Tech Admin Demo ===\n\n");csm_init();
ps("Skincare...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;csm_skincare(t,c,204+(i*17),189+(i*14),169+(i*10),151+(i*6),2020+(i%5));}
ps("\nMakeup...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;csm_makeup(t,c,193+(i*15),179+(i*12),161+(i*8),148+(i*5),2021+(i%4));}
ps("\nPerfume...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;csm_perfume(t,c,185+(i*13),171+(i*10),155+(i*7),144+(i*4),2022+(i%3));}
ps("\nHaircare...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csm_haircare(t,c,177+(i*11),165+(i*9),151+(i*6),141+(i*3),2023+(i%2));}
ps("\nCosmetics marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csm_market(t,c,171+(i*9),160+(i*7),147+(i*5),139+(i*3),2024);}
ps("\n");csm_report();csm_state();ps("\n=== Demo Complete ===\n");return 0;}
