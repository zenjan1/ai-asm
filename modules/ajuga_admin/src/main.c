/* ajuga_admin: Ajuga management technology administration (v1.0)
 * Ajuga planning, ajuga execution, ajuga evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ajug_t;
typedef struct{int n_ajugp,n_ajug,n_ajug2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ajug_state_t;
static ajug_t ajugs[N],ajuge[N-2],ajugv[N-4],ajugac[N-6],ajugm[N-6]; static ajug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ajug_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ajug_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AJUG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ajug_init(void){if(init)return -1;st.n_ajugp=0;st.n_ajug=0;st.n_ajug2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ajugs[i].active=0;for(int i=0;i<N-2;i++)ajuge[i].active=0;for(int i=0;i<N-4;i++)ajugv[i].active=0;for(int i=0;i<N-6;i++)ajugac[i].active=0;for(int i=0;i<N-6;i++)ajugm[i].active=0;init=1;ps("[AJUG] Ajuga initialized\n");return 0;}
int ajug_planning(int t,int c,int a,int b,int d,int e,int y){return add(ajugs,&st.n_ajugp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ajug_execution(int t,int c,int a,int b,int d,int e,int y){return add(ajuge,&st.n_ajug,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ajug_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ajugv,&st.n_ajug2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ajug_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ajugac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ajug_market(int t,int c,int a,int b,int d,int e,int y){return add(ajugm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ajug_report(void){ps("[AJUG] Jgp: ");pi(st.n_ajugp);ps(" PCS=");pi(st.t_f1);ps("\nJge: ");pi(st.n_ajug);ps(" PCS=");pi(st.t_f2);ps("\nJgv: ");pi(st.n_ajug2);ps(" PCS=");pi(st.t_f3);ps("\nJgac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ajug_state(void){ps("[AJUG] Jgp=");pi(st.n_ajugp);ps(" Jge=");pi(st.n_ajug);ps(" Jgv=");pi(st.n_ajug2);ps(" Jgac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ajuga Admin Demo ===\n\n");ajug_init();
ps("Ajuga planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ajug_planning(t,c,1396+(i*17),1385+(i*14),1365+(i*10),1347+(i*6),2020+(i%5));}
ps("\nAjuga execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ajug_execution(t,c,1385+(i*15),1374+(i*12),1356+(i*8),1343+(i*5),2021+(i%4));}
ps("\nAjuga evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ajug_evaluation(t,c,1377+(i*13),1366+(i*10),1350+(i*7),1339+(i*4),2022+(i%3));}
ps("\nAjuga accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ajug_accessory(t,c,1369+(i*11),1360+(i*9),1346+(i*6),1336+(i*3),2023+(i%2));}
ps("\nAjuga marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ajug_market(t,c,1363+(i*9),1354+(i*7),1341+(i*5),1333+(i*3),2024);}
ps("\n");ajug_report();ajug_state();ps("\n=== Demo Complete ===\n");return 0;}
