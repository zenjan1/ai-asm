/* customer_admin: Customer management technology administration (v1.0)
 * Customer development, customer service, customer maintenance, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctm_t;
typedef struct{int n_cdd,n_csv,n_cmm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctm_state_t;
static ctm_t cdds[N],csvs[N-2],cmms[N-4],acs[N-6],mks[N-6]; static ctm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctm_init(void){if(init)return -1;st.n_cdd=0;st.n_csv=0;st.n_cmm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cdds[i].active=0;for(int i=0;i<N-2;i++)csvs[i].active=0;for(int i=0;i<N-4;i++)cmms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTM] Customer initialized\n");return 0;}
int ctm_development(int t,int c,int a,int b,int d,int e,int y){return add(cdds,&st.n_cdd,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctm_service(int t,int c,int a,int b,int d,int e,int y){return add(csvs,&st.n_csv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctm_maintenance(int t,int c,int a,int b,int d,int e,int y){return add(cmms,&st.n_cmm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctm_report(void){ps("[CTM] Cdd: ");pi(st.n_cdd);ps(" PCS=");pi(st.t_f1);ps("\nCsv: ");pi(st.n_csv);ps(" PCS=");pi(st.t_f2);ps("\nCmm: ");pi(st.n_cmm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctm_state(void){ps("[CTM] Cdd=");pi(st.n_cdd);ps(" Csv=");pi(st.n_csv);ps(" Cmm=");pi(st.n_cmm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Customer Admin Demo ===\n\n");ctm_init();
ps("Customer development...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctm_development(t,c,461+(i*17),450+(i*14),430+(i*10),412+(i*6),2020+(i%5));}
ps("\nCustomer service...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctm_service(t,c,450+(i*15),439+(i*12),421+(i*8),408+(i*5),2021+(i%4));}
ps("\nCustomer maintenance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctm_maintenance(t,c,442+(i*13),431+(i*10),415+(i*7),404+(i*4),2022+(i%3));}
ps("\nCustomer accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctm_accessory(t,c,434+(i*11),425+(i*9),411+(i*6),401+(i*3),2023+(i%2));}
ps("\nCustomer marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctm_market(t,c,428+(i*9),419+(i*7),406+(i*5),398+(i*3),2024);}
ps("\n");ctm_report();ctm_state();ps("\n=== Demo Complete ===\n");return 0;}
