/* anise_admin: Anise management technology administration (v1.0)
 * Anise planning, anise execution, anise evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} an_t;
typedef struct{int n_anp,n_ane,n_anv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} an_state_t;
static an_t anps[N],anss[N-2],anvss[N-4],anas[N-6],anmks[N-6]; static an_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(an_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;an_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int an_init(void){if(init)return -1;st.n_anp=0;st.n_ane=0;st.n_anv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anps[i].active=0;for(int i=0;i<N-2;i++)anss[i].active=0;for(int i=0;i<N-4;i++)anvss[i].active=0;for(int i=0;i<N-6;i++)anas[i].active=0;for(int i=0;i<N-6;i++)anmks[i].active=0;init=1;ps("[ANS] Anise initialized\n");return 0;}
int an_planning(int t,int c,int a,int b,int d,int e,int y){return add(anps,&st.n_anp,&st.t_f1,N,t,c,a,b,d,e,y);}
int an_execution(int t,int c,int a,int b,int d,int e,int y){return add(anss,&st.n_ane,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int an_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anvss,&st.n_anv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int an_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int an_market(int t,int c,int a,int b,int d,int e,int y){return add(anmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void an_report(void){ps("[ANS] Anp: ");pi(st.n_anp);ps(" PCS=");pi(st.t_f1);ps("\nAne: ");pi(st.n_ane);ps(" PCS=");pi(st.t_f2);ps("\nAnv: ");pi(st.n_anv);ps(" PCS=");pi(st.t_f3);ps("\nAnc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void an_state(void){ps("[ANS] Anp=");pi(st.n_anp);ps(" Ane=");pi(st.n_ane);ps(" Anv=");pi(st.n_anv);ps(" Anc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anise Admin Demo ===\n\n");an_init();
ps("Anise planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;an_planning(t,c,679+(i*17),668+(i*14),648+(i*10),630+(i*6),2020+(i%5));}
ps("\nAnise execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;an_execution(t,c,668+(i*15),657+(i*12),639+(i*8),626+(i*5),2021+(i%4));}
ps("\nAnise evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;an_evaluation(t,c,660+(i*13),649+(i*10),633+(i*7),622+(i*4),2022+(i%3));}
ps("\nAnise accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;an_accessory(t,c,652+(i*11),643+(i*9),629+(i*6),619+(i*3),2023+(i%2));}
ps("\nAnise marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;an_market(t,c,646+(i*9),637+(i*7),624+(i*5),616+(i*3),2024);}
ps("\n");an_report();an_state();ps("\n=== Demo Complete ===\n");return 0;}
