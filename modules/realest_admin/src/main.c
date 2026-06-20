/* realest_admin: Real estate technology administration (v1.0)
 * Real estate development, property management, commercial real estate, services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rel_t;
typedef struct{int n_rd,n_pm,n_cr,n_rs,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rel_state_t;
static rel_t rds[N],pms[N-2],crs[N-4],rss[N-6],mks[N-6]; static rel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rel_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rel_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[REL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rel_init(void){if(init)return -1;st.n_rd=0;st.n_pm=0;st.n_cr=0;st.n_rs=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rds[i].active=0;for(int i=0;i<N-2;i++)pms[i].active=0;for(int i=0;i<N-4;i++)crs[i].active=0;for(int i=0;i<N-6;i++)rss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[REL] Realest initialized\n");return 0;}
int rel_develop(int t,int c,int a,int b,int d,int e,int y){return add(rds,&st.n_rd,&st.t_f1,N,t,c,a,b,d,e,y);}
int rel_property(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rel_commercial(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rel_service(int t,int c,int a,int b,int d,int e,int y){return add(rss,&st.n_rs,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rel_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rel_report(void){ps("[REL] Rd: ");pi(st.n_rd);ps(" PCS=");pi(st.t_f1);ps("\nPm: ");pi(st.n_pm);ps(" PCS=");pi(st.t_f2);ps("\nCr: ");pi(st.n_cr);ps(" PCS=");pi(st.t_f3);ps("\nRs: ");pi(st.n_rs);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rel_state(void){ps("[REL] Rd=");pi(st.n_rd);ps(" Pm=");pi(st.n_pm);ps(" Cr=");pi(st.n_cr);ps(" Rs=");pi(st.n_rs);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Real Estate Admin Demo ===\n\n");rel_init();
ps("Real estate development...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rel_develop(t,c,241+(i*17),226+(i*14),206+(i*10),188+(i*6),2020+(i%5));}
ps("\nProperty management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rel_property(t,c,230+(i*15),216+(i*12),198+(i*8),185+(i*5),2021+(i%4));}
ps("\nCommercial real estate...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rel_commercial(t,c,222+(i*13),208+(i*10),192+(i*7),181+(i*4),2022+(i%3));}
ps("\nReal estate services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rel_service(t,c,214+(i*11),202+(i*9),188+(i*6),178+(i*3),2023+(i%2));}
ps("\nReal estate marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rel_market(t,c,208+(i*9),197+(i*7),184+(i*5),176+(i*3),2024);}
ps("\n");rel_report();rel_state();ps("\n=== Demo Complete ===\n");return 0;}
