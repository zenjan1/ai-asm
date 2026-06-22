/* carthamus_admin: Carthamus management technology administration (v1.0)
 * Carthamus planning, carthamus execution, carthamus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rth_t;
typedef struct{int n_rthp,n_rthe,n_rth2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rth_state_t;
static rth_t rthps[N],rthss[N-2],rthvss[N-4],rthas[N-6],rthmks[N-6]; static rth_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(rth_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rth_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RTH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rth_init(void){if(init)return -1;st.n_rthp=0;st.n_rthe=0;st.n_rth2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rthps[i].active=0;for(int i=0;i<N-2;i++)rthss[i].active=0;for(int i=0;i<N-4;i++)rthvss[i].active=0;for(int i=0;i<N-6;i++)rthas[i].active=0;for(int i=0;i<N-6;i++)rthmks[i].active=0;init=1;ps("[RTH] Carthamus initialized\n");return 0;}
int rth_planning(int t,int c,int a,int b,int d,int e,int y){return add(rthps,&st.n_rthp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rth_execution(int t,int c,int a,int b,int d,int e,int y){return add(rthss,&st.n_rthe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rth_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rthvss,&st.n_rth2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rth_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rthas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rth_market(int t,int c,int a,int b,int d,int e,int y){return add(rthmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rth_report(void){ps("[RTH] Carpp: ");pi(st.n_rthp);ps(" PCS=");pi(st.t_f1);ps("\nCarpe: ");pi(st.n_rthe);ps(" PCS=");pi(st.t_f2);ps("\nCarv: ");pi(st.n_rth2);ps(" PCS=");pi(st.t_f3);ps("\nCarc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rth_state(void){ps("[RTH] Carpp=");pi(st.n_rthp);ps(" Carpe=");pi(st.n_rthe);ps(" Carv=");pi(st.n_rth2);ps(" Carc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Carthamus Admin Demo ===\n\n");rth_init();
ps("Carthamus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rth_planning(t,c,1228+(i*17),1217+(i*14),1197+(i*10),1179+(i*6),2020+(i%5));}
ps("\nCarthamus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rth_execution(t,c,1217+(i*15),1206+(i*12),1188+(i*8),1175+(i*5),2021+(i%4));}
ps("\nCarthamus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rth_evaluation(t,c,1209+(i*13),1198+(i*10),1182+(i*7),1171+(i*4),2022+(i%3));}
ps("\nCarthamus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rth_accessory(t,c,1201+(i*11),1192+(i*9),1178+(i*6),1168+(i*3),2023+(i%2));}
ps("\nCarthamus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rth_market(t,c,1195+(i*9),1186+(i*7),1173+(i*5),1165+(i*3),2024);}
ps("\n");rth_report();rth_state();ps("\n=== Demo Complete ===\n");return 0;}
