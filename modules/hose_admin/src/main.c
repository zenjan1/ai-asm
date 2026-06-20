/* hose_admin: Hose technology administration (v1.0)
 * Rubber hose, plastic hose, metal hose, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hsa_t;
typedef struct{int n_rb,n_pl,n_mt,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hsa_state_t;
static hsa_t rbs[N],pls[N-2],mts[N-4],acs[N-6],mks[N-6]; static hsa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hsa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hsa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HSA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hsa_init(void){if(init)return -1;st.n_rb=0;st.n_pl=0;st.n_mt=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rbs[i].active=0;for(int i=0;i<N-2;i++)pls[i].active=0;for(int i=0;i<N-4;i++)mts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[HSA] Hose initialized\n");return 0;}
int hsa_rubber(int t,int c,int a,int b,int d,int e,int y){return add(rbs,&st.n_rb,&st.t_f1,N,t,c,a,b,d,e,y);}
int hsa_plastic(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hsa_metal(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hsa_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hsa_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hsa_report(void){ps("[HSA] Rb: ");pi(st.n_rb);ps(" PCS=");pi(st.t_f1);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f2);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hsa_state(void){ps("[HSA] Rb=");pi(st.n_rb);ps(" Pl=");pi(st.n_pl);ps(" Mt=");pi(st.n_mt);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hose Admin Demo ===\n\n");hsa_init();
ps("Rubber hoses...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hsa_rubber(t,c,366+(i*17),351+(i*14),331+(i*10),313+(i*6),2020+(i%5));}
ps("\nPlastic hoses...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hsa_plastic(t,c,355+(i*15),341+(i*12),323+(i*8),310+(i*5),2021+(i%4));}
ps("\nMetal hoses...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hsa_metal(t,c,347+(i*13),333+(i*10),317+(i*7),306+(i*4),2022+(i%3));}
ps("\nHose accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hsa_accessory(t,c,339+(i*11),327+(i*9),313+(i*6),303+(i*3),2023+(i%2));}
ps("\nHose marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hsa_market(t,c,333+(i*9),322+(i*7),309+(i*5),301+(i*3),2024);}
ps("\n");hsa_report();hsa_state();ps("\n=== Demo Complete ===\n");return 0;}
