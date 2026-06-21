/* puschkinia_admin: Puschkinia management technology administration (v1.0)
 * Puschkinia planning, puschkinia execution, puschkinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pus_t;
typedef struct{int n_pup,n_pue,n_puv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pus_state_t;
static pus_t pups[N],pues[N-2],puvs[N-4],acs[N-6],mks[N-6]; static pus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pus_init(void){if(init)return -1;st.n_pup=0;st.n_pue=0;st.n_puv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pups[i].active=0;for(int i=0;i<N-2;i++)pues[i].active=0;for(int i=0;i<N-4;i++)puvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PUS] Puschkinia initialized\n");return 0;}
int pus_planning(int t,int c,int a,int b,int d,int e,int y){return add(pups,&st.n_pup,&st.t_f1,N,t,c,a,b,d,e,y);}
int pus_execution(int t,int c,int a,int b,int d,int e,int y){return add(pues,&st.n_pue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pus_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(puvs,&st.n_puv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pus_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pus_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pus_report(void){ps("[PUS] Pup: ");pi(st.n_pup);ps(" PCS=");pi(st.t_f1);ps("\nPue: ");pi(st.n_pue);ps(" PCS=");pi(st.t_f2);ps("\nPuv: ");pi(st.n_puv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pus_state(void){ps("[PUS] Pup=");pi(st.n_pup);ps(" Pue=");pi(st.n_pue);ps(" Puv=");pi(st.n_puv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Puschkinia Admin Demo ===\n\n");pus_init();
ps("Puschkinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pus_planning(t,c,610+(i*17),599+(i*14),579+(i*10),561+(i*6),2020+(i%5));}
ps("\nPuschkinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pus_execution(t,c,599+(i*15),588+(i*12),570+(i*8),557+(i*5),2021+(i%4));}
ps("\nPuschkinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pus_evaluation(t,c,591+(i*13),580+(i*10),564+(i*7),553+(i*4),2022+(i%3));}
ps("\nPuschkinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pus_accessory(t,c,583+(i*11),574+(i*9),560+(i*6),550+(i*3),2023+(i%2));}
ps("\nPuschkinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pus_market(t,c,577+(i*9),568+(i*7),555+(i*5),547+(i*3),2024);}
ps("\n");pus_report();pus_state();ps("\n=== Demo Complete ===\n");return 0;}
