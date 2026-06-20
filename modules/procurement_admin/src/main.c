/* procurement_admin: Procurement technology administration (v1.0)
 * Procurement planning, procurement execution, strategic procurement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prc_t;
typedef struct{int n_pl,n_ex,n_st,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prc_state_t;
static prc_t pls[N],exs[N-2],sts[N-4],acs[N-6],mks[N-6]; static prc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prc_init(void){if(init)return -1;st.n_pl=0;st.n_ex=0;st.n_st=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)sts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PRC] Procurement initialized\n");return 0;}
int prc_planning(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int prc_execution(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prc_strategic(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prc_report(void){ps("[PRC] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nEx: ");pi(st.n_ex);ps(" PCS=");pi(st.t_f2);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prc_state(void){ps("[PRC] Pl=");pi(st.n_pl);ps(" Ex=");pi(st.n_ex);ps(" St=");pi(st.n_st);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Procurement Admin Demo ===\n\n");prc_init();
ps("Procurement planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prc_planning(t,c,437+(i*17),426+(i*14),406+(i*10),388+(i*6),2020+(i%5));}
ps("\nProcurement execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prc_execution(t,c,426+(i*15),415+(i*12),397+(i*8),384+(i*5),2021+(i%4));}
ps("\nStrategic procurement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prc_strategic(t,c,418+(i*13),407+(i*10),391+(i*7),380+(i*4),2022+(i%3));}
ps("\nProcurement accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prc_accessory(t,c,410+(i*11),401+(i*9),387+(i*6),377+(i*3),2023+(i%2));}
ps("\nProcurement marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prc_market(t,c,404+(i*9),395+(i*7),382+(i*5),374+(i*3),2024);}
ps("\n");prc_report();prc_state();ps("\n=== Demo Complete ===\n");return 0;}
