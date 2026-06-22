/* aurinia_admin: Aurinia management technology administration (v1.0)
 * Aurinia planning, aurinia execution, aurinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} auri_t;
typedef struct{int n_aurip,n_auri,n_auri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} auri_state_t;
static auri_t auris[N],aurie[N-2],auri2[N-4],auriac[N-6],auriam[N-6]; static auri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(auri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;auri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AURI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int auri_init(void){if(init)return -1;st.n_aurip=0;st.n_auri=0;st.n_auri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)auris[i].active=0;for(int i=0;i<N-2;i++)aurie[i].active=0;for(int i=0;i<N-4;i++)auri2[i].active=0;for(int i=0;i<N-6;i++)auriac[i].active=0;for(int i=0;i<N-6;i++)auriam[i].active=0;init=1;ps("[AURI] Aurinia initialized\n");return 0;}
int auri_planning(int t,int c,int a,int b,int d,int e,int y){return add(auris,&st.n_aurip,&st.t_f1,N,t,c,a,b,d,e,y);}
int auri_execution(int t,int c,int a,int b,int d,int e,int y){return add(aurie,&st.n_auri,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int auri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(auri2,&st.n_auri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int auri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(auriac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int auri_market(int t,int c,int a,int b,int d,int e,int y){return add(auriam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void auri_report(void){ps("[AURI] Urip: ");pi(st.n_aurip);ps(" PCS=");pi(st.t_f1);ps("\nUrie: ");pi(st.n_auri);ps(" PCS=");pi(st.t_f2);ps("\nUri2: ");pi(st.n_auri2);ps(" PCS=");pi(st.t_f3);ps("\nUriac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void auri_state(void){ps("[AURI] Urip=");pi(st.n_aurip);ps(" Urie=");pi(st.n_auri);ps(" Uri2=");pi(st.n_auri2);ps(" Uriac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aurinia Admin Demo ===\n\n");auri_init();
ps("Aurinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;auri_planning(t,c,1423+(i*17),1412+(i*14),1392+(i*10),1374+(i*6),2020+(i%5));}
ps("\nAurinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;auri_execution(t,c,1412+(i*15),1401+(i*12),1383+(i*8),1370+(i*5),2021+(i%4));}
ps("\nAurinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;auri_evaluation(t,c,1404+(i*13),1393+(i*10),1377+(i*7),1366+(i*4),2022+(i%3));}
ps("\nAurinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;auri_accessory(t,c,1396+(i*11),1387+(i*9),1373+(i*6),1363+(i*3),2023+(i%2));}
ps("\nAurinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;auri_market(t,c,1390+(i*9),1381+(i*7),1368+(i*5),1360+(i*3),2024);}
ps("\n");auri_report();auri_state();ps("\n=== Demo Complete ===\n");return 0;}
