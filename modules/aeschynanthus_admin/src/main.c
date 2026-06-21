/* aeschynanthus_admin: Aeschynanthus management technology administration (v1.0)
 * Aeschynanthus planning, aeschynanthus execution, aeschynanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aes_t;
typedef struct{int n_aep,n_aee,n_aev,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aes_state_t;
static aes_t aeps[N],aees[N-2],aevs[N-4],acs[N-6],mks[N-6]; static aes_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aes_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aes_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aes_init(void){if(init)return -1;st.n_aep=0;st.n_aee=0;st.n_aev=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aeps[i].active=0;for(int i=0;i<N-2;i++)aees[i].active=0;for(int i=0;i<N-4;i++)aevs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AES] Aeschynanthus initialized\n");return 0;}
int aes_planning(int t,int c,int a,int b,int d,int e,int y){return add(aeps,&st.n_aep,&st.t_f1,N,t,c,a,b,d,e,y);}
int aes_execution(int t,int c,int a,int b,int d,int e,int y){return add(aees,&st.n_aee,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aes_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aevs,&st.n_aev,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aes_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aes_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aes_report(void){ps("[AES] Aep: ");pi(st.n_aep);ps(" PCS=");pi(st.t_f1);ps("\nAee: ");pi(st.n_aee);ps(" PCS=");pi(st.t_f2);ps("\nAev: ");pi(st.n_aev);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aes_state(void){ps("[AES] Aep=");pi(st.n_aep);ps(" Aee=");pi(st.n_aee);ps(" Aev=");pi(st.n_aev);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aeschynanthus Admin Demo ===\n\n");aes_init();
ps("Aeschynanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aes_planning(t,c,606+(i*17),595+(i*14),575+(i*10),557+(i*6),2020+(i%5));}
ps("\nAeschynanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aes_execution(t,c,595+(i*15),584+(i*12),566+(i*8),553+(i*5),2021+(i%4));}
ps("\nAeschynanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aes_evaluation(t,c,587+(i*13),576+(i*10),560+(i*7),549+(i*4),2022+(i%3));}
ps("\nAeschynanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aes_accessory(t,c,579+(i*11),570+(i*9),556+(i*6),546+(i*3),2023+(i%2));}
ps("\nAeschynanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aes_market(t,c,573+(i*9),564+(i*7),551+(i*5),543+(i*3),2024);}
ps("\n");aes_report();aes_state();ps("\n=== Demo Complete ===\n");return 0;}
