/* acanthus_admin: Acanthus management technology administration (v1.0)
 * Acanthus planning, acanthus execution, acanthus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acn_t;
typedef struct{int n_acnp,n_acne,n_acn2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acn_state_t;
static acn_t acnps[N],acnss[N-2],acnvss[N-4],acnas[N-6],acnmks[N-6]; static acn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acn_init(void){if(init)return -1;st.n_acnp=0;st.n_acne=0;st.n_acn2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acnps[i].active=0;for(int i=0;i<N-2;i++)acnss[i].active=0;for(int i=0;i<N-4;i++)acnvss[i].active=0;for(int i=0;i<N-6;i++)acnas[i].active=0;for(int i=0;i<N-6;i++)acnmks[i].active=0;init=1;ps("[ACN] Acanthus initialized\n");return 0;}
int acn_planning(int t,int c,int a,int b,int d,int e,int y){return add(acnps,&st.n_acnp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acn_execution(int t,int c,int a,int b,int d,int e,int y){return add(acnss,&st.n_acne,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acnvss,&st.n_acn2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acn_market(int t,int c,int a,int b,int d,int e,int y){return add(acnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acn_report(void){ps("[ACN] Acapp: ");pi(st.n_acnp);ps(" PCS=");pi(st.t_f1);ps("\nAcape: ");pi(st.n_acne);ps(" PCS=");pi(st.t_f2);ps("\nAcav: ");pi(st.n_acn2);ps(" PCS=");pi(st.t_f3);ps("\nAcac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acn_state(void){ps("[ACN] Acapp=");pi(st.n_acnp);ps(" Acape=");pi(st.n_acne);ps(" Acav=");pi(st.n_acn2);ps(" Acac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acanthus Admin Demo ===\n\n");acn_init();
ps("Acanthus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acn_planning(t,c,1211+(i*17),1200+(i*14),1180+(i*10),1162+(i*6),2020+(i%5));}
ps("\nAcanthus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acn_execution(t,c,1200+(i*15),1189+(i*12),1171+(i*8),1158+(i*5),2021+(i%4));}
ps("\nAcanthus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acn_evaluation(t,c,1192+(i*13),1181+(i*10),1165+(i*7),1154+(i*4),2022+(i%3));}
ps("\nAcanthus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acn_accessory(t,c,1184+(i*11),1175+(i*9),1161+(i*6),1151+(i*3),2023+(i%2));}
ps("\nAcanthus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acn_market(t,c,1178+(i*9),1169+(i*7),1156+(i*5),1148+(i*3),2024);}
ps("\n");acn_report();acn_state();ps("\n=== Demo Complete ===\n");return 0;}
