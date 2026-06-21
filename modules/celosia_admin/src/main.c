/* celosia_admin: Celosia management technology administration (v1.0)
 * Celosia planning, celosia execution, celosia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} csl_t;
typedef struct{int n_csp,n_cse,n_csv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} csl_state_t;
static csl_t csps[N],csss[N-2],csvss[N-4],csas[N-6],csmks[N-6]; static csl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(csl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;csl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CSL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int csl_init(void){if(init)return -1;st.n_csp=0;st.n_cse=0;st.n_csv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)csps[i].active=0;for(int i=0;i<N-2;i++)csss[i].active=0;for(int i=0;i<N-4;i++)csvss[i].active=0;for(int i=0;i<N-6;i++)csas[i].active=0;for(int i=0;i<N-6;i++)csmks[i].active=0;init=1;ps("[CSL] Celosia initialized\n");return 0;}
int csl_planning(int t,int c,int a,int b,int d,int e,int y){return add(csps,&st.n_csp,&st.t_f1,N,t,c,a,b,d,e,y);}
int csl_execution(int t,int c,int a,int b,int d,int e,int y){return add(csss,&st.n_cse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int csl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(csvss,&st.n_csv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int csl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(csas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int csl_market(int t,int c,int a,int b,int d,int e,int y){return add(csmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void csl_report(void){ps("[CSL] Csp: ");pi(st.n_csp);ps(" PCS=");pi(st.t_f1);ps("\nCse: ");pi(st.n_cse);ps(" PCS=");pi(st.t_f2);ps("\nCsv: ");pi(st.n_csv);ps(" PCS=");pi(st.t_f3);ps("\nCsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void csl_state(void){ps("[CSL] Csp=");pi(st.n_csp);ps(" Cse=");pi(st.n_cse);ps(" Csv=");pi(st.n_csv);ps(" Csc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Celosia Admin Demo ===\n\n");csl_init();
ps("Celosia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;csl_planning(t,c,812+(i*17),801+(i*14),781+(i*10),763+(i*6),2020+(i%5));}
ps("\nCelosia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;csl_execution(t,c,801+(i*15),790+(i*12),772+(i*8),759+(i*5),2021+(i%4));}
ps("\nCelosia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;csl_evaluation(t,c,793+(i*13),782+(i*10),768+(i*7),757+(i*4),2022+(i%3));}
ps("\nCelosia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csl_accessory(t,c,785+(i*11),776+(i*9),762+(i*6),752+(i*3),2023+(i%2));}
ps("\nCelosia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csl_market(t,c,779+(i*9),770+(i*7),757+(i*5),749+(i*3),2024);}
ps("\n");csl_report();csl_state();ps("\n=== Demo Complete ===\n");return 0;}
