/* terminalia_admin: Terminalia management technology administration (v1.0)
 * Terminalia planning, terminalia execution, terminalia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} term_t;
typedef struct{int n_termp,n_terme,n_term2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} term_state_t;
static term_t terms[N],termess[N-2],termvss[N-4],termas[N-6],termmks[N-6]; static term_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(term_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;term_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TERM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int term_init(void){if(init)return -1;st.n_termp=0;st.n_terme=0;st.n_term2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)terms[i].active=0;for(int i=0;i<N-2;i++)termess[i].active=0;for(int i=0;i<N-4;i++)termvss[i].active=0;for(int i=0;i<N-6;i++)termas[i].active=0;for(int i=0;i<N-6;i++)termmks[i].active=0;init=1;ps("[TERM] Terminalia initialized\n");return 0;}
int term_planning(int t,int c,int a,int b,int d,int e,int y){return add(terms,&st.n_termp,&st.t_f1,N,t,c,a,b,d,e,y);}
int term_execution(int t,int c,int a,int b,int d,int e,int y){return add(termess,&st.n_terme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int term_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(termvss,&st.n_term2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int term_accessory(int t,int c,int a,int b,int d,int e,int y){return add(termas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int term_market(int t,int c,int a,int b,int d,int e,int y){return add(termmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void term_report(void){ps("[TERM] Termp: ");pi(st.n_termp);ps(" PCS=");pi(st.t_f1);ps("\nTerme: ");pi(st.n_terme);ps(" PCS=");pi(st.t_f2);ps("\nTermv: ");pi(st.n_term2);ps(" PCS=");pi(st.t_f3);ps("\nTermc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void term_state(void){ps("[TERM] Termp=");pi(st.n_termp);ps(" Terme=");pi(st.n_terme);ps(" Termv=");pi(st.n_term2);ps(" Termc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Terminalia Admin Demo ===\n\n");term_init();
ps("Terminalia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;term_planning(t,c,1348+(i*17),1337+(i*14),1317+(i*10),1299+(i*6),2020+(i%5));}
ps("\nTerminalia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;term_execution(t,c,1337+(i*15),1326+(i*12),1308+(i*8),1295+(i*5),2021+(i%4));}
ps("\nTerminalia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;term_evaluation(t,c,1329+(i*13),1318+(i*10),1302+(i*7),1291+(i*4),2022+(i%3));}
ps("\nTerminalia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;term_accessory(t,c,1321+(i*11),1312+(i*9),1298+(i*6),1288+(i*3),2023+(i%2));}
ps("\nTerminalia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;term_market(t,c,1315+(i*9),1306+(i*7),1293+(i*5),1285+(i*3),2024);}
ps("\n");term_report();term_state();ps("\n=== Demo Complete ===\n");return 0;}
