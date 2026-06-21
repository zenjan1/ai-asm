/* pendulina_admin: Pendulina management technology administration (v1.0)
 * Pendulina planning, pendulina execution, pendulina evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pd_t;
typedef struct{int n_pdp,n_pde,n_pdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pd_state_t;
static pd_t pdps[N],pdss[N-2],pdvss[N-4],pdas[N-6],pdmks[N-6]; static pd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PDN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pdn_init(void){if(init)return -1;st.n_pdp=0;st.n_pde=0;st.n_pdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pdps[i].active=0;for(int i=0;i<N-2;i++)pdss[i].active=0;for(int i=0;i<N-4;i++)pdvss[i].active=0;for(int i=0;i<N-6;i++)pdas[i].active=0;for(int i=0;i<N-6;i++)pdmks[i].active=0;init=1;ps("[PDN] Pendulina initialized\n");return 0;}
int pdn_planning(int t,int c,int a,int b,int d,int e,int y){return add(pdps,&st.n_pdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pdn_execution(int t,int c,int a,int b,int d,int e,int y){return add(pdss,&st.n_pde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pdn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pdvss,&st.n_pdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pdn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pdn_market(int t,int c,int a,int b,int d,int e,int y){return add(pdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pdn_report(void){ps("[PDN] Pdp: ");pi(st.n_pdp);ps(" PCS=");pi(st.t_f1);ps("\nPde: ");pi(st.n_pde);ps(" PCS=");pi(st.t_f2);ps("\nPdv: ");pi(st.n_pdv);ps(" PCS=");pi(st.t_f3);ps("\nPdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pdn_state(void){ps("[PDN] Pdp=");pi(st.n_pdp);ps(" Pde=");pi(st.n_pde);ps(" Pdv=");pi(st.n_pdv);ps(" Pdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pendulina Admin Demo ===\n\n");pdn_init();
ps("Pendulina planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pdn_planning(t,c,795+(i*17),784+(i*14),764+(i*10),746+(i*6),2020+(i%5));}
ps("\nPendulina execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pdn_execution(t,c,784+(i*15),773+(i*12),755+(i*8),742+(i*5),2021+(i%4));}
ps("\nPendulina evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pdn_evaluation(t,c,776+(i*13),765+(i*10),751+(i*7),740+(i*4),2022+(i%3));}
ps("\nPendulina accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pdn_accessory(t,c,768+(i*11),759+(i*9),745+(i*6),735+(i*3),2023+(i%2));}
ps("\nPendulina marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pdn_market(t,c,762+(i*9),753+(i*7),740+(i*5),732+(i*3),2024);}
ps("\n");pdn_report();pdn_state();ps("\n=== Demo Complete ===\n");return 0;}
