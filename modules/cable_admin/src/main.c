/* cable_admin: Cable technology administration (v1.0)
 * Power cable, control cable, special cable, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cbl_t;
typedef struct{int n_pw,n_ct,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cbl_state_t;
static cbl_t pws[N],cts[N-2],sps[N-4],acs[N-6],mks[N-6]; static cbl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cbl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cbl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CBL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cbl_init(void){if(init)return -1;st.n_pw=0;st.n_ct=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pws[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CBL] Cable initialized\n");return 0;}
int cbl_power(int t,int c,int a,int b,int d,int e,int y){return add(pws,&st.n_pw,&st.t_f1,N,t,c,a,b,d,e,y);}
int cbl_control(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cbl_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cbl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cbl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cbl_report(void){ps("[CBL] Pw: ");pi(st.n_pw);ps(" PCS=");pi(st.t_f1);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cbl_state(void){ps("[CBL] Pw=");pi(st.n_pw);ps(" Ct=");pi(st.n_ct);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cable Admin Demo ===\n\n");cbl_init();
ps("Power cables...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cbl_power(t,c,382+(i*17),367+(i*14),347+(i*10),329+(i*6),2020+(i%5));}
ps("\nControl cables...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cbl_control(t,c,371+(i*15),357+(i*12),339+(i*8),326+(i*5),2021+(i%4));}
ps("\nSpecial cables...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cbl_special(t,c,363+(i*13),349+(i*10),333+(i*7),322+(i*4),2022+(i%3));}
ps("\nCable accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cbl_accessory(t,c,355+(i*11),343+(i*9),329+(i*6),319+(i*3),2023+(i%2));}
ps("\nCable marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cbl_market(t,c,349+(i*9),338+(i*7),325+(i*5),317+(i*3),2024);}
ps("\n");cbl_report();cbl_state();ps("\n=== Demo Complete ===\n");return 0;}
