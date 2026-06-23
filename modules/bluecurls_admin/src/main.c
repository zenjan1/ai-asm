/* bluecurls_admin: Bluecurls management technology administration (v1.0)
 * Bluecurls planning, bluecurls execution, bluecurls evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} curl_t;
typedef struct{int n_planning,n_execution,n_evaluation,n_accessory,n_market,t_f1,t_f2,t_f3,t_f4,t_f5;} curl_state_t;
static curl_t curlp[N],curlx[N-2],curl2[N-4],curlac[N-6],curlm[N-6]; static curl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(curl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;curl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CURL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int curl_init(void){if(init)return -1;st.n_planning=0;st.n_execution=0;st.n_evaluation=0;st.n_accessory=0;st.n_market=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)curlp[i].active=0;for(int i=0;i<N-2;i++)curlx[i].active=0;for(int i=0;i<N-4;i++)curl2[i].active=0;for(int i=0;i<N-6;i++)curlac[i].active=0;for(int i=0;i<N-6;i++)curlm[i].active=0;init=1;ps("[CURL] Bluecurls initialized\n");return 0;}
int curl_planning(int t,int c,int a,int b,int d,int e,int y){return add(curlp,&st.n_planning,&st.t_f1,N,t,c,a,b,d,e,y);}
int curl_execution(int t,int c,int a,int b,int d,int e,int y){return add(curlx,&st.n_execution,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int curl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(curl2,&st.n_evaluation,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int curl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(curlac,&st.n_accessory,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int curl_market(int t,int c,int a,int b,int d,int e,int y){return add(curlm,&st.n_market,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void curl_report(void){ps("[CURL] Planning: ");pi(st.n_planning);ps(" PCS=");pi(st.t_f1);ps("\nExecution: ");pi(st.n_execution);ps(" PCS=");pi(st.t_f2);ps("\nEvaluation: ");pi(st.n_evaluation);ps(" PCS=");pi(st.t_f3);ps("\nAccessory: ");pi(st.n_accessory);ps(" PCS=");pi(st.t_f4);ps("\nMarket: ");pi(st.n_market);ps(" USD=");pi(st.t_f5);ps("\n");}
void curl_state(void){ps("[CURL] P=");pi(st.n_planning);ps(" E=");pi(st.n_execution);ps(" V=");pi(st.n_evaluation);ps(" A=");pi(st.n_accessory);ps(" M=");pi(st.n_market);ps("\n");}
int main(void){
ps("=== Bluecurls Admin Demo ===\n\n");curl_init();
ps("Bluecurls planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;curl_planning(t,c,1804+(i*17),1793+(i*14),1773+(i*10),1755+(i*6),2020+(i%5));}
ps("\nBluecurls execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;curl_execution(t,c,1793+(i*15),1782+(i*12),1764+(i*8),1751+(i*5),2021+(i%4));}
ps("\nBluecurls evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;curl_evaluation(t,c,1785+(i*13),1774+(i*10),1758+(i*7),1747+(i*4),2022+(i%3));}
ps("\nBluecurls accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;curl_accessory(t,c,1777+(i*11),1768+(i*9),1754+(i*6),1744+(i*3),2023+(i%2));}
ps("\nBluecurls marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;curl_market(t,c,1771+(i*9),1762+(i*7),1749+(i*5),1741+(i*3),2024);}
ps("\n");curl_report();curl_state();ps("\n=== Demo Complete ===\n");return 0;}
