/* coating_admin: Coating technology administration (v1.0)
 * PVD coating, CVD coating, thermal spray, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctd_t;
typedef struct{int n_pv,n_cv,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctd_state_t;
static ctd_t pvs[N],cvs[N-2],sps[N-4],acs[N-6],mks[N-6]; static ctd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctd_init(void){if(init)return -1;st.n_pv=0;st.n_cv=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pvs[i].active=0;for(int i=0;i<N-2;i++)cvs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTD] Coating initialized\n");return 0;}
int ctd_pvd(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctd_cvd(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctd_spray(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctd_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctd_report(void){ps("[CTD] Pv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f1);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctd_state(void){ps("[CTD] Pv=");pi(st.n_pv);ps(" Cv=");pi(st.n_cv);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coating Admin Demo ===\n\n");ctd_init();
ps("PVD coating...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctd_pvd(t,c,413+(i*17),402+(i*14),382+(i*10),364+(i*6),2020+(i%5));}
ps("\nCVD coating...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctd_cvd(t,c,402+(i*15),391+(i*12),373+(i*8),360+(i*5),2021+(i%4));}
ps("\nThermal spray...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctd_spray(t,c,394+(i*13),383+(i*10),367+(i*7),356+(i*4),2022+(i%3));}
ps("\nCoating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctd_accessory(t,c,386+(i*11),377+(i*9),363+(i*6),353+(i*3),2023+(i%2));}
ps("\nCoating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctd_market(t,c,380+(i*9),371+(i*7),358+(i*5),350+(i*3),2024);}
ps("\n");ctd_report();ctd_state();ps("\n=== Demo Complete ===\n");return 0;}
