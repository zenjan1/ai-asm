/* law_economics_admin: Law and economics administration (v1.0)
 * Property rights, contract law, tort law, antitrust law, law enforcement
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} law_t;
typedef struct{int n_pr,n_ct,n_to,n_an,n_le,t_f1,t_f2,t_f3,t_f4,t_f5;} law_state_t;
static law_t prs[N],cts[N-2],tos[N-4],ans[N-6],les[N-6]; static law_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(law_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;law_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LAW] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int law_init(void){if(init)return -1;st.n_pr=0;st.n_ct=0;st.n_to=0;st.n_an=0;st.n_le=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prs[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)tos[i].active=0;for(int i=0;i<N-6;i++)ans[i].active=0;for(int i=0;i<N-6;i++)les[i].active=0;init=1;ps("[LAW] Law economics initialized\n");return 0;}
int law_property(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f1,N,t,c,a,b,d,e,y);}
int law_contract(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int law_tort(int t,int c,int a,int b,int d,int e,int y){return add(tos,&st.n_to,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int law_antitrust(int t,int c,int a,int b,int d,int e,int y){return add(ans,&st.n_an,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int law_enforce(int t,int c,int a,int b,int d,int e,int y){return add(les,&st.n_le,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void law_report(void){ps("[LAW] Property: ");pi(st.n_pr);ps(" Coase=");pi(st.t_f1);ps("\nContract: ");pi(st.n_ct);ps(" Breach=");pi(st.t_f2);ps("\nTort: ");pi(st.n_to);ps(" Liable=");pi(st.t_f3);ps("\nAntitrust: ");pi(st.n_an);ps(" Merger=");pi(st.t_f4);ps("\nEnforce: ");pi(st.n_le);ps(" Court=");pi(st.t_f5);ps("\n");}
void law_state(void){ps("[LAW] Pr=");pi(st.n_pr);ps(" Ct=");pi(st.n_ct);ps(" To=");pi(st.n_to);ps(" An=");pi(st.n_an);ps(" Le=");pi(st.n_le);ps("\n");}
int main(void){
ps("=== Law Economics Admin Demo ===\n\n");law_init();
ps("Property rights...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;law_property(t,c,90+(i*17),75+(i*14),55+(i*10),38+(i*6),2020+(i%5));}
ps("\nContract law...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;law_contract(t,c,79+(i*15),65+(i*12),47+(i*8),34+(i*5),2021+(i%4));}
ps("\nTort law...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;law_tort(t,c,71+(i*13),57+(i*10),41+(i*7),30+(i*4),2022+(i%3));}
ps("\nAntitrust law...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;law_antitrust(t,c,63+(i*11),51+(i*9),37+(i*6),27+(i*3),2023+(i%2));}
ps("\nLaw enforcement...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;law_enforce(t,c,57+(i*9),46+(i*7),33+(i*5),25+(i*3),2024);}
ps("\n");law_report();law_state();ps("\n=== Demo Complete ===\n");return 0;}
