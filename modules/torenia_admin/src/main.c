/* torenia_admin: Torenia management technology administration (v1.0)
 * Torenia planning, torenia execution, torenia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tor_t;
typedef struct{int n_top,n_toe,n_tov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tor_state_t;
static tor_t torps[N],tors[N-2],torvs[N-4],acs[N-6],mks[N-6]; static tor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tor_init(void){if(init)return -1;st.n_top=0;st.n_toe=0;st.n_tov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)torps[i].active=0;for(int i=0;i<N-2;i++)tors[i].active=0;for(int i=0;i<N-4;i++)torvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TOR] Torenia initialized\n");return 0;}
int tor_planning(int t,int c,int a,int b,int d,int e,int y){return add(torps,&st.n_top,&st.t_f1,N,t,c,a,b,d,e,y);}
int tor_execution(int t,int c,int a,int b,int d,int e,int y){return add(tors,&st.n_toe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(torvs,&st.n_tov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tor_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tor_report(void){ps("[TOR] Top: ");pi(st.n_top);ps(" PCS=");pi(st.t_f1);ps("\nToe: ");pi(st.n_toe);ps(" PCS=");pi(st.t_f2);ps("\nTov: ");pi(st.n_tov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tor_state(void){ps("[TOR] Top=");pi(st.n_top);ps(" Toe=");pi(st.n_toe);ps(" Tov=");pi(st.n_tov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Torenia Admin Demo ===\n\n");tor_init();
ps("Torenia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tor_planning(t,c,599+(i*17),588+(i*14),568+(i*10),550+(i*6),2020+(i%5));}
ps("\nTorenia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tor_execution(t,c,588+(i*15),577+(i*12),559+(i*8),546+(i*5),2021+(i%4));}
ps("\nTorenia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tor_evaluation(t,c,580+(i*13),569+(i*10),553+(i*7),542+(i*4),2022+(i%3));}
ps("\nTorenia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tor_accessory(t,c,572+(i*11),563+(i*9),549+(i*6),539+(i*3),2023+(i%2));}
ps("\nTorenia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tor_market(t,c,566+(i*9),557+(i*7),544+(i*5),536+(i*3),2024);}
ps("\n");tor_report();tor_state();ps("\n=== Demo Complete ===\n");return 0;}
