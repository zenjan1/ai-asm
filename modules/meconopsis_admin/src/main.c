/* meconopsis_admin: Meconopsis management technology administration (v1.0)
 * Meconopsis planning, meconopsis execution, meconopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mcp_t;
typedef struct{int n_mcp,n_mce,n_mcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mcp_state_t;
static mcp_t mcp_ps[N],mcp_ss[N-2],mcp_vs[N-4],mcp_as[N-6],mcp_ms[N-6]; static mcp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mcp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mcp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MCP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mcp_init(void){if(init)return -1;st.n_mcp=0;st.n_mce=0;st.n_mcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcp_ps[i].active=0;for(int i=0;i<N-2;i++)mcp_ss[i].active=0;for(int i=0;i<N-4;i++)mcp_vs[i].active=0;for(int i=0;i<N-6;i++)mcp_as[i].active=0;for(int i=0;i<N-6;i++)mcp_ms[i].active=0;init=1;ps("[MCP] Meconopsis initialized\n");return 0;}
int mcp_planning(int t,int c,int a,int b,int d,int e,int y){return add(mcp_ps,&st.n_mcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mcp_execution(int t,int c,int a,int b,int d,int e,int y){return add(mcp_ss,&st.n_mce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mcp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mcp_vs,&st.n_mcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mcp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mcp_as,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mcp_market(int t,int c,int a,int b,int d,int e,int y){return add(mcp_ms,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mcp_report(void){ps("[MCP] Mcp: ");pi(st.n_mcp);ps(" PCS=");pi(st.t_f1);ps("\nMce: ");pi(st.n_mce);ps(" PCS=");pi(st.t_f2);ps("\nMcv: ");pi(st.n_mcv);ps(" PCS=");pi(st.t_f3);ps("\nMcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mcp_state(void){ps("[MCP] Mcp=");pi(st.n_mcp);ps(" Mce=");pi(st.n_mce);ps(" Mcv=");pi(st.n_mcv);ps(" Mcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Meconopsis Admin Demo ===\n\n");mcp_init();
ps("Meconopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mcp_planning(t,c,836+(i*17),825+(i*14),805+(i*10),787+(i*6),2020+(i%5));}
ps("\nMeconopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mcp_execution(t,c,825+(i*15),814+(i*12),796+(i*8),783+(i*5),2021+(i%4));}
ps("\nMeconopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mcp_evaluation(t,c,817+(i*13),806+(i*10),790+(i*7),779+(i*4),2022+(i%3));}
ps("\nMeconopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcp_accessory(t,c,809+(i*11),800+(i*9),786+(i*6),776+(i*3),2023+(i%2));}
ps("\nMeconopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mcp_market(t,c,803+(i*9),794+(i*7),781+(i*5),773+(i*3),2024);}
ps("\n");mcp_report();mcp_state();ps("\n=== Demo Complete ===\n");return 0;}
