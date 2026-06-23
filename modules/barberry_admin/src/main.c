/* barberry_admin: Barberry management technology administration (v1.0)
 * Barberry planning, barberry execution, barberry evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} barb_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} barb_state_t;
static barb_t barbp[N],barbx[N-2],barb2[N-4],barbac[N-6],barbm[N-6]; static barb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(barb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;barb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BARB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int barb_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)barbp[i].active=0;for(int i=0;i<N-2;i++)barbx[i].active=0;for(int i=0;i<N-4;i++)barb2[i].active=0;for(int i=0;i<N-6;i++)barbac[i].active=0;for(int i=0;i<N-6;i++)barbm[i].active=0;init=1;ps("[BARB] Barberry initialized\n");return 0;}
int barb_planning(int t,int c,int a,int b,int d,int e,int y){return add(barbp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int barb_execution(int t,int c,int a,int b,int d,int e,int y){return add(barbx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int barb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(barb2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int barb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(barbac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int barb_market(int t,int c,int a,int b,int d,int e,int y){return add(barbm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void barb_report(void){ps("[BARB] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void barb_state(void){ps("[BARB] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Barberry Admin Demo ===\n\n");barb_init();
ps("Barberry planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;barb_planning(t,c,1783+(i*17),1772+(i*14),1752+(i*10),1734+(i*6),2020+(i%5));}
ps("\nBarberry execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;barb_execution(t,c,1772+(i*15),1761+(i*12),1743+(i*8),1730+(i*5),2021+(i%4));}
ps("\nBarberry evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;barb_evaluation(t,c,1764+(i*13),1753+(i*10),1737+(i*7),1726+(i*4),2022+(i%3));}
ps("\nBarberry accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;barb_accessory(t,c,1756+(i*11),1747+(i*9),1733+(i*6),1723+(i*3),2023+(i%2));}
ps("\nBarberry marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;barb_market(t,c,1750+(i*9),1741+(i*7),1728+(i*5),1720+(i*3),2024);}
ps("\n");barb_report();barb_state();ps("\n=== Demo Complete ===\n");return 0;}
