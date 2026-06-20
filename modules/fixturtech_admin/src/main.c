/* fixturtech_admin: Fixture technology administration (v1.0)
 * Machine fixtures, welding fixtures, assembly fixtures, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fxt_t;
typedef struct{int n_mc,n_wd,n_as,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fxt_state_t;
static fxt_t mcs[N],wds[N-2],ass[N-4],acs[N-6],mks[N-6]; static fxt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fxt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fxt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FXT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fxt_init(void){if(init)return -1;st.n_mc=0;st.n_wd=0;st.n_as=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)wds[i].active=0;for(int i=0;i<N-4;i++)ass[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FXT] Fixturtech initialized\n");return 0;}
int fxt_machine(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int fxt_welding(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fxt_assembly(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fxt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fxt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fxt_report(void){ps("[FXT] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f2);ps("\nAs: ");pi(st.n_as);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fxt_state(void){ps("[FXT] Mc=");pi(st.n_mc);ps(" Wd=");pi(st.n_wd);ps(" As=");pi(st.n_as);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fixture Tech Admin Demo ===\n\n");fxt_init();
ps("Machine fixtures...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fxt_machine(t,c,292+(i*17),277+(i*14),257+(i*10),239+(i*6),2020+(i%5));}
ps("\nWelding fixtures...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fxt_welding(t,c,281+(i*15),267+(i*12),249+(i*8),236+(i*5),2021+(i%4));}
ps("\nAssembly fixtures...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fxt_assembly(t,c,273+(i*13),259+(i*10),243+(i*7),232+(i*4),2022+(i%3));}
ps("\nFixture accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxt_accessory(t,c,265+(i*11),253+(i*9),239+(i*6),229+(i*3),2023+(i%2));}
ps("\nFixture marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fxt_market(t,c,259+(i*9),248+(i*7),235+(i*5),227+(i*3),2024);}
ps("\n");fxt_report();fxt_state();ps("\n=== Demo Complete ===\n");return 0;}
