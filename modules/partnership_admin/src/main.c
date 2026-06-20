/* partnership_admin: Partnership management technology administration (v1.0)
 * Partnership planning, partnership execution, partnership evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ptn_t;
typedef struct{int n_pp,n_pe,n_pv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ptn_state_t;
static ptn_t pps[N],pes[N-2],pvs[N-4],acs[N-6],mks[N-6]; static ptn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ptn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ptn_init(void){if(init)return -1;st.n_pp=0;st.n_pe=0;st.n_pv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pps[i].active=0;for(int i=0;i<N-2;i++)pes[i].active=0;for(int i=0;i<N-4;i++)pvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PTN] Partnership initialized\n");return 0;}
int ptn_planning(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ptn_execution(int t,int c,int a,int b,int d,int e,int y){return add(pes,&st.n_pe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ptn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ptn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ptn_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ptn_report(void){ps("[PTN] Pp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f1);ps("\nPe: ");pi(st.n_pe);ps(" PCS=");pi(st.t_f2);ps("\nPv: ");pi(st.n_pv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ptn_state(void){ps("[PTN] Pp=");pi(st.n_pp);ps(" Pe=");pi(st.n_pe);ps(" Pv=");pi(st.n_pv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Partnership Admin Demo ===\n\n");ptn_init();
ps("Partnership planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ptn_planning(t,c,469+(i*17),458+(i*14),438+(i*10),420+(i*6),2020+(i%5));}
ps("\nPartnership execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ptn_execution(t,c,458+(i*15),447+(i*12),429+(i*8),416+(i*5),2021+(i%4));}
ps("\nPartnership evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ptn_evaluation(t,c,450+(i*13),439+(i*10),423+(i*7),412+(i*4),2022+(i%3));}
ps("\nPartnership accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptn_accessory(t,c,442+(i*11),433+(i*9),419+(i*6),409+(i*3),2023+(i%2));}
ps("\nPartnership marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptn_market(t,c,436+(i*9),427+(i*7),414+(i*5),406+(i*3),2024);}
ps("\n");ptn_report();ptn_state();ps("\n=== Demo Complete ===\n");return 0;}
