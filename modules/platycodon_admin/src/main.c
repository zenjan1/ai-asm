/* platycodon_admin: Platycodon management technology administration (v1.0)
 * Platycodon planning, platycodon execution, platycodon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pe_t;
typedef struct{int n_pcp,n_pce,n_pcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pe_state_t;
static pe_t pcps[N],pcss[N-2],pcvss[N-4],pcas[N-6],pcmks[N-6]; static pe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PCT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pct_init(void){if(init)return -1;st.n_pcp=0;st.n_pce=0;st.n_pcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pcps[i].active=0;for(int i=0;i<N-2;i++)pcss[i].active=0;for(int i=0;i<N-4;i++)pcvss[i].active=0;for(int i=0;i<N-6;i++)pcas[i].active=0;for(int i=0;i<N-6;i++)pcmks[i].active=0;init=1;ps("[PCT] Platycodon initialized\n");return 0;}
int pct_planning(int t,int c,int a,int b,int d,int e,int y){return add(pcps,&st.n_pcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pct_execution(int t,int c,int a,int b,int d,int e,int y){return add(pcss,&st.n_pce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pct_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pcvss,&st.n_pcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pct_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pct_market(int t,int c,int a,int b,int d,int e,int y){return add(pcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pct_report(void){ps("[PCT] Pcp: ");pi(st.n_pcp);ps(" PCS=");pi(st.t_f1);ps("\nPce: ");pi(st.n_pce);ps(" PCS=");pi(st.t_f2);ps("\nPcv: ");pi(st.n_pcv);ps(" PCS=");pi(st.t_f3);ps("\nPcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pct_state(void){ps("[PCT] Pcp=");pi(st.n_pcp);ps(" Pce=");pi(st.n_pce);ps(" Pcv=");pi(st.n_pcv);ps(" Pcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Platycodon Admin Demo ===\n\n");pct_init();
ps("Platycodon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pct_planning(t,c,735+(i*17),724+(i*14),704+(i*10),686+(i*6),2020+(i%5));}
ps("\nPlatycodon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pct_execution(t,c,724+(i*15),713+(i*12),695+(i*8),682+(i*5),2021+(i%4));}
ps("\nPlatycodon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pct_evaluation(t,c,716+(i*13),705+(i*10),689+(i*7),678+(i*4),2022+(i%3));}
ps("\nPlatycodon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pct_accessory(t,c,708+(i*11),699+(i*9),685+(i*6),675+(i*3),2023+(i%2));}
ps("\nPlatycodon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pct_market(t,c,702+(i*9),693+(i*7),680+(i*5),672+(i*3),2024);}
ps("\n");pct_report();pct_state();ps("\n=== Demo Complete ===\n");return 0;}
