/* tibouchina_admin: Tibouchina management technology administration (v1.0)
 * Tibouchina planning, tibouchina execution, tibouchina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tib_t;
typedef struct{int n_tib,n_tie,n_tiv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tib_state_t;
static tib_t tibs[N],tiss[N-2],tivss[N-4],tias[N-6],timks[N-6]; static tib_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tib_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tib_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tbn_init(void){if(init)return -1;st.n_tib=0;st.n_tie=0;st.n_tiv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tibs[i].active=0;for(int i=0;i<N-2;i++)tiss[i].active=0;for(int i=0;i<N-4;i++)tivss[i].active=0;for(int i=0;i<N-6;i++)tias[i].active=0;for(int i=0;i<N-6;i++)timks[i].active=0;init=1;ps("[TBN] Tibouchina initialized\n");return 0;}
int tbn_planning(int t,int c,int a,int b,int d,int e,int y){return add(tibs,&st.n_tib,&st.t_f1,N,t,c,a,b,d,e,y);}
int tbn_execution(int t,int c,int a,int b,int d,int e,int y){return add(tiss,&st.n_tie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tbn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tivss,&st.n_tiv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tbn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tbn_market(int t,int c,int a,int b,int d,int e,int y){return add(timks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tbn_report(void){ps("[TBN] Tib: ");pi(st.n_tib);ps(" PCS=");pi(st.t_f1);ps("\nTie: ");pi(st.n_tie);ps(" PCS=");pi(st.t_f2);ps("\nTiv: ");pi(st.n_tiv);ps(" PCS=");pi(st.t_f3);ps("\nTic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tbn_state(void){ps("[TBN] Tib=");pi(st.n_tib);ps(" Tie=");pi(st.n_tie);ps(" Tiv=");pi(st.n_tiv);ps(" Tic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tibouchina Admin Demo ===\n\n");tbn_init();
ps("Tibouchina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tbn_planning(t,c,806+(i*17),795+(i*14),775+(i*10),757+(i*6),2020+(i%5));}
ps("\nTibouchina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tbn_execution(t,c,795+(i*15),784+(i*12),766+(i*8),753+(i*5),2021+(i%4));}
ps("\nTibouchina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tbn_evaluation(t,c,787+(i*13),776+(i*10),762+(i*7),751+(i*4),2022+(i%3));}
ps("\nTibouchina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbn_accessory(t,c,779+(i*11),770+(i*9),756+(i*6),746+(i*3),2023+(i%2));}
ps("\nTibouchina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbn_market(t,c,773+(i*9),764+(i*7),751+(i*5),743+(i*3),2024);}
ps("\n");tbn_report();tbn_state();ps("\n=== Demo Complete ===\n");return 0;}
