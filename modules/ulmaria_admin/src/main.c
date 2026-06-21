/* ulmaria_admin: Ulmaria management technology administration (v1.0)
 * Ulmaria planning, ulmaria execution, ulmaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ulm_t;
typedef struct{int n_ulp,n_ule,n_ulv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ulm_state_t;
static ulm_t ulmps[N],ulmss[N-2],ulmvss[N-4],ulmas[N-6],ulmmks[N-6]; static ulm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ulm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ulm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ULM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ulm_init(void){if(init)return -1;st.n_ulp=0;st.n_ule=0;st.n_ulv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ulmps[i].active=0;for(int i=0;i<N-2;i++)ulmss[i].active=0;for(int i=0;i<N-4;i++)ulmvss[i].active=0;for(int i=0;i<N-6;i++)ulmas[i].active=0;for(int i=0;i<N-6;i++)ulmmks[i].active=0;init=1;ps("[ULM] Ulmaria initialized\n");return 0;}
int ulm_planning(int t,int c,int a,int b,int d,int e,int y){return add(ulmps,&st.n_ulp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ulm_execution(int t,int c,int a,int b,int d,int e,int y){return add(ulmss,&st.n_ule,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ulm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ulmvss,&st.n_ulv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ulm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ulmas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ulm_market(int t,int c,int a,int b,int d,int e,int y){return add(ulmmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ulm_report(void){ps("[ULM] Ulp: ");pi(st.n_ulp);ps(" PCS=");pi(st.t_f1);ps("\nUle: ");pi(st.n_ule);ps(" PCS=");pi(st.t_f2);ps("\nUlv: ");pi(st.n_ulv);ps(" PCS=");pi(st.t_f3);ps("\nUlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ulm_state(void){ps("[ULM] Ulp=");pi(st.n_ulp);ps(" Ule=");pi(st.n_ule);ps(" Ulv=");pi(st.n_ulv);ps(" Ulc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ulmaria Admin Demo ===\n\n");ulm_init();
ps("Ulmaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ulm_planning(t,c,871+(i*17),860+(i*14),840+(i*10),822+(i*6),2020+(i%5));}
ps("\nUlmaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ulm_execution(t,c,860+(i*15),849+(i*12),831+(i*8),818+(i*5),2021+(i%4));}
ps("\nUlmaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ulm_evaluation(t,c,852+(i*13),841+(i*10),825+(i*7),814+(i*4),2022+(i%3));}
ps("\nUlmaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ulm_accessory(t,c,844+(i*11),835+(i*9),821+(i*6),811+(i*3),2023+(i%2));}
ps("\nUlmaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ulm_market(t,c,838+(i*9),829+(i*7),816+(i*5),808+(i*3),2024);}
ps("\n");ulm_report();ulm_state();ps("\n=== Demo Complete ===\n");return 0;}
