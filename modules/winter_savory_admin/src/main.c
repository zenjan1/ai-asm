/* winter_savory_admin: Winter savory management technology administration (v1.0)
 * Winter savory planning, winter savory execution, winter savory evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ws_t;
typedef struct{int n_wsp,n_wse,n_wsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ws_state_t;
static ws_t wsps[N],wsss[N-2],wsvss[N-4],wsas[N-6],wsmks[N-6]; static ws_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ws_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ws_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WSV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ws_init(void){if(init)return -1;st.n_wsp=0;st.n_wse=0;st.n_wsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wsps[i].active=0;for(int i=0;i<N-2;i++)wsss[i].active=0;for(int i=0;i<N-4;i++)wsvss[i].active=0;for(int i=0;i<N-6;i++)wsas[i].active=0;for(int i=0;i<N-6;i++)wsmks[i].active=0;init=1;ps("[WSV] Winter savory initialized\n");return 0;}
int ws_planning(int t,int c,int a,int b,int d,int e,int y){return add(wsps,&st.n_wsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ws_execution(int t,int c,int a,int b,int d,int e,int y){return add(wsss,&st.n_wse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ws_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wsvss,&st.n_wsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ws_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ws_market(int t,int c,int a,int b,int d,int e,int y){return add(wsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ws_report(void){ps("[WSV] Wsp: ");pi(st.n_wsp);ps(" PCS=");pi(st.t_f1);ps("\nWse: ");pi(st.n_wse);ps(" PCS=");pi(st.t_f2);ps("\nWsv: ");pi(st.n_wsv);ps(" PCS=");pi(st.t_f3);ps("\nWsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ws_state(void){ps("[WSV] Wsp=");pi(st.n_wsp);ps(" Wse=");pi(st.n_wse);ps(" Wsv=");pi(st.n_wsv);ps(" Wsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Winter Savory Admin Demo ===\n\n");ws_init();
ps("Winter savory planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ws_planning(t,c,714+(i*17),703+(i*14),683+(i*10),665+(i*6),2020+(i%5));}
ps("\nWinter savory execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ws_execution(t,c,703+(i*15),692+(i*12),674+(i*8),661+(i*5),2021+(i%4));}
ps("\nWinter savory evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ws_evaluation(t,c,695+(i*13),684+(i*10),668+(i*7),657+(i*4),2022+(i%3));}
ps("\nWinter savory accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ws_accessory(t,c,687+(i*11),678+(i*9),664+(i*6),654+(i*3),2023+(i%2));}
ps("\nWinter savory marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ws_market(t,c,681+(i*9),672+(i*7),659+(i*5),651+(i*3),2024);}
ps("\n");ws_report();ws_state();ps("\n=== Demo Complete ===\n");return 0;}
