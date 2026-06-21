/* pachypodium_admin: Pachypodium management technology administration (v1.0)
 * Pachypodium planning, pachypodium execution, pachypodium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pac_t;
typedef struct{int n_pap,n_pae,n_pav,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pac_state_t;
static pac_t paps[N],paes[N-2],pavs[N-4],acs[N-6],mks[N-6]; static pac_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pac_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pac_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PAC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pac_init(void){if(init)return -1;st.n_pap=0;st.n_pae=0;st.n_pav=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)paps[i].active=0;for(int i=0;i<N-2;i++)paes[i].active=0;for(int i=0;i<N-4;i++)pavs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PAC] Pachypodium initialized\n");return 0;}
int pac_planning(int t,int c,int a,int b,int d,int e,int y){return add(paps,&st.n_pap,&st.t_f1,N,t,c,a,b,d,e,y);}
int pac_execution(int t,int c,int a,int b,int d,int e,int y){return add(paes,&st.n_pae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pac_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pavs,&st.n_pav,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pac_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pac_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pac_report(void){ps("[PAC] Pap: ");pi(st.n_pap);ps(" PCS=");pi(st.t_f1);ps("\nPae: ");pi(st.n_pae);ps(" PCS=");pi(st.t_f2);ps("\nPav: ");pi(st.n_pav);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pac_state(void){ps("[PAC] Pap=");pi(st.n_pap);ps(" Pae=");pi(st.n_pae);ps(" Pav=");pi(st.n_pav);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pachypodium Admin Demo ===\n\n");pac_init();
ps("Pachypodium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pac_planning(t,c,629+(i*17),618+(i*14),598+(i*10),580+(i*6),2020+(i%5));}
ps("\nPachypodium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pac_execution(t,c,618+(i*15),607+(i*12),589+(i*8),576+(i*5),2021+(i%4));}
ps("\nPachypodium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pac_evaluation(t,c,610+(i*13),599+(i*10),583+(i*7),572+(i*4),2022+(i%3));}
ps("\nPachypodium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pac_accessory(t,c,602+(i*11),593+(i*9),579+(i*6),569+(i*3),2023+(i%2));}
ps("\nPachypodium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pac_market(t,c,596+(i*9),587+(i*7),574+(i*5),566+(i*3),2024);}
ps("\n");pac_report();pac_state();ps("\n=== Demo Complete ===\n");return 0;}
