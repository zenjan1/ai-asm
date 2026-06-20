/* drillingtech_admin: Drilling technology administration (v1.0)
 * Drilling, expanding, reaming, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} drl_t;
typedef struct{int n_dr,n_ex,n_re,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} drl_state_t;
static drl_t drs[N],exs[N-2],res[N-4],acs[N-6],mks[N-6]; static drl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(drl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;drl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DRL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int drl_init(void){if(init)return -1;st.n_dr=0;st.n_ex=0;st.n_re=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)drs[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)res[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[DRL] Drillingtech initialized\n");return 0;}
int drl_drilling(int t,int c,int a,int b,int d,int e,int y){return add(drs,&st.n_dr,&st.t_f1,N,t,c,a,b,d,e,y);}
int drl_expanding(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int drl_reaming(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int drl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int drl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void drl_report(void){ps("[DRL] Dr: ");pi(st.n_dr);ps(" PCS=");pi(st.t_f1);ps("\nEx: ");pi(st.n_ex);ps(" PCS=");pi(st.t_f2);ps("\nRe: ");pi(st.n_re);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void drl_state(void){ps("[DRL] Dr=");pi(st.n_dr);ps(" Ex=");pi(st.n_ex);ps(" Re=");pi(st.n_re);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Drilling Tech Admin Demo ===\n\n");drl_init();
ps("Drilling...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;drl_drilling(t,c,316+(i*17),301+(i*14),281+(i*10),263+(i*6),2020+(i%5));}
ps("\nExpanding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;drl_expanding(t,c,305+(i*15),291+(i*12),273+(i*8),260+(i*5),2021+(i%4));}
ps("\nReaming...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;drl_reaming(t,c,297+(i*13),283+(i*10),267+(i*7),256+(i*4),2022+(i%3));}
ps("\nDrilling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drl_accessory(t,c,289+(i*11),277+(i*9),263+(i*6),253+(i*3),2023+(i%2));}
ps("\nDrilling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;drl_market(t,c,283+(i*9),272+(i*7),259+(i*5),251+(i*3),2024);}
ps("\n");drl_report();drl_state();ps("\n=== Demo Complete ===\n");return 0;}
