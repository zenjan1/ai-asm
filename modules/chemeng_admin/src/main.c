/* chemeng_admin: Chemical engineering technology administration (v1.0)
 * Basic chemicals, fine chemicals, polymer chemicals, chemical equipment, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cme_t;
typedef struct{int n_bc,n_fc,n_pc,n_ce,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cme_state_t;
static cme_t bcs[N],fcs[N-2],pcss[N-4],ces[N-6],mks[N-6]; static cme_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cme_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cme_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CME] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cme_init(void){if(init)return -1;st.n_bc=0;st.n_fc=0;st.n_pc=0;st.n_ce=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bcs[i].active=0;for(int i=0;i<N-2;i++)fcs[i].active=0;for(int i=0;i<N-4;i++)pcss[i].active=0;for(int i=0;i<N-6;i++)ces[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CME] Chemeng initialized\n");return 0;}
int cme_basic(int t,int c,int a,int b,int d,int e,int y){return add(bcs,&st.n_bc,&st.t_f1,N,t,c,a,b,d,e,y);}
int cme_fine(int t,int c,int a,int b,int d,int e,int y){return add(fcs,&st.n_fc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cme_polymer(int t,int c,int a,int b,int d,int e,int y){return add(pcss,&st.n_pc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cme_equipment(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cme_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cme_report(void){ps("[CME] Bc: ");pi(st.n_bc);ps(" Ton=");pi(st.t_f1);ps("\nFc: ");pi(st.n_fc);ps(" kg=");pi(st.t_f2);ps("\nPc: ");pi(st.n_pc);ps(" kg=");pi(st.t_f3);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cme_state(void){ps("[CME] Bc=");pi(st.n_bc);ps(" Fc=");pi(st.n_fc);ps(" Pc=");pi(st.n_pc);ps(" Ce=");pi(st.n_ce);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chemical Eng Tech Admin Demo ===\n\n");cme_init();
ps("Basic chemicals...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cme_basic(t,c,229+(i*17),214+(i*14),194+(i*10),176+(i*6),2020+(i%5));}
ps("\nFine chemicals...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cme_fine(t,c,218+(i*15),204+(i*12),186+(i*8),173+(i*5),2021+(i%4));}
ps("\nPolymer chemicals...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cme_polymer(t,c,210+(i*13),196+(i*10),180+(i*7),169+(i*4),2022+(i%3));}
ps("\nChemical equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cme_equipment(t,c,202+(i*11),190+(i*9),176+(i*6),166+(i*3),2023+(i%2));}
ps("\nChemical marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cme_market(t,c,196+(i*9),185+(i*7),172+(i*5),164+(i*3),2024);}
ps("\n");cme_report();cme_state();ps("\n=== Demo Complete ===\n");return 0;}
