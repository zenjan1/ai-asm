/* medtech_admin: Medtech administration (v1.0)
 * Telemedicine, smart diagnosis, medical platform, health management, medical devices
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mdt_t;
typedef struct{int n_tm,n_sd,n_mp,n_hm,n_md,t_f1,t_f2,t_f3,t_f4,t_f5;} mdt_state_t;
static mdt_t tms[N],sds[N-2],mps[N-4],hms[N-6],mds[N-6]; static mdt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mdt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mdt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MDT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mdt_init(void){if(init)return -1;st.n_tm=0;st.n_sd=0;st.n_mp=0;st.n_hm=0;st.n_md=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tms[i].active=0;for(int i=0;i<N-2;i++)sds[i].active=0;for(int i=0;i<N-4;i++)mps[i].active=0;for(int i=0;i<N-6;i++)hms[i].active=0;for(int i=0;i<N-6;i++)mds[i].active=0;init=1;ps("[MDT] Medtech initialized\n");return 0;}
int mdt_tele(int t,int c,int a,int b,int d,int e,int y){return add(tms,&st.n_tm,&st.t_f1,N,t,c,a,b,d,e,y);}
int mdt_smart(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mdt_platform(int t,int c,int a,int b,int d,int e,int y){return add(mps,&st.n_mp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mdt_health(int t,int c,int a,int b,int d,int e,int y){return add(hms,&st.n_hm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mdt_device(int t,int c,int a,int b,int d,int e,int y){return add(mds,&st.n_md,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mdt_report(void){ps("[MDT] Tele: ");pi(st.n_tm);ps(" Rx=");pi(st.t_f1);ps("\nSmart: ");pi(st.n_sd);ps(" AI=");pi(st.t_f2);ps("\nPlat: ");pi(st.n_mp);ps(" EMR=");pi(st.t_f3);ps("\nHealth: ");pi(st.n_hm);ps(" CHR=");pi(st.t_f4);ps("\nDev: ");pi(st.n_md);ps(" FDA=");pi(st.t_f5);ps("\n");}
void mdt_state(void){ps("[MDT] Tm=");pi(st.n_tm);ps(" Sd=");pi(st.n_sd);ps(" Mp=");pi(st.n_mp);ps(" Hm=");pi(st.n_hm);ps(" Md=");pi(st.n_md);ps("\n");}
int main(void){
ps("=== Medtech Admin Demo ===\n\n");mdt_init();
ps("Telemedicine...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mdt_tele(t,c,140+(i*17),125+(i*14),105+(i*10),88+(i*6),2020+(i%5));}
ps("\nSmart diagnosis...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mdt_smart(t,c,129+(i*15),115+(i*12),97+(i*8),84+(i*5),2021+(i%4));}
ps("\nMedical platform...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mdt_platform(t,c,121+(i*13),107+(i*10),91+(i*7),80+(i*4),2022+(i%3));}
ps("\nHealth management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdt_health(t,c,113+(i*11),101+(i*9),87+(i*6),77+(i*3),2023+(i%2));}
ps("\nMedical devices...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mdt_device(t,c,107+(i*9),96+(i*7),83+(i*5),75+(i*3),2024);}
ps("\n");mdt_report();mdt_state();ps("\n=== Demo Complete ===\n");return 0;}
