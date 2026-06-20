/* cementtech_admin: Cement technology administration (v1.0)
 * Limestone mining, clinker production, cement grinding, cement products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cmt_t;
typedef struct{int n_lm,n_cl,n_cg,n_cpr,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cmt_state_t;
static cmt_t lms[N],cls[N-2],cgs[N-4],cprs[N-6],mks[N-6]; static cmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cmt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cmt_init(void){if(init)return -1;st.n_lm=0;st.n_cl=0;st.n_cg=0;st.n_cpr=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lms[i].active=0;for(int i=0;i<N-2;i++)cls[i].active=0;for(int i=0;i<N-4;i++)cgs[i].active=0;for(int i=0;i<N-6;i++)cprs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CMT] Cementtech initialized\n");return 0;}
int cmt_limestone(int t,int c,int a,int b,int d,int e,int y){return add(lms,&st.n_lm,&st.t_f1,N,t,c,a,b,d,e,y);}
int cmt_clinker(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cmt_grind(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cmt_product(int t,int c,int a,int b,int d,int e,int y){return add(cprs,&st.n_cpr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cmt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cmt_report(void){ps("[CMT] Lime: ");pi(st.n_lm);ps(" Ton=");pi(st.t_f1);ps("\nClink: ");pi(st.n_cl);ps(" Ton=");pi(st.t_f2);ps("\nGrind: ");pi(st.n_cg);ps(" Ton=");pi(st.t_f3);ps("\nProd: ");pi(st.n_cpr);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cmt_state(void){ps("[CMT] Lm=");pi(st.n_lm);ps(" Cl=");pi(st.n_cl);ps(" Cg=");pi(st.n_cg);ps(" Cpr=");pi(st.n_cpr);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cement Tech Admin Demo ===\n\n");cmt_init();
ps("Limestone mining...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cmt_limestone(t,c,209+(i*17),194+(i*14),174+(i*10),156+(i*6),2020+(i%5));}
ps("\nClinker production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cmt_clinker(t,c,198+(i*15),184+(i*12),166+(i*8),153+(i*5),2021+(i%4));}
ps("\nCement grinding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cmt_grind(t,c,190+(i*13),176+(i*10),160+(i*7),149+(i*4),2022+(i%3));}
ps("\nCement products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmt_product(t,c,182+(i*11),170+(i*9),156+(i*6),146+(i*3),2023+(i%2));}
ps("\nCement marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmt_market(t,c,176+(i*9),165+(i*7),152+(i*5),144+(i*3),2024);}
ps("\n");cmt_report();cmt_state();ps("\n=== Demo Complete ===\n");return 0;}
