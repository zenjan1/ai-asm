/* automatetech_admin: Automation technology administration (v1.0)
 * Automated assembly, automated welding, automated coating, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ate_t;
typedef struct{int n_as,n_wd,n_ct,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ate_state_t;
static ate_t ass[N],wds[N-2],cts[N-4],acs[N-6],mks[N-6]; static ate_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ate_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ate_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ATE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ate_init(void){if(init)return -1;st.n_as=0;st.n_wd=0;st.n_ct=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ass[i].active=0;for(int i=0;i<N-2;i++)wds[i].active=0;for(int i=0;i<N-4;i++)cts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ATE] Automatetech initialized\n");return 0;}
int ate_assembly(int t,int c,int a,int b,int d,int e,int y){return add(ass,&st.n_as,&st.t_f1,N,t,c,a,b,d,e,y);}
int ate_welding(int t,int c,int a,int b,int d,int e,int y){return add(wds,&st.n_wd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ate_coating(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ate_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ate_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ate_report(void){ps("[ATE] As: ");pi(st.n_as);ps(" PCS=");pi(st.t_f1);ps("\nWd: ");pi(st.n_wd);ps(" PCS=");pi(st.t_f2);ps("\nCt: ");pi(st.n_ct);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ate_state(void){ps("[ATE] As=");pi(st.n_as);ps(" Wd=");pi(st.n_wd);ps(" Ct=");pi(st.n_ct);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Automation Tech Admin Demo ===\n\n");ate_init();
ps("Automated assembly...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ate_assembly(t,c,322+(i*17),307+(i*14),287+(i*10),269+(i*6),2020+(i%5));}
ps("\nAutomated welding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ate_welding(t,c,311+(i*15),297+(i*12),279+(i*8),266+(i*5),2021+(i%4));}
ps("\nAutomated coating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ate_coating(t,c,303+(i*13),289+(i*10),273+(i*7),262+(i*4),2022+(i%3));}
ps("\nAutomation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ate_accessory(t,c,295+(i*11),283+(i*9),269+(i*6),259+(i*3),2023+(i%2));}
ps("\nAutomation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ate_market(t,c,289+(i*9),278+(i*7),265+(i*5),257+(i*3),2024);}
ps("\n");ate_report();ate_state();ps("\n=== Demo Complete ===\n");return 0;}
