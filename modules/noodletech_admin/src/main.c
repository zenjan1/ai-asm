/* noodletech_admin: Noodle technology administration (v1.0)
 * Wheat noodles, rice noodles, instant noodles, dried noodles, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ndl_t;
typedef struct{int n_wn,n_rn,n_in,n_dn,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ndl_state_t;
static ndl_t wns[N],rns[N-2],ins[N-4],dns[N-6],mks[N-6]; static ndl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ndl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ndl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NDL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ndl_init(void){if(init)return -1;st.n_wn=0;st.n_rn=0;st.n_in=0;st.n_dn=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wns[i].active=0;for(int i=0;i<N-2;i++)rns[i].active=0;for(int i=0;i<N-4;i++)ins[i].active=0;for(int i=0;i<N-6;i++)dns[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[NDL] Noodletech initialized\n");return 0;}
int ndl_wheat(int t,int c,int a,int b,int d,int e,int y){return add(wns,&st.n_wn,&st.t_f1,N,t,c,a,b,d,e,y);}
int ndl_rice(int t,int c,int a,int b,int d,int e,int y){return add(rns,&st.n_rn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ndl_instant(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ndl_dried(int t,int c,int a,int b,int d,int e,int y){return add(dns,&st.n_dn,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ndl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ndl_report(void){ps("[NDL] Wheat: ");pi(st.n_wn);ps(" kg=");pi(st.t_f1);ps("\nRice: ");pi(st.n_rn);ps(" kg=");pi(st.t_f2);ps("\nInst: ");pi(st.n_in);ps(" PCS=");pi(st.t_f3);ps("\nDried: ");pi(st.n_dn);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ndl_state(void){ps("[NDL] Wn=");pi(st.n_wn);ps(" Rn=");pi(st.n_rn);ps(" In=");pi(st.n_in);ps(" Dn=");pi(st.n_dn);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Noodle Tech Admin Demo ===\n\n");ndl_init();
ps("Wheat noodles...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ndl_wheat(t,c,187+(i*17),172+(i*14),152+(i*10),134+(i*6),2020+(i%5));}
ps("\nRice noodles...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ndl_rice(t,c,176+(i*15),162+(i*12),144+(i*8),131+(i*5),2021+(i%4));}
ps("\nInstant noodles...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ndl_instant(t,c,168+(i*13),154+(i*10),138+(i*7),127+(i*4),2022+(i%3));}
ps("\nDried noodles...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ndl_dried(t,c,160+(i*11),148+(i*9),134+(i*6),124+(i*3),2023+(i%2));}
ps("\nNoodle marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ndl_market(t,c,154+(i*9),143+(i*7),130+(i*5),122+(i*3),2024);}
ps("\n");ndl_report();ndl_state();ps("\n=== Demo Complete ===\n");return 0;}
