/* sectech_admin: Security technology administration (v1.0)
 * Video surveillance, intrusion alarm, access control, intercom system, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sec_t;
typedef struct{int n_vs,n_ia,n_ac,n_is,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sec_state_t;
static sec_t vss[N],ias[N-2],acs[N-4],iss[N-6],mks[N-6]; static sec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sec_init(void){if(init)return -1;st.n_vs=0;st.n_ia=0;st.n_ac=0;st.n_is=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vss[i].active=0;for(int i=0;i<N-2;i++)ias[i].active=0;for(int i=0;i<N-4;i++)acs[i].active=0;for(int i=0;i<N-6;i++)iss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SEC] Sectech initialized\n");return 0;}
int sec_video(int t,int c,int a,int b,int d,int e,int y){return add(vss,&st.n_vs,&st.t_f1,N,t,c,a,b,d,e,y);}
int sec_intrusion(int t,int c,int a,int b,int d,int e,int y){return add(ias,&st.n_ia,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sec_access(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sec_intercom(int t,int c,int a,int b,int d,int e,int y){return add(iss,&st.n_is,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sec_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sec_report(void){ps("[SEC] Vs: ");pi(st.n_vs);ps(" PCS=");pi(st.t_f1);ps("\nIa: ");pi(st.n_ia);ps(" PCS=");pi(st.t_f2);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f3);ps("\nIs: ");pi(st.n_is);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sec_state(void){ps("[SEC] Vs=");pi(st.n_vs);ps(" Ia=");pi(st.n_ia);ps(" Ac=");pi(st.n_ac);ps(" Is=");pi(st.n_is);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Security Tech Admin Demo ===\n\n");sec_init();
ps("Video surveillance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sec_video(t,c,247+(i*17),232+(i*14),212+(i*10),194+(i*6),2020+(i%5));}
ps("\nIntrusion alarm...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sec_intrusion(t,c,236+(i*15),222+(i*12),204+(i*8),191+(i*5),2021+(i%4));}
ps("\nAccess control...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sec_access(t,c,228+(i*13),214+(i*10),198+(i*7),187+(i*4),2022+(i%3));}
ps("\nIntercom system...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sec_intercom(t,c,220+(i*11),208+(i*9),194+(i*6),184+(i*3),2023+(i%2));}
ps("\nSecurity marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sec_market(t,c,214+(i*9),203+(i*7),190+(i*5),182+(i*3),2024);}
ps("\n");sec_report();sec_state();ps("\n=== Demo Complete ===\n");return 0;}
