/* caltrop_admin: Caltrop management technology administration (v1.0)
 * Caltrop planning, caltrop execution, caltrop evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cltr_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} cltr_state_t;
static cltr_t cltrp[N],cltrx[N-2],cltr2[N-4],cltrac[N-6],cltrm[N-6]; static cltr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cltr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cltr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CLTR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cltr_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cltrp[i].active=0;for(int i=0;i<N-2;i++)cltrx[i].active=0;for(int i=0;i<N-4;i++)cltr2[i].active=0;for(int i=0;i<N-6;i++)cltrac[i].active=0;for(int i=0;i<N-6;i++)cltrm[i].active=0;init=1;ps("[CLTR] Caltrop initialized\n");return 0;}
int cltr_planning(int t,int c,int a,int b,int d,int e,int y){return add(cltrp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int cltr_execution(int t,int c,int a,int b,int d,int e,int y){return add(cltrx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cltr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cltr2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cltr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cltrac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cltr_market(int t,int c,int a,int b,int d,int e,int y){return add(cltrm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cltr_report(void){ps("[CLTR] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void cltr_state(void){ps("[CLTR] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Caltrop Admin Demo ===\n\n");cltr_init();
ps("Caltrop planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cltr_planning(t,c,1821+(i*17),1810+(i*14),1790+(i*10),1772+(i*6),2020+(i%5));}
ps("\nCaltrop execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cltr_execution(t,c,1810+(i*15),1799+(i*12),1781+(i*8),1768+(i*5),2021+(i%4));}
ps("\nCaltrop evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cltr_evaluation(t,c,1802+(i*13),1791+(i*10),1775+(i*7),1764+(i*4),2022+(i%3));}
ps("\nCaltrop accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cltr_accessory(t,c,1794+(i*11),1785+(i*9),1771+(i*6),1761+(i*3),2023+(i%2));}
ps("\nCaltrop marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cltr_market(t,c,1788+(i*9),1779+(i*7),1766+(i*5),1758+(i*3),2024);}
ps("\n");cltr_report();cltr_state();ps("\n=== Demo Complete ===\n");return 0;}
