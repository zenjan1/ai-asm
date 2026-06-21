/* dicentra_admin: Dicentra management technology administration (v1.0)
 * Dicentra planning, dicentra execution, dicentra evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dcn_t;
typedef struct{int n_dcp,n_dce,n_dcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dcn_state_t;
static dcn_t dcps[N],dcss[N-2],dcvss[N-4],dcas[N-6],dcmks[N-6]; static dcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dcn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dcn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dcn_init(void){if(init)return -1;st.n_dcp=0;st.n_dce=0;st.n_dcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dcps[i].active=0;for(int i=0;i<N-2;i++)dcss[i].active=0;for(int i=0;i<N-4;i++)dcvss[i].active=0;for(int i=0;i<N-6;i++)dcas[i].active=0;for(int i=0;i<N-6;i++)dcmks[i].active=0;init=1;ps("[DCN] Dicentra initialized\n");return 0;}
int dcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(dcps,&st.n_dcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(dcss,&st.n_dce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dcvss,&st.n_dcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dcn_market(int t,int c,int a,int b,int d,int e,int y){return add(dcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dcn_report(void){ps("[DCN] Dcp: ");pi(st.n_dcp);ps(" PCS=");pi(st.t_f1);ps("\nDce: ");pi(st.n_dce);ps(" PCS=");pi(st.t_f2);ps("\nDcv: ");pi(st.n_dcv);ps(" PCS=");pi(st.t_f3);ps("\nDcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dcn_state(void){ps("[DCN] Dcp=");pi(st.n_dcp);ps(" Dce=");pi(st.n_dce);ps(" Dcv=");pi(st.n_dcv);ps(" Dcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dicentra Admin Demo ===\n\n");dcn_init();
ps("Dicentra planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dcn_planning(t,c,818+(i*17),807+(i*14),787+(i*10),769+(i*6),2020+(i%5));}
ps("\nDicentra execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dcn_execution(t,c,807+(i*15),796+(i*12),778+(i*8),765+(i*5),2021+(i%4));}
ps("\nDicentra evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dcn_evaluation(t,c,799+(i*13),788+(i*10),774+(i*7),763+(i*4),2022+(i%3));}
ps("\nDicentra accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dcn_accessory(t,c,791+(i*11),782+(i*9),768+(i*6),758+(i*3),2023+(i%2));}
ps("\nDicentra marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dcn_market(t,c,785+(i*9),776+(i*7),763+(i*5),755+(i*3),2024);}
ps("\n");dcn_report();dcn_state();ps("\n=== Demo Complete ===\n");return 0;}
