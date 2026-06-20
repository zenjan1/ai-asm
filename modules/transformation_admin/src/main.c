/* transformation_admin: Transformation management technology administration (v1.0)
 * Transformation planning, transformation execution, transformation evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tfx_t;
typedef struct{int n_tp,n_te,n_tv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tfx_state_t;
static tfx_t tps[N],tes[N-2],tvs[N-4],acs[N-6],mks[N-6]; static tfx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tfx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tfx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TFX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tfx_init(void){if(init)return -1;st.n_tp=0;st.n_te=0;st.n_tv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tps[i].active=0;for(int i=0;i<N-2;i++)tes[i].active=0;for(int i=0;i<N-4;i++)tvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TFX] Transformation initialized\n");return 0;}
int tfx_planning(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tfx_execution(int t,int c,int a,int b,int d,int e,int y){return add(tes,&st.n_te,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tfx_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tvs,&st.n_tv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tfx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tfx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tfx_report(void){ps("[TFX] Tp: ");pi(st.n_tp);ps(" PCS=");pi(st.t_f1);ps("\nTe: ");pi(st.n_te);ps(" PCS=");pi(st.t_f2);ps("\nTv: ");pi(st.n_tv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tfx_state(void){ps("[TFX] Tp=");pi(st.n_tp);ps(" Te=");pi(st.n_te);ps(" Tv=");pi(st.n_tv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Transformation Admin Demo ===\n\n");tfx_init();
ps("Transformation planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tfx_planning(t,c,476+(i*17),465+(i*14),445+(i*10),427+(i*6),2020+(i%5));}
ps("\nTransformation execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tfx_execution(t,c,465+(i*15),454+(i*12),436+(i*8),423+(i*5),2021+(i%4));}
ps("\nTransformation evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tfx_evaluation(t,c,457+(i*13),446+(i*10),430+(i*7),419+(i*4),2022+(i%3));}
ps("\nTransformation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tfx_accessory(t,c,449+(i*11),440+(i*9),426+(i*6),416+(i*3),2023+(i%2));}
ps("\nTransformation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tfx_market(t,c,443+(i*9),434+(i*7),421+(i*5),413+(i*3),2024);}
ps("\n");tfx_report();tfx_state();ps("\n=== Demo Complete ===\n");return 0;}
