/* stokesia_admin: Stokesia management technology administration (v1.0)
 * Stokesia planning, stokesia execution, stokesia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sto_t;
typedef struct{int n_stop,n_stoe,n_sto2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sto_state_t;
static sto_t stops[N],stoss[N-2],stovss[N-4],stoas[N-6],stomks[N-6]; static sto_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sto_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sto_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sto_init(void){if(init)return -1;st.n_stop=0;st.n_stoe=0;st.n_sto2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)stops[i].active=0;for(int i=0;i<N-2;i++)stoss[i].active=0;for(int i=0;i<N-4;i++)stovss[i].active=0;for(int i=0;i<N-6;i++)stoas[i].active=0;for(int i=0;i<N-6;i++)stomks[i].active=0;init=1;ps("[STO] Stokesia initialized\n");return 0;}
int sto_planning(int t,int c,int a,int b,int d,int e,int y){return add(stops,&st.n_stop,&st.t_f1,N,t,c,a,b,d,e,y);}
int sto_execution(int t,int c,int a,int b,int d,int e,int y){return add(stoss,&st.n_stoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sto_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(stovss,&st.n_sto2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sto_accessory(int t,int c,int a,int b,int d,int e,int y){return add(stoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sto_market(int t,int c,int a,int b,int d,int e,int y){return add(stomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sto_report(void){ps("[STO] Stop: ");pi(st.n_stop);ps(" PCS=");pi(st.t_f1);ps("\nStoe: ");pi(st.n_stoe);ps(" PCS=");pi(st.t_f2);ps("\nStov: ");pi(st.n_sto2);ps(" PCS=");pi(st.t_f3);ps("\nStoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sto_state(void){ps("[STO] Stop=");pi(st.n_stop);ps(" Stoe=");pi(st.n_stoe);ps(" Stov=");pi(st.n_sto2);ps(" Stoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Stokesia Admin Demo ===\n\n");sto_init();
ps("Stokesia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sto_planning(t,c,950+(i*17),939+(i*14),919+(i*10),901+(i*6),2020+(i%5));}
ps("\nStokesia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sto_execution(t,c,939+(i*15),928+(i*12),910+(i*8),897+(i*5),2021+(i%4));}
ps("\nStokesia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sto_evaluation(t,c,931+(i*13),920+(i*10),904+(i*7),893+(i*4),2022+(i%3));}
ps("\nStokesia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sto_accessory(t,c,923+(i*11),914+(i*9),900+(i*6),890+(i*3),2023+(i%2));}
ps("\nStokesia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sto_market(t,c,917+(i*9),908+(i*7),895+(i*5),887+(i*3),2024);}
ps("\n");sto_report();sto_state();ps("\n=== Demo Complete ===\n");return 0;}
