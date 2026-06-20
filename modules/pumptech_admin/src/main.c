/* pumptech_admin: Pump & valve technology administration (v1.0)
 * Water pumps, oil pumps, special pumps, valves, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pmp_t;
typedef struct{int n_wp,n_op,n_sp,n_vl,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pmp_state_t;
static pmp_t wps[N],ops[N-2],sps[N-4],vls[N-6],mks[N-6]; static pmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pmp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pmp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pmp_init(void){if(init)return -1;st.n_wp=0;st.n_op=0;st.n_sp=0;st.n_vl=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wps[i].active=0;for(int i=0;i<N-2;i++)ops[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)vls[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PMP] Pumptech initialized\n");return 0;}
int pmp_water(int t,int c,int a,int b,int d,int e,int y){return add(wps,&st.n_wp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pmp_oil(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pmp_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pmp_valve(int t,int c,int a,int b,int d,int e,int y){return add(vls,&st.n_vl,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pmp_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pmp_report(void){ps("[PMP] Wp: ");pi(st.n_wp);ps(" PCS=");pi(st.t_f1);ps("\nOp: ");pi(st.n_op);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nVl: ");pi(st.n_vl);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pmp_state(void){ps("[PMP] Wp=");pi(st.n_wp);ps(" Op=");pi(st.n_op);ps(" Sp=");pi(st.n_sp);ps(" Vl=");pi(st.n_vl);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pump Valve Tech Admin Demo ===\n\n");pmp_init();
ps("Water pumps...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pmp_water(t,c,254+(i*17),239+(i*14),219+(i*10),201+(i*6),2020+(i%5));}
ps("\nOil pumps...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pmp_oil(t,c,243+(i*15),229+(i*12),211+(i*8),198+(i*5),2021+(i%4));}
ps("\nSpecial pumps...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pmp_special(t,c,235+(i*13),221+(i*10),205+(i*7),194+(i*4),2022+(i%3));}
ps("\nValves...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pmp_valve(t,c,227+(i*11),215+(i*9),201+(i*6),191+(i*3),2023+(i%2));}
ps("\nPump marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pmp_market(t,c,221+(i*9),210+(i*7),197+(i*5),189+(i*3),2024);}
ps("\n");pmp_report();pmp_state();ps("\n=== Demo Complete ===\n");return 0;}
