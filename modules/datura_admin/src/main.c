/* datura_admin: Datura management technology administration (v1.0)
 * Datura planning, datura execution, datura evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dtr_t;
typedef struct{int n_dtp,n_dte,n_dtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dtr_state_t;
static dtr_t dtps[N],dtss[N-2],dtvss[N-4],dtas[N-6],dtmks[N-6]; static dtr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dtr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dtr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dtr_init(void){if(init)return -1;st.n_dtp=0;st.n_dte=0;st.n_dtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dtps[i].active=0;for(int i=0;i<N-2;i++)dtss[i].active=0;for(int i=0;i<N-4;i++)dtvss[i].active=0;for(int i=0;i<N-6;i++)dtas[i].active=0;for(int i=0;i<N-6;i++)dtmks[i].active=0;init=1;ps("[DTR] Datura initialized\n");return 0;}
int dtr_planning(int t,int c,int a,int b,int d,int e,int y){return add(dtps,&st.n_dtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dtr_execution(int t,int c,int a,int b,int d,int e,int y){return add(dtss,&st.n_dte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dtr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dtvss,&st.n_dtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dtr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dtr_market(int t,int c,int a,int b,int d,int e,int y){return add(dtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dtr_report(void){ps("[DTR] Dtp: ");pi(st.n_dtp);ps(" PCS=");pi(st.t_f1);ps("\nDte: ");pi(st.n_dte);ps(" PCS=");pi(st.t_f2);ps("\nDtv: ");pi(st.n_dtv);ps(" PCS=");pi(st.t_f3);ps("\nDtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dtr_state(void){ps("[DTR] Dtp=");pi(st.n_dtp);ps(" Dte=");pi(st.n_dte);ps(" Dtv=");pi(st.n_dtv);ps(" Dtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Datura Admin Demo ===\n\n");dtr_init();
ps("Datura planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dtr_planning(t,c,817+(i*17),806+(i*14),786+(i*10),768+(i*6),2020+(i%5));}
ps("\nDatura execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dtr_execution(t,c,806+(i*15),795+(i*12),777+(i*8),764+(i*5),2021+(i%4));}
ps("\nDatura evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dtr_evaluation(t,c,798+(i*13),787+(i*10),773+(i*7),762+(i*4),2022+(i%3));}
ps("\nDatura accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dtr_accessory(t,c,790+(i*11),781+(i*9),767+(i*6),757+(i*3),2023+(i%2));}
ps("\nDatura marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dtr_market(t,c,784+(i*9),775+(i*7),762+(i*5),754+(i*3),2024);}
ps("\n");dtr_report();dtr_state();ps("\n=== Demo Complete ===\n");return 0;}
