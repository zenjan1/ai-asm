/* cynara_admin: Cynara management technology administration (v1.0)
 * Cynara planning, cynara execution, cynara evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyn_t;
typedef struct{int n_cynp,n_cyne,n_cyn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyn_state_t;
static cyn_t cynps[N],cynss[N-2],cynvss[N-4],cynas[N-6],cynmks[N-6]; static cyn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyn_init(void){if(init)return -1;st.n_cynp=0;st.n_cyne=0;st.n_cyn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cynps[i].active=0;for(int i=0;i<N-2;i++)cynss[i].active=0;for(int i=0;i<N-4;i++)cynvss[i].active=0;for(int i=0;i<N-6;i++)cynas[i].active=0;for(int i=0;i<N-6;i++)cynmks[i].active=0;init=1;ps("[CYN] Cynara initialized\n");return 0;}
int cyn_planning(int t,int c,int a,int b,int d,int e,int y){return add(cynps,&st.n_cynp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyn_execution(int t,int c,int a,int b,int d,int e,int y){return add(cynss,&st.n_cyne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cynvss,&st.n_cyn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cynas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyn_market(int t,int c,int a,int b,int d,int e,int y){return add(cynmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyn_report(void){ps("[CYN] Cynp: ");pi(st.n_cynp);ps(" PCS=");pi(st.t_f1);ps("\nCyne: ");pi(st.n_cyne);ps(" PCS=");pi(st.t_f2);ps("\nCynv: ");pi(st.n_cyn2);ps(" PCS=");pi(st.t_f3);ps("\nCync: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyn_state(void){ps("[CYN] Cynp=");pi(st.n_cynp);ps(" Cyne=");pi(st.n_cyne);ps(" Cynv=");pi(st.n_cyn2);ps(" Cync=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cynara Admin Demo ===\n\n");cyn_init();
ps("Cynara planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyn_planning(t,c,914+(i*17),903+(i*14),883+(i*10),865+(i*6),2020+(i%5));}
ps("\nCynara execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyn_execution(t,c,903+(i*15),892+(i*12),874+(i*8),861+(i*5),2021+(i%4));}
ps("\nCynara evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyn_evaluation(t,c,895+(i*13),884+(i*10),868+(i*7),857+(i*4),2022+(i%3));}
ps("\nCynara accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyn_accessory(t,c,887+(i*11),878+(i*9),864+(i*6),854+(i*3),2023+(i%2));}
ps("\nCynara marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyn_market(t,c,881+(i*9),872+(i*7),859+(i*5),851+(i*3),2024);}
ps("\n");cyn_report();cyn_state();ps("\n=== Demo Complete ===\n");return 0;}
