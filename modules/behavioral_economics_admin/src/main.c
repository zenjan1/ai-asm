/* behavioral_economics_admin: Behavioral economics administration system (v1.0)
 * Bounded rationality, prospect theory, mental accounting, heuristics biases, behavioral finance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} beh_sub_t;
typedef struct{int n_rat,n_pro,n_men,n_heu,n_bhf,t_f1,t_f2,t_f3,t_f4,t_f5;} beh_state_t;
static beh_sub_t rats[N],pros[N-2],mens[N-4],heus[N-6],bhfs[N-6]; static beh_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(beh_sub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;beh_sub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BEH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int beh_init(void){if(init)return -1;st.n_rat=0;st.n_pro=0;st.n_men=0;st.n_heu=0;st.n_bhf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rats[i].active=0;for(int i=0;i<N-2;i++)pros[i].active=0;for(int i=0;i<N-4;i++)mens[i].active=0;for(int i=0;i<N-6;i++)heus[i].active=0;for(int i=0;i<N-6;i++)bhfs[i].active=0;init=1;ps("[BEH] Behavioral economics initialized\n");return 0;}
int beh_rational(int t,int c,int a,int b,int d,int e,int y){return add(rats,&st.n_rat,&st.t_f1,N,t,c,a,b,d,e,y);}
int beh_prospect(int t,int c,int a,int b,int d,int e,int y){return add(pros,&st.n_pro,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int beh_mental(int t,int c,int a,int b,int d,int e,int y){return add(mens,&st.n_men,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int beh_heuristic(int t,int c,int a,int b,int d,int e,int y){return add(heus,&st.n_heu,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int beh_finance(int t,int c,int a,int b,int d,int e,int y){return add(bhfs,&st.n_bhf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void beh_report(void){ps("[BEH] Rationality: ");pi(st.n_rat);ps(" Rat=");pi(st.t_f1);ps("\nProspect: ");pi(st.n_pro);ps(" Pro=");pi(st.t_f2);ps("\nMental: ");pi(st.n_men);ps(" Men=");pi(st.t_f3);ps("\nHeuristic: ");pi(st.n_heu);ps(" Heu=");pi(st.t_f4);ps("\nFinance: ");pi(st.n_bhf);ps(" Bhf=");pi(st.t_f5);ps("\n");}
void beh_state(void){ps("[BEH] Ra=");pi(st.n_rat);ps(" Pr=");pi(st.n_pro);ps(" Me=");pi(st.n_men);ps(" He=");pi(st.n_heu);ps(" Bf=");pi(st.n_bhf);ps("\n");}
int main(void){
ps("=== Behavioral Economics Admin Demo ===\n\n");beh_init();
ps("Bounded rationality...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;beh_rational(t,c,78+(i*18),64+(i*15),44+(i*11),27+(i*7),2020+(i%5));}
ps("\nProspect theory...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;beh_prospect(t,c,68+(i*16),54+(i*13),36+(i*9),23+(i*6),2021+(i%4));}
ps("\nMental accounting...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;beh_mental(t,c,60+(i*14),46+(i*11),30+(i*7),19+(i*4),2022+(i%3));}
ps("\nHeuristics & biases...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beh_heuristic(t,c,52+(i*12),40+(i*9),26+(i*6),17+(i*3),2023+(i%2));}
ps("\nBehavioral finance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beh_finance(t,c,46+(i*10),35+(i*8),22+(i*5),15+(i*3),2024);}
ps("\n");beh_report();beh_state();ps("\n=== Demo Complete ===\n");return 0;}
