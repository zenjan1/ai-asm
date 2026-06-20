/* cnc_admin: CNC technology administration (v1.0)
 * CNC lathes, CNC milling machines, special CNC, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cnx_t;
typedef struct{int n_lt,n_ml,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cnx_state_t;
static cnx_t lts[N],mls[N-2],sps[N-4],acs[N-6],mks[N-6]; static cnx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cnx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cnx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CNX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cnx_init(void){if(init)return -1;st.n_lt=0;st.n_ml=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lts[i].active=0;for(int i=0;i<N-2;i++)mls[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CNX] CNC initialized\n");return 0;}
int cnx_lathe(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f1,N,t,c,a,b,d,e,y);}
int cnx_mill(int t,int c,int a,int b,int d,int e,int y){return add(mls,&st.n_ml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cnx_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cnx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cnx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cnx_report(void){ps("[CNX] Lt: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f1);ps("\nMl: ");pi(st.n_ml);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cnx_state(void){ps("[CNX] Lt=");pi(st.n_lt);ps(" Ml=");pi(st.n_ml);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== CNC Admin Demo ===\n\n");cnx_init();
ps("CNC lathes...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cnx_lathe(t,c,424+(i*17),413+(i*14),393+(i*10),375+(i*6),2020+(i%5));}
ps("\nCNC milling machines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cnx_mill(t,c,413+(i*15),402+(i*12),384+(i*8),371+(i*5),2021+(i%4));}
ps("\nSpecial CNC...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cnx_special(t,c,405+(i*13),394+(i*10),378+(i*7),367+(i*4),2022+(i%3));}
ps("\nCNC accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnx_accessory(t,c,397+(i*11),388+(i*9),374+(i*6),364+(i*3),2023+(i%2));}
ps("\nCNC marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cnx_market(t,c,391+(i*9),382+(i*7),369+(i*5),361+(i*3),2024);}
ps("\n");cnx_report();cnx_state();ps("\n=== Demo Complete ===\n");return 0;}
