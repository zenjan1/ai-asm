/* quassia_admin: Quassia management technology administration (v1.0)
 * Quassia planning, quassia execution, quassia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} qss_t;
typedef struct{int n_qsp,n_qse,n_qsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} qss_state_t;
static qss_t qssps[N],qssss[N-2],qssvss[N-4],qssas[N-6],qssmks[N-6]; static qss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qss_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;qss_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[QSS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int qss_init(void){if(init)return -1;st.n_qsp=0;st.n_qse=0;st.n_qsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)qssps[i].active=0;for(int i=0;i<N-2;i++)qssss[i].active=0;for(int i=0;i<N-4;i++)qssvss[i].active=0;for(int i=0;i<N-6;i++)qssas[i].active=0;for(int i=0;i<N-6;i++)qssmks[i].active=0;init=1;ps("[QSS] Quassia initialized\n");return 0;}
int qss_planning(int t,int c,int a,int b,int d,int e,int y){return add(qssps,&st.n_qsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int qss_execution(int t,int c,int a,int b,int d,int e,int y){return add(qssss,&st.n_qse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int qss_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(qssvss,&st.n_qsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int qss_accessory(int t,int c,int a,int b,int d,int e,int y){return add(qssas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int qss_market(int t,int c,int a,int b,int d,int e,int y){return add(qssmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void qss_report(void){ps("[QSS] Qsp: ");pi(st.n_qsp);ps(" PCS=");pi(st.t_f1);ps("\nQse: ");pi(st.n_qse);ps(" PCS=");pi(st.t_f2);ps("\nQsv: ");pi(st.n_qsv);ps(" PCS=");pi(st.t_f3);ps("\nQsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void qss_state(void){ps("[QSS] Qsp=");pi(st.n_qsp);ps(" Qse=");pi(st.n_qse);ps(" Qsv=");pi(st.n_qsv);ps(" Qsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Quassia Admin Demo ===\n\n");qss_init();
ps("Quassia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;qss_planning(t,c,853+(i*17),842+(i*14),822+(i*10),804+(i*6),2020+(i%5));}
ps("\nQuassia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;qss_execution(t,c,842+(i*15),831+(i*12),813+(i*8),800+(i*5),2021+(i%4));}
ps("\nQuassia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;qss_evaluation(t,c,834+(i*13),823+(i*10),807+(i*7),796+(i*4),2022+(i%3));}
ps("\nQuassia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qss_accessory(t,c,826+(i*11),817+(i*9),803+(i*6),793+(i*3),2023+(i%2));}
ps("\nQuassia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qss_market(t,c,820+(i*9),811+(i*7),798+(i*5),790+(i*3),2024);}
ps("\n");qss_report();qss_state();ps("\n=== Demo Complete ===\n");return 0;}
