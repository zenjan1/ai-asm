/* agave_admin: Agave management technology administration (v1.0)
 * Agave planning, agave execution, agave evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ag_t;
typedef struct{int n_agp,n_age,n_agv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ag_state_t;
static ag_t agps[N],ages[N-2],agvs[N-4],agacs[N-6],agmks[N-6]; static ag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ag_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ag_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AGV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ag_init(void){if(init)return -1;st.n_agp=0;st.n_age=0;st.n_agv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)agps[i].active=0;for(int i=0;i<N-2;i++)ages[i].active=0;for(int i=0;i<N-4;i++)agvs[i].active=0;for(int i=0;i<N-6;i++)agacs[i].active=0;for(int i=0;i<N-6;i++)agmks[i].active=0;init=1;ps("[AGV] Agave initialized\n");return 0;}
int ag_planning(int t,int c,int a,int b,int d,int e,int y){return add(agps,&st.n_agp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ag_execution(int t,int c,int a,int b,int d,int e,int y){return add(ages,&st.n_age,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ag_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(agvs,&st.n_agv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ag_accessory(int t,int c,int a,int b,int d,int e,int y){return add(agacs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ag_market(int t,int c,int a,int b,int d,int e,int y){return add(agmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ag_report(void){ps("[AGV] Agp: ");pi(st.n_agp);ps(" PCS=");pi(st.t_f1);ps("\nAge: ");pi(st.n_age);ps(" PCS=");pi(st.t_f2);ps("\nAgv: ");pi(st.n_agv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ag_state(void){ps("[AGV] Agp=");pi(st.n_agp);ps(" Age=");pi(st.n_age);ps(" Agv=");pi(st.n_agv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Agave Admin Demo ===\n\n");ag_init();
ps("Agave planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ag_planning(t,c,639+(i*17),628+(i*14),608+(i*10),590+(i*6),2020+(i%5));}
ps("\nAgave execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ag_execution(t,c,628+(i*15),617+(i*12),599+(i*8),586+(i*5),2021+(i%4));}
ps("\nAgave evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ag_evaluation(t,c,620+(i*13),609+(i*10),593+(i*7),582+(i*4),2022+(i%3));}
ps("\nAgave accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ag_accessory(t,c,612+(i*11),603+(i*9),589+(i*6),579+(i*3),2023+(i%2));}
ps("\nAgave marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ag_market(t,c,606+(i*9),597+(i*7),584+(i*5),576+(i*3),2024);}
ps("\n");ag_report();ag_state();ps("\n=== Demo Complete ===\n");return 0;}
