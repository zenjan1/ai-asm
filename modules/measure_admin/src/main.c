/* measure_admin: Measurement technology administration (v1.0)
 * Dimensional measurement, geometric measurement, online measurement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} msu_t;
typedef struct{int n_dm,n_gm,n_ol,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} msu_state_t;
static msu_t dms[N],gms[N-2],ols[N-4],acs[N-6],mks[N-6]; static msu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(msu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;msu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MSU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int msu_init(void){if(init)return -1;st.n_dm=0;st.n_gm=0;st.n_ol=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dms[i].active=0;for(int i=0;i<N-2;i++)gms[i].active=0;for(int i=0;i<N-4;i++)ols[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MSU] Measure initialized\n");return 0;}
int msu_dimensional(int t,int c,int a,int b,int d,int e,int y){return add(dms,&st.n_dm,&st.t_f1,N,t,c,a,b,d,e,y);}
int msu_geometric(int t,int c,int a,int b,int d,int e,int y){return add(gms,&st.n_gm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int msu_online(int t,int c,int a,int b,int d,int e,int y){return add(ols,&st.n_ol,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int msu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int msu_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void msu_report(void){ps("[MSU] Dm: ");pi(st.n_dm);ps(" PCS=");pi(st.t_f1);ps("\nGm: ");pi(st.n_gm);ps(" PCS=");pi(st.t_f2);ps("\nOl: ");pi(st.n_ol);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void msu_state(void){ps("[MSU] Dm=");pi(st.n_dm);ps(" Gm=");pi(st.n_gm);ps(" Ol=");pi(st.n_ol);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Measure Admin Demo ===\n\n");msu_init();
ps("Dimensional measurement...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;msu_dimensional(t,c,422+(i*17),411+(i*14),391+(i*10),373+(i*6),2020+(i%5));}
ps("\nGeometric measurement...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;msu_geometric(t,c,411+(i*15),400+(i*12),382+(i*8),369+(i*5),2021+(i%4));}
ps("\nOnline measurement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;msu_online(t,c,403+(i*13),392+(i*10),376+(i*7),365+(i*4),2022+(i%3));}
ps("\nMeasurement accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msu_accessory(t,c,395+(i*11),386+(i*9),372+(i*6),362+(i*3),2023+(i%2));}
ps("\nMeasurement marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;msu_market(t,c,389+(i*9),380+(i*7),367+(i*5),359+(i*3),2024);}
ps("\n");msu_report();msu_state();ps("\n=== Demo Complete ===\n");return 0;}
