/* sempervivum_admin: Sempervivum management technology administration (v1.0)
 * Sempervivum planning, sempervivum execution, sempervivum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} so_t;
typedef struct{int n_smp,n_sme,n_smv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} so_state_t;
static so_t smsp[N],smss[N-2],smvss[N-4],smas[N-6],smmks[N-6]; static so_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(so_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;so_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sm_init(void){if(init)return -1;st.n_smp=0;st.n_sme=0;st.n_smv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)smsp[i].active=0;for(int i=0;i<N-2;i++)smss[i].active=0;for(int i=0;i<N-4;i++)smvss[i].active=0;for(int i=0;i<N-6;i++)smas[i].active=0;for(int i=0;i<N-6;i++)smmks[i].active=0;init=1;ps("[SMP] Sempervivum initialized\n");return 0;}
int sm_planning(int t,int c,int a,int b,int d,int e,int y){return add(smsp,&st.n_smp,&st.t_f1,N,t,c,a,b,d,e,y);}
int sm_execution(int t,int c,int a,int b,int d,int e,int y){return add(smss,&st.n_sme,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(smvss,&st.n_smv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(smas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sm_market(int t,int c,int a,int b,int d,int e,int y){return add(smmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sm_report(void){ps("[SMP] Smp: ");pi(st.n_smp);ps(" PCS=");pi(st.t_f1);ps("\nSme: ");pi(st.n_sme);ps(" PCS=");pi(st.t_f2);ps("\nSmv: ");pi(st.n_smv);ps(" PCS=");pi(st.t_f3);ps("\nSmc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sm_state(void){ps("[SMP] Smp=");pi(st.n_smp);ps(" Sme=");pi(st.n_sme);ps(" Smv=");pi(st.n_smv);ps(" Smc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Sempervivum Admin Demo ===\n\n");sm_init();
ps("Sempervivum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sm_planning(t,c,723+(i*17),712+(i*14),692+(i*10),674+(i*6),2020+(i%5));}
ps("\nSempervivum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sm_execution(t,c,712+(i*15),701+(i*12),683+(i*8),670+(i*5),2021+(i%4));}
ps("\nSempervivum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sm_evaluation(t,c,704+(i*13),693+(i*10),677+(i*7),666+(i*4),2022+(i%3));}
ps("\nSempervivum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sm_accessory(t,c,696+(i*11),687+(i*9),673+(i*6),663+(i*3),2023+(i%2));}
ps("\nSempervivum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sm_market(t,c,690+(i*9),681+(i*7),668+(i*5),660+(i*3),2024);}
ps("\n");sm_report();sm_state();ps("\n=== Demo Complete ===\n");return 0;}
