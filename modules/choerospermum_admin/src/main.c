/* choerospermum_admin: Choerospermum management technology administration (v1.0)
 * Choerospermum planning, choerospermum execution, choerospermum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} choe_t;
typedef struct{int n_choep,n_choe,n_choe2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} choe_state_t;
static choe_t choes[N],choee[N-2],choe2[N-4],choeac[N-6],choeam[N-6]; static choe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(choe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;choe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHOE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int choe_init(void){if(init)return -1;st.n_choep=0;st.n_choe=0;st.n_choe2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)choes[i].active=0;for(int i=0;i<N-2;i++)choee[i].active=0;for(int i=0;i<N-4;i++)choe2[i].active=0;for(int i=0;i<N-6;i++)choeac[i].active=0;for(int i=0;i<N-6;i++)choeam[i].active=0;init=1;ps("[CHOE] Choerospermum initialized\n");return 0;}
int choe_planning(int t,int c,int a,int b,int d,int e,int y){return add(choes,&st.n_choep,&st.t_f1,N,t,c,a,b,d,e,y);}
int choe_execution(int t,int c,int a,int b,int d,int e,int y){return add(choee,&st.n_choe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int choe_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(choe2,&st.n_choe2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int choe_accessory(int t,int c,int a,int b,int d,int e,int y){return add(choeac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int choe_market(int t,int c,int a,int b,int d,int e,int y){return add(choeam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void choe_report(void){ps("[CHOE] Oep: ");pi(st.n_choep);ps(" PCS=");pi(st.t_f1);ps("\nOee: ");pi(st.n_choe);ps(" PCS=");pi(st.t_f2);ps("\nOe2: ");pi(st.n_choe2);ps(" PCS=");pi(st.t_f3);ps("\nOac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void choe_state(void){ps("[CHOE] Oep=");pi(st.n_choep);ps(" Oe=");pi(st.n_choe);ps(" Oe2=");pi(st.n_choe2);ps(" Oac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Choerospermum Admin Demo ===\n\n");choe_init();
ps("Choerospermum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;choe_planning(t,c,1475+(i*17),1464+(i*14),1444+(i*10),1426+(i*6),2020+(i%5));}
ps("\nChoerospermum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;choe_execution(t,c,1464+(i*15),1453+(i*12),1435+(i*8),1422+(i*5),2021+(i%4));}
ps("\nChoerospermum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;choe_evaluation(t,c,1456+(i*13),1445+(i*10),1429+(i*7),1418+(i*4),2022+(i%3));}
ps("\nChoerospermum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;choe_accessory(t,c,1448+(i*11),1439+(i*9),1425+(i*6),1415+(i*3),2023+(i%2));}
ps("\nChoerospermum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;choe_market(t,c,1442+(i*9),1433+(i*7),1420+(i*5),1412+(i*3),2024);}
ps("\n");choe_report();choe_state();ps("\n=== Demo Complete ===\n");return 0;}
