/* rivet_admin: Riveting technology administration (v1.0)
 * Solid rivets, hollow rivets, special riveting, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rvt_t;
typedef struct{int n_sl,n_hl,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rvt_state_t;
static rvt_t sls[N],hls[N-2],sps[N-4],acs[N-6],mks[N-6]; static rvt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rvt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rvt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RVT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rvt_init(void){if(init)return -1;st.n_sl=0;st.n_hl=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sls[i].active=0;for(int i=0;i<N-2;i++)hls[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RVT] Rivet initialized\n");return 0;}
int rvt_solid(int t,int c,int a,int b,int d,int e,int y){return add(sls,&st.n_sl,&st.t_f1,N,t,c,a,b,d,e,y);}
int rvt_hollow(int t,int c,int a,int b,int d,int e,int y){return add(hls,&st.n_hl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rvt_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rvt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rvt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rvt_report(void){ps("[RVT] Sl: ");pi(st.n_sl);ps(" PCS=");pi(st.t_f1);ps("\nHl: ");pi(st.n_hl);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rvt_state(void){ps("[RVT] Sl=");pi(st.n_sl);ps(" Hl=");pi(st.n_hl);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rivet Admin Demo ===\n\n");rvt_init();
ps("Solid rivets...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rvt_solid(t,c,416+(i*17),405+(i*14),385+(i*10),367+(i*6),2020+(i%5));}
ps("\nHollow rivets...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rvt_hollow(t,c,405+(i*15),394+(i*12),376+(i*8),363+(i*5),2021+(i%4));}
ps("\nSpecial riveting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rvt_special(t,c,397+(i*13),386+(i*10),370+(i*7),359+(i*4),2022+(i%3));}
ps("\nRiveting accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rvt_accessory(t,c,389+(i*11),380+(i*9),366+(i*6),356+(i*3),2023+(i%2));}
ps("\nRiveting marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rvt_market(t,c,383+(i*9),374+(i*7),361+(i*5),353+(i*3),2024);}
ps("\n");rvt_report();rvt_state();ps("\n=== Demo Complete ===\n");return 0;}
