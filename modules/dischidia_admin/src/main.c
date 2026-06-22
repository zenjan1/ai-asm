/* dischidia_admin: Dischidia management technology administration (v1.0)
 * Dischidia planning, dischidia execution, dischidia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dsch_t;
typedef struct{int n_dschp,n_dsche,n_dsch2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} dsch_state_t;
static dsch_t dschps[N],dschss[N-2],dschvss[N-4],dschas[N-6],dschmks[N-6]; static dsch_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(dsch_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dsch_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DSCH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dsch_init(void){if(init)return -1;st.n_dschp=0;st.n_dsche=0;st.n_dsch2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dschps[i].active=0;for(int i=0;i<N-2;i++)dschss[i].active=0;for(int i=0;i<N-4;i++)dschvss[i].active=0;for(int i=0;i<N-6;i++)dschas[i].active=0;for(int i=0;i<N-6;i++)dschmks[i].active=0;init=1;ps("[DSCH] Dischidia initialized\n");return 0;}
int dsch_planning(int t,int c,int a,int b,int d,int e,int y){return add(dschps,&st.n_dschp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dsch_execution(int t,int c,int a,int b,int d,int e,int y){return add(dschss,&st.n_dsche,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dsch_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dschvss,&st.n_dsch2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dsch_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dschas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dsch_market(int t,int c,int a,int b,int d,int e,int y){return add(dschmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dsch_report(void){ps("[DSCH] Disp: ");pi(st.n_dschp);ps(" PCS=");pi(st.t_f1);ps("\nDise: ");pi(st.n_dsche);ps(" PCS=");pi(st.t_f2);ps("\nDisv: ");pi(st.n_dsch2);ps(" PCS=");pi(st.t_f3);ps("\nDisc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dsch_state(void){ps("[DSCH] Disp=");pi(st.n_dschp);ps(" Dise=");pi(st.n_dsche);ps(" Disv=");pi(st.n_dsch2);ps(" Disc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Dischidia Admin Demo ===\n\n");dsch_init();
ps("Dischidia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dsch_planning(t,c,1269+(i*17),1258+(i*14),1238+(i*10),1220+(i*6),2020+(i%5));}
ps("\nDischidia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dsch_execution(t,c,1258+(i*15),1247+(i*12),1229+(i*8),1216+(i*5),2021+(i%4));}
ps("\nDischidia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dsch_evaluation(t,c,1250+(i*13),1239+(i*10),1223+(i*7),1212+(i*4),2022+(i%3));}
ps("\nDischidia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsch_accessory(t,c,1242+(i*11),1233+(i*9),1219+(i*6),1209+(i*3),2023+(i%2));}
ps("\nDischidia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsch_market(t,c,1236+(i*9),1227+(i*7),1214+(i*5),1206+(i*3),2024);}
ps("\n");dsch_report();dsch_state();ps("\n=== Demo Complete ===\n");return 0;}
