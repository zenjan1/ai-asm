/* zinniaea_admin: Zinniaea management technology administration (v1.0)
 * Zinniaea planning, zinniaea execution, zinniaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zinn_t;
typedef struct{int n_zinnp,n_zinn,n_zinn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zinn_state_t;
static zinn_t zinns[N],zinne[N-2],zinnv[N-4],zinnac[N-6],zinnm[N-6]; static zinn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zinn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zinn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZINN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zinn_init(void){if(init)return -1;st.n_zinnp=0;st.n_zinn=0;st.n_zinn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zinns[i].active=0;for(int i=0;i<N-2;i++)zinne[i].active=0;for(int i=0;i<N-4;i++)zinnv[i].active=0;for(int i=0;i<N-6;i++)zinnac[i].active=0;for(int i=0;i<N-6;i++)zinnm[i].active=0;init=1;ps("[ZINN] Zinniaea initialized\n");return 0;}
int zinn_planning(int t,int c,int a,int b,int d,int e,int y){return add(zinns,&st.n_zinnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zinn_execution(int t,int c,int a,int b,int d,int e,int y){return add(zinne,&st.n_zinn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zinn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zinnv,&st.n_zinn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zinn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zinnac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zinn_market(int t,int c,int a,int b,int d,int e,int y){return add(zinnm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zinn_report(void){ps("[ZINN] Zinnp: ");pi(st.n_zinnp);ps(" PCS=");pi(st.t_f1);ps("\nZinne: ");pi(st.n_zinn);ps(" PCS=");pi(st.t_f2);ps("\nZinnv: ");pi(st.n_zinn2);ps(" PCS=");pi(st.t_f3);ps("\nZinnac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zinn_state(void){ps("[ZINN] Zinnp=");pi(st.n_zinnp);ps(" Zinne=");pi(st.n_zinn);ps(" Zinnv=");pi(st.n_zinn2);ps(" Zinnac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zinniaea Admin Demo ===\n\n");zinn_init();
ps("Zinniaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zinn_planning(t,c,1377+(i*17),1366+(i*14),1346+(i*10),1328+(i*6),2020+(i%5));}
ps("\nZinniaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zinn_execution(t,c,1366+(i*15),1355+(i*12),1337+(i*8),1324+(i*5),2021+(i%4));}
ps("\nZinniaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zinn_evaluation(t,c,1358+(i*13),1347+(i*10),1331+(i*7),1320+(i*4),2022+(i%3));}
ps("\nZinniaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zinn_accessory(t,c,1350+(i*11),1341+(i*9),1327+(i*6),1317+(i*3),2023+(i%2));}
ps("\nZinniaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zinn_market(t,c,1344+(i*9),1335+(i*7),1322+(i*5),1314+(i*3),2024);}
ps("\n");zinn_report();zinn_state();ps("\n=== Demo Complete ===\n");return 0;}
