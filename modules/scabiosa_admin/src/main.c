/* scabiosa_admin: Scabiosa management technology administration (v1.0)
 * Scabiosa planning, scabiosa execution, scabiosa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} si_t;
typedef struct{int n_sbp,n_sbe,n_sbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} si_state_t;
static si_t sbps[N],sbss[N-2],sbvss[N-4],sbas[N-6],sbmks[N-6]; static si_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(si_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;si_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SCB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sb_init(void){if(init)return -1;st.n_sbp=0;st.n_sbe=0;st.n_sbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sbps[i].active=0;for(int i=0;i<N-2;i++)sbss[i].active=0;for(int i=0;i<N-4;i++)sbvss[i].active=0;for(int i=0;i<N-6;i++)sbas[i].active=0;for(int i=0;i<N-6;i++)sbmks[i].active=0;init=1;ps("[SCB] Scabiosa initialized\n");return 0;}
int sb_planning(int t,int c,int a,int b,int d,int e,int y){return add(sbps,&st.n_sbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sb_execution(int t,int c,int a,int b,int d,int e,int y){return add(sbss,&st.n_sbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(sbvss,&st.n_sbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(sbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sb_market(int t,int c,int a,int b,int d,int e,int y){return add(sbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sb_report(void){ps("[SCB] Sbp: ");pi(st.n_sbp);ps(" PCS=");pi(st.t_f1);ps("\nSbe: ");pi(st.n_sbe);ps(" PCS=");pi(st.t_f2);ps("\nSbv: ");pi(st.n_sbv);ps(" PCS=");pi(st.t_f3);ps("\nSbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sb_state(void){ps("[SCB] Sbp=");pi(st.n_sbp);ps(" Sbe=");pi(st.n_sbe);ps(" Sbv=");pi(st.n_sbv);ps(" Sbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Scabiosa Admin Demo ===\n\n");sb_init();
ps("Scabiosa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sb_planning(t,c,721+(i*17),710+(i*14),690+(i*10),672+(i*6),2020+(i%5));}
ps("\nScabiosa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sb_execution(t,c,710+(i*15),699+(i*12),681+(i*8),668+(i*5),2021+(i%4));}
ps("\nScabiosa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sb_evaluation(t,c,702+(i*13),691+(i*10),675+(i*7),664+(i*4),2022+(i%3));}
ps("\nScabiosa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sb_accessory(t,c,694+(i*11),685+(i*9),671+(i*6),661+(i*3),2023+(i%2));}
ps("\nScabiosa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sb_market(t,c,688+(i*9),679+(i*7),666+(i*5),658+(i*3),2024);}
ps("\n");sb_report();sb_state();ps("\n=== Demo Complete ===\n");return 0;}
