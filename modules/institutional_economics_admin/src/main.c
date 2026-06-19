/* institutional_economics_admin: Institutional economics administration (v1.0)
 * Institutional theory, property rights, transaction costs, principal-agent, new institutional
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ins_t;
typedef struct{int n_th,n_pr,n_tc,n_pa,n_ni,t_f1,t_f2,t_f3,t_f4,t_f5;} ins_state_t;
static ins_t ths[N],prs[N-2],tcs[N-4],pas[N-6],nis[N-6]; static ins_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ins_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ins_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[INS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ins_init(void){if(init)return -1;st.n_th=0;st.n_pr=0;st.n_tc=0;st.n_pa=0;st.n_ni=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ths[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)tcs[i].active=0;for(int i=0;i<N-6;i++)pas[i].active=0;for(int i=0;i<N-6;i++)nis[i].active=0;init=1;ps("[INS] Institutional economics initialized\n");return 0;}
int ins_theory(int t,int c,int a,int b,int d,int e,int y){return add(ths,&st.n_th,&st.t_f1,N,t,c,a,b,d,e,y);}
int ins_property(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ins_transaction(int t,int c,int a,int b,int d,int e,int y){return add(tcs,&st.n_tc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ins_agency(int t,int c,int a,int b,int d,int e,int y){return add(pas,&st.n_pa,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ins_newinst(int t,int c,int a,int b,int d,int e,int y){return add(nis,&st.n_ni,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ins_report(void){ps("[INS] Theory: ");pi(st.n_th);ps(" Def=");pi(st.t_f1);ps("\nProperty: ");pi(st.n_pr);ps(" Rights=");pi(st.t_f2);ps("\nTransaction: ");pi(st.n_tc);ps(" Cost=");pi(st.t_f3);ps("\nAgency: ");pi(st.n_pa);ps(" Contract=");pi(st.t_f4);ps("\nNewInst: ");pi(st.n_ni);ps(" Coase=");pi(st.t_f5);ps("\n");}
void ins_state(void){ps("[INS] Th=");pi(st.n_th);ps(" Pr=");pi(st.n_pr);ps(" Tc=");pi(st.n_tc);ps(" Pa=");pi(st.n_pa);ps(" Ni=");pi(st.n_ni);ps("\n");}
int main(void){
ps("=== Institutional Economics Admin Demo ===\n\n");ins_init();
ps("Institutional theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ins_theory(t,c,82+(i*18),68+(i*15),48+(i*11),31+(i*7),2020+(i%5));}
ps("\nProperty rights...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ins_property(t,c,72+(i*16),58+(i*13),40+(i*9),27+(i*6),2021+(i%4));}
ps("\nTransaction costs...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ins_transaction(t,c,64+(i*14),50+(i*11),34+(i*7),23+(i*4),2022+(i%3));}
ps("\nPrincipal-agent...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ins_agency(t,c,56+(i*12),44+(i*9),30+(i*5),21+(i*3),2023+(i%2));}
ps("\nNew institutional...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ins_newinst(t,c,50+(i*10),39+(i*8),26+(i*5),18+(i*3),2024);}
ps("\n");ins_report();ins_state();ps("\n=== Demo Complete ===\n");return 0;}
