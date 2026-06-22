/* chaenomeles_admin: Chaenomeles management technology administration (v1.0)
 * Chaenomeles planning, chaenomeles execution, chaenomeles evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} chae_t;
typedef struct{int n_chaep,n_chae,n_chae2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} chae_state_t;
static chae_t chaes[N],chaee[N-2],chae2[N-4],chaeac[N-6],chaeam[N-6]; static chae_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(chae_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;chae_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHAE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int chae_init(void){if(init)return -1;st.n_chaep=0;st.n_chae=0;st.n_chae2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chaes[i].active=0;for(int i=0;i<N-2;i++)chaee[i].active=0;for(int i=0;i<N-4;i++)chae2[i].active=0;for(int i=0;i<N-6;i++)chaeac[i].active=0;for(int i=0;i<N-6;i++)chaeam[i].active=0;init=1;ps("[CHAE] Chaenomeles initialized\n");return 0;}
int chae_planning(int t,int c,int a,int b,int d,int e,int y){return add(chaes,&st.n_chaep,&st.t_f1,N,t,c,a,b,d,e,y);}
int chae_execution(int t,int c,int a,int b,int d,int e,int y){return add(chaee,&st.n_chae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int chae_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chae2,&st.n_chae2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int chae_accessory(int t,int c,int a,int b,int d,int e,int y){return add(chaeac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int chae_market(int t,int c,int a,int b,int d,int e,int y){return add(chaeam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void chae_report(void){ps("[CHAE] Haep: ");pi(st.n_chaep);ps(" PCS=");pi(st.t_f1);ps("\nHae: ");pi(st.n_chae);ps(" PCS=");pi(st.t_f2);ps("\nHa2: ");pi(st.n_chae2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void chae_state(void){ps("[CHAE] Haep=");pi(st.n_chaep);ps(" Hae=");pi(st.n_chae);ps(" Ha2=");pi(st.n_chae2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chaenomeles Admin Demo ===\n\n");chae_init();
ps("Chaenomeles planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;chae_planning(t,c,1471+(i*17),1460+(i*14),1440+(i*10),1422+(i*6),2020+(i%5));}
ps("\nChaenomeles execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;chae_execution(t,c,1460+(i*15),1449+(i*12),1431+(i*8),1418+(i*5),2021+(i%4));}
ps("\nChaenomeles evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;chae_evaluation(t,c,1452+(i*13),1441+(i*10),1425+(i*7),1414+(i*4),2022+(i%3));}
ps("\nChaenomeles accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chae_accessory(t,c,1444+(i*11),1435+(i*9),1421+(i*6),1411+(i*3),2023+(i%2));}
ps("\nChaenomeles marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;chae_market(t,c,1438+(i*9),1429+(i*7),1416+(i*5),1408+(i*3),2024);}
ps("\n");chae_report();chae_state();ps("\n=== Demo Complete ===\n");return 0;}
