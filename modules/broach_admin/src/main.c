/* broach_admin: Broaching machine technology administration (v1.0)
 * Horizontal broach, vertical broach, special broach, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brh_t;
typedef struct{int n_hz,n_vt,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brh_state_t;
static brh_t hzs[N],vts[N-2],sps[N-4],acs[N-6],mks[N-6]; static brh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(brh_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brh_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BRH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brh_init(void){if(init)return -1;st.n_hz=0;st.n_vt=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hzs[i].active=0;for(int i=0;i<N-2;i++)vts[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[BRH] Broach initialized\n");return 0;}
int brh_horizontal(int t,int c,int a,int b,int d,int e,int y){return add(hzs,&st.n_hz,&st.t_f1,N,t,c,a,b,d,e,y);}
int brh_vertical(int t,int c,int a,int b,int d,int e,int y){return add(vts,&st.n_vt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brh_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brh_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brh_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brh_report(void){ps("[BRH] Hz: ");pi(st.n_hz);ps(" PCS=");pi(st.t_f1);ps("\nVt: ");pi(st.n_vt);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brh_state(void){ps("[BRH] Hz=");pi(st.n_hz);ps(" Vt=");pi(st.n_vt);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Broach Admin Demo ===\n\n");brh_init();
ps("Horizontal broaches...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brh_horizontal(t,c,402+(i*17),387+(i*14),367+(i*10),349+(i*6),2020+(i%5));}
ps("\nVertical broaches...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brh_vertical(t,c,391+(i*15),377+(i*12),359+(i*8),346+(i*5),2021+(i%4));}
ps("\nSpecial broaches...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brh_special(t,c,383+(i*13),369+(i*10),353+(i*7),342+(i*4),2022+(i%3));}
ps("\nBroach accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brh_accessory(t,c,375+(i*11),363+(i*9),349+(i*6),339+(i*3),2023+(i%2));}
ps("\nBroach marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brh_market(t,c,369+(i*9),358+(i*7),345+(i*5),337+(i*3),2024);}
ps("\n");brh_report();brh_state();ps("\n=== Demo Complete ===\n");return 0;}
