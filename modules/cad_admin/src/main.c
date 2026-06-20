/* cad_admin: CAD technology administration (v1.0)
 * Mechanical CAD, electrical CAD, architectural CAD, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cdx_t;
typedef struct{int n_mc,n_el,n_ar,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cdx_state_t;
static cdx_t mcs[N],els[N-2],ars[N-4],acs[N-6],mks[N-6]; static cdx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cdx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cdx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CDX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cdx_init(void){if(init)return -1;st.n_mc=0;st.n_el=0;st.n_ar=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)els[i].active=0;for(int i=0;i<N-4;i++)ars[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CDX] CAD initialized\n");return 0;}
int cdx_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int cdx_electrical(int t,int c,int a,int b,int d,int e,int y){return add(els,&st.n_el,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cdx_architectural(int t,int c,int a,int b,int d,int e,int y){return add(ars,&st.n_ar,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cdx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cdx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cdx_report(void){ps("[CDX] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nEl: ");pi(st.n_el);ps(" PCS=");pi(st.t_f2);ps("\nAr: ");pi(st.n_ar);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cdx_state(void){ps("[CDX] Mc=");pi(st.n_mc);ps(" El=");pi(st.n_el);ps(" Ar=");pi(st.n_ar);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== CAD Admin Demo ===\n\n");cdx_init();
ps("Mechanical CAD...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cdx_mechanical(t,c,425+(i*17),414+(i*14),394+(i*10),376+(i*6),2020+(i%5));}
ps("\nElectrical CAD...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cdx_electrical(t,c,414+(i*15),403+(i*12),385+(i*8),372+(i*5),2021+(i%4));}
ps("\nArchitectural CAD...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cdx_architectural(t,c,406+(i*13),395+(i*10),379+(i*7),368+(i*4),2022+(i%3));}
ps("\nCAD accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cdx_accessory(t,c,398+(i*11),389+(i*9),375+(i*6),365+(i*3),2023+(i%2));}
ps("\nCAD marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cdx_market(t,c,392+(i*9),383+(i*7),370+(i*5),362+(i*3),2024);}
ps("\n");cdx_report();cdx_state();ps("\n=== Demo Complete ===\n");return 0;}
