/* phygelius_admin: Phygelius management technology administration (v1.0)
 * Phygelius planning, phygelius execution, phygelius evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pc_t;
typedef struct{int n_pcp2,n_pce,n_pcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pc_state_t;
static pc_t pcps2[N],pcss2[N-2],pcvss2[N-4],pcas2[N-6],pcmks2[N-6]; static pc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pcn_init(void){if(init)return -1;st.n_pcp2=0;st.n_pce=0;st.n_pcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pcps2[i].active=0;for(int i=0;i<N-2;i++)pcss2[i].active=0;for(int i=0;i<N-4;i++)pcvss2[i].active=0;for(int i=0;i<N-6;i++)pcas2[i].active=0;for(int i=0;i<N-6;i++)pcmks2[i].active=0;init=1;ps("[PCN] Phygelius initialized\n");return 0;}
int pcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(pcps2,&st.n_pcp2,&st.t_f1,N,t,c,a,b,d,e,y);}
int pcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(pcss2,&st.n_pce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pcvss2,&st.n_pcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pcas2,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pcn_market(int t,int c,int a,int b,int d,int e,int y){return add(pcmks2,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pcn_report(void){ps("[PCN] Pcp: ");pi(st.n_pcp2);ps(" PCS=");pi(st.t_f1);ps("\nPce: ");pi(st.n_pce);ps(" PCS=");pi(st.t_f2);ps("\nPcv: ");pi(st.n_pcv);ps(" PCS=");pi(st.t_f3);ps("\nPcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pcn_state(void){ps("[PCN] Pcp=");pi(st.n_pcp2);ps(" Pce=");pi(st.n_pce);ps(" Pcv=");pi(st.n_pcv);ps(" Pcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Phygelius Admin Demo ===\n\n");pcn_init();
ps("Phygelius planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pcn_planning(t,c,749+(i*17),738+(i*14),718+(i*10),700+(i*6),2020+(i%5));}
ps("\nPhygelius execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pcn_execution(t,c,738+(i*15),727+(i*12),709+(i*8),696+(i*5),2021+(i%4));}
ps("\nPhygelius evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pcn_evaluation(t,c,730+(i*13),719+(i*10),703+(i*7),692+(i*4),2022+(i%3));}
ps("\nPhygelius accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pcn_accessory(t,c,722+(i*11),713+(i*9),699+(i*6),689+(i*3),2023+(i%2));}
ps("\nPhygelius marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pcn_market(t,c,716+(i*9),707+(i*7),694+(i*5),686+(i*3),2024);}
ps("\n");pcn_report();pcn_state();ps("\n=== Demo Complete ===\n");return 0;}
