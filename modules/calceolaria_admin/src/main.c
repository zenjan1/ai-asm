/* calceolaria_admin: Calceolaria management technology administration (v1.0)
 * Calceolaria planning, calceolaria execution, calceolaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} calc_t;
typedef struct{int n_calcp,n_calc,n_calc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} calc_state_t;
static calc_t calcs[N],calce[N-2],calc2[N-4],calcac[N-6],calcam[N-6]; static calc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(calc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;calc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CALC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int calc_init(void){if(init)return -1;st.n_calcp=0;st.n_calc=0;st.n_calc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)calcs[i].active=0;for(int i=0;i<N-2;i++)calce[i].active=0;for(int i=0;i<N-4;i++)calc2[i].active=0;for(int i=0;i<N-6;i++)calcac[i].active=0;for(int i=0;i<N-6;i++)calcam[i].active=0;init=1;ps("[CALC] Calceolaria initialized\n");return 0;}
int calc_planning(int t,int c,int a,int b,int d,int e,int y){return add(calcs,&st.n_calcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int calc_execution(int t,int c,int a,int b,int d,int e,int y){return add(calce,&st.n_calc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int calc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(calc2,&st.n_calc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int calc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(calcac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int calc_market(int t,int c,int a,int b,int d,int e,int y){return add(calcam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void calc_report(void){ps("[CALC] Lcp: ");pi(st.n_calcp);ps(" PCS=");pi(st.t_f1);ps("\nLce: ");pi(st.n_calc);ps(" PCS=");pi(st.t_f2);ps("\nLc2: ");pi(st.n_calc2);ps(" PCS=");pi(st.t_f3);ps("\nLac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void calc_state(void){ps("[CALC] Lcp=");pi(st.n_calcp);ps(" Lc=");pi(st.n_calc);ps(" Lc2=");pi(st.n_calc2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calceolaria Admin Demo ===\n\n");calc_init();
ps("Calceolaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;calc_planning(t,c,1453+(i*17),1442+(i*14),1422+(i*10),1404+(i*6),2020+(i%5));}
ps("\nCalceolaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;calc_execution(t,c,1442+(i*15),1431+(i*12),1413+(i*8),1400+(i*5),2021+(i%4));}
ps("\nCalceolaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;calc_evaluation(t,c,1434+(i*13),1423+(i*10),1407+(i*7),1396+(i*4),2022+(i%3));}
ps("\nCalceolaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;calc_accessory(t,c,1426+(i*11),1417+(i*9),1403+(i*6),1393+(i*3),2023+(i%2));}
ps("\nCalceolaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;calc_market(t,c,1420+(i*9),1411+(i*7),1398+(i*5),1390+(i*3),2024);}
ps("\n");calc_report();calc_state();ps("\n=== Demo Complete ===\n");return 0;}
