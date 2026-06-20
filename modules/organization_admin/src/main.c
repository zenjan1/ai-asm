/* organization_admin: Organization management technology administration (v1.0)
 * Organization design, organization operations, organization change, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ogx_t;
typedef struct{int n_od,n_oo,n_oc,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ogx_state_t;
static ogx_t ods[N],oos[N-2],ocs[N-4],acs[N-6],mks[N-6]; static ogx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ogx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ogx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OGX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ogx_init(void){if(init)return -1;st.n_od=0;st.n_oo=0;st.n_oc=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ods[i].active=0;for(int i=0;i<N-2;i++)oos[i].active=0;for(int i=0;i<N-4;i++)ocs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OGX] Organization initialized\n");return 0;}
int ogx_design(int t,int c,int a,int b,int d,int e,int y){return add(ods,&st.n_od,&st.t_f1,N,t,c,a,b,d,e,y);}
int ogx_operations(int t,int c,int a,int b,int d,int e,int y){return add(oos,&st.n_oo,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ogx_change(int t,int c,int a,int b,int d,int e,int y){return add(ocs,&st.n_oc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ogx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ogx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ogx_report(void){ps("[OGX] Od: ");pi(st.n_od);ps(" PCS=");pi(st.t_f1);ps("\nOo: ");pi(st.n_oo);ps(" PCS=");pi(st.t_f2);ps("\nOc: ");pi(st.n_oc);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ogx_state(void){ps("[OGX] Od=");pi(st.n_od);ps(" Oo=");pi(st.n_oo);ps(" Oc=");pi(st.n_oc);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Organization Admin Demo ===\n\n");ogx_init();
ps("Organization design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ogx_design(t,c,452+(i*17),441+(i*14),421+(i*10),403+(i*6),2020+(i%5));}
ps("\nOrganization operations...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ogx_operations(t,c,441+(i*15),430+(i*12),412+(i*8),399+(i*5),2021+(i%4));}
ps("\nOrganization change...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ogx_change(t,c,433+(i*13),422+(i*10),406+(i*7),395+(i*4),2022+(i%3));}
ps("\nOrganization accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ogx_accessory(t,c,425+(i*11),416+(i*9),402+(i*6),392+(i*3),2023+(i%2));}
ps("\nOrganization marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ogx_market(t,c,419+(i*9),410+(i*7),397+(i*5),389+(i*3),2024);}
ps("\n");ogx_report();ogx_state();ps("\n=== Demo Complete ===\n");return 0;}
