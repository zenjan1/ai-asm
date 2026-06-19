/* compliance_admin: Compliance administration (v1.0)
 * Regulatory compliance, anti-money laundering, KYC management, sanctions compliance, regulatory reporting
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cmp_t;
typedef struct{int n_rc,n_aml,n_kyc,n_sc,n_rr,t_f1,t_f2,t_f3,t_f4,t_f5;} cmp_state_t;
static cmp_t rcs[N],amls[N-2],kycs[N-4],scs[N-6],rrs[N-6]; static cmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cmp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cmp_init(void){if(init)return -1;st.n_rc=0;st.n_aml=0;st.n_kyc=0;st.n_sc=0;st.n_rr=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rcs[i].active=0;for(int i=0;i<N-2;i++)amls[i].active=0;for(int i=0;i<N-4;i++)kycs[i].active=0;for(int i=0;i<N-6;i++)scs[i].active=0;for(int i=0;i<N-6;i++)rrs[i].active=0;init=1;ps("[CMP] Compliance initialized\n");return 0;}
int cmp_regulatory(int t,int c,int a,int b,int d,int e,int y){return add(rcs,&st.n_rc,&st.t_f1,N,t,c,a,b,d,e,y);}
int cmp_aml(int t,int c,int a,int b,int d,int e,int y){return add(amls,&st.n_aml,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cmp_kyc(int t,int c,int a,int b,int d,int e,int y){return add(kycs,&st.n_kyc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cmp_sanctions(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cmp_reporting(int t,int c,int a,int b,int d,int e,int y){return add(rrs,&st.n_rr,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cmp_report(void){ps("[CMP] Reg: ");pi(st.n_rc);ps(" BS=");pi(st.t_f1);ps("\nAML: ");pi(st.n_aml);ps(" CTR=");pi(st.t_f2);ps("\nKYC: ");pi(st.n_kyc);ps(" CDD=");pi(st.t_f3);ps("\nSanct: ");pi(st.n_sc);ps(" OFAC=");pi(st.t_f4);ps("\nRep: ");pi(st.n_rr);ps(" SAR=");pi(st.t_f5);ps("\n");}
void cmp_state(void){ps("[CMP] Rc=");pi(st.n_rc);ps(" Aml=");pi(st.n_aml);ps(" Kyc=");pi(st.n_kyc);ps(" Sc=");pi(st.n_sc);ps(" Rr=");pi(st.n_rr);ps("\n");}
int main(void){
ps("=== Compliance Admin Demo ===\n\n");cmp_init();
ps("Regulatory compliance...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cmp_regulatory(t,c,121+(i*17),106+(i*14),86+(i*10),69+(i*6),2020+(i%5));}
ps("\nAnti-money laundering...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cmp_aml(t,c,110+(i*15),96+(i*12),78+(i*8),65+(i*5),2021+(i%4));}
ps("\nKYC management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cmp_kyc(t,c,102+(i*13),88+(i*10),72+(i*7),61+(i*4),2022+(i%3));}
ps("\nSanctions compliance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_sanctions(t,c,94+(i*11),82+(i*9),68+(i*6),58+(i*3),2023+(i%2));}
ps("\nRegulatory reporting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_reporting(t,c,88+(i*9),77+(i*7),64+(i*5),56+(i*3),2024);}
ps("\n");cmp_report();cmp_state();ps("\n=== Demo Complete ===\n");return 0;}
