/* juncus_admin: Juncus management technology administration (v1.0)
 * Juncus planning, juncus execution, juncus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} junc_t;
typedef struct{int n_juncp,n_junc,n_junc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} junc_state_t;
static junc_t junccs[N],junc_e[N-2],junc2[N-4],juncac[N-6],juncam[N-6]; static junc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(junc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;junc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JUNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int junc_init(void){if(init)return -1;st.n_juncp=0;st.n_junc=0;st.n_junc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)junccs[i].active=0;for(int i=0;i<N-2;i++)junc_e[i].active=0;for(int i=0;i<N-4;i++)junc2[i].active=0;for(int i=0;i<N-6;i++)juncac[i].active=0;for(int i=0;i<N-6;i++)juncam[i].active=0;init=1;ps("[JUNC] Juncus initialized\n");return 0;}
int junc_planning(int t,int c,int a,int b,int d,int e,int y){return add(junccs,&st.n_juncp,&st.t_f1,N,t,c,a,b,d,e,y);}
int junc_execution(int t,int c,int a,int b,int d,int e,int y){return add(junc_e,&st.n_junc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int junc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(junc2,&st.n_junc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int junc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(juncac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int junc_market(int t,int c,int a,int b,int d,int e,int y){return add(juncam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void junc_report(void){ps("[JUNC] Juncp: ");pi(st.n_juncp);ps(" PCS=");pi(st.t_f1);ps("\nJunc: ");pi(st.n_junc);ps(" PCS=");pi(st.t_f2);ps("\nJunc2: ");pi(st.n_junc2);ps(" PCS=");pi(st.t_f3);ps("\nJac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void junc_state(void){ps("[JUNC] Juncp=");pi(st.n_juncp);ps(" Junc=");pi(st.n_junc);ps(" Junc2=");pi(st.n_junc2);ps(" Jac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Juncus Admin Demo ===\n\n");junc_init();
ps("Juncus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;junc_planning(t,c,1547+(i*17),1536+(i*14),1516+(i*10),1498+(i*6),2020+(i%5));}
ps("\nJuncus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;junc_execution(t,c,1536+(i*15),1525+(i*12),1507+(i*8),1494+(i*5),2021+(i%4));}
ps("\nJuncus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;junc_evaluation(t,c,1528+(i*13),1517+(i*10),1501+(i*7),1490+(i*4),2022+(i%3));}
ps("\nJuncus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;junc_accessory(t,c,1520+(i*11),1511+(i*9),1497+(i*6),1487+(i*3),2023+(i%2));}
ps("\nJuncus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;junc_market(t,c,1514+(i*9),1505+(i*7),1492+(i*5),1484+(i*3),2024);}
ps("\n");junc_report();junc_state();ps("\n=== Demo Complete ===\n");return 0;}
