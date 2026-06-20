/* data_admin: Data management technology administration (v1.0)
 * Data collection, data storage, data analysis, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dtx_t;
typedef struct{int n_dc,n_ds,n_da,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dtx_state_t;
static dtx_t dcs[N],dss[N-2],das[N-4],acs[N-6],mks[N-6]; static dtx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dtx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dtx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DTX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dtx_init(void){if(init)return -1;st.n_dc=0;st.n_ds=0;st.n_da=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dcs[i].active=0;for(int i=0;i<N-2;i++)dss[i].active=0;for(int i=0;i<N-4;i++)das[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DTX] Data initialized\n");return 0;}
int dtx_collection(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f1,N,t,c,a,b,d,e,y);}
int dtx_storage(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dtx_analysis(int t,int c,int a,int b,int d,int e,int y){return add(das,&st.n_da,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dtx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dtx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dtx_report(void){ps("[DTX] Dc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f1);ps("\nDs: ");pi(st.n_ds);ps(" PCS=");pi(st.t_f2);ps("\nDa: ");pi(st.n_da);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dtx_state(void){ps("[DTX] Dc=");pi(st.n_dc);ps(" Ds=");pi(st.n_ds);ps(" Da=");pi(st.n_da);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Data Admin Demo ===\n\n");dtx_init();
ps("Data collection...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dtx_collection(t,c,455+(i*17),444+(i*14),424+(i*10),406+(i*6),2020+(i%5));}
ps("\nData storage...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dtx_storage(t,c,444+(i*15),433+(i*12),415+(i*8),402+(i*5),2021+(i%4));}
ps("\nData analysis...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dtx_analysis(t,c,436+(i*13),425+(i*10),409+(i*7),398+(i*4),2022+(i%3));}
ps("\nData accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dtx_accessory(t,c,428+(i*11),419+(i*9),405+(i*6),395+(i*3),2023+(i%2));}
ps("\nData marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dtx_market(t,c,422+(i*9),413+(i*7),400+(i*5),392+(i*3),2024);}
ps("\n");dtx_report();dtx_state();ps("\n=== Demo Complete ===\n");return 0;}
