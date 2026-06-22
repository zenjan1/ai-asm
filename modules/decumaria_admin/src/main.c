/* decumaria_admin: Decumaria management technology administration (v1.0)
 * Decumaria planning, decumaria execution, decumaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} decu_t;
typedef struct{int n_decup,n_decu,n_decu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} decu_state_t;
static decu_t decus[N],decue[N-2],decu2[N-4],decuac[N-6],decuam[N-6]; static decu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(decu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;decu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DECU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int decu_init(void){if(init)return -1;st.n_decup=0;st.n_decu=0;st.n_decu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)decus[i].active=0;for(int i=0;i<N-2;i++)decue[i].active=0;for(int i=0;i<N-4;i++)decu2[i].active=0;for(int i=0;i<N-6;i++)decuac[i].active=0;for(int i=0;i<N-6;i++)decuam[i].active=0;init=1;ps("[DECU] Decumaria initialized\n");return 0;}
int decu_planning(int t,int c,int a,int b,int d,int e,int y){return add(decus,&st.n_decup,&st.t_f1,N,t,c,a,b,d,e,y);}
int decu_execution(int t,int c,int a,int b,int d,int e,int y){return add(decue,&st.n_decu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int decu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(decu2,&st.n_decu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int decu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(decuac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int decu_market(int t,int c,int a,int b,int d,int e,int y){return add(decuam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void decu_report(void){ps("[DECU] Cup: ");pi(st.n_decup);ps(" PCS=");pi(st.t_f1);ps("\nCue: ");pi(st.n_decu);ps(" PCS=");pi(st.t_f2);ps("\nCu2: ");pi(st.n_decu2);ps(" PCS=");pi(st.t_f3);ps("\nCac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void decu_state(void){ps("[DECU] Cup=");pi(st.n_decup);ps(" Cu=");pi(st.n_decu);ps(" Cu2=");pi(st.n_decu2);ps(" Cac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Decumaria Admin Demo ===\n\n");decu_init();
ps("Decumaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;decu_planning(t,c,1492+(i*17),1481+(i*14),1461+(i*10),1443+(i*6),2020+(i%5));}
ps("\nDecumaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;decu_execution(t,c,1481+(i*15),1470+(i*12),1452+(i*8),1439+(i*5),2021+(i%4));}
ps("\nDecumaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;decu_evaluation(t,c,1473+(i*13),1462+(i*10),1446+(i*7),1435+(i*4),2022+(i%3));}
ps("\nDecumaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;decu_accessory(t,c,1465+(i*11),1456+(i*9),1442+(i*6),1432+(i*3),2023+(i%2));}
ps("\nDecumaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;decu_market(t,c,1459+(i*9),1450+(i*7),1437+(i*5),1429+(i*3),2024);}
ps("\n");decu_report();decu_state();ps("\n=== Demo Complete ===\n");return 0;}
