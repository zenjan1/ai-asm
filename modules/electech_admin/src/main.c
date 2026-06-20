/* electech_admin: Electronics technology administration (v1.0)
 * Electronic components, circuit boards, consumer electronics, electronic instruments, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} elt_t;
typedef struct{int n_ec,n_pcb,n_ce,n_ei,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} elt_state_t;
static elt_t ecs[N],pcbs[N-2],ces[N-4],eis[N-6],mks[N-6]; static elt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;elt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ELT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int elt_init(void){if(init)return -1;st.n_ec=0;st.n_pcb=0;st.n_ce=0;st.n_ei=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ecs[i].active=0;for(int i=0;i<N-2;i++)pcbs[i].active=0;for(int i=0;i<N-4;i++)ces[i].active=0;for(int i=0;i<N-6;i++)eis[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ELT] Electech initialized\n");return 0;}
int elt_component(int t,int c,int a,int b,int d,int e,int y){return add(ecs,&st.n_ec,&st.t_f1,N,t,c,a,b,d,e,y);}
int elt_board(int t,int c,int a,int b,int d,int e,int y){return add(pcbs,&st.n_pcb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int elt_consumer(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int elt_instrument(int t,int c,int a,int b,int d,int e,int y){return add(eis,&st.n_ei,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int elt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void elt_report(void){ps("[ELT] Comp: ");pi(st.n_ec);ps(" PCS=");pi(st.t_f1);ps("\nPCB: ");pi(st.n_pcb);ps(" PCS=");pi(st.t_f2);ps("\nCE: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f3);ps("\nIns: ");pi(st.n_ei);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void elt_state(void){ps("[ELT] Ec=");pi(st.n_ec);ps(" Pcb=");pi(st.n_pcb);ps(" Ce=");pi(st.n_ce);ps(" Ei=");pi(st.n_ei);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Electronics Tech Admin Demo ===\n\n");elt_init();
ps("Electronic components...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;elt_component(t,c,218+(i*17),203+(i*14),183+(i*10),165+(i*6),2020+(i%5));}
ps("\nCircuit boards...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;elt_board(t,c,207+(i*15),193+(i*12),175+(i*8),162+(i*5),2021+(i%4));}
ps("\nConsumer electronics...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;elt_consumer(t,c,199+(i*13),185+(i*10),169+(i*7),158+(i*4),2022+(i%3));}
ps("\nElectronic instruments...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elt_instrument(t,c,191+(i*11),179+(i*9),165+(i*6),155+(i*3),2023+(i%2));}
ps("\nElectronics marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;elt_market(t,c,185+(i*9),174+(i*7),161+(i*5),153+(i*3),2024);}
ps("\n");elt_report();elt_state();ps("\n=== Demo Complete ===\n");return 0;}
