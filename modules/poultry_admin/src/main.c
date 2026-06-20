/* poultry_admin: Poultry management technology administration (v1.0)
 * Poultry planning, poultry execution, poultry evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} plx_t;
typedef struct{int n_pp,n_pe,n_pv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} plx_state_t;
static plx_t pps[N],pes[N-2],pvs[N-4],acs[N-6],mks[N-6]; static plx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;plx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int plx_init(void){if(init)return -1;st.n_pp=0;st.n_pe=0;st.n_pv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)pes[i].active=0;for(int i=0;i<N-4;i++)pvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PLX] Poultry initialized\n");return 0;}
int plx_planning(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int plx_execution(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int plx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int plx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int plx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void plx_report(void){ps("[PLX] Pp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f1);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f2);ps("\nPv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void plx_state(void){ps("[PLX] Pp=");pi(st.n_pp);ps(" Pe=");pi(st.n_pe);ps(" Pv=");pi(st.n_pv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Poultry Admin Demo ===\n\n");plx_init();
ps("Poultry planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;plx_planning(t,c,508+(i*17),497+(i*14),477+(i*10),459+(i*6),2020+(i%5));}
ps("\nPoultry execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;plx_execution(t,c,497+(i*15),486+(i*12),468+(i*8),455+(i*5),2021+(i%4));}
ps("\nPoultry evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;plx_evaluation(t,c,489+(i*13),478+(i*10),462+(i*7),451+(i*4),2022+(i%3));}
ps("\nPoultry accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plx_accessory(t,c,481+(i*11),472+(i*9),458+(i*6),448+(i*3),2023+(i%2));}
ps("\nPoultry marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plx_market(t,c,475+(i*9),466+(i*7),453+(i*5),445+(i*3),2024);}
ps("\n");plx_report();plx_state();ps("\n=== Demo Complete ===\n");return 0;}
