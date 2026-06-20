/* candytech_admin: Candy technology administration (v1.0)
 * Hard candy, soft candy, chocolate candy, chewing gum, candy marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cdy_t;
typedef struct{int n_hd,n_sf,n_cc,n_cg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cdy_state_t;
static cdy_t hds[N],sfs[N-2],ccs[N-4],cgs[N-6],mks[N-6]; static cdy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cdy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cdy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CDY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cdy_init(void){if(init)return -1;st.n_hd=0;st.n_sf=0;st.n_cc=0;st.n_cg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hds[i].active=0;for(int i=0;i<N-2;i++)sfs[i].active=0;for(int i=0;i<N-4;i++)ccs[i].active=0;for(int i=0;i<N-6;i++)cgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CDY] Candytech initialized\n");return 0;}
int cdy_hard(int t,int c,int a,int b,int d,int e,int y){return add(hds,&st.n_hd,&st.t_f1,N,t,c,a,b,d,e,y);}
int cdy_soft(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cdy_choco(int t,int c,int a,int b,int d,int e,int y){return add(ccs,&st.n_cc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cdy_gum(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cdy_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cdy_report(void){ps("[CDY] Hard: ");pi(st.n_hd);ps(" PCS=");pi(st.t_f1);ps("\nSoft: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f2);ps("\nChoco: ");pi(st.n_cc);ps(" PCS=");pi(st.t_f3);ps("\nGum: ");pi(st.n_cg);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cdy_state(void){ps("[CDY] Hd=");pi(st.n_hd);ps(" Sf=");pi(st.n_sf);ps(" Cc=");pi(st.n_cc);ps(" Cg=");pi(st.n_cg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Candy Tech Admin Demo ===\n\n");cdy_init();
ps("Hard candy...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cdy_hard(t,c,183+(i*17),168+(i*14),148+(i*10),130+(i*6),2020+(i%5));}
ps("\nSoft candy...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cdy_soft(t,c,172+(i*15),158+(i*12),140+(i*8),127+(i*5),2021+(i%4));}
ps("\nChocolate candy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cdy_choco(t,c,164+(i*13),150+(i*10),134+(i*7),123+(i*4),2022+(i%3));}
ps("\nChewing gum...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cdy_gum(t,c,156+(i*11),144+(i*9),130+(i*6),120+(i*3),2023+(i%2));}
ps("\nCandy marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cdy_market(t,c,150+(i*9),139+(i*7),126+(i*5),118+(i*3),2024);}
ps("\n");cdy_report();cdy_state();ps("\n=== Demo Complete ===\n");return 0;}
