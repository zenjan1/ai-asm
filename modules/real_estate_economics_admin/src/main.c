/* real_estate_economics_admin: Real estate economics administration (v1.0)
 * Housing market, land economics, real estate finance, urban renewal, real estate policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} est_t;
typedef struct{int n_hm,n_le,n_rf,n_ur,n_ep,t_f1,t_f2,t_f3,t_f4,t_f5;} est_state_t;
static est_t hms[N],les[N-2],rfs[N-4],urs[N-6],eps[N-6]; static est_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(est_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;est_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int est_init(void){if(init)return -1;st.n_hm=0;st.n_le=0;st.n_rf=0;st.n_ur=0;st.n_ep=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hms[i].active=0;for(int i=0;i<N-2;i++)les[i].active=0;for(int i=0;i<N-4;i++)rfs[i].active=0;for(int i=0;i<N-6;i++)urs[i].active=0;for(int i=0;i<N-6;i++)eps[i].active=0;init=1;ps("[EST] Real estate economics initialized\n");return 0;}
int est_housing(int t,int c,int a,int b,int d,int e,int y){return add(hms,&st.n_hm,&st.t_f1,N,t,c,a,b,d,e,y);}
int est_land(int t,int c,int a,int b,int d,int e,int y){return add(les,&st.n_le,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int est_finance(int t,int c,int a,int b,int d,int e,int y){return add(rfs,&st.n_rf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int est_urban(int t,int c,int a,int b,int d,int e,int y){return add(urs,&st.n_ur,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int est_policy(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void est_report(void){ps("[EST] Housing: ");pi(st.n_hm);ps(" Price=");pi(st.t_f1);ps("\nLand: ");pi(st.n_le);ps(" Value=");pi(st.t_f2);ps("\nFinance: ");pi(st.n_rf);ps(" Mortgage=");pi(st.t_f3);ps("\nUrban: ");pi(st.n_ur);ps(" Renew=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_ep);ps(" Tax=");pi(st.t_f5);ps("\n");}
void est_state(void){ps("[EST] Hm=");pi(st.n_hm);ps(" Le=");pi(st.n_le);ps(" Rf=");pi(st.n_rf);ps(" Ur=");pi(st.n_ur);ps(" Ep=");pi(st.n_ep);ps("\n");}
int main(void){
ps("=== Real Estate Economics Admin Demo ===\n\n");est_init();
ps("Housing market...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;est_housing(t,c,109+(i*17),94+(i*14),74+(i*10),57+(i*6),2020+(i%5));}
ps("\nLand economics...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;est_land(t,c,98+(i*15),84+(i*12),66+(i*8),53+(i*5),2021+(i%4));}
ps("\nReal estate finance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;est_finance(t,c,90+(i*13),76+(i*10),60+(i*7),49+(i*4),2022+(i%3));}
ps("\nUrban renewal...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;est_urban(t,c,82+(i*11),70+(i*9),56+(i*6),46+(i*3),2023+(i%2));}
ps("\nReal estate policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;est_policy(t,c,76+(i*9),65+(i*7),52+(i*5),44+(i*3),2024);}
ps("\n");est_report();est_state();ps("\n=== Demo Complete ===\n");return 0;}
