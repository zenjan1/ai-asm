/* salvia_admin: Salvia management technology administration (v1.0)
 * Salvia planning, salvia execution, salvia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sl_t;
typedef struct{int n_slp,n_sle,n_slv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sl_state_t;
static sl_t slps[N],slss[N-2],slvss[N-4],slas[N-6],slmks[N-6]; static sl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SLV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sl_init(void){if(init)return -1;st.n_slp=0;st.n_sle=0;st.n_slv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)slps[i].active=0;for(int i=0;i<N-2;i++)slss[i].active=0;for(int i=0;i<N-4;i++)slvss[i].active=0;for(int i=0;i<N-6;i++)slas[i].active=0;for(int i=0;i<N-6;i++)slmks[i].active=0;init=1;ps("[SLV] Salvia initialized\n");return 0;}
int sl_planning(int t,int c,int a,int b,int d,int e,int y){return add(slps,&st.n_slp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sl_execution(int t,int c,int a,int b,int d,int e,int y){return add(slss,&st.n_sle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(slvss,&st.n_slv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(slas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sl_market(int t,int c,int a,int b,int d,int e,int y){return add(slmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sl_report(void){ps("[SLV] Slp: ");pi(st.n_slp);ps(" PCS=");pi(st.t_f1);ps("\nSle: ");pi(st.n_sle);ps(" PCS=");pi(st.t_f2);ps("\nSlv: ");pi(st.n_slv);ps(" PCS=");pi(st.t_f3);ps("\nSlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sl_state(void){ps("[SLV] Slp=");pi(st.n_slp);ps(" Sle=");pi(st.n_sle);ps(" Slv=");pi(st.n_slv);ps(" Slc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Salvia Admin Demo ===\n\n");sl_init();
ps("Salvia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sl_planning(t,c,650+(i*17),639+(i*14),619+(i*10),601+(i*6),2020+(i%5));}
ps("\nSalvia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sl_execution(t,c,639+(i*15),628+(i*12),610+(i*8),597+(i*5),2021+(i%4));}
ps("\nSalvia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sl_evaluation(t,c,631+(i*13),620+(i*10),604+(i*7),593+(i*4),2022+(i%3));}
ps("\nSalvia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sl_accessory(t,c,623+(i*11),614+(i*9),600+(i*6),590+(i*3),2023+(i%2));}
ps("\nSalvia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sl_market(t,c,617+(i*9),608+(i*7),595+(i*5),587+(i*3),2024);}
ps("\n");sl_report();sl_state();ps("\n=== Demo Complete ===\n");return 0;}
