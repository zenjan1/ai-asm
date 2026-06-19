/* securitization_admin: Securitization administration (v1.0)
 * Underlying assets, structure design, credit rating, issuance sales, servicing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sec_t;
typedef struct{int n_ua,n_sd,n_cr,n_is,n_sv,t_f1,t_f2,t_f3,t_f4,t_f5;} sec_state_t;
static sec_t uas[N],sds[N-2],crs[N-4],iss[N-6],svs[N-6]; static sec_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sec_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sec_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SEC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sec_init(void){if(init)return -1;st.n_ua=0;st.n_sd=0;st.n_cr=0;st.n_is=0;st.n_sv=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uas[i].active=0;for(int i=0;i<N-2;i++)sds[i].active=0;for(int i=0;i<N-4;i++)crs[i].active=0;for(int i=0;i<N-6;i++)iss[i].active=0;for(int i=0;i<N-6;i++)svs[i].active=0;init=1;ps("[SEC] Securitization initialized\n");return 0;}
int sec_underlying(int t,int c,int a,int b,int d,int e,int y){return add(uas,&st.n_ua,&st.t_f1,N,t,c,a,b,d,e,y);}
int sec_structure(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sec_rating(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sec_issuance(int t,int c,int a,int b,int d,int e,int y){return add(iss,&st.n_is,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sec_servicing(int t,int c,int a,int b,int d,int e,int y){return add(svs,&st.n_sv,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sec_report(void){ps("[SEC] UA: ");pi(st.n_ua);ps(" Pool=");pi(st.t_f1);ps("\nStruct: ");pi(st.n_sd);ps(" SPV=");pi(st.t_f2);ps("\nRate: ");pi(st.n_cr);ps(" AAA=");pi(st.t_f3);ps("\nIss: ");pi(st.n_is);ps(" IPO=");pi(st.t_f4);ps("\nServ: ");pi(st.n_sv);ps(" Col=");pi(st.t_f5);ps("\n");}
void sec_state(void){ps("[SEC] Ua=");pi(st.n_ua);ps(" Sd=");pi(st.n_sd);ps(" Cr=");pi(st.n_cr);ps(" Is=");pi(st.n_is);ps(" Sv=");pi(st.n_sv);ps("\n");}
int main(void){
ps("=== Securitization Admin Demo ===\n\n");sec_init();
ps("Underlying assets...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sec_underlying(t,c,126+(i*17),111+(i*14),91+(i*10),74+(i*6),2020+(i%5));}
ps("\nStructure design...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sec_structure(t,c,115+(i*15),101+(i*12),83+(i*8),70+(i*5),2021+(i%4));}
ps("\nCredit rating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sec_rating(t,c,107+(i*13),93+(i*10),77+(i*7),66+(i*4),2022+(i%3));}
ps("\nIssuance sales...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sec_issuance(t,c,99+(i*11),87+(i*9),73+(i*6),63+(i*3),2023+(i%2));}
ps("\nServicing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sec_servicing(t,c,93+(i*9),82+(i*7),69+(i*5),61+(i*3),2024);}
ps("\n");sec_report();sec_state();ps("\n=== Demo Complete ===\n");return 0;}
