/* clethra_admin: Clethra management technology administration (v1.0)
 * Clethra planning, clethra execution, clethra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cth_t;
typedef struct{int n_ctp,n_cte,n_ctv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cth_state_t;
static cth_t ctps[N],ctss[N-2],ctvss[N-4],ctas[N-6],ctmks[N-6]; static cth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cth_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cth_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cth_init(void){if(init)return -1;st.n_ctp=0;st.n_cte=0;st.n_ctv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ctps[i].active=0;for(int i=0;i<N-2;i++)ctss[i].active=0;for(int i=0;i<N-4;i++)ctvss[i].active=0;for(int i=0;i<N-6;i++)ctas[i].active=0;for(int i=0;i<N-6;i++)ctmks[i].active=0;init=1;ps("[CTH] Clethra initialized\n");return 0;}
int cth_planning(int t,int c,int a,int b,int d,int e,int y){return add(ctps,&st.n_ctp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cth_execution(int t,int c,int a,int b,int d,int e,int y){return add(ctss,&st.n_cte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cth_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ctvss,&st.n_ctv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cth_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ctas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cth_market(int t,int c,int a,int b,int d,int e,int y){return add(ctmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cth_report(void){ps("[CTH] Ctp: ");pi(st.n_ctp);ps(" PCS=");pi(st.t_f1);ps("\nCte: ");pi(st.n_cte);ps(" PCS=");pi(st.t_f2);ps("\nCtv: ");pi(st.n_ctv);ps(" PCS=");pi(st.t_f3);ps("\nCtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cth_state(void){ps("[CTH] Ctp=");pi(st.n_ctp);ps(" Cte=");pi(st.n_cte);ps(" Ctv=");pi(st.n_ctv);ps(" Ctc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clethra Admin Demo ===\n\n");cth_init();
ps("Clethra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cth_planning(t,c,814+(i*17),803+(i*14),783+(i*10),765+(i*6),2020+(i%5));}
ps("\nClethra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cth_execution(t,c,803+(i*15),792+(i*12),774+(i*8),761+(i*5),2021+(i%4));}
ps("\nClethra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cth_evaluation(t,c,795+(i*13),784+(i*10),770+(i*7),759+(i*4),2022+(i%3));}
ps("\nClethra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cth_accessory(t,c,787+(i*11),778+(i*9),764+(i*6),754+(i*3),2023+(i%2));}
ps("\nClethra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cth_market(t,c,781+(i*9),772+(i*7),759+(i*5),751+(i*3),2024);}
ps("\n");cth_report();cth_state();ps("\n=== Demo Complete ===\n");return 0;}
