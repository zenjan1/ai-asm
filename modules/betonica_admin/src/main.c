/* betonica_admin: Betonica management technology administration (v1.0)
 * Betonica planning, betonica execution, betonica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} beto_t;
typedef struct{int n_betop,n_beto,n_beto2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} beto_state_t;
static beto_t betos[N],betoe[N-2],beto2[N-4],betoac[N-6],betoam[N-6]; static beto_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(beto_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;beto_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BETO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int beto_init(void){if(init)return -1;st.n_betop=0;st.n_beto=0;st.n_beto2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)betos[i].active=0;for(int i=0;i<N-2;i++)betoe[i].active=0;for(int i=0;i<N-4;i++)beto2[i].active=0;for(int i=0;i<N-6;i++)betoac[i].active=0;for(int i=0;i<N-6;i++)betoam[i].active=0;init=1;ps("[BETO] Betonica initialized\n");return 0;}
int beto_planning(int t,int c,int a,int b,int d,int e,int y){return add(betos,&st.n_betop,&st.t_f1,N,t,c,a,b,d,e,y);}
int beto_execution(int t,int c,int a,int b,int d,int e,int y){return add(betoe,&st.n_beto,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int beto_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(beto2,&st.n_beto2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int beto_accessory(int t,int c,int a,int b,int d,int e,int y){return add(betoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int beto_market(int t,int c,int a,int b,int d,int e,int y){return add(betoam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void beto_report(void){ps("[BETO] Etop: ");pi(st.n_betop);ps(" PCS=");pi(st.t_f1);ps("\nEtoes: ");pi(st.n_beto);ps(" PCS=");pi(st.t_f2);ps("\nEt2: ");pi(st.n_beto2);ps(" PCS=");pi(st.t_f3);ps("\nEtac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void beto_state(void){ps("[BETO] Etop=");pi(st.n_betop);ps(" Eto=");pi(st.n_beto);ps(" Et2=");pi(st.n_beto2);ps(" Etac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Betonica Admin Demo ===\n\n");beto_init();
ps("Betonica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;beto_planning(t,c,1436+(i*17),1425+(i*14),1405+(i*10),1387+(i*6),2020+(i%5));}
ps("\nBetonica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;beto_execution(t,c,1425+(i*15),1414+(i*12),1396+(i*8),1383+(i*5),2021+(i%4));}
ps("\nBetonica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;beto_evaluation(t,c,1417+(i*13),1406+(i*10),1390+(i*7),1379+(i*4),2022+(i%3));}
ps("\nBetonica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beto_accessory(t,c,1409+(i*11),1400+(i*9),1386+(i*6),1376+(i*3),2023+(i%2));}
ps("\nBetonica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;beto_market(t,c,1403+(i*9),1394+(i*7),1381+(i*5),1373+(i*3),2024);}
ps("\n");beto_report();beto_state();ps("\n=== Demo Complete ===\n");return 0;}
