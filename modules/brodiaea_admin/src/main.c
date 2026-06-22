/* brodiaea_admin: Brodiaea management technology administration (v1.0)
 * Brodiaea planning, brodiaea execution, brodiaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brod_t;
typedef struct{int n_brodp,n_brod,n_brod2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brod_state_t;
static brod_t brods[N],brode[N-2],brod2[N-4],brodac[N-6],brodam[N-6]; static brod_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(brod_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brod_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BROD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brod_init(void){if(init)return -1;st.n_brodp=0;st.n_brod=0;st.n_brod2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brods[i].active=0;for(int i=0;i<N-2;i++)brode[i].active=0;for(int i=0;i<N-4;i++)brod2[i].active=0;for(int i=0;i<N-6;i++)brodac[i].active=0;for(int i=0;i<N-6;i++)brodam[i].active=0;init=1;ps("[BROD] Brodiaea initialized\n");return 0;}
int brod_planning(int t,int c,int a,int b,int d,int e,int y){return add(brods,&st.n_brodp,&st.t_f1,N,t,c,a,b,d,e,y);}
int brod_execution(int t,int c,int a,int b,int d,int e,int y){return add(brode,&st.n_brod,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brod_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(brod2,&st.n_brod2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brod_accessory(int t,int c,int a,int b,int d,int e,int y){return add(brodac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brod_market(int t,int c,int a,int b,int d,int e,int y){return add(brodam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brod_report(void){ps("[BROD] Op: ");pi(st.n_brodp);ps(" PCS=");pi(st.t_f1);ps("\nOde: ");pi(st.n_brod);ps(" PCS=");pi(st.t_f2);ps("\nOd2: ");pi(st.n_brod2);ps(" PCS=");pi(st.t_f3);ps("\nOac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brod_state(void){ps("[BROD] Op=");pi(st.n_brodp);ps(" Od=");pi(st.n_brod);ps(" Od2=");pi(st.n_brod2);ps(" Oac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Brodiaea Admin Demo ===\n\n");brod_init();
ps("Brodiaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brod_planning(t,c,1444+(i*17),1433+(i*14),1413+(i*10),1395+(i*6),2020+(i%5));}
ps("\nBrodiaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brod_execution(t,c,1433+(i*15),1422+(i*12),1404+(i*8),1391+(i*5),2021+(i%4));}
ps("\nBrodiaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brod_evaluation(t,c,1425+(i*13),1414+(i*10),1398+(i*7),1387+(i*4),2022+(i%3));}
ps("\nBrodiaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brod_accessory(t,c,1417+(i*11),1408+(i*9),1394+(i*6),1384+(i*3),2023+(i%2));}
ps("\nBrodiaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brod_market(t,c,1411+(i*9),1402+(i*7),1389+(i*5),1381+(i*3),2024);}
ps("\n");brod_report();brod_state();ps("\n=== Demo Complete ===\n");return 0;}
