/* pulmonaria_admin: Pulmonaria management technology administration (v1.0)
 * Pulmonaria planning, pulmonaria execution, pulmonaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pul_t;
typedef struct{int n_pulp,n_pule,n_pul2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pul_state_t;
static pul_t pulps[N],pulss[N-2],pulvss[N-4],pulac[N-6],pulmks[N-6]; static pul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pul_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pul_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PUL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pul_init(void){if(init)return -1;st.n_pulp=0;st.n_pule=0;st.n_pul2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pulps[i].active=0;for(int i=0;i<N-2;i++)pulss[i].active=0;for(int i=0;i<N-4;i++)pulvss[i].active=0;for(int i=0;i<N-6;i++)pulac[i].active=0;for(int i=0;i<N-6;i++)pulmks[i].active=0;init=1;ps("[PUL] Pulmonaria initialized\n");return 0;}
int pul_planning(int t,int c,int a,int b,int d,int e,int y){return add(pulps,&st.n_pulp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pul_execution(int t,int c,int a,int b,int d,int e,int y){return add(pulss,&st.n_pule,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pul_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pulvss,&st.n_pul2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pul_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pulac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pul_market(int t,int c,int a,int b,int d,int e,int y){return add(pulmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pul_report(void){ps("[PUL] Pulp: ");pi(st.n_pulp);ps(" PCS=");pi(st.t_f1);ps("\nPule: ");pi(st.n_pule);ps(" PCS=");pi(st.t_f2);ps("\nPulv: ");pi(st.n_pul2);ps(" PCS=");pi(st.t_f3);ps("\nPulc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pul_state(void){ps("[PUL] Pulp=");pi(st.n_pulp);ps(" Pule=");pi(st.n_pule);ps(" Pulv=");pi(st.n_pul2);ps(" Pulc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pulmonaria Admin Demo ===\n\n");pul_init();
ps("Pulmonaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pul_planning(t,c,946+(i*17),935+(i*14),915+(i*10),897+(i*6),2020+(i%5));}
ps("\nPulmonaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pul_execution(t,c,935+(i*15),924+(i*12),906+(i*8),893+(i*5),2021+(i%4));}
ps("\nPulmonaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pul_evaluation(t,c,927+(i*13),916+(i*10),900+(i*7),889+(i*4),2022+(i%3));}
ps("\nPulmonaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pul_accessory(t,c,919+(i*11),910+(i*9),896+(i*6),886+(i*3),2023+(i%2));}
ps("\nPulmonaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pul_market(t,c,913+(i*9),904+(i*7),891+(i*5),883+(i*3),2024);}
ps("\n");pul_report();pul_state();ps("\n=== Demo Complete ===\n");return 0;}
