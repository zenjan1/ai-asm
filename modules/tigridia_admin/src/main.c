/* tigridia_admin: Tigridia management technology administration (v1.0)
 * Tigridia planning, tigridia execution, tigridia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tig_t;
typedef struct{int n_tip,n_tie,n_tiv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tig_state_t;
static tig_t tips[N],ties[N-2],tivs[N-4],acs[N-6],mks[N-6]; static tig_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tig_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tig_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TIG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tig_init(void){if(init)return -1;st.n_tip=0;st.n_tie=0;st.n_tiv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tips[i].active=0;for(int i=0;i<N-2;i++)ties[i].active=0;for(int i=0;i<N-4;i++)tivs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TIG] Tigridia initialized\n");return 0;}
int tig_planning(int t,int c,int a,int b,int d,int e,int y){return add(tips,&st.n_tip,&st.t_f1,N,t,c,a,b,d,e,y);}
int tig_execution(int t,int c,int a,int b,int d,int e,int y){return add(ties,&st.n_tie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tig_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tivs,&st.n_tiv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tig_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tig_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tig_report(void){ps("[TIG] Tip: ");pi(st.n_tip);ps(" PCS=");pi(st.t_f1);ps("\nTie: ");pi(st.n_tie);ps(" PCS=");pi(st.t_f2);ps("\nTiv: ");pi(st.n_tiv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tig_state(void){ps("[TIG] Tip=");pi(st.n_tip);ps(" Tie=");pi(st.n_tie);ps(" Tiv=");pi(st.n_tiv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tigridia Admin Demo ===\n\n");tig_init();
ps("Tigridia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tig_planning(t,c,616+(i*17),605+(i*14),585+(i*10),567+(i*6),2020+(i%5));}
ps("\nTigridia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tig_execution(t,c,605+(i*15),594+(i*12),576+(i*8),563+(i*5),2021+(i%4));}
ps("\nTigridia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tig_evaluation(t,c,597+(i*13),586+(i*10),570+(i*7),559+(i*4),2022+(i%3));}
ps("\nTigridia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tig_accessory(t,c,589+(i*11),580+(i*9),566+(i*6),556+(i*3),2023+(i%2));}
ps("\nTigridia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tig_market(t,c,583+(i*9),574+(i*7),561+(i*5),553+(i*3),2024);}
ps("\n");tig_report();tig_state();ps("\n=== Demo Complete ===\n");return 0;}
