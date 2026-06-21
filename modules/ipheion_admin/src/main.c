/* ipheion_admin: Ipheion management technology administration (v1.0)
 * Ipheion planning, ipheion execution, ipheion evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} iph_t;
typedef struct{int n_ipp,n_ipe,n_ipv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} iph_state_t;
static iph_t ipps[N],ipes[N-2],ipvs[N-4],acs[N-6],mks[N-6]; static iph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(iph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;iph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[IPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int iph_init(void){if(init)return -1;st.n_ipp=0;st.n_ipe=0;st.n_ipv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ipps[i].active=0;for(int i=0;i<N-2;i++)ipes[i].active=0;for(int i=0;i<N-4;i++)ipvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[IPH] Ipheion initialized\n");return 0;}
int iph_planning(int t,int c,int a,int b,int d,int e,int y){return add(ipps,&st.n_ipp,&st.t_f1,N,t,c,a,b,d,e,y);}
int iph_execution(int t,int c,int a,int b,int d,int e,int y){return add(ipes,&st.n_ipe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int iph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ipvs,&st.n_ipv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int iph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int iph_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void iph_report(void){ps("[IPH] Ipp: ");pi(st.n_ipp);ps(" PCS=");pi(st.t_f1);ps("\nIpe: ");pi(st.n_ipe);ps(" PCS=");pi(st.t_f2);ps("\nIpv: ");pi(st.n_ipv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void iph_state(void){ps("[IPH] Ipp=");pi(st.n_ipp);ps(" Ipe=");pi(st.n_ipe);ps(" Ipv=");pi(st.n_ipv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ipheion Admin Demo ===\n\n");iph_init();
ps("Ipheion planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;iph_planning(t,c,611+(i*17),600+(i*14),580+(i*10),562+(i*6),2020+(i%5));}
ps("\nIpheion execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;iph_execution(t,c,600+(i*15),589+(i*12),571+(i*8),558+(i*5),2021+(i%4));}
ps("\nIpheion evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;iph_evaluation(t,c,592+(i*13),581+(i*10),565+(i*7),554+(i*4),2022+(i%3));}
ps("\nIpheion accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;iph_accessory(t,c,584+(i*11),575+(i*9),561+(i*6),551+(i*3),2023+(i%2));}
ps("\nIpheion marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;iph_market(t,c,578+(i*9),569+(i*7),556+(i*5),548+(i*3),2024);}
ps("\n");iph_report();iph_state();ps("\n=== Demo Complete ===\n");return 0;}
