/* reliability_admin: Reliability management technology administration (v1.0)
 * Reliability design, reliability testing, reliability management, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rlx_t;
typedef struct{int n_rd,n_rt,n_rm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rlx_state_t;
static rlx_t rds[N],rts[N-2],rms[N-4],acs[N-6],mks[N-6]; static rlx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rlx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rlx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RLX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rlx_init(void){if(init)return -1;st.n_rd=0;st.n_rt=0;st.n_rm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rds[i].active=0;for(int i=0;i<N-2;i++)rts[i].active=0;for(int i=0;i<N-4;i++)rms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RLX] Reliability initialized\n");return 0;}
int rlx_design(int t,int c,int a,int b,int d,int e,int y){return add(rds,&st.n_rd,&st.t_f1,N,t,c,a,b,d,e,y);}
int rlx_testing(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rlx_management(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rlx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rlx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rlx_report(void){ps("[RLX] Rd: ");pi(st.n_rd);ps(" PCS=");pi(st.t_f1);ps("\nRt: ");pi(st.n_rt);ps(" PCS=");pi(st.t_f2);ps("\nRm: ");pi(st.n_rm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rlx_state(void){ps("[RLX] Rd=");pi(st.n_rd);ps(" Rt=");pi(st.n_rt);ps(" Rm=");pi(st.n_rm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Reliability Admin Demo ===\n\n");rlx_init();
ps("Reliability design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rlx_design(t,c,443+(i*17),432+(i*14),412+(i*10),394+(i*6),2020+(i%5));}
ps("\nReliability testing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rlx_testing(t,c,432+(i*15),421+(i*12),403+(i*8),390+(i*5),2021+(i%4));}
ps("\nReliability management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rlx_management(t,c,424+(i*13),413+(i*10),397+(i*7),386+(i*4),2022+(i%3));}
ps("\nReliability accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rlx_accessory(t,c,416+(i*11),407+(i*9),393+(i*6),383+(i*3),2023+(i%2));}
ps("\nReliability marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rlx_market(t,c,410+(i*9),401+(i*7),388+(i*5),380+(i*3),2024);}
ps("\n");rlx_report();rlx_state();ps("\n=== Demo Complete ===\n");return 0;}
