/* delosperma_admin: Delosperma management technology administration (v1.0)
 * Delosperma planning, delosperma execution, delosperma evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dc_t;
typedef struct{int n_dcp,n_dce,n_dcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dc_state_t;
static dc_t dcps[N],dcss[N-2],dcvss[N-4],dcas[N-6],dcmks[N-6]; static dc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DCR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dcr_init(void){if(init)return -1;st.n_dcp=0;st.n_dce=0;st.n_dcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dcps[i].active=0;for(int i=0;i<N-2;i++)dcss[i].active=0;for(int i=0;i<N-4;i++)dcvss[i].active=0;for(int i=0;i<N-6;i++)dcas[i].active=0;for(int i=0;i<N-6;i++)dcmks[i].active=0;init=1;ps("[DCR] Delosperma initialized\n");return 0;}
int dcr_planning(int t,int c,int a,int b,int d,int e,int y){return add(dcps,&st.n_dcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dcr_execution(int t,int c,int a,int b,int d,int e,int y){return add(dcss,&st.n_dce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dcr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dcvss,&st.n_dcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dcr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dcr_market(int t,int c,int a,int b,int d,int e,int y){return add(dcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dcr_report(void){ps("[DCR] Dcp: ");pi(st.n_dcp);ps(" PCS=");pi(st.t_f1);ps("\nDce: ");pi(st.n_dce);ps(" PCS=");pi(st.t_f2);ps("\nDcv: ");pi(st.n_dcv);ps(" PCS=");pi(st.t_f3);ps("\nDcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dcr_state(void){ps("[DCR] Dcp=");pi(st.n_dcp);ps(" Dce=");pi(st.n_dce);ps(" Dcv=");pi(st.n_dcv);ps(" Dcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Delosperma Admin Demo ===\n\n");dcr_init();
ps("Delosperma planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dcr_planning(t,c,743+(i*17),732+(i*14),712+(i*10),694+(i*6),2020+(i%5));}
ps("\nDelosperma execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dcr_execution(t,c,732+(i*15),721+(i*12),703+(i*8),690+(i*5),2021+(i%4));}
ps("\nDelosperma evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dcr_evaluation(t,c,724+(i*13),713+(i*10),697+(i*7),686+(i*4),2022+(i%3));}
ps("\nDelosperma accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dcr_accessory(t,c,716+(i*11),707+(i*9),693+(i*6),683+(i*3),2023+(i%2));}
ps("\nDelosperma marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dcr_market(t,c,710+(i*9),701+(i*7),688+(i*5),680+(i*3),2024);}
ps("\n");dcr_report();dcr_state();ps("\n=== Demo Complete ===\n");return 0;}
