/* cascabella_admin: Cascabella management technology administration (v1.0)
 * Cascabella planning, cascabella execution, cascabella evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} csc_t;
typedef struct{int n_csp,n_cse,n_csv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} csc_state_t;
static csc_t csps[N],cses[N-2],csvs[N-4],cacs[N-6],cmks[N-6]; static csc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(csc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;csc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int csc_init(void){if(init)return -1;st.n_csp=0;st.n_cse=0;st.n_csv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)csps[i].active=0;for(int i=0;i<N-2;i++)cses[i].active=0;for(int i=0;i<N-4;i++)csvs[i].active=0;for(int i=0;i<N-6;i++)cacs[i].active=0;for(int i=0;i<N-6;i++)cmks[i].active=0;init=1;ps("[CSC] Cascabella initialized\n");return 0;}
int csc_planning(int t,int c,int a,int b,int d,int e,int y){return add(csps,&st.n_csp,&st.t_f1,N,t,c,a,b,d,e,y);}
int csc_execution(int t,int c,int a,int b,int d,int e,int y){return add(cses,&st.n_cse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int csc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(csvs,&st.n_csv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int csc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cacs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int csc_market(int t,int c,int a,int b,int d,int e,int y){return add(cmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void csc_report(void){ps("[CSC] Csp: ");pi(st.n_csp);ps(" PCS=");pi(st.t_f1);ps("\nCse: ");pi(st.n_cse);ps(" PCS=");pi(st.t_f2);ps("\nCsv: ");pi(st.n_csv);ps(" PCS=");pi(st.t_f3);ps("\nCc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void csc_state(void){ps("[CSC] Csp=");pi(st.n_csp);ps(" Cse=");pi(st.n_cse);ps(" Csv=");pi(st.n_csv);ps(" Cc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cascabella Admin Demo ===\n\n");csc_init();
ps("Cascabella planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;csc_planning(t,c,636+(i*17),625+(i*14),605+(i*10),587+(i*6),2020+(i%5));}
ps("\nCascabella execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;csc_execution(t,c,625+(i*15),614+(i*12),596+(i*8),583+(i*5),2021+(i%4));}
ps("\nCascabella evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;csc_evaluation(t,c,617+(i*13),606+(i*10),590+(i*7),579+(i*4),2022+(i%3));}
ps("\nCascabella accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csc_accessory(t,c,609+(i*11),600+(i*9),586+(i*6),576+(i*3),2023+(i%2));}
ps("\nCascabella marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;csc_market(t,c,603+(i*9),594+(i*7),581+(i*5),573+(i*3),2024);}
ps("\n");csc_report();csc_state();ps("\n=== Demo Complete ===\n");return 0;}
