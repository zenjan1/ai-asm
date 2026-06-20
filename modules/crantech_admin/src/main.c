/* crantech_admin: Crane technology administration (v1.0)
 * Bridge cranes, gantry cranes, jib cranes, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cra_t;
typedef struct{int n_br,n_ga,n_ji,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cra_state_t;
static cra_t brs[N],gas[N-2],jis[N-4],acs[N-6],mks[N-6]; static cra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cra_init(void){if(init)return -1;st.n_br=0;st.n_ga=0;st.n_ji=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brs[i].active=0;for(int i=0;i<N-2;i++)gas[i].active=0;for(int i=0;i<N-4;i++)jis[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRA] Crantech initialized\n");return 0;}
int cra_bridge(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f1,N,t,c,a,b,d,e,y);}
int cra_gantry(int t,int c,int a,int b,int d,int e,int y){return add(gas,&st.n_ga,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cra_jib(int t,int c,int a,int b,int d,int e,int y){return add(jis,&st.n_ji,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cra_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cra_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cra_report(void){ps("[CRA] Br: ");pi(st.n_br);ps(" PCS=");pi(st.t_f1);ps("\nGa: ");pi(st.n_ga);ps(" PCS=");pi(st.t_f2);ps("\nJi: ");pi(st.n_ji);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cra_state(void){ps("[CRA] Br=");pi(st.n_br);ps(" Ga=");pi(st.n_ga);ps(" Ji=");pi(st.n_ji);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crane Tech Admin Demo ===\n\n");cra_init();
ps("Bridge cranes...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cra_bridge(t,c,253+(i*17),238+(i*14),218+(i*10),200+(i*6),2020+(i%5));}
ps("\nGantry cranes...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cra_gantry(t,c,242+(i*15),228+(i*12),210+(i*8),197+(i*5),2021+(i%4));}
ps("\nJib cranes...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cra_jib(t,c,234+(i*13),220+(i*10),204+(i*7),193+(i*4),2022+(i%3));}
ps("\nCrane accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cra_accessory(t,c,226+(i*11),214+(i*9),200+(i*6),190+(i*3),2023+(i%2));}
ps("\nCrane marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cra_market(t,c,220+(i*9),209+(i*7),196+(i*5),188+(i*3),2024);}
ps("\n");cra_report();cra_state();ps("\n=== Demo Complete ===\n");return 0;}
