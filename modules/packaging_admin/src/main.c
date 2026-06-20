/* packaging_admin: Packaging technology administration (v1.0)
 * Packaging materials, packaging containers, packaging equipment, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pkx_t;
typedef struct{int n_mt,n_ct,n_eq,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pkx_state_t;
static pkx_t mts[N],cts[N-2],eqs[N-4],acs[N-6],mks[N-6]; static pkx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pkx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pkx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PKX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pkx_init(void){if(init)return -1;st.n_mt=0;st.n_ct=0;st.n_eq=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mts[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PKX] Packaging initialized\n");return 0;}
int pkx_material(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f1,N,t,c,a,b,d,e,y);}
int pkx_container(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pkx_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pkx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pkx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pkx_report(void){ps("[PKX] Mt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nEq: ");pi(st.n_eq);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pkx_state(void){ps("[PKX] Mt=");pi(st.n_mt);ps(" Ct=");pi(st.n_ct);ps(" Eq=");pi(st.n_eq);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Packaging Admin Demo ===\n\n");pkx_init();
ps("Packaging materials...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pkx_material(t,c,438+(i*17),427+(i*14),407+(i*10),389+(i*6),2020+(i%5));}
ps("\nPackaging containers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pkx_container(t,c,427+(i*15),416+(i*12),398+(i*8),385+(i*5),2021+(i%4));}
ps("\nPackaging equipment...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pkx_equipment(t,c,419+(i*13),408+(i*10),392+(i*7),381+(i*4),2022+(i%3));}
ps("\nPackaging accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pkx_accessory(t,c,411+(i*11),402+(i*9),388+(i*6),378+(i*3),2023+(i%2));}
ps("\nPackaging marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pkx_market(t,c,405+(i*9),396+(i*7),383+(i*5),375+(i*3),2024);}
ps("\n");pkx_report();pkx_state();ps("\n=== Demo Complete ===\n");return 0;}
