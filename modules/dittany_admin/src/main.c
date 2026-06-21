/* dittany_admin: Dittany management technology administration (v1.0)
 * Dittany planning, dittany execution, dittany evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dt_t;
typedef struct{int n_dtp,n_dte,n_dtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dt_state_t;
static dt_t dtps[N],dtss[N-2],dtvss[N-4],dtas[N-6],dtmks[N-6]; static dt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DTN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dt_init(void){if(init)return -1;st.n_dtp=0;st.n_dte=0;st.n_dtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dtps[i].active=0;for(int i=0;i<N-2;i++)dtss[i].active=0;for(int i=0;i<N-4;i++)dtvss[i].active=0;for(int i=0;i<N-6;i++)dtas[i].active=0;for(int i=0;i<N-6;i++)dtmks[i].active=0;init=1;ps("[DTN] Dittany initialized\n");return 0;}
int dt_planning(int t,int c,int a,int b,int d,int e,int y){return add(dtps,&st.n_dtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dt_execution(int t,int c,int a,int b,int d,int e,int y){return add(dtss,&st.n_dte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dtvss,&st.n_dtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dt_market(int t,int c,int a,int b,int d,int e,int y){return add(dtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dt_report(void){ps("[DTN] Dtp: ");pi(st.n_dtp);ps(" PCS=");pi(st.t_f1);ps("\nDte: ");pi(st.n_dte);ps(" PCS=");pi(st.t_f2);ps("\nDtv: ");pi(st.n_dtv);ps(" PCS=");pi(st.t_f3);ps("\nDtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dt_state(void){ps("[DTN] Dtp=");pi(st.n_dtp);ps(" Dte=");pi(st.n_dte);ps(" Dtv=");pi(st.n_dtv);ps(" Dtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dittany Admin Demo ===\n\n");dt_init();
ps("Dittany planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dt_planning(t,c,694+(i*17),683+(i*14),663+(i*10),645+(i*6),2020+(i%5));}
ps("\nDittany execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dt_execution(t,c,683+(i*15),672+(i*12),654+(i*8),641+(i*5),2021+(i%4));}
ps("\nDittany evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dt_evaluation(t,c,675+(i*13),664+(i*10),648+(i*7),637+(i*4),2022+(i%3));}
ps("\nDittany accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dt_accessory(t,c,667+(i*11),658+(i*9),644+(i*6),634+(i*3),2023+(i%2));}
ps("\nDittany marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dt_market(t,c,661+(i*9),652+(i*7),639+(i*5),631+(i*3),2024);}
ps("\n");dt_report();dt_state();ps("\n=== Demo Complete ===\n");return 0;}
