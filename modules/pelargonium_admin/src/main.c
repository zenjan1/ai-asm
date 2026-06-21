/* pelargonium_admin: Pelargonium management technology administration (v1.0)
 * Pelargonium planning, pelargonium execution, pelargonium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pn_t;
typedef struct{int n_plp,n_ple,n_plv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pn_state_t;
static pn_t plps[N],plss[N-2],plvss[N-4],plas[N-6],plmks[N-6]; static pn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pl_init(void){if(init)return -1;st.n_plp=0;st.n_ple=0;st.n_plv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)plps[i].active=0;for(int i=0;i<N-2;i++)plss[i].active=0;for(int i=0;i<N-4;i++)plvss[i].active=0;for(int i=0;i<N-6;i++)plas[i].active=0;for(int i=0;i<N-6;i++)plmks[i].active=0;init=1;ps("[PLG] Pelargonium initialized\n");return 0;}
int pl_planning(int t,int c,int a,int b,int d,int e,int y){return add(plps,&st.n_plp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pl_execution(int t,int c,int a,int b,int d,int e,int y){return add(plss,&st.n_ple,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(plvss,&st.n_plv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(plas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pl_market(int t,int c,int a,int b,int d,int e,int y){return add(plmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pl_report(void){ps("[PLG] Plp: ");pi(st.n_plp);ps(" PCS=");pi(st.t_f1);ps("\nPle: ");pi(st.n_ple);ps(" PCS=");pi(st.t_f2);ps("\nPlv: ");pi(st.n_plv);ps(" PCS=");pi(st.t_f3);ps("\nPlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pl_state(void){ps("[PLG] Plp=");pi(st.n_plp);ps(" Ple=");pi(st.n_ple);ps(" Plv=");pi(st.n_plv);ps(" Plc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pelargonium Admin Demo ===\n\n");pl_init();
ps("Pelargonium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pl_planning(t,c,720+(i*17),709+(i*14),689+(i*10),671+(i*6),2020+(i%5));}
ps("\nPelargonium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pl_execution(t,c,709+(i*15),698+(i*12),680+(i*8),667+(i*5),2021+(i%4));}
ps("\nPelargonium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pl_evaluation(t,c,701+(i*13),690+(i*10),674+(i*7),663+(i*4),2022+(i%3));}
ps("\nPelargonium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pl_accessory(t,c,693+(i*11),684+(i*9),670+(i*6),660+(i*3),2023+(i%2));}
ps("\nPelargonium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pl_market(t,c,687+(i*9),678+(i*7),665+(i*5),657+(i*3),2024);}
ps("\n");pl_report();pl_state();ps("\n=== Demo Complete ===\n");return 0;}
