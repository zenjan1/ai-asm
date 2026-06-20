/* calibrtech_admin: Calibration technology administration (v1.0)
 * Geometry, mechanical, electromagnetic calibration, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} clb_t;
typedef struct{int n_gm,n_mc,n_em,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} clb_state_t;
static clb_t gms[N],mcs[N-2],ems[N-4],acs[N-6],mks[N-6]; static clb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(clb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;clb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int clb_init(void){if(init)return -1;st.n_gm=0;st.n_mc=0;st.n_em=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)gms[i].active=0;for(int i=0;i<N-2;i++)mcs[i].active=0;for(int i=0;i<N-4;i++)ems[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CLB] Calibrtech initialized\n");return 0;}
int clb_geometry(int t,int c,int a,int b,int d,int e,int y){return add(gms,&st.n_gm,&st.t_f1,N,t,c,a,b,d,e,y);}
int clb_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int clb_electromagnetic(int t,int c,int a,int b,int d,int e,int y){return add(ems,&st.n_em,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int clb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int clb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void clb_report(void){ps("[CLB] Gm: ");pi(st.n_gm);ps(" PCS=");pi(st.t_f1);ps("\nMc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f2);ps("\nEm: ");pi(st.n_em);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void clb_state(void){ps("[CLB] Gm=");pi(st.n_gm);ps(" Mc=");pi(st.n_mc);ps(" Em=");pi(st.n_em);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Calibration Tech Admin Demo ===\n\n");clb_init();
ps("Geometry calibration...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;clb_geometry(t,c,291+(i*17),276+(i*14),256+(i*10),238+(i*6),2020+(i%5));}
ps("\nMechanical calibration...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;clb_mechanical(t,c,280+(i*15),266+(i*12),248+(i*8),235+(i*5),2021+(i%4));}
ps("\nElectromagnetic calibration...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;clb_electromagnetic(t,c,272+(i*13),258+(i*10),242+(i*7),231+(i*4),2022+(i%3));}
ps("\nCalibration accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clb_accessory(t,c,264+(i*11),252+(i*9),238+(i*6),228+(i*3),2023+(i%2));}
ps("\nCalibration marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;clb_market(t,c,258+(i*9),247+(i*7),234+(i*5),226+(i*3),2024);}
ps("\n");clb_report();clb_state();ps("\n=== Demo Complete ===\n");return 0;}
