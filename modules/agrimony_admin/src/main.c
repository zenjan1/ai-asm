/* agrimony_admin: Agrimony management technology administration (v1.0)
 * Agrimony planning, agrimony execution, agrimony evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} agri_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} agri_state_t;
static agri_t agrip[N],agrix[N-2],agri2[N-4],agriac[N-6],agrim[N-6]; static agri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(agri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;agri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int agri_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)agrip[i].active=0;for(int i=0;i<N-2;i++)agrix[i].active=0;for(int i=0;i<N-4;i++)agri2[i].active=0;for(int i=0;i<N-6;i++)agriac[i].active=0;for(int i=0;i<N-6;i++)agrim[i].active=0;init=1;ps("[AGRI] Agrimony initialized\n");return 0;}
int agri_planning(int t,int c,int a,int b,int d,int e,int y){return add(agrip,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int agri_execution(int t,int c,int a,int b,int d,int e,int y){return add(agrix,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int agri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(agri2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int agri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(agriac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int agri_market(int t,int c,int a,int b,int d,int e,int y){return add(agrim,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void agri_report(void){ps("[AGRI] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void agri_state(void){ps("[AGRI] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Agrimony Admin Demo ===\n\n");agri_init();
ps("Agrimony planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;agri_planning(t,c,1760+(i*17),1749+(i*14),1729+(i*10),1711+(i*6),2020+(i%5));}
ps("\nAgrimony execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;agri_execution(t,c,1749+(i*15),1738+(i*12),1720+(i*8),1707+(i*5),2021+(i%4));}
ps("\nAgrimony evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;agri_evaluation(t,c,1741+(i*13),1730+(i*10),1714+(i*7),1703+(i*4),2022+(i%3));}
ps("\nAgrimony accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agri_accessory(t,c,1733+(i*11),1724+(i*9),1710+(i*6),1700+(i*3),2023+(i%2));}
ps("\nAgrimony marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;agri_market(t,c,1727+(i*9),1718+(i*7),1705+(i*5),1697+(i*3),2024);}
ps("\n");agri_report();agri_state();ps("\n=== Demo Complete ===\n");return 0;}
