/* zornias_admin: Zornias management technology administration (v1.0)
 * Zornias planning, zornias execution, zornias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zorn_t;
typedef struct{int n_zornp,n_zorn,n_zorn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zorn_state_t;
static zorn_t zorns[N],zorne[N-2],zornv[N-4],zornac[N-6],zornm[N-6]; static zorn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zorn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zorn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZORN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zorn_init(void){if(init)return -1;st.n_zornp=0;st.n_zorn=0;st.n_zorn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zorns[i].active=0;for(int i=0;i<N-2;i++)zorne[i].active=0;for(int i=0;i<N-4;i++)zornv[i].active=0;for(int i=0;i<N-6;i++)zornac[i].active=0;for(int i=0;i<N-6;i++)zornm[i].active=0;init=1;ps("[ZORN] Zornias initialized\n");return 0;}
int zorn_planning(int t,int c,int a,int b,int d,int e,int y){return add(zorns,&st.n_zornp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zorn_execution(int t,int c,int a,int b,int d,int e,int y){return add(zorne,&st.n_zorn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zorn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zornv,&st.n_zorn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zorn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zornac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zorn_market(int t,int c,int a,int b,int d,int e,int y){return add(zornm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zorn_report(void){ps("[ZORN] Zornp: ");pi(st.n_zornp);ps(" PCS=");pi(st.t_f1);ps("\nZorne: ");pi(st.n_zorn);ps(" PCS=");pi(st.t_f2);ps("\nZornv: ");pi(st.n_zorn2);ps(" PCS=");pi(st.t_f3);ps("\nZornac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zorn_state(void){ps("[ZORN] Zornp=");pi(st.n_zornp);ps(" Zorne=");pi(st.n_zorn);ps(" Zornv=");pi(st.n_zorn2);ps(" Zornac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zornias Admin Demo ===\n\n");zorn_init();
ps("Zornias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zorn_planning(t,c,1381+(i*17),1370+(i*14),1350+(i*10),1332+(i*6),2020+(i%5));}
ps("\nZornias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zorn_execution(t,c,1370+(i*15),1359+(i*12),1341+(i*8),1328+(i*5),2021+(i%4));}
ps("\nZornias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zorn_evaluation(t,c,1362+(i*13),1351+(i*10),1335+(i*7),1324+(i*4),2022+(i%3));}
ps("\nZornias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zorn_accessory(t,c,1354+(i*11),1345+(i*9),1331+(i*6),1321+(i*3),2023+(i%2));}
ps("\nZornias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zorn_market(t,c,1348+(i*9),1339+(i*7),1326+(i*5),1318+(i*3),2024);}
ps("\n");zorn_report();zorn_state();ps("\n=== Demo Complete ===\n");return 0;}
