/* transition_economics_admin: Transition economics administration (v1.0)
 * Transition theory, privatization, marketization, opening up, social security
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tra_t;
typedef struct{int n_tt,n_pv,n_mk,n_op,n_ss,t_f1,t_f2,t_f3,t_f4,t_f5;} tra_state_t;
static tra_t tts[N],pvs[N-2],mks[N-4],ops[N-6],sss[N-6]; static tra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tra_init(void){if(init)return -1;st.n_tt=0;st.n_pv=0;st.n_mk=0;st.n_op=0;st.n_ss=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tts[i].active=0;for(int i=0;i<N-2;i++)pvs[i].active=0;for(int i=0;i<N-4;i++)mks[i].active=0;for(int i=0;i<N-6;i++)ops[i].active=0;for(int i=0;i<N-6;i++)sss[i].active=0;init=1;ps("[TRA] Transition economics initialized\n");return 0;}
int tra_theory(int t,int c,int a,int b,int d,int e,int y){return add(tts,&st.n_tt,&st.t_f1,N,t,c,a,b,d,e,y);}
int tra_privatize(int t,int c,int a,int b,int d,int e,int y){return add(pvs,&st.n_pv,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tra_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tra_open(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tra_security(int t,int c,int a,int b,int d,int e,int y){return add(sss,&st.n_ss,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tra_report(void){ps("[TRA] Theory: ");pi(st.n_tt);ps(" Path=");pi(st.t_f1);ps("\nPrivatize: ");pi(st.n_pv);ps(" SOE=");pi(st.t_f2);ps("\nMarket: ");pi(st.n_mk);ps(" Price=");pi(st.t_f3);ps("\nOpen: ");pi(st.n_op);ps(" Trade=");pi(st.t_f4);ps("\nSecurity: ");pi(st.n_ss);ps(" Pension=");pi(st.t_f5);ps("\n");}
void tra_state(void){ps("[TRA] Tt=");pi(st.n_tt);ps(" Pv=");pi(st.n_pv);ps(" Mk=");pi(st.n_mk);ps(" Op=");pi(st.n_op);ps(" Ss=");pi(st.n_ss);ps("\n");}
int main(void){
ps("=== Transition Economics Admin Demo ===\n\n");tra_init();
ps("Transition theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tra_theory(t,c,87+(i*17),72+(i*14),52+(i*10),35+(i*6),2020+(i%5));}
ps("\nPrivatization...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tra_privatize(t,c,76+(i*15),62+(i*12),44+(i*8),31+(i*5),2021+(i%4));}
ps("\nMarketization...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tra_market(t,c,68+(i*13),54+(i*10),38+(i*7),27+(i*4),2022+(i%3));}
ps("\nOpening up...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tra_open(t,c,60+(i*11),48+(i*9),34+(i*6),24+(i*3),2023+(i%2));}
ps("\nSocial security...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tra_security(t,c,54+(i*9),43+(i*7),30+(i*5),22+(i*3),2024);}
ps("\n");tra_report();tra_state();ps("\n=== Demo Complete ===\n");return 0;}
