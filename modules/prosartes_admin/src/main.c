/* prosartes_admin: Prosartes management technology administration (v1.0)
 * Prosartes planning, prosartes execution, prosartes evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pros_t;
typedef struct{int n_prosp,n_prose,n_pros2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pros_state_t;
static pros_t prosps[N],prosss[N-2],prosvss[N-4],prosas[N-6],prosmks[N-6]; static pros_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pros_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pros_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PROS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pros_init(void){if(init)return -1;st.n_prosp=0;st.n_prose=0;st.n_pros2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prosps[i].active=0;for(int i=0;i<N-2;i++)prosss[i].active=0;for(int i=0;i<N-4;i++)prosvss[i].active=0;for(int i=0;i<N-6;i++)prosas[i].active=0;for(int i=0;i<N-6;i++)prosmks[i].active=0;init=1;ps("[PROS] Prosartes initialized\n");return 0;}
int pros_planning(int t,int c,int a,int b,int d,int e,int y){return add(prosps,&st.n_prosp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pros_execution(int t,int c,int a,int b,int d,int e,int y){return add(prosss,&st.n_prose,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pros_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(prosvss,&st.n_pros2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pros_accessory(int t,int c,int a,int b,int d,int e,int y){return add(prosas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pros_market(int t,int c,int a,int b,int d,int e,int y){return add(prosmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pros_report(void){ps("[PROS] Propp: ");pi(st.n_prosp);ps(" PCS=");pi(st.t_f1);ps("\nProe: ");pi(st.n_prose);ps(" PCS=");pi(st.t_f2);ps("\nProv: ");pi(st.n_pros2);ps(" PCS=");pi(st.t_f3);ps("\nProc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pros_state(void){ps("[PROS] Propp=");pi(st.n_prosp);ps(" Proe=");pi(st.n_prose);ps(" Prov=");pi(st.n_pros2);ps(" Proc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Prosartes Admin Demo ===\n\n");pros_init();
ps("Prosartes planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pros_planning(t,c,1173+(i*17),1162+(i*14),1142+(i*10),1124+(i*6),2020+(i%5));}
ps("\nProsartes execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pros_execution(t,c,1162+(i*15),1151+(i*12),1133+(i*8),1120+(i*5),2021+(i%4));}
ps("\nProsartes evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pros_evaluation(t,c,1154+(i*13),1143+(i*10),1127+(i*7),1116+(i*4),2022+(i%3));}
ps("\nProsartes accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pros_accessory(t,c,1146+(i*11),1137+(i*9),1123+(i*6),1113+(i*3),2023+(i%2));}
ps("\nProsartes marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pros_market(t,c,1140+(i*9),1131+(i*7),1118+(i*5),1110+(i*3),2024);}
ps("\n");pros_report();pros_state();ps("\n=== Demo Complete ===\n");return 0;}
