/* acousttech_admin: Acoustic technology administration (v1.0)
 * Acoustic measurement, noise control, ultrasonic tech, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acs_t;
typedef struct{int n_ms,n_nc,n_us,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acs_state_t;
static acs_t mss[N],ncs[N-2],uss[N-4],acs[N-6],mks[N-6]; static acs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(acs_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acs_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acs_init(void){if(init)return -1;st.n_ms=0;st.n_nc=0;st.n_us=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mss[i].active=0;for(int i=0;i<N-2;i++)ncs[i].active=0;for(int i=0;i<N-4;i++)uss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ACS] Acousttech initialized\n");return 0;}
int acs_measurement(int t,int c,int a,int b,int d,int e,int y){return add(mss,&st.n_ms,&st.t_f1,N,t,c,a,b,d,e,y);}
int acs_noise(int t,int c,int a,int b,int d,int e,int y){return add(ncs,&st.n_nc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acs_ultrasonic(int t,int c,int a,int b,int d,int e,int y){return add(uss,&st.n_us,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acs_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acs_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acs_report(void){ps("[ACS] Ms: ");pi(st.n_ms);ps(" PCS=");pi(st.t_f1);ps("\nNc: ");pi(st.n_nc);ps(" PCS=");pi(st.t_f2);ps("\nUs: ");pi(st.n_us);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acs_state(void){ps("[ACS] Ms=");pi(st.n_ms);ps(" Nc=");pi(st.n_nc);ps(" Us=");pi(st.n_us);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acoustic Tech Admin Demo ===\n\n");acs_init();
ps("Acoustic measurement...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acs_measurement(t,c,311+(i*17),296+(i*14),276+(i*10),258+(i*6),2020+(i%5));}
ps("\nNoise control...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acs_noise(t,c,300+(i*15),286+(i*12),268+(i*8),255+(i*5),2021+(i%4));}
ps("\nUltrasonic tech...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acs_ultrasonic(t,c,292+(i*13),278+(i*10),262+(i*7),251+(i*4),2022+(i%3));}
ps("\nAcoustic accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acs_accessory(t,c,284+(i*11),272+(i*9),258+(i*6),248+(i*3),2023+(i%2));}
ps("\nAcoustic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acs_market(t,c,278+(i*9),267+(i*7),254+(i*5),246+(i*3),2024);}
ps("\n");acs_report();acs_state();ps("\n=== Demo Complete ===\n");return 0;}
