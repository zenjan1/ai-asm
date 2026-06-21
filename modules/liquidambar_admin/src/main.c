/* liquidambar_admin: Liquidambar management technology administration (v1.0)
 * Liquidambar planning, liquidambar execution, liquidambar evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} liq_t;
typedef struct{int n_liqp,n_liqe,n_liq2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} liq_state_t;
static liq_t liqps[N],liqss[N-2],liqvss[N-4],liqas[N-6],liqmks[N-6]; static liq_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(liq_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;liq_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LIQ] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int liq_init(void){if(init)return -1;st.n_liqp=0;st.n_liqe=0;st.n_liq2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)liqps[i].active=0;for(int i=0;i<N-2;i++)liqss[i].active=0;for(int i=0;i<N-4;i++)liqvss[i].active=0;for(int i=0;i<N-6;i++)liqas[i].active=0;for(int i=0;i<N-6;i++)liqmks[i].active=0;init=1;ps("[LIQ] Liquidambar initialized\n");return 0;}
int liq_planning(int t,int c,int a,int b,int d,int e,int y){return add(liqps,&st.n_liqp,&st.t_f1,N,t,c,a,b,d,e,y);}
int liq_execution(int t,int c,int a,int b,int d,int e,int y){return add(liqss,&st.n_liqe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int liq_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(liqvss,&st.n_liq2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int liq_accessory(int t,int c,int a,int b,int d,int e,int y){return add(liqas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int liq_market(int t,int c,int a,int b,int d,int e,int y){return add(liqmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void liq_report(void){ps("[LIQ] Liqp: ");pi(st.n_liqp);ps(" PCS=");pi(st.t_f1);ps("\nLiqe: ");pi(st.n_liqe);ps(" PCS=");pi(st.t_f2);ps("\nLiqv: ");pi(st.n_liq2);ps(" PCS=");pi(st.t_f3);ps("\nLiqc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void liq_state(void){ps("[LIQ] Liqp=");pi(st.n_liqp);ps(" Liqe=");pi(st.n_liqe);ps(" Liqv=");pi(st.n_liq2);ps(" Liqc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Liquidambar Admin Demo ===\n\n");liq_init();
ps("Liquidambar planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;liq_planning(t,c,1056+(i*17),1045+(i*14),1025+(i*10),1007+(i*6),2020+(i%5));}
ps("\nLiquidambar execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;liq_execution(t,c,1045+(i*15),1034+(i*12),1016+(i*8),1003+(i*5),2021+(i%4));}
ps("\nLiquidambar evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;liq_evaluation(t,c,1037+(i*13),1026+(i*10),1010+(i*7),999+(i*4),2022+(i%3));}
ps("\nLiquidambar accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;liq_accessory(t,c,1029+(i*11),1020+(i*9),1006+(i*6),996+(i*3),2023+(i%2));}
ps("\nLiquidambar marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;liq_market(t,c,1023+(i*9),1014+(i*7),1001+(i*5),993+(i*3),2024);}
ps("\n");liq_report();liq_state();ps("\n=== Demo Complete ===\n");return 0;}
