/* metallizing_admin: Metallizing technology administration (v1.0)
 * Flame spraying, arc spraying, plasma spraying, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtl_t;
typedef struct{int n_fm,n_ar,n_pl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mtl_state_t;
static mtl_t fms[N],ars[N-2],pls[N-4],acs[N-6],mks[N-6]; static mtl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtl_init(void){if(init)return -1;st.n_fm=0;st.n_ar=0;st.n_pl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fms[i].active=0;for(int i=0;i<N-2;i++)ars[i].active=0;for(int i=0;i<N-4;i++)pls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MTL] Metallizing initialized\n");return 0;}
int mtl_flame(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtl_arc(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtl_plasma(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtl_report(void){ps("[MTL] Fm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f1);ps("\nAr: ");pi(st.n_ar);ps(" PCS=");pi(st.t_f2);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mtl_state(void){ps("[MTL] Fm=");pi(st.n_fm);ps(" Ar=");pi(st.n_ar);ps(" Pl=");pi(st.n_pl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Metallizing Admin Demo ===\n\n");mtl_init();
ps("Flame spraying...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtl_flame(t,c,342+(i*17),327+(i*14),307+(i*10),289+(i*6),2020+(i%5));}
ps("\nArc spraying...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtl_arc(t,c,331+(i*15),317+(i*12),299+(i*8),286+(i*5),2021+(i%4));}
ps("\nPlasma spraying...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtl_plasma(t,c,323+(i*13),309+(i*10),293+(i*7),282+(i*4),2022+(i%3));}
ps("\nMetallizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtl_accessory(t,c,315+(i*11),303+(i*9),289+(i*6),279+(i*3),2023+(i%2));}
ps("\nMetallizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtl_market(t,c,309+(i*9),298+(i*7),285+(i*5),277+(i*3),2024);}
ps("\n");mtl_report();mtl_state();ps("\n=== Demo Complete ===\n");return 0;}
