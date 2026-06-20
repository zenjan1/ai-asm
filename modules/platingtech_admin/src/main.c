/* platingtech_admin: Plating technology administration (v1.0)
 * Zinc plating, nickel plating, chrome plating, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} plt_t;
typedef struct{int n_zn,n_ni,n_cr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} plt_state_t;
static plt_t zns[N],nis[N-2],crs[N-4],acs[N-6],mks[N-6]; static plt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(plt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;plt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int plt_init(void){if(init)return -1;st.n_zn=0;st.n_ni=0;st.n_cr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zns[i].active=0;for(int i=0;i<N-2;i++)nis[i].active=0;for(int i=0;i<N-4;i++)crs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PLT] Platingtech initialized\n");return 0;}
int plt_zinc(int t,int c,int a,int b,int d,int e,int y){return add(zns,&st.n_zn,&st.t_f1,N,t,c,a,b,d,e,y);}
int plt_nickel(int t,int c,int a,int b,int d,int e,int y){return add(nis,&st.n_ni,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int plt_chrome(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int plt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int plt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void plt_report(void){ps("[PLT] Zn: ");pi(st.n_zn);ps(" PCS=");pi(st.t_f1);ps("\nNi: ");pi(st.n_ni);ps(" PCS=");pi(st.t_f2);ps("\nCr: ");pi(st.n_cr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void plt_state(void){ps("[PLT] Zn=");pi(st.n_zn);ps(" Ni=");pi(st.n_ni);ps(" Cr=");pi(st.n_cr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Plating Tech Admin Demo ===\n\n");plt_init();
ps("Zinc plating...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;plt_zinc(t,c,337+(i*17),322+(i*14),302+(i*10),284+(i*6),2020+(i%5));}
ps("\nNickel plating...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;plt_nickel(t,c,326+(i*15),312+(i*12),294+(i*8),281+(i*5),2021+(i%4));}
ps("\nChrome plating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;plt_chrome(t,c,318+(i*13),304+(i*10),288+(i*7),277+(i*4),2022+(i%3));}
ps("\nPlating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plt_accessory(t,c,310+(i*11),298+(i*9),284+(i*6),274+(i*3),2023+(i%2));}
ps("\nPlating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;plt_market(t,c,304+(i*9),293+(i*7),280+(i*5),272+(i*3),2024);}
ps("\n");plt_report();plt_state();ps("\n=== Demo Complete ===\n");return 0;}
