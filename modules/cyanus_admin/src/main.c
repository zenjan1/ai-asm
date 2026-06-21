/* cyanus_admin: Cyanus management technology administration (v1.0)
 * Cyanus planning, cyanus execution, cyanus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cya_t;
typedef struct{int n_cyap,n_cyae,n_cya2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cya_state_t;
static cya_t cyaps[N],cyass[N-2],cyavss[N-4],cyas[N-6],cyamks[N-6]; static cya_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cya_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cya_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cya_init(void){if(init)return -1;st.n_cyap=0;st.n_cyae=0;st.n_cya2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cyaps[i].active=0;for(int i=0;i<N-2;i++)cyass[i].active=0;for(int i=0;i<N-4;i++)cyavss[i].active=0;for(int i=0;i<N-6;i++)cyas[i].active=0;for(int i=0;i<N-6;i++)cyamks[i].active=0;init=1;ps("[CYA] Cyanus initialized\n");return 0;}
int cya_planning(int t,int c,int a,int b,int d,int e,int y){return add(cyaps,&st.n_cyap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cya_execution(int t,int c,int a,int b,int d,int e,int y){return add(cyass,&st.n_cyae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cya_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cyavss,&st.n_cya2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cya_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cyas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cya_market(int t,int c,int a,int b,int d,int e,int y){return add(cyamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cya_report(void){ps("[CYA] Cyapp: ");pi(st.n_cyap);ps(" PCS=");pi(st.t_f1);ps("\nCyae: ");pi(st.n_cyae);ps(" PCS=");pi(st.t_f2);ps("\nCyav: ");pi(st.n_cya2);ps(" PCS=");pi(st.t_f3);ps("\nCyac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cya_state(void){ps("[CYA] Cyapp=");pi(st.n_cyap);ps(" Cyae=");pi(st.n_cyae);ps(" Cyav=");pi(st.n_cya2);ps(" Cyac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyanus Admin Demo ===\n\n");cya_init();
ps("Cyanus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cya_planning(t,c,990+(i*17),979+(i*14),959+(i*10),941+(i*6),2020+(i%5));}
ps("\nCyanus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cya_execution(t,c,979+(i*15),968+(i*12),950+(i*8),937+(i*5),2021+(i%4));}
ps("\nCyanus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cya_evaluation(t,c,971+(i*13),960+(i*10),944+(i*7),933+(i*4),2022+(i%3));}
ps("\nCyanus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cya_accessory(t,c,963+(i*11),954+(i*9),940+(i*6),930+(i*3),2023+(i%2));}
ps("\nCyanus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cya_market(t,c,957+(i*9),948+(i*7),935+(i*5),927+(i*3),2024);}
ps("\n");cya_report();cya_state();ps("\n=== Demo Complete ===\n");return 0;}
