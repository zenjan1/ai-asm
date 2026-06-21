/* lewisia_admin: Lewisia management technology administration (v1.0)
 * Lewisia planning, lewisia execution, lewisia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lew_t;
typedef struct{int n_lwp,n_lwe,n_lwv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lew_state_t;
static lew_t lwps[N],lwss[N-2],lwvss[N-4],lwas[N-6],lwmks[N-6]; static lew_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lew_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lew_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LWS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lws_init(void){if(init)return -1;st.n_lwp=0;st.n_lwe=0;st.n_lwv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lwps[i].active=0;for(int i=0;i<N-2;i++)lwss[i].active=0;for(int i=0;i<N-4;i++)lwvss[i].active=0;for(int i=0;i<N-6;i++)lwas[i].active=0;for(int i=0;i<N-6;i++)lwmks[i].active=0;init=1;ps("[LWS] Lewisia initialized\n");return 0;}
int lws_planning(int t,int c,int a,int b,int d,int e,int y){return add(lwps,&st.n_lwp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lws_execution(int t,int c,int a,int b,int d,int e,int y){return add(lwss,&st.n_lwe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lws_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lwvss,&st.n_lwv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lws_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lwas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lws_market(int t,int c,int a,int b,int d,int e,int y){return add(lwmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lws_report(void){ps("[LWS] Lwp: ");pi(st.n_lwp);ps(" PCS=");pi(st.t_f1);ps("\nLwe: ");pi(st.n_lwe);ps(" PCS=");pi(st.t_f2);ps("\nLwv: ");pi(st.n_lwv);ps(" PCS=");pi(st.t_f3);ps("\nLwc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lws_state(void){ps("[LWS] Lwp=");pi(st.n_lwp);ps(" Lwe=");pi(st.n_lwe);ps(" Lwv=");pi(st.n_lwv);ps(" Lwc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lewisia Admin Demo ===\n\n");lws_init();
ps("Lewisia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lws_planning(t,c,790+(i*17),779+(i*14),759+(i*10),741+(i*6),2020+(i%5));}
ps("\nLewisia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lws_execution(t,c,779+(i*15),768+(i*12),750+(i*8),737+(i*5),2021+(i%4));}
ps("\nLewisia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lws_evaluation(t,c,771+(i*13),760+(i*10),746+(i*7),735+(i*4),2022+(i%3));}
ps("\nLewisia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lws_accessory(t,c,763+(i*11),754+(i*9),740+(i*6),730+(i*3),2023+(i%2));}
ps("\nLewisia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lws_market(t,c,757+(i*9),748+(i*7),735+(i*5),727+(i*3),2024);}
ps("\n");lws_report();lws_state();ps("\n=== Demo Complete ===\n");return 0;}
