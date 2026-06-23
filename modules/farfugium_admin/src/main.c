/* farfugium_admin: Farfugium management technology administration (v1.0)
 * Farfugium planning, farfugium execution, farfugium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} farf_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} farf_state_t;
static farf_t farfp[N],farfx[N-2],farf2[N-4],farfac[N-6],farfm[N-6]; static farf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(farf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;farf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FARF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int farf_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)farfp[i].active=0;for(int i=0;i<N-2;i++)farfx[i].active=0;for(int i=0;i<N-4;i++)farf2[i].active=0;for(int i=0;i<N-6;i++)farfac[i].active=0;for(int i=0;i<N-6;i++)farfm[i].active=0;init=1;ps("[FARF] Farfugium initialized\n");return 0;}
int farf_planning(int t,int c,int a,int b,int d,int e,int y){return add(farfp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int farf_execution(int t,int c,int a,int b,int d,int e,int y){return add(farfx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int farf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(farf2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int farf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(farfac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int farf_market(int t,int c,int a,int b,int d,int e,int y){return add(farfm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void farf_report(void){ps("[FARF] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void farf_state(void){ps("[FARF] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Farfugium Admin Demo ===\n\n");farf_init();
ps("Farfugium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;farf_planning(t,c,1844+(i*17),1833+(i*14),1813+(i*10),1795+(i*6),2020+(i%5));}
ps("\nFarfugium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;farf_execution(t,c,1833+(i*15),1822+(i*12),1804+(i*8),1791+(i*5),2021+(i%4));}
ps("\nFarfugium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;farf_evaluation(t,c,1825+(i*13),1814+(i*10),1798+(i*7),1787+(i*4),2022+(i%3));}
ps("\nFarfugium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;farf_accessory(t,c,1817+(i*11),1808+(i*9),1794+(i*6),1784+(i*3),2023+(i%2));}
ps("\nFarfugium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;farf_market(t,c,1811+(i*9),1802+(i*7),1789+(i*5),1781+(i*3),2024);}
ps("\n");farf_report();farf_state();ps("\n=== Demo Complete ===\n");return 0;}
