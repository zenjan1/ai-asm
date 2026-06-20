/* communication_admin: Communication management technology administration (v1.0)
 * Communication planning, communication execution, communication evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctc_t;
typedef struct{int n_cpl,n_ce,n_cv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctc_state_t;
static ctc_t cpls[N],ces[N-2],cvs[N-4],acs[N-6],mks[N-6]; static ctc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctc_init(void){if(init)return -1;st.n_cpl=0;st.n_ce=0;st.n_cv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cpls[i].active=0;for(int i=0;i<N-2;i++)ces[i].active=0;for(int i=0;i<N-4;i++)cvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTC] Communication initialized\n");return 0;}
int ctc_planning(int t,int c,int a,int b,int d,int e,int y){return add(cpls,&st.n_cpl,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctc_execution(int t,int c,int a,int b,int d,int e,int y){return add(ces,&st.n_ce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctc_report(void){ps("[CTC] Cpl: ");pi(st.n_cpl);ps(" PCS=");pi(st.t_f1);ps("\nCe: ");pi(st.n_ce);ps(" PCS=");pi(st.t_f2);ps("\nCv: ");pi(st.n_cv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctc_state(void){ps("[CTC] Cpl=");pi(st.n_cpl);ps(" Ce=");pi(st.n_ce);ps(" Cv=");pi(st.n_cv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Communication Admin Demo ===\n\n");ctc_init();
ps("Communication planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctc_planning(t,c,463+(i*17),452+(i*14),432+(i*10),414+(i*6),2020+(i%5));}
ps("\nCommunication execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctc_execution(t,c,452+(i*15),441+(i*12),423+(i*8),410+(i*5),2021+(i%4));}
ps("\nCommunication evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctc_evaluation(t,c,444+(i*13),433+(i*10),417+(i*7),406+(i*4),2022+(i%3));}
ps("\nCommunication accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctc_accessory(t,c,436+(i*11),427+(i*9),413+(i*6),403+(i*3),2023+(i%2));}
ps("\nCommunication marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctc_market(t,c,430+(i*9),421+(i*7),408+(i*5),400+(i*3),2024);}
ps("\n");ctc_report();ctc_state();ps("\n=== Demo Complete ===\n");return 0;}
