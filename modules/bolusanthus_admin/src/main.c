/* bolusanthus_admin: Bolusanthus management technology administration (v1.0)
 * Bolusanthus planning, bolusanthus execution, bolusanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bolu_t;
typedef struct{int n_bolup,n_bolu,n_bolu2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bolu_state_t;
static bolu_t bolus[N],bolue[N-2],bolu2[N-4],boluac[N-6],boluam[N-6]; static bolu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bolu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bolu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BOLU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bolu_init(void){if(init)return -1;st.n_bolup=0;st.n_bolu=0;st.n_bolu2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bolus[i].active=0;for(int i=0;i<N-2;i++)bolue[i].active=0;for(int i=0;i<N-4;i++)bolu2[i].active=0;for(int i=0;i<N-6;i++)boluac[i].active=0;for(int i=0;i<N-6;i++)boluam[i].active=0;init=1;ps("[BOLU] Bolusanthus initialized\n");return 0;}
int bolu_planning(int t,int c,int a,int b,int d,int e,int y){return add(bolus,&st.n_bolup,&st.t_f1,N,t,c,a,b,d,e,y);}
int bolu_execution(int t,int c,int a,int b,int d,int e,int y){return add(bolue,&st.n_bolu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bolu_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bolu2,&st.n_bolu2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bolu_accessory(int t,int c,int a,int b,int d,int e,int y){return add(boluac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bolu_market(int t,int c,int a,int b,int d,int e,int y){return add(boluam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bolu_report(void){ps("[BOLU] Lup: ");pi(st.n_bolup);ps(" PCS=");pi(st.t_f1);ps("\nLue: ");pi(st.n_bolu);ps(" PCS=");pi(st.t_f2);ps("\nLu2: ");pi(st.n_bolu2);ps(" PCS=");pi(st.t_f3);ps("\nLuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bolu_state(void){ps("[BOLU] Lup=");pi(st.n_bolup);ps(" Lu=");pi(st.n_bolu);ps(" Lu2=");pi(st.n_bolu2);ps(" Luac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bolusanthus Admin Demo ===\n\n");bolu_init();
ps("Bolusanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bolu_planning(t,c,1439+(i*17),1428+(i*14),1408+(i*10),1390+(i*6),2020+(i%5));}
ps("\nBolusanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bolu_execution(t,c,1428+(i*15),1417+(i*12),1399+(i*8),1386+(i*5),2021+(i%4));}
ps("\nBolusanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bolu_evaluation(t,c,1420+(i*13),1409+(i*10),1393+(i*7),1382+(i*4),2022+(i%3));}
ps("\nBolusanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bolu_accessory(t,c,1412+(i*11),1403+(i*9),1389+(i*6),1379+(i*3),2023+(i%2));}
ps("\nBolusanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bolu_market(t,c,1406+(i*9),1397+(i*7),1384+(i*5),1376+(i*3),2024);}
ps("\n");bolu_report();bolu_state();ps("\n=== Demo Complete ===\n");return 0;}
