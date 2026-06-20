/* diamondcoat_admin: Diamond coating technology administration (v1.0)
 * CVD diamond, PVD diamond, diamond film, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dmc_t;
typedef struct{int n_cv,n_pv,n_fm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dmc_state_t;
static dmc_t cvs[N],pvs[N-2],fms[N-4],acs[N-6],mks[N-6]; static dmc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dmc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dmc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DMC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dmc_init(void){if(init)return -1;st.n_cv=0;st.n_pv=0;st.n_fm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cvs[i].active=0;for(int i=0;i<N-2;i++)pvs[i].active=0;for(int i=0;i<N-4;i++)fms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DMC] Diamondcoat initialized\n");return 0;}
int dmc_cvd(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f1,N,t,c,a,b,d,e,y);}
int dmc_pvd(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dmc_film(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dmc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dmc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dmc_report(void){ps("[DMC] Cv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f1);ps("\nPv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f2);ps("\nFm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dmc_state(void){ps("[DMC] Cv=");pi(st.n_cv);ps(" Pv=");pi(st.n_pv);ps(" Fm=");pi(st.n_fm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Diamond Coat Admin Demo ===\n\n");dmc_init();
ps("CVD diamond coating...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dmc_cvd(t,c,345+(i*17),330+(i*14),310+(i*10),292+(i*6),2020+(i%5));}
ps("\nPVD diamond coating...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dmc_pvd(t,c,334+(i*15),320+(i*12),302+(i*8),289+(i*5),2021+(i%4));}
ps("\nDiamond film coating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dmc_film(t,c,326+(i*13),312+(i*10),296+(i*7),285+(i*4),2022+(i%3));}
ps("\nDiamond coating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dmc_accessory(t,c,318+(i*11),306+(i*9),292+(i*6),282+(i*3),2023+(i%2));}
ps("\nDiamond coating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dmc_market(t,c,312+(i*9),301+(i*7),288+(i*5),280+(i*3),2024);}
ps("\n");dmc_report();dmc_state();ps("\n=== Demo Complete ===\n");return 0;}
