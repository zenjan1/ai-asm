/* experimental_economics_admin: Experimental economics administration (v1.0)
 * Experimental methods, market experiments, game experiments, decision experiments, behavioral experiments
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} exp_t;
typedef struct{int n_em,n_me,n_ge,n_de,n_be,t_f1,t_f2,t_f3,t_f4,t_f5;} exp_state_t;
static exp_t ems[N],mes[N-2],ges[N-4],des[N-6],bes[N-6]; static exp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(exp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;exp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EXP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int exp_init(void){if(init)return -1;st.n_em=0;st.n_me=0;st.n_ge=0;st.n_de=0;st.n_be=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ems[i].active=0;for(int i=0;i<N-2;i++)mes[i].active=0;for(int i=0;i<N-4;i++)ges[i].active=0;for(int i=0;i<N-6;i++)des[i].active=0;for(int i=0;i<N-6;i++)bes[i].active=0;init=1;ps("[EXP] Experimental economics initialized\n");return 0;}
int exp_method(int t,int c,int a,int b,int d,int e,int y){return add(ems,&st.n_em,&st.t_f1,N,t,c,a,b,d,e,y);}
int exp_market(int t,int c,int a,int b,int d,int e,int y){return add(mes,&st.n_me,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int exp_game(int t,int c,int a,int b,int d,int e,int y){return add(ges,&st.n_ge,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int exp_decision(int t,int c,int a,int b,int d,int e,int y){return add(des,&st.n_de,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int exp_behavior(int t,int c,int a,int b,int d,int e,int y){return add(bes,&st.n_be,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void exp_report(void){ps("[EXP] Method: ");pi(st.n_em);ps(" Design=");pi(st.t_f1);ps("\nMarket: ");pi(st.n_me);ps(" Auction=");pi(st.t_f2);ps("\nGame: ");pi(st.n_ge);ps(" Prison=");pi(st.t_f3);ps("\nDecision: ");pi(st.n_de);ps(" Risk=");pi(st.t_f4);ps("\nBehavior: ");pi(st.n_be);ps(" Altru=");pi(st.t_f5);ps("\n");}
void exp_state(void){ps("[EXP] Em=");pi(st.n_em);ps(" Me=");pi(st.n_me);ps(" Ge=");pi(st.n_ge);ps(" De=");pi(st.n_de);ps(" Be=");pi(st.n_be);ps("\n");}
int main(void){
ps("=== Experimental Economics Admin Demo ===\n\n");exp_init();
ps("Experimental methods...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;exp_method(t,c,91+(i*17),76+(i*14),56+(i*10),39+(i*6),2020+(i%5));}
ps("\nMarket experiments...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;exp_market(t,c,80+(i*15),66+(i*12),48+(i*8),35+(i*5),2021+(i%4));}
ps("\nGame experiments...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;exp_game(t,c,72+(i*13),58+(i*10),42+(i*7),31+(i*4),2022+(i%3));}
ps("\nDecision experiments...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exp_decision(t,c,64+(i*11),52+(i*9),38+(i*6),28+(i*3),2023+(i%2));}
ps("\nBehavioral experiments...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;exp_behavior(t,c,58+(i*9),47+(i*7),34+(i*5),26+(i*3),2024);}
ps("\n");exp_report();exp_state();ps("\n=== Demo Complete ===\n");return 0;}
