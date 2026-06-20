/* castech_admin: Casting technology administration (v1.0)
 * Sand casting, investment casting, die casting, centrifugal casting, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cst_t;
typedef struct{int n_sc,n_ic,n_dc,n_cc,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cst_state_t;
static cst_t scs[N],ics[N-2],dcs[N-4],ccs[N-6],mks[N-6]; static cst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cst_init(void){if(init)return -1;st.n_sc=0;st.n_ic=0;st.n_dc=0;st.n_cc=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)scs[i].active=0;for(int i=0;i<N-2;i++)ics[i].active=0;for(int i=0;i<N-4;i++)dcs[i].active=0;for(int i=0;i<N-6;i++)ccs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CST] Castech initialized\n");return 0;}
int cst_sand(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f1,N,t,c,a,b,d,e,y);}
int cst_investment(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cst_die(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cst_centrifugal(int t,int c,int a,int b,int d,int e,int y){return add(ccs,&st.n_cc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cst_report(void){ps("[CST] Sc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f1);ps("\nIc: ");pi(st.n_ic);ps(" PCS=");pi(st.t_f2);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f3);ps("\nCc: ");pi(st.n_cc);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cst_state(void){ps("[CST] Sc=");pi(st.n_sc);ps(" Ic=");pi(st.n_ic);ps(" Dc=");pi(st.n_dc);ps(" Cc=");pi(st.n_cc);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Casting Tech Admin Demo ===\n\n");cst_init();
ps("Sand casting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cst_sand(t,c,249+(i*17),234+(i*14),214+(i*10),196+(i*6),2020+(i%5));}
ps("\nInvestment casting...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cst_investment(t,c,238+(i*15),224+(i*12),206+(i*8),193+(i*5),2021+(i%4));}
ps("\nDie casting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cst_die(t,c,230+(i*13),216+(i*10),200+(i*7),189+(i*4),2022+(i%3));}
ps("\nCentrifugal casting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_centrifugal(t,c,222+(i*11),210+(i*9),196+(i*6),186+(i*3),2023+(i%2));}
ps("\nCasting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_market(t,c,216+(i*9),205+(i*7),192+(i*5),184+(i*3),2024);}
ps("\n");cst_report();cst_state();ps("\n=== Demo Complete ===\n");return 0;}
