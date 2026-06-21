/* thunbergia_admin: Thunbergia management technology administration (v1.0)
 * Thunbergia planning, thunbergia execution, thunbergia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} thu_t;
typedef struct{int n_thp,n_the,n_thv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} thu_state_t;
static thu_t thps[N],thss[N-2],thvss[N-4],thas[N-6],thmks[N-6]; static thu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(thu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;thu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[THB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int thb_init(void){if(init)return -1;st.n_thp=0;st.n_the=0;st.n_thv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)thps[i].active=0;for(int i=0;i<N-2;i++)thss[i].active=0;for(int i=0;i<N-4;i++)thvss[i].active=0;for(int i=0;i<N-6;i++)thas[i].active=0;for(int i=0;i<N-6;i++)thmks[i].active=0;init=1;ps("[THB] Thunbergia initialized\n");return 0;}
int thb_planning(int t,int c,int a,int b,int d,int e,int y){return add(thps,&st.n_thp,&st.t_f1,N,t,c,a,b,d,e,y);}
int thb_execution(int t,int c,int a,int b,int d,int e,int y){return add(thss,&st.n_the,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int thb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(thvss,&st.n_thv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int thb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(thas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int thb_market(int t,int c,int a,int b,int d,int e,int y){return add(thmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void thb_report(void){ps("[THB] Thp: ");pi(st.n_thp);ps(" PCS=");pi(st.t_f1);ps("\nThe: ");pi(st.n_the);ps(" PCS=");pi(st.t_f2);ps("\nThv: ");pi(st.n_thv);ps(" PCS=");pi(st.t_f3);ps("\nThc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void thb_state(void){ps("[THB] Thp=");pi(st.n_thp);ps(" The=");pi(st.n_the);ps(" Thv=");pi(st.n_thv);ps(" Thc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Thunbergia Admin Demo ===\n\n");thb_init();
ps("Thunbergia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;thb_planning(t,c,805+(i*17),794+(i*14),774+(i*10),756+(i*6),2020+(i%5));}
ps("\nThunbergia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;thb_execution(t,c,794+(i*15),783+(i*12),765+(i*8),752+(i*5),2021+(i%4));}
ps("\nThunbergia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;thb_evaluation(t,c,786+(i*13),775+(i*10),761+(i*7),750+(i*4),2022+(i%3));}
ps("\nThunbergia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thb_accessory(t,c,778+(i*11),769+(i*9),755+(i*6),745+(i*3),2023+(i%2));}
ps("\nThunbergia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;thb_market(t,c,772+(i*9),763+(i*7),750+(i*5),742+(i*3),2024);}
ps("\n");thb_report();thb_state();ps("\n=== Demo Complete ===\n");return 0;}
