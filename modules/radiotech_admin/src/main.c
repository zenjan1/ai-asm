/* radiotech_admin: Radiation technology administration (v1.0)
 * Radiation sources, radiation applications, radiation protection, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rad_t;
typedef struct{int n_sr,n_ap,n_pt,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rad_state_t;
static rad_t srs[N],aps[N-2],pts[N-4],acs[N-6],mks[N-6]; static rad_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rad_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rad_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RAD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rad_init(void){if(init)return -1;st.n_sr=0;st.n_ap=0;st.n_pt=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)srs[i].active=0;for(int i=0;i<N-2;i++)aps[i].active=0;for(int i=0;i<N-4;i++)pts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RAD] Radiotech initialized\n");return 0;}
int rad_source(int t,int c,int a,int b,int d,int e,int y){return add(srs,&st.n_sr,&st.t_f1,N,t,c,a,b,d,e,y);}
int rad_application(int t,int c,int a,int b,int d,int e,int y){return add(aps,&st.n_ap,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rad_protection(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rad_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rad_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rad_report(void){ps("[RAD] Sr: ");pi(st.n_sr);ps(" PCS=");pi(st.t_f1);ps("\nAp: ");pi(st.n_ap);ps(" PCS=");pi(st.t_f2);ps("\nPt: ");pi(st.n_pt);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rad_state(void){ps("[RAD] Sr=");pi(st.n_sr);ps(" Ap=");pi(st.n_ap);ps(" Pt=");pi(st.n_pt);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Radiation Tech Admin Demo ===\n\n");rad_init();
ps("Radiation sources...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rad_source(t,c,315+(i*17),300+(i*14),280+(i*10),262+(i*6),2020+(i%5));}
ps("\nRadiation applications...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rad_application(t,c,304+(i*15),290+(i*12),272+(i*8),259+(i*5),2021+(i%4));}
ps("\nRadiation protection...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rad_protection(t,c,296+(i*13),282+(i*10),266+(i*7),255+(i*4),2022+(i%3));}
ps("\nRadiation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rad_accessory(t,c,288+(i*11),276+(i*9),262+(i*6),252+(i*3),2023+(i%2));}
ps("\nRadiation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rad_market(t,c,282+(i*9),271+(i*7),258+(i*5),250+(i*3),2024);}
ps("\n");rad_report();rad_state();ps("\n=== Demo Complete ===\n");return 0;}
