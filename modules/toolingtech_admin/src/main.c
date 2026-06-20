/* toolingtech_admin: Tooling technology administration (v1.0)
 * Assembly tooling, machining tooling, inspection tooling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tlg_t;
typedef struct{int n_as,n_mc,n_in,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tlg_state_t;
static tlg_t ass[N],mcs[N-2],ins[N-4],acs[N-6],mks[N-6]; static tlg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tlg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tlg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TLG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tlg_init(void){if(init)return -1;st.n_as=0;st.n_mc=0;st.n_in=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ass[i].active=0;for(int i=0;i<N-2;i++)mcs[i].active=0;for(int i=0;i<N-4;i++)ins[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TLG] Toolingtech initialized\n");return 0;}
int tlg_assembly(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f1,N,t,c,a,b,d,e,y);}
int tlg_machining(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tlg_inspection(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tlg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tlg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tlg_report(void){ps("[TLG] As: ");pi(st.n_as);ps(" PCS=");pi(st.t_f1);ps("\nMc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f2);ps("\nIn: ");pi(st.n_in);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tlg_state(void){ps("[TLG] As=");pi(st.n_as);ps(" Mc=");pi(st.n_mc);ps(" In=");pi(st.n_in);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tooling Tech Admin Demo ===\n\n");tlg_init();
ps("Assembly tooling...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tlg_assembly(t,c,302+(i*17),287+(i*14),267+(i*10),249+(i*6),2020+(i%5));}
ps("\nMachining tooling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tlg_machining(t,c,291+(i*15),277+(i*12),259+(i*8),246+(i*5),2021+(i%4));}
ps("\nInspection tooling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tlg_inspection(t,c,283+(i*13),269+(i*10),253+(i*7),242+(i*4),2022+(i%3));}
ps("\nTooling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tlg_accessory(t,c,275+(i*11),263+(i*9),249+(i*6),239+(i*3),2023+(i%2));}
ps("\nTooling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tlg_market(t,c,269+(i*9),258+(i*7),245+(i*5),237+(i*3),2024);}
ps("\n");tlg_report();tlg_state();ps("\n=== Demo Complete ===\n");return 0;}
