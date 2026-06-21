/* quillaja_admin: Quillaja management technology administration (v1.0)
 * Quillaja planning, quillaja execution, quillaja evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} qja_t;
typedef struct{int n_qjp,n_qje,n_qjv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} qja_state_t;
static qja_t qjaps[N],qjass[N-2],qjavss[N-4],qjas[N-6],qjamks[N-6]; static qja_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(qja_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;qja_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[QJA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int qja_init(void){if(init)return -1;st.n_qjp=0;st.n_qje=0;st.n_qjv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)qjaps[i].active=0;for(int i=0;i<N-2;i++)qjass[i].active=0;for(int i=0;i<N-4;i++)qjavss[i].active=0;for(int i=0;i<N-6;i++)qjas[i].active=0;for(int i=0;i<N-6;i++)qjamks[i].active=0;init=1;ps("[QJA] Quillaja initialized\n");return 0;}
int qja_planning(int t,int c,int a,int b,int d,int e,int y){return add(qjaps,&st.n_qjp,&st.t_f1,N,t,c,a,b,d,e,y);}
int qja_execution(int t,int c,int a,int b,int d,int e,int y){return add(qjass,&st.n_qje,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int qja_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(qjavss,&st.n_qjv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int qja_accessory(int t,int c,int a,int b,int d,int e,int y){return add(qjas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int qja_market(int t,int c,int a,int b,int d,int e,int y){return add(qjamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void qja_report(void){ps("[QJA] Qjp: ");pi(st.n_qjp);ps(" PCS=");pi(st.t_f1);ps("\nQje: ");pi(st.n_qje);ps(" PCS=");pi(st.t_f2);ps("\nQjv: ");pi(st.n_qjv);ps(" PCS=");pi(st.t_f3);ps("\nQjc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void qja_state(void){ps("[QJA] Qjp=");pi(st.n_qjp);ps(" Qje=");pi(st.n_qje);ps(" Qjv=");pi(st.n_qjv);ps(" Qjc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Quillaja Admin Demo ===\n\n");qja_init();
ps("Quillaja planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;qja_planning(t,c,855+(i*17),844+(i*14),824+(i*10),806+(i*6),2020+(i%5));}
ps("\nQuillaja execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;qja_execution(t,c,844+(i*15),833+(i*12),815+(i*8),802+(i*5),2021+(i%4));}
ps("\nQuillaja evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;qja_evaluation(t,c,836+(i*13),825+(i*10),809+(i*7),798+(i*4),2022+(i%3));}
ps("\nQuillaja accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qja_accessory(t,c,828+(i*11),819+(i*9),805+(i*6),795+(i*3),2023+(i%2));}
ps("\nQuillaja marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;qja_market(t,c,822+(i*9),813+(i*7),800+(i*5),792+(i*3),2024);}
ps("\n");qja_report();qja_state();ps("\n=== Demo Complete ===\n");return 0;}
