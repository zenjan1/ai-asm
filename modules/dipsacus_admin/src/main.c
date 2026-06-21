/* dipsacus_admin: Dipsacus management technology administration (v1.0)
 * Dipsacus planning, dipsacus execution, dipsacus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dps_t;
typedef struct{int n_dpp,n_dpe,n_dpv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dps_state_t;
static dps_t dpps[N],dpss[N-2],dpvss[N-4],dpas[N-6],dpmks[N-6]; static dps_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dps_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dps_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DPS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dps_init(void){if(init)return -1;st.n_dpp=0;st.n_dpe=0;st.n_dpv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dpps[i].active=0;for(int i=0;i<N-2;i++)dpss[i].active=0;for(int i=0;i<N-4;i++)dpvss[i].active=0;for(int i=0;i<N-6;i++)dpas[i].active=0;for(int i=0;i<N-6;i++)dpmks[i].active=0;init=1;ps("[DPS] Dipsacus initialized\n");return 0;}
int dps_planning(int t,int c,int a,int b,int d,int e,int y){return add(dpps,&st.n_dpp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dps_execution(int t,int c,int a,int b,int d,int e,int y){return add(dpss,&st.n_dpe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dps_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dpvss,&st.n_dpv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dps_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dpas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dps_market(int t,int c,int a,int b,int d,int e,int y){return add(dpmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dps_report(void){ps("[DPS] Dpp: ");pi(st.n_dpp);ps(" PCS=");pi(st.t_f1);ps("\nDpe: ");pi(st.n_dpe);ps(" PCS=");pi(st.t_f2);ps("\nDpv: ");pi(st.n_dpv);ps(" PCS=");pi(st.t_f3);ps("\nDpc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dps_state(void){ps("[DPS] Dpp=");pi(st.n_dpp);ps(" Dpe=");pi(st.n_dpe);ps(" Dpv=");pi(st.n_dpv);ps(" Dpc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dipsacus Admin Demo ===\n\n");dps_init();
ps("Dipsacus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dps_planning(t,c,819+(i*17),808+(i*14),788+(i*10),770+(i*6),2020+(i%5));}
ps("\nDipsacus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dps_execution(t,c,808+(i*15),797+(i*12),779+(i*8),766+(i*5),2021+(i%4));}
ps("\nDipsacus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dps_evaluation(t,c,800+(i*13),789+(i*10),775+(i*7),764+(i*4),2022+(i%3));}
ps("\nDipsacus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dps_accessory(t,c,792+(i*11),783+(i*9),769+(i*6),759+(i*3),2023+(i%2));}
ps("\nDipsacus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dps_market(t,c,786+(i*9),777+(i*7),764+(i*5),756+(i*3),2024);}
ps("\n");dps_report();dps_state();ps("\n=== Demo Complete ===\n");return 0;}
