/* aquilegia_admin: Aquilegia management technology administration (v1.0)
 * Aquilegia planning, aquilegia execution, aquilegia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aq_t;
typedef struct{int n_aqp,n_aqe,n_aqv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aq_state_t;
static aq_t aqps[N],aqss[N-2],aqvss[N-4],aqas[N-6],aqmks[N-6]; static aq_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aq_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aq_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AQL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aql_init(void){if(init)return -1;st.n_aqp=0;st.n_aqe=0;st.n_aqv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aqps[i].active=0;for(int i=0;i<N-2;i++)aqss[i].active=0;for(int i=0;i<N-4;i++)aqvss[i].active=0;for(int i=0;i<N-6;i++)aqas[i].active=0;for(int i=0;i<N-6;i++)aqmks[i].active=0;init=1;ps("[AQL] Aquilegia initialized\n");return 0;}
int aql_planning(int t,int c,int a,int b,int d,int e,int y){return add(aqps,&st.n_aqp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aql_execution(int t,int c,int a,int b,int d,int e,int y){return add(aqss,&st.n_aqe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aql_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aqvss,&st.n_aqv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aql_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aqas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aql_market(int t,int c,int a,int b,int d,int e,int y){return add(aqmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aql_report(void){ps("[AQL] Aqp: ");pi(st.n_aqp);ps(" PCS=");pi(st.t_f1);ps("\nAqe: ");pi(st.n_aqe);ps(" PCS=");pi(st.t_f2);ps("\nAqv: ");pi(st.n_aqv);ps(" PCS=");pi(st.t_f3);ps("\nAqc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aql_state(void){ps("[AQL] Aqp=");pi(st.n_aqp);ps(" Aqe=");pi(st.n_aqe);ps(" Aqv=");pi(st.n_aqv);ps(" Aqc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aquilegia Admin Demo ===\n\n");aql_init();
ps("Aquilegia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aql_planning(t,c,758+(i*17),747+(i*14),727+(i*10),709+(i*6),2020+(i%5));}
ps("\nAquilegia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aql_execution(t,c,747+(i*15),736+(i*12),718+(i*8),705+(i*5),2021+(i%4));}
ps("\nAquilegia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aql_evaluation(t,c,739+(i*13),728+(i*10),714+(i*7),703+(i*4),2022+(i%3));}
ps("\nAquilegia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aql_accessory(t,c,731+(i*11),722+(i*9),708+(i*6),698+(i*3),2023+(i%2));}
ps("\nAquilegia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aql_market(t,c,725+(i*9),716+(i*7),703+(i*5),695+(i*3),2024);}
ps("\n");aql_report();aql_state();ps("\n=== Demo Complete ===\n");return 0;}
