/* echinopsis_admin: Echinopsis management technology administration (v1.0)
 * Echinopsis planning, echinopsis execution, echinopsis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eps_t;
typedef struct{int n_epsp,n_epse,n_eps2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eps_state_t;
static eps_t epsps[N],epsss[N-2],epsvss[N-4],epsas[N-6],epsmks[N-6]; static eps_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eps_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eps_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EPS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eps_init(void){if(init)return -1;st.n_epsp=0;st.n_epse=0;st.n_eps2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)epsps[i].active=0;for(int i=0;i<N-2;i++)epsss[i].active=0;for(int i=0;i<N-4;i++)epsvss[i].active=0;for(int i=0;i<N-6;i++)epsas[i].active=0;for(int i=0;i<N-6;i++)epsmks[i].active=0;init=1;ps("[EPS] Echinopsis initialized\n");return 0;}
int eps_planning(int t,int c,int a,int b,int d,int e,int y){return add(epsps,&st.n_epsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eps_execution(int t,int c,int a,int b,int d,int e,int y){return add(epsss,&st.n_epse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eps_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(epsvss,&st.n_eps2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eps_accessory(int t,int c,int a,int b,int d,int e,int y){return add(epsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eps_market(int t,int c,int a,int b,int d,int e,int y){return add(epsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eps_report(void){ps("[EPS] Echp: ");pi(st.n_epsp);ps(" PCS=");pi(st.t_f1);ps("\nEche: ");pi(st.n_epse);ps(" PCS=");pi(st.t_f2);ps("\nEchv: ");pi(st.n_eps2);ps(" PCS=");pi(st.t_f3);ps("\nEchc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eps_state(void){ps("[EPS] Echp=");pi(st.n_epsp);ps(" Eche=");pi(st.n_epse);ps(" Echv=");pi(st.n_eps2);ps(" Echc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Echinopsis Admin Demo ===\n\n");eps_init();
ps("Echinopsis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eps_planning(t,c,1272+(i*17),1261+(i*14),1241+(i*10),1223+(i*6),2020+(i%5));}
ps("\nEchinopsis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eps_execution(t,c,1261+(i*15),1250+(i*12),1232+(i*8),1219+(i*5),2021+(i%4));}
ps("\nEchinopsis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eps_evaluation(t,c,1253+(i*13),1242+(i*10),1226+(i*7),1215+(i*4),2022+(i%3));}
ps("\nEchinopsis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eps_accessory(t,c,1245+(i*11),1236+(i*9),1222+(i*6),1212+(i*3),2023+(i%2));}
ps("\nEchinopsis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eps_market(t,c,1239+(i*9),1230+(i*7),1217+(i*5),1209+(i*3),2024);}
ps("\n");eps_report();eps_state();ps("\n=== Demo Complete ===\n");return 0;}
