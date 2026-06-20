/* rollingtech_admin: Rolling technology administration (v1.0)
 * Hot rolling, cold rolling, special rolling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rll_t;
typedef struct{int n_hr,n_cr,n_sr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rll_state_t;
static rll_t hrs[N],crs[N-2],srs[N-4],acs[N-6],mks[N-6]; static rll_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rll_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rll_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RLL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rll_init(void){if(init)return -1;st.n_hr=0;st.n_cr=0;st.n_sr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hrs[i].active=0;for(int i=0;i<N-2;i++)crs[i].active=0;for(int i=0;i<N-4;i++)srs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RLL] Rollingtech initialized\n");return 0;}
int rll_hot(int t,int c,int a,int b,int d,int e,int y){return add(hrs,&st.n_hr,&st.t_f1,N,t,c,a,b,d,e,y);}
int rll_cold(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rll_special(int t,int c,int a,int b,int d,int e,int y){return add(srs,&st.n_sr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rll_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rll_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rll_report(void){ps("[RLL] Hr: ");pi(st.n_hr);ps(" PCS=");pi(st.t_f1);ps("\nCr: ");pi(st.n_cr);ps(" PCS=");pi(st.t_f2);ps("\nSr: ");pi(st.n_sr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rll_state(void){ps("[RLL] Hr=");pi(st.n_hr);ps(" Cr=");pi(st.n_cr);ps(" Sr=");pi(st.n_sr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rolling Tech Admin Demo ===\n\n");rll_init();
ps("Hot rolling...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rll_hot(t,c,277+(i*17),262+(i*14),242+(i*10),224+(i*6),2020+(i%5));}
ps("\nCold rolling...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rll_cold(t,c,266+(i*15),252+(i*12),234+(i*8),221+(i*5),2021+(i%4));}
ps("\nSpecial rolling...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rll_special(t,c,258+(i*13),244+(i*10),228+(i*7),217+(i*4),2022+(i%3));}
ps("\nRolling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rll_accessory(t,c,250+(i*11),238+(i*9),224+(i*6),214+(i*3),2023+(i%2));}
ps("\nRolling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rll_market(t,c,244+(i*9),233+(i*7),220+(i*5),212+(i*3),2024);}
ps("\n");rll_report();rll_state();ps("\n=== Demo Complete ===\n");return 0;}
