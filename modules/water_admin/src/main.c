/* water_admin: Water resources management technology administration (v1.0)
 * Water planning, water execution, water evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wtr_t;
typedef struct{int n_wp,n_we,n_wv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wtr_state_t;
static wtr_t wps[N],wes[N-2],wvs[N-4],acs[N-6],mks[N-6]; static wtr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wtr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wtr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wtr_init(void){if(init)return -1;st.n_wp=0;st.n_we=0;st.n_wv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wps[i].active=0;for(int i=0;i<N-2;i++)wes[i].active=0;for(int i=0;i<N-4;i++)wvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[WTR] Water initialized\n");return 0;}
int wtr_planning(int t,int c,int a,int b,int d,int e,int y){return add(wps,&st.n_wp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wtr_execution(int t,int c,int a,int b,int d,int e,int y){return add(wes,&st.n_we,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wtr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wvs,&st.n_wv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wtr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wtr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wtr_report(void){ps("[WTR] Wp: ");pi(st.n_wp);ps(" PCS=");pi(st.t_f1);ps("\nWe: ");pi(st.n_we);ps(" PCS=");pi(st.t_f2);ps("\nWv: ");pi(st.n_wv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wtr_state(void){ps("[WTR] Wp=");pi(st.n_wp);ps(" We=");pi(st.n_we);ps(" Wv=");pi(st.n_wv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Water Admin Demo ===\n\n");wtr_init();
ps("Water planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wtr_planning(t,c,501+(i*17),490+(i*14),470+(i*10),452+(i*6),2020+(i%5));}
ps("\nWater execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wtr_execution(t,c,490+(i*15),479+(i*12),461+(i*8),448+(i*5),2021+(i%4));}
ps("\nWater evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wtr_evaluation(t,c,482+(i*13),471+(i*10),455+(i*7),444+(i*4),2022+(i%3));}
ps("\nWater accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wtr_accessory(t,c,474+(i*11),465+(i*9),451+(i*6),441+(i*3),2023+(i%2));}
ps("\nWater marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wtr_market(t,c,468+(i*9),459+(i*7),446+(i*5),438+(i*3),2024);}
ps("\n");wtr_report();wtr_state();ps("\n=== Demo Complete ===\n");return 0;}
