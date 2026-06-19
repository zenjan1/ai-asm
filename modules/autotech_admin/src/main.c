/* autotech_admin: Auto technology administration (v1.0)
 * Autonomous driving, V2X connectivity, new energy vehicles, car sharing, smart cabin
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} atc_t;
typedef struct{int n_ad,n_v2,n_ev,n_sh,n_cb,t_f1,t_f2,t_f3,t_f4,t_f5;} atc_state_t;
static atc_t ads[N],v2s[N-2],evs[N-4],shs[N-6],cbs[N-6]; static atc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(atc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;atc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ATC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int atc_init(void){if(init)return -1;st.n_ad=0;st.n_v2=0;st.n_ev=0;st.n_sh=0;st.n_cb=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ads[i].active=0;for(int i=0;i<N-2;i++)v2s[i].active=0;for(int i=0;i<N-4;i++)evs[i].active=0;for(int i=0;i<N-6;i++)shs[i].active=0;for(int i=0;i<N-6;i++)cbs[i].active=0;init=1;ps("[ATC] Autotech initialized\n");return 0;}
int atc_autodrive(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f1,N,t,c,a,b,d,e,y);}
int atc_v2x(int t,int c,int a,int b,int d,int e,int y){return add(v2s,&st.n_v2,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int atc_ev(int t,int c,int a,int b,int d,int e,int y){return add(evs,&st.n_ev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int atc_sharing(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int atc_cabin(int t,int c,int a,int b,int d,int e,int y){return add(cbs,&st.n_cb,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void atc_report(void){ps("[ATC] Auto: ");pi(st.n_ad);ps(" L5=");pi(st.t_f1);ps("\nV2X: ");pi(st.n_v2);ps(" C2X=");pi(st.t_f2);ps("\nEV: ");pi(st.n_ev);ps(" BMS=");pi(st.t_f3);ps("\nShare: ");pi(st.n_sh);ps(" MaaS=");pi(st.t_f4);ps("\nCabin: ");pi(st.n_cb);ps(" HMI=");pi(st.t_f5);ps("\n");}
void atc_state(void){ps("[ATC] Ad=");pi(st.n_ad);ps(" V2=");pi(st.n_v2);ps(" Ev=");pi(st.n_ev);ps(" Sh=");pi(st.n_sh);ps(" Cb=");pi(st.n_cb);ps("\n");}
int main(void){
ps("=== Auto Tech Admin Demo ===\n\n");atc_init();
ps("Autonomous driving...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;atc_autodrive(t,c,151+(i*17),136+(i*14),116+(i*10),99+(i*6),2020+(i%5));}
ps("\nV2X connectivity...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;atc_v2x(t,c,140+(i*15),126+(i*12),108+(i*8),95+(i*5),2021+(i%4));}
ps("\nNew energy vehicles...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;atc_ev(t,c,132+(i*13),118+(i*10),102+(i*7),91+(i*4),2022+(i%3));}
ps("\nCar sharing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atc_sharing(t,c,124+(i*11),112+(i*9),98+(i*6),88+(i*3),2023+(i%2));}
ps("\nSmart cabin...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;atc_cabin(t,c,118+(i*9),107+(i*7),94+(i*5),86+(i*3),2024);}
ps("\n");atc_report();atc_state();ps("\n=== Demo Complete ===\n");return 0;}
