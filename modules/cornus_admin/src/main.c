/* cornus_admin: Cornus management technology administration (v1.0)
 * Cornus planning, cornus execution, cornus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} corn_t;
typedef struct{int n_cornp,n_corn,n_corn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} corn_state_t;
static corn_t corns[N],corne[N-2],corn2[N-4],cornac[N-6],cornam[N-6]; static corn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(corn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;corn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CORN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int corn_init(void){if(init)return -1;st.n_cornp=0;st.n_corn=0;st.n_corn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)corns[i].active=0;for(int i=0;i<N-2;i++)corne[i].active=0;for(int i=0;i<N-4;i++)corn2[i].active=0;for(int i=0;i<N-6;i++)cornac[i].active=0;for(int i=0;i<N-6;i++)cornam[i].active=0;init=1;ps("[CORN] Cornus initialized\n");return 0;}
int corn_planning(int t,int c,int a,int b,int d,int e,int y){return add(corns,&st.n_cornp,&st.t_f1,N,t,c,a,b,d,e,y);}
int corn_execution(int t,int c,int a,int b,int d,int e,int y){return add(corne,&st.n_corn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int corn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(corn2,&st.n_corn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int corn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cornac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int corn_market(int t,int c,int a,int b,int d,int e,int y){return add(cornam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void corn_report(void){ps("[CORN] Rnp: ");pi(st.n_cornp);ps(" PCS=");pi(st.t_f1);ps("\nRne: ");pi(st.n_corn);ps(" PCS=");pi(st.t_f2);ps("\nRn2: ");pi(st.n_corn2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void corn_state(void){ps("[CORN] Rnp=");pi(st.n_cornp);ps(" Rn=");pi(st.n_corn);ps(" Rn2=");pi(st.n_corn2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cornus Admin Demo ===\n\n");corn_init();
ps("Cornus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;corn_planning(t,c,1480+(i*17),1469+(i*14),1449+(i*10),1431+(i*6),2020+(i%5));}
ps("\nCornus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;corn_execution(t,c,1469+(i*15),1458+(i*12),1440+(i*8),1427+(i*5),2021+(i%4));}
ps("\nCornus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;corn_evaluation(t,c,1461+(i*13),1450+(i*10),1434+(i*7),1423+(i*4),2022+(i%3));}
ps("\nCornus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;corn_accessory(t,c,1453+(i*11),1444+(i*9),1430+(i*6),1420+(i*3),2023+(i%2));}
ps("\nCornus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;corn_market(t,c,1447+(i*9),1438+(i*7),1425+(i*5),1417+(i*3),2024);}
ps("\n");corn_report();corn_state();ps("\n=== Demo Complete ===\n");return 0;}
