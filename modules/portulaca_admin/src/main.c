/* portulaca_admin: Portulaca management technology administration (v1.0)
 * Portulaca planning, portulaca execution, portulaca evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} por_t;
typedef struct{int n_pop,n_poe,n_pov,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} por_state_t;
static por_t porps[N],pors[N-2],porvs[N-4],acs[N-6],mks[N-6]; static por_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(por_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;por_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[POR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int por_init(void){if(init)return -1;st.n_pop=0;st.n_poe=0;st.n_pov=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)porps[i].active=0;for(int i=0;i<N-2;i++)pors[i].active=0;for(int i=0;i<N-4;i++)porvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[POR] Portulaca initialized\n");return 0;}
int por_planning(int t,int c,int a,int b,int d,int e,int y){return add(porps,&st.n_pop,&st.t_f1,N,t,c,a,b,d,e,y);}
int por_execution(int t,int c,int a,int b,int d,int e,int y){return add(pors,&st.n_poe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int por_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(porvs,&st.n_pov,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int por_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int por_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void por_report(void){ps("[POR] Pop: ");pi(st.n_pop);ps(" PCS=");pi(st.t_f1);ps("\nPoe: ");pi(st.n_poe);ps(" PCS=");pi(st.t_f2);ps("\nPov: ");pi(st.n_pov);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void por_state(void){ps("[POR] Pop=");pi(st.n_pop);ps(" Poe=");pi(st.n_poe);ps(" Pov=");pi(st.n_pov);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Portulaca Admin Demo ===\n\n");por_init();
ps("Portulaca planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;por_planning(t,c,600+(i*17),589+(i*14),569+(i*10),551+(i*6),2020+(i%5));}
ps("\nPortulaca execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;por_execution(t,c,589+(i*15),578+(i*12),560+(i*8),547+(i*5),2021+(i%4));}
ps("\nPortulaca evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;por_evaluation(t,c,581+(i*13),570+(i*10),554+(i*7),543+(i*4),2022+(i%3));}
ps("\nPortulaca accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;por_accessory(t,c,573+(i*11),564+(i*9),550+(i*6),540+(i*3),2023+(i%2));}
ps("\nPortulaca marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;por_market(t,c,567+(i*9),558+(i*7),545+(i*5),537+(i*3),2024);}
ps("\n");por_report();por_state();ps("\n=== Demo Complete ===\n");return 0;}
