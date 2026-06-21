/* mace_admin: Mace management technology administration (v1.0)
 * Mace planning, mace execution, mace evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mc_t;
typedef struct{int n_mcp,n_mce,n_mcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mc_state_t;
static mc_t mcps[N],mcss[N-2],mcvss[N-4],mcas[N-6],mcmks[N-6]; static mc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mc_init(void){if(init)return -1;st.n_mcp=0;st.n_mce=0;st.n_mcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcps[i].active=0;for(int i=0;i<N-2;i++)mcss[i].active=0;for(int i=0;i<N-4;i++)mcvss[i].active=0;for(int i=0;i<N-6;i++)mcas[i].active=0;for(int i=0;i<N-6;i++)mcmks[i].active=0;init=1;ps("[MCE] Mace initialized\n");return 0;}
int mc_planning(int t,int c,int a,int b,int d,int e,int y){return add(mcps,&st.n_mcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mc_execution(int t,int c,int a,int b,int d,int e,int y){return add(mcss,&st.n_mce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mcvss,&st.n_mcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mc_market(int t,int c,int a,int b,int d,int e,int y){return add(mcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mc_report(void){ps("[MCE] Mcp: ");pi(st.n_mcp);ps(" PCS=");pi(st.t_f1);ps("\nMce: ");pi(st.n_mce);ps(" PCS=");pi(st.t_f2);ps("\nMcv: ");pi(st.n_mcv);ps(" PCS=");pi(st.t_f3);ps("\nMcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mc_state(void){ps("[MCE] Mcp=");pi(st.n_mcp);ps(" Mce=");pi(st.n_mce);ps(" Mcv=");pi(st.n_mcv);ps(" Mcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mace Admin Demo ===\n\n");mc_init();
ps("Mace planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mc_planning(t,c,678+(i*17),667+(i*14),647+(i*10),629+(i*6),2020+(i%5));}
ps("\nMace execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mc_execution(t,c,667+(i*15),656+(i*12),638+(i*8),625+(i*5),2021+(i%4));}
ps("\nMace evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mc_evaluation(t,c,659+(i*13),648+(i*10),632+(i*7),621+(i*4),2022+(i%3));}
ps("\nMace accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mc_accessory(t,c,651+(i*11),642+(i*9),628+(i*6),618+(i*3),2023+(i%2));}
ps("\nMace marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mc_market(t,c,645+(i*9),636+(i*7),623+(i*5),615+(i*3),2024);}
ps("\n");mc_report();mc_state();ps("\n=== Demo Complete ===\n");return 0;}
