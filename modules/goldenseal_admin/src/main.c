/* goldenseal_admin: Goldenseal management technology administration (v1.0)
 * Goldenseal planning, goldenseal execution, goldenseal evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gold_t;
typedef struct{int n_goldp,n_gold,n_gold2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gold_state_t;
static gold_t golds[N],golde[N-2],gold2[N-4],goldac[N-6],goldam[N-6]; static gold_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(gold_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gold_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GOLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gold_init(void){if(init)return -1;st.n_goldp=0;st.n_gold=0;st.n_gold2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)golds[i].active=0;for(int i=0;i<N-2;i++)golde[i].active=0;for(int i=0;i<N-4;i++)gold2[i].active=0;for(int i=0;i<N-6;i++)goldac[i].active=0;for(int i=0;i<N-6;i++)goldam[i].active=0;init=1;ps("[GOLD] Goldenseal initialized\n");return 0;}
int gold_planning(int t,int c,int a,int b,int d,int e,int y){return add(golds,&st.n_goldp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gold_execution(int t,int c,int a,int b,int d,int e,int y){return add(golde,&st.n_gold,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gold_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(gold2,&st.n_gold2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gold_accessory(int t,int c,int a,int b,int d,int e,int y){return add(goldac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gold_market(int t,int c,int a,int b,int d,int e,int y){return add(goldam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gold_report(void){ps("[GOLD] Goldp: ");pi(st.n_goldp);ps(" PCS=");pi(st.t_f1);ps("\nGolde: ");pi(st.n_gold);ps(" PCS=");pi(st.t_f2);ps("\nGold2: ");pi(st.n_gold2);ps(" PCS=");pi(st.t_f3);ps("\nGac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gold_state(void){ps("[GOLD] Goldp=");pi(st.n_goldp);ps(" Gold=");pi(st.n_gold);ps(" Gold2=");pi(st.n_gold2);ps(" Gac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Goldenseal Admin Demo ===\n\n");gold_init();
ps("Goldenseal planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gold_planning(t,c,1519+(i*17),1508+(i*14),1488+(i*10),1470+(i*6),2020+(i%5));}
ps("\nGoldenseal execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gold_execution(t,c,1508+(i*15),1497+(i*12),1479+(i*8),1466+(i*5),2021+(i%4));}
ps("\nGoldenseal evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gold_evaluation(t,c,1500+(i*13),1489+(i*10),1473+(i*7),1462+(i*4),2022+(i%3));}
ps("\nGoldenseal accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gold_accessory(t,c,1492+(i*11),1483+(i*9),1469+(i*6),1459+(i*3),2023+(i%2));}
ps("\nGoldenseal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gold_market(t,c,1486+(i*9),1477+(i*7),1464+(i*5),1456+(i*3),2024);}
ps("\n");gold_report();gold_state();ps("\n=== Demo Complete ===\n");return 0;}
