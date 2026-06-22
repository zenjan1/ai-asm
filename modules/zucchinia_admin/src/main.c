/* zucchinia_admin: Zucchinia management technology administration (v1.0)
 * Zucchinia planning, zucchinia execution, zucchinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zuc_t;
typedef struct{int n_zucp,n_zuce,n_zuc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zuc_state_t;
static zuc_t zucps[N],zucss[N-2],zucvss[N-4],zucas[N-6],zucmks[N-6]; static zuc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zuc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zuc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZUC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zuc_init(void){if(init)return -1;st.n_zucp=0;st.n_zuce=0;st.n_zuc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zucps[i].active=0;for(int i=0;i<N-2;i++)zucss[i].active=0;for(int i=0;i<N-4;i++)zucvss[i].active=0;for(int i=0;i<N-6;i++)zucas[i].active=0;for(int i=0;i<N-6;i++)zucmks[i].active=0;init=1;ps("[ZUC] Zucchinia initialized\n");return 0;}
int zuc_planning(int t,int c,int a,int b,int d,int e,int y){return add(zucps,&st.n_zucp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zuc_execution(int t,int c,int a,int b,int d,int e,int y){return add(zucss,&st.n_zuce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zuc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zucvss,&st.n_zuc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zuc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zucas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zuc_market(int t,int c,int a,int b,int d,int e,int y){return add(zucmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zuc_report(void){ps("[ZUC] Zucpp: ");pi(st.n_zucp);ps(" PCS=");pi(st.t_f1);ps("\nZucpe: ");pi(st.n_zuce);ps(" PCS=");pi(st.t_f2);ps("\nZucv: ");pi(st.n_zuc2);ps(" PCS=");pi(st.t_f3);ps("\nZucc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zuc_state(void){ps("[ZUC] Zucpp=");pi(st.n_zucp);ps(" Zucpe=");pi(st.n_zuce);ps(" Zucv=");pi(st.n_zuc2);ps(" Zucc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zucchinia Admin Demo ===\n\n");zuc_init();
ps("Zucchinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zuc_planning(t,c,1207+(i*17),1196+(i*14),1176+(i*10),1158+(i*6),2020+(i%5));}
ps("\nZucchinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zuc_execution(t,c,1196+(i*15),1185+(i*12),1167+(i*8),1154+(i*5),2021+(i%4));}
ps("\nZucchinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zuc_evaluation(t,c,1188+(i*13),1177+(i*10),1161+(i*7),1150+(i*4),2022+(i%3));}
ps("\nZucchinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zuc_accessory(t,c,1180+(i*11),1171+(i*9),1157+(i*6),1147+(i*3),2023+(i%2));}
ps("\nZucchinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zuc_market(t,c,1174+(i*9),1165+(i*7),1152+(i*5),1144+(i*3),2024);}
ps("\n");zuc_report();zuc_state();ps("\n=== Demo Complete ===\n");return 0;}
