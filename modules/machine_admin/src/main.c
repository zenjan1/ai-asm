/* machine_admin: Machine tool technology administration (v1.0)
 * Cutting machine, forming machine, special machining machine, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mcn_t;
typedef struct{int n_ct,n_fm,n_sm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mcn_state_t;
static mcn_t cts[N],fms[N-2],sms[N-4],acs[N-6],mks[N-6]; static mcn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mcn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mcn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mcn_init(void){if(init)return -1;st.n_ct=0;st.n_fm=0;st.n_sm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cts[i].active=0;for(int i=0;i<N-2;i++)fms[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MCN] Machine initialized\n");return 0;}
int mcn_cutting(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f1,N,t,c,a,b,d,e,y);}
int mcn_forming(int t,int c,int a,int b,int d,int e,int y){return add(fms,&st.n_fm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mcn_special(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mcn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mcn_report(void){ps("[MCN] Ct: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f1);ps("\nFm: ");pi(st.n_fm);ps(" PCS=");pi(st.t_f2);ps("\nSm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mcn_state(void){ps("[MCN] Ct=");pi(st.n_ct);ps(" Fm=");pi(st.n_fm);ps(" Sm=");pi(st.n_sm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Machine Admin Demo ===\n\n");mcn_init();
ps("Cutting machines...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mcn_cutting(t,c,392+(i*17),377+(i*14),357+(i*10),339+(i*6),2020+(i%5));}
ps("\nForming machines...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mcn_forming(t,c,381+(i*15),367+(i*12),349+(i*8),336+(i*5),2021+(i%4));}
ps("\nSpecial machining machines...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mcn_special(t,c,373+(i*13),359+(i*10),343+(i*7),332+(i*4),2022+(i%3));}
ps("\nMachine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcn_accessory(t,c,365+(i*11),353+(i*9),339+(i*6),329+(i*3),2023+(i%2));}
ps("\nMachine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcn_market(t,c,359+(i*9),348+(i*7),335+(i*5),327+(i*3),2024);}
ps("\n");mcn_report();mcn_state();ps("\n=== Demo Complete ===\n");return 0;}
