/* achimenes_admin: Achimenes management technology administration (v1.0)
 * Achimenes planning, achimenes execution, achimenes evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ach_t;
typedef struct{int n_acp,n_ace,n_acv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ach_state_t;
static ach_t achps[N],aches[N-2],achvs[N-4],acs[N-6],mks[N-6]; static ach_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ach_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ach_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ach_init(void){if(init)return -1;st.n_acp=0;st.n_ace=0;st.n_acv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)achps[i].active=0;for(int i=0;i<N-2;i++)aches[i].active=0;for(int i=0;i<N-4;i++)achvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ACH] Achimenes initialized\n");return 0;}
int ach_planning(int t,int c,int a,int b,int d,int e,int y){return add(achps,&st.n_acp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ach_execution(int t,int c,int a,int b,int d,int e,int y){return add(aches,&st.n_ace,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ach_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(achvs,&st.n_acv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ach_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ach_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ach_report(void){ps("[ACH] Acp: ");pi(st.n_acp);ps(" PCS=");pi(st.t_f1);ps("\nAce: ");pi(st.n_ace);ps(" PCS=");pi(st.t_f2);ps("\nAcv: ");pi(st.n_acv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ach_state(void){ps("[ACH] Acp=");pi(st.n_acp);ps(" Ace=");pi(st.n_ace);ps(" Acv=");pi(st.n_acv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Achimenes Admin Demo ===\n\n");ach_init();
ps("Achimenes planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ach_planning(t,c,601+(i*17),590+(i*14),570+(i*10),552+(i*6),2020+(i%5));}
ps("\nAchimenes execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ach_execution(t,c,590+(i*15),579+(i*12),561+(i*8),548+(i*5),2021+(i%4));}
ps("\nAchimenes evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ach_evaluation(t,c,582+(i*13),571+(i*10),555+(i*7),544+(i*4),2022+(i%3));}
ps("\nAchimenes accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ach_accessory(t,c,574+(i*11),565+(i*9),551+(i*6),541+(i*3),2023+(i%2));}
ps("\nAchimenes marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ach_market(t,c,568+(i*9),559+(i*7),546+(i*5),538+(i*3),2024);}
ps("\n");ach_report();ach_state();ps("\n=== Demo Complete ===\n");return 0;}
