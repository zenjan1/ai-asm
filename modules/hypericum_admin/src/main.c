/* hypericum_admin: Hypericum management technology administration (v1.0)
 * Hypericum planning, hypericum execution, hypericum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hyp_t;
typedef struct{int n_hypp,n_hype,n_hyp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hyp_state_t;
static hyp_t hypps[N],hypss[N-2],hypvss[N-4],hymas[N-6],hypmks[N-6]; static hyp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hyp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hyp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hyp_init(void){if(init)return -1;st.n_hypp=0;st.n_hype=0;st.n_hyp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hypps[i].active=0;for(int i=0;i<N-2;i++)hypss[i].active=0;for(int i=0;i<N-4;i++)hypvss[i].active=0;for(int i=0;i<N-6;i++)hymas[i].active=0;for(int i=0;i<N-6;i++)hypmks[i].active=0;init=1;ps("[HYP] Hypericum initialized\n");return 0;}
int hyp_planning(int t,int c,int a,int b,int d,int e,int y){return add(hypps,&st.n_hypp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hyp_execution(int t,int c,int a,int b,int d,int e,int y){return add(hypss,&st.n_hype,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hyp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hypvss,&st.n_hyp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hyp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hymas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hyp_market(int t,int c,int a,int b,int d,int e,int y){return add(hypmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hyp_report(void){ps("[HYP] Hypp: ");pi(st.n_hypp);ps(" PCS=");pi(st.t_f1);ps("\nHype: ");pi(st.n_hype);ps(" PCS=");pi(st.t_f2);ps("\nHypv: ");pi(st.n_hyp2);ps(" PCS=");pi(st.t_f3);ps("\nHypc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hyp_state(void){ps("[HYP] Hypp=");pi(st.n_hypp);ps(" Hype=");pi(st.n_hype);ps(" Hypv=");pi(st.n_hyp2);ps(" Hyc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hypericum Admin Demo ===\n\n");hyp_init();
ps("Hypericum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hyp_planning(t,c,1022+(i*17),1011+(i*14),991+(i*10),973+(i*6),2020+(i%5));}
ps("\nHypericum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hyp_execution(t,c,1011+(i*15),1000+(i*12),982+(i*8),969+(i*5),2021+(i%4));}
ps("\nHypericum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hyp_evaluation(t,c,1003+(i*13),992+(i*10),976+(i*7),965+(i*4),2022+(i%3));}
ps("\nHypericum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyp_accessory(t,c,995+(i*11),986+(i*9),972+(i*6),962+(i*3),2023+(i%2));}
ps("\nHypericum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hyp_market(t,c,989+(i*9),980+(i*7),967+(i*5),959+(i*3),2024);}
ps("\n");hyp_report();hyp_state();ps("\n=== Demo Complete ===\n");return 0;}
