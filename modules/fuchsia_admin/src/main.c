/* fuchsia_admin: Fuchsia management technology administration (v1.0)
 * Fuchsia planning, fuchsia execution, fuchsia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fuc_t;
typedef struct{int n_fup,n_fue,n_fuv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fuc_state_t;
static fuc_t fups[N],fues[N-2],fuvs[N-4],acs[N-6],mks[N-6]; static fuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fuc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fuc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fuc_init(void){if(init)return -1;st.n_fup=0;st.n_fue=0;st.n_fuv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fups[i].active=0;for(int i=0;i<N-2;i++)fues[i].active=0;for(int i=0;i<N-4;i++)fuvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FUC] Fuchsia initialized\n");return 0;}
int fuc_planning(int t,int c,int a,int b,int d,int e,int y){return add(fups,&st.n_fup,&st.t_f1,N,t,c,a,b,d,e,y);}
int fuc_execution(int t,int c,int a,int b,int d,int e,int y){return add(fues,&st.n_fue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fuc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fuvs,&st.n_fuv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fuc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fuc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fuc_report(void){ps("[FUC] Fup: ");pi(st.n_fup);ps(" PCS=");pi(st.t_f1);ps("\nFue: ");pi(st.n_fue);ps(" PCS=");pi(st.t_f2);ps("\nFuv: ");pi(st.n_fuv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fuc_state(void){ps("[FUC] Fup=");pi(st.n_fup);ps(" Fue=");pi(st.n_fue);ps(" Fuv=");pi(st.n_fuv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fuchsia Admin Demo ===\n\n");fuc_init();
ps("Fuchsia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fuc_planning(t,c,593+(i*17),582+(i*14),562+(i*10),544+(i*6),2020+(i%5));}
ps("\nFuchsia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fuc_execution(t,c,582+(i*15),571+(i*12),553+(i*8),540+(i*5),2021+(i%4));}
ps("\nFuchsia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fuc_evaluation(t,c,574+(i*13),563+(i*10),547+(i*7),536+(i*4),2022+(i%3));}
ps("\nFuchsia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fuc_accessory(t,c,566+(i*11),557+(i*9),543+(i*6),533+(i*3),2023+(i%2));}
ps("\nFuchsia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fuc_market(t,c,560+(i*9),551+(i*7),538+(i*5),530+(i*3),2024);}
ps("\n");fuc_report();fuc_state();ps("\n=== Demo Complete ===\n");return 0;}
