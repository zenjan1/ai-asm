/* jacobinia_admin: Jacobinia management technology administration (v1.0)
 * Jacobinia planning, jacobinia execution, jacobinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jd_t;
typedef struct{int n_jcp,n_jce,n_jcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jd_state_t;
static jd_t jcps[N],jcss[N-2],jcvss[N-4],jcas[N-6],jcmks[N-6]; static jd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jcn_init(void){if(init)return -1;st.n_jcp=0;st.n_jce=0;st.n_jcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)jcps[i].active=0;for(int i=0;i<N-2;i++)jcss[i].active=0;for(int i=0;i<N-4;i++)jcvss[i].active=0;for(int i=0;i<N-6;i++)jcas[i].active=0;for(int i=0;i<N-6;i++)jcmks[i].active=0;init=1;ps("[JCN] Jacobinia initialized\n");return 0;}
int jcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(jcps,&st.n_jcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int jcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(jcss,&st.n_jce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(jcvss,&st.n_jcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(jcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jcn_market(int t,int c,int a,int b,int d,int e,int y){return add(jcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jcn_report(void){ps("[JCN] Jcp: ");pi(st.n_jcp);ps(" PCS=");pi(st.t_f1);ps("\nJce: ");pi(st.n_jce);ps(" PCS=");pi(st.t_f2);ps("\nJcv: ");pi(st.n_jcv);ps(" PCS=");pi(st.t_f3);ps("\nJcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jcn_state(void){ps("[JCN] Jcp=");pi(st.n_jcp);ps(" Jce=");pi(st.n_jce);ps(" Jcv=");pi(st.n_jcv);ps(" Jcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Jacobinia Admin Demo ===\n\n");jcn_init();
ps("Jacobinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jcn_planning(t,c,744+(i*17),733+(i*14),713+(i*10),695+(i*6),2020+(i%5));}
ps("\nJacobinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jcn_execution(t,c,733+(i*15),722+(i*12),704+(i*8),691+(i*5),2021+(i%4));}
ps("\nJacobinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jcn_evaluation(t,c,725+(i*13),714+(i*10),698+(i*7),687+(i*4),2022+(i%3));}
ps("\nJacobinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jcn_accessory(t,c,717+(i*11),708+(i*9),694+(i*6),684+(i*3),2023+(i%2));}
ps("\nJacobinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jcn_market(t,c,711+(i*9),702+(i*7),689+(i*5),681+(i*3),2024);}
ps("\n");jcn_report();jcn_state();ps("\n=== Demo Complete ===\n");return 0;}
