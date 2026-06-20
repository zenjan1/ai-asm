/* flighttech_admin: Flight technology administration (v1.0)
 * Civil aviation, general aviation, aircraft maintenance, aircraft manufacturing, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} flt_t;
typedef struct{int n_ca,n_ga,n_am,n_fm,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} flt_state_t;
static flt_t cas[N],gas[N-2],ams[N-4],fms[N-6],mks[N-6]; static flt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(flt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;flt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int flt_init(void){if(init)return -1;st.n_ca=0;st.n_ga=0;st.n_am=0;st.n_fm=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cas[i].active=0;for(int i=0;i<N-2;i++)gas[i].active=0;for(int i=0;i<N-4;i++)ams[i].active=0;for(int i=0;i<N-6;i++)fms[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FLT] Flighttech initialized\n");return 0;}
int flt_civil(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f1,N,t,c,a,b,d,e,y);}
int flt_general(int t,int c,int a,int b,int d,int e,int y){return add(gas,&st.n_ga,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int flt_maintenance(int t,int c,int a,int b,int d,int e,int y){return add(ams,&st.n_am,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int flt_manufacture(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int flt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void flt_report(void){ps("[FLT] Ca: ");pi(st.n_ca);ps(" PCS=");pi(st.t_f1);ps("\nGa: ");pi(st.n_ga);ps(" PCS=");pi(st.t_f2);ps("\nAm: ");pi(st.n_am);ps(" PCS=");pi(st.t_f3);ps("\nFm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void flt_state(void){ps("[FLT] Ca=");pi(st.n_ca);ps(" Ga=");pi(st.n_ga);ps(" Am=");pi(st.n_am);ps(" Fm=");pi(st.n_fm);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Flight Tech Admin Demo ===\n\n");flt_init();
ps("Civil aviation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;flt_civil(t,c,247+(i*17),232+(i*14),212+(i*10),194+(i*6),2020+(i%5));}
ps("\nGeneral aviation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;flt_general(t,c,236+(i*15),222+(i*12),204+(i*8),191+(i*5),2021+(i%4));}
ps("\nAircraft maintenance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;flt_maintenance(t,c,228+(i*13),214+(i*10),198+(i*7),187+(i*4),2022+(i%3));}
ps("\nAircraft manufacturing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flt_manufacture(t,c,220+(i*11),208+(i*9),194+(i*6),184+(i*3),2023+(i%2));}
ps("\nAviation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;flt_market(t,c,214+(i*9),203+(i*7),190+(i*5),182+(i*3),2024);}
ps("\n");flt_report();flt_state();ps("\n=== Demo Complete ===\n");return 0;}
