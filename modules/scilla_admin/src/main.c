/* scilla_admin: Scilla management technology administration (v1.0)
 * Scilla planning, scilla execution, scilla evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sc_t;
typedef struct{int n_scp,n_sce,n_scv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sc_state_t;
static sc_t scps[N],scss[N-2],scvss[N-4],scas[N-6],scmks[N-6]; static sc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SCL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int scl_init(void){if(init)return -1;st.n_scp=0;st.n_sce=0;st.n_scv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)scps[i].active=0;for(int i=0;i<N-2;i++)scss[i].active=0;for(int i=0;i<N-4;i++)scvss[i].active=0;for(int i=0;i<N-6;i++)scas[i].active=0;for(int i=0;i<N-6;i++)scmks[i].active=0;init=1;ps("[SCL] Scilla initialized\n");return 0;}
int scl_planning(int t,int c,int a,int b,int d,int e,int y){return add(scps,&st.n_scp,&st.t_f1,N,t,c,a,b,d,e,y);}
int scl_execution(int t,int c,int a,int b,int d,int e,int y){return add(scss,&st.n_sce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int scl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(scvss,&st.n_scv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int scl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(scas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int scl_market(int t,int c,int a,int b,int d,int e,int y){return add(scmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void scl_report(void){ps("[SCL] Scp: ");pi(st.n_scp);ps(" PCS=");pi(st.t_f1);ps("\nSce: ");pi(st.n_sce);ps(" PCS=");pi(st.t_f2);ps("\nScv: ");pi(st.n_scv);ps(" PCS=");pi(st.t_f3);ps("\nScc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void scl_state(void){ps("[SCL] Scp=");pi(st.n_scp);ps(" Sce=");pi(st.n_sce);ps(" Scv=");pi(st.n_scv);ps(" Scc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Scilla Admin Demo ===\n\n");scl_init();
ps("Scilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;scl_planning(t,c,736+(i*17),725+(i*14),705+(i*10),687+(i*6),2020+(i%5));}
ps("\nScilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;scl_execution(t,c,725+(i*15),714+(i*12),696+(i*8),683+(i*5),2021+(i%4));}
ps("\nScilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;scl_evaluation(t,c,717+(i*13),706+(i*10),690+(i*7),679+(i*4),2022+(i%3));}
ps("\nScilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;scl_accessory(t,c,709+(i*11),700+(i*9),686+(i*6),676+(i*3),2023+(i%2));}
ps("\nScilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;scl_market(t,c,703+(i*9),694+(i*7),681+(i*5),673+(i*3),2024);}
ps("\n");scl_report();scl_state();ps("\n=== Demo Complete ===\n");return 0;}
