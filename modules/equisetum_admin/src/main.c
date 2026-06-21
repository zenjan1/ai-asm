/* equisetum_admin: Equisetum management technology administration (v1.0)
 * Equisetum planning, equisetum execution, equisetum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eq_t;
typedef struct{int n_qlp,n_qle,n_qlv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eq_state_t;
static eq_t qlps[N],qlss[N-2],qlvss[N-4],qlas[N-6],qlmks[N-6]; static eq_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eq_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eq_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EQL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eql_init(void){if(init)return -1;st.n_qlp=0;st.n_qle=0;st.n_qlv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)qlps[i].active=0;for(int i=0;i<N-2;i++)qlss[i].active=0;for(int i=0;i<N-4;i++)qlvss[i].active=0;for(int i=0;i<N-6;i++)qlas[i].active=0;for(int i=0;i<N-6;i++)qlmks[i].active=0;init=1;ps("[EQL] Equisetum initialized\n");return 0;}
int eql_planning(int t,int c,int a,int b,int d,int e,int y){return add(qlps,&st.n_qlp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eql_execution(int t,int c,int a,int b,int d,int e,int y){return add(qlss,&st.n_qle,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eql_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(qlvss,&st.n_qlv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eql_accessory(int t,int c,int a,int b,int d,int e,int y){return add(qlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eql_market(int t,int c,int a,int b,int d,int e,int y){return add(qlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eql_report(void){ps("[EQL] Qlp: ");pi(st.n_qlp);ps(" PCS=");pi(st.t_f1);ps("\nQle: ");pi(st.n_qle);ps(" PCS=");pi(st.t_f2);ps("\nQlv: ");pi(st.n_qlv);ps(" PCS=");pi(st.t_f3);ps("\nQlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eql_state(void){ps("[EQL] Qlp=");pi(st.n_qlp);ps(" Qle=");pi(st.n_qle);ps(" Qlv=");pi(st.n_qlv);ps(" Qlc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Equisetum Admin Demo ===\n\n");eql_init();
ps("Equisetum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eql_planning(t,c,740+(i*17),729+(i*14),709+(i*10),691+(i*6),2020+(i%5));}
ps("\nEquisetum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eql_execution(t,c,729+(i*15),718+(i*12),700+(i*8),687+(i*5),2021+(i%4));}
ps("\nEquisetum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eql_evaluation(t,c,721+(i*13),710+(i*10),694+(i*7),683+(i*4),2022+(i%3));}
ps("\nEquisetum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eql_accessory(t,c,713+(i*11),704+(i*9),690+(i*6),680+(i*3),2023+(i%2));}
ps("\nEquisetum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eql_market(t,c,707+(i*9),698+(i*7),685+(i*5),677+(i*3),2024);}
ps("\n");eql_report();eql_state();ps("\n=== Demo Complete ===\n");return 0;}
