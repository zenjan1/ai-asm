/* risktech_admin: Risk and insurance technology administration (v1.0)
 * Life insurance, property insurance, reinsurance, insurance services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rsk_t;
typedef struct{int n_li,n_pi,n_ri,n_is,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rsk_state_t;
static rsk_t lis[N],pis[N-2],ris[N-4],iss[N-6],mks[N-6]; static rsk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rsk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rsk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RSK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rsk_init(void){if(init)return -1;st.n_li=0;st.n_pi=0;st.n_ri=0;st.n_is=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lis[i].active=0;for(int i=0;i<N-2;i++)pis[i].active=0;for(int i=0;i<N-4;i++)ris[i].active=0;for(int i=0;i<N-6;i++)iss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RSK] Risktech initialized\n");return 0;}
int rsk_life(int t,int c,int a,int b,int d,int e,int y){return add(lis,&st.n_li,&st.t_f1,N,t,c,a,b,d,e,y);}
int rsk_property(int t,int c,int a,int b,int d,int e,int y){return add(pis,&st.n_pi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rsk_reinsurance(int t,int c,int a,int b,int d,int e,int y){return add(ris,&st.n_ri,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rsk_service(int t,int c,int a,int b,int d,int e,int y){return add(iss,&st.n_is,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rsk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rsk_report(void){ps("[RSK] Li: ");pi(st.n_li);ps(" PCS=");pi(st.t_f1);ps("\nPi: ");pi(st.n_pi);ps(" PCS=");pi(st.t_f2);ps("\nRi: ");pi(st.n_ri);ps(" PCS=");pi(st.t_f3);ps("\nIs: ");pi(st.n_is);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rsk_state(void){ps("[RSK] Li=");pi(st.n_li);ps(" Pi=");pi(st.n_pi);ps(" Ri=");pi(st.n_ri);ps(" Is=");pi(st.n_is);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Risk Insurance Tech Admin Demo ===\n\n");rsk_init();
ps("Life insurance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rsk_life(t,c,245+(i*17),230+(i*14),210+(i*10),192+(i*6),2020+(i%5));}
ps("\nProperty insurance...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rsk_property(t,c,234+(i*15),220+(i*12),202+(i*8),189+(i*5),2021+(i%4));}
ps("\nReinsurance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rsk_reinsurance(t,c,226+(i*13),212+(i*10),196+(i*7),185+(i*4),2022+(i%3));}
ps("\nInsurance services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rsk_service(t,c,218+(i*11),206+(i*9),192+(i*6),182+(i*3),2023+(i%2));}
ps("\nInsurance marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rsk_market(t,c,212+(i*9),201+(i*7),188+(i*5),180+(i*3),2024);}
ps("\n");rsk_report();rsk_state();ps("\n=== Demo Complete ===\n");return 0;}
