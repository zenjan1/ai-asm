/* managerial_economics_admin: Managerial economics administration (v1.0)
 * Demand analysis, production decisions, cost analysis, pricing strategy, competition strategy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mgt_t;
typedef struct{int n_da,n_pd,n_ca,n_ps,n_cs,t_f1,t_f2,t_f3,t_f4,t_f5;} mgt_state_t;
static mgt_t das[N],pds[N-2],cas[N-4],pss[N-6],css[N-6]; static mgt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mgt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mgt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mgt_init(void){if(init)return -1;st.n_da=0;st.n_pd=0;st.n_ca=0;st.n_ps=0;st.n_cs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)das[i].active=0;for(int i=0;i<N-2;i++)pds[i].active=0;for(int i=0;i<N-4;i++)cas[i].active=0;for(int i=0;i<N-6;i++)pss[i].active=0;for(int i=0;i<N-6;i++)css[i].active=0;init=1;ps("[MGT] Managerial economics initialized\n");return 0;}
int mgt_demand(int t,int c,int a,int b,int d,int e,int y){return add(das,&st.n_da,&st.t_f1,N,t,c,a,b,d,e,y);}
int mgt_production(int t,int c,int a,int b,int d,int e,int y){return add(pds,&st.n_pd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mgt_cost(int t,int c,int a,int b,int d,int e,int y){return add(cas,&st.n_ca,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mgt_pricing(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mgt_compete(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mgt_report(void){ps("[MGT] Demand: ");pi(st.n_da);ps(" Elastic=");pi(st.t_f1);ps("\nProduct: ");pi(st.n_pd);ps(" Scale=");pi(st.t_f2);ps("\nCost: ");pi(st.n_ca);ps(" Marginal=");pi(st.t_f3);ps("\nPricing: ");pi(st.n_ps);ps(" Discr=");pi(st.t_f4);ps("\nCompete: ");pi(st.n_cs);ps(" Barrier=");pi(st.t_f5);ps("\n");}
void mgt_state(void){ps("[MGT] Da=");pi(st.n_da);ps(" Pd=");pi(st.n_pd);ps(" Ca=");pi(st.n_ca);ps(" Ps=");pi(st.n_ps);ps(" Cs=");pi(st.n_cs);ps("\n");}
int main(void){
ps("=== Managerial Economics Admin Demo ===\n\n");mgt_init();
ps("Demand analysis...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mgt_demand(t,c,89+(i*17),74+(i*14),54+(i*10),37+(i*6),2020+(i%5));}
ps("\nProduction decisions...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mgt_production(t,c,78+(i*15),64+(i*12),46+(i*8),33+(i*5),2021+(i%4));}
ps("\nCost analysis...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mgt_cost(t,c,70+(i*13),56+(i*10),40+(i*7),29+(i*4),2022+(i%3));}
ps("\nPricing strategy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mgt_pricing(t,c,62+(i*11),50+(i*9),36+(i*6),26+(i*3),2023+(i%2));}
ps("\nCompetition strategy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mgt_compete(t,c,56+(i*9),45+(i*7),32+(i*5),24+(i*3),2024);}
ps("\n");mgt_report();mgt_state();ps("\n=== Demo Complete ===\n");return 0;}
