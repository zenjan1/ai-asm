/* cicely_admin: Cicely management technology administration (v1.0)
 * Cicely planning, cicely execution, cicely evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cc_t;
typedef struct{int n_ccp,n_cce,n_ccv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cc_state_t;
static cc_t ccps[N],ccss[N-2],ccvss[N-4],ccas[N-6],ccmks[N-6]; static cc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CCL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cc_init(void){if(init)return -1;st.n_ccp=0;st.n_cce=0;st.n_ccv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ccps[i].active=0;for(int i=0;i<N-2;i++)ccss[i].active=0;for(int i=0;i<N-4;i++)ccvss[i].active=0;for(int i=0;i<N-6;i++)ccas[i].active=0;for(int i=0;i<N-6;i++)ccmks[i].active=0;init=1;ps("[CCL] Cicely initialized\n");return 0;}
int cc_planning(int t,int c,int a,int b,int d,int e,int y){return add(ccps,&st.n_ccp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cc_execution(int t,int c,int a,int b,int d,int e,int y){return add(ccss,&st.n_cce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ccvss,&st.n_ccv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ccas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cc_market(int t,int c,int a,int b,int d,int e,int y){return add(ccmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cc_report(void){ps("[CCL] Ccp: ");pi(st.n_ccp);ps(" PCS=");pi(st.t_f1);ps("\nCce: ");pi(st.n_cce);ps(" PCS=");pi(st.t_f2);ps("\nCcv: ");pi(st.n_ccv);ps(" PCS=");pi(st.t_f3);ps("\nCcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cc_state(void){ps("[CCL] Ccp=");pi(st.n_ccp);ps(" Cce=");pi(st.n_cce);ps(" Ccv=");pi(st.n_ccv);ps(" Ccc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cicely Admin Demo ===\n\n");cc_init();
ps("Cicely planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cc_planning(t,c,688+(i*17),677+(i*14),657+(i*10),639+(i*6),2020+(i%5));}
ps("\nCicely execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cc_execution(t,c,677+(i*15),666+(i*12),648+(i*8),635+(i*5),2021+(i%4));}
ps("\nCicely evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cc_evaluation(t,c,669+(i*13),658+(i*10),642+(i*7),631+(i*4),2022+(i%3));}
ps("\nCicely accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cc_accessory(t,c,661+(i*11),652+(i*9),638+(i*6),628+(i*3),2023+(i%2));}
ps("\nCicely marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cc_market(t,c,655+(i*9),646+(i*7),633+(i*5),625+(i*3),2024);}
ps("\n");cc_report();cc_state();ps("\n=== Demo Complete ===\n");return 0;}
