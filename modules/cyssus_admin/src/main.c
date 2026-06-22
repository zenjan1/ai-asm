/* cyssus_admin: Cyssus management technology administration (v1.0)
 * Cyssus planning, cyssus execution, cyssus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyss_t;
typedef struct{int n_cyssp,n_cysse,n_cyss2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyss_state_t;
static cyss_t cyssps[N],cyssss[N-2],cyssvss[N-4],cyssas[N-6],cyssmks[N-6]; static cyss_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyss_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyss_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYSS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyss_init(void){if(init)return -1;st.n_cyssp=0;st.n_cysse=0;st.n_cyss2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cyssps[i].active=0;for(int i=0;i<N-2;i++)cyssss[i].active=0;for(int i=0;i<N-4;i++)cyssvss[i].active=0;for(int i=0;i<N-6;i++)cyssas[i].active=0;for(int i=0;i<N-6;i++)cyssmks[i].active=0;init=1;ps("[CYSS] Cyssus initialized\n");return 0;}
int cyss_planning(int t,int c,int a,int b,int d,int e,int y){return add(cyssps,&st.n_cyssp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyss_execution(int t,int c,int a,int b,int d,int e,int y){return add(cyssss,&st.n_cysse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyss_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cyssvss,&st.n_cyss2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyss_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cyssas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyss_market(int t,int c,int a,int b,int d,int e,int y){return add(cyssmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyss_report(void){ps("[CYSS] Cyspp: ");pi(st.n_cyssp);ps(" PCS=");pi(st.t_f1);ps("\nCyse: ");pi(st.n_cysse);ps(" PCS=");pi(st.t_f2);ps("\nCysv: ");pi(st.n_cyss2);ps(" PCS=");pi(st.t_f3);ps("\nCysc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyss_state(void){ps("[CYSS] Cyspp=");pi(st.n_cyssp);ps(" Cyse=");pi(st.n_cysse);ps(" Cysv=");pi(st.n_cyss2);ps(" Cysc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyssus Admin Demo ===\n\n");cyss_init();
ps("Cyssus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyss_planning(t,c,1136+(i*17),1125+(i*14),1105+(i*10),1087+(i*6),2020+(i%5));}
ps("\nCyssus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyss_execution(t,c,1125+(i*15),1114+(i*12),1096+(i*8),1083+(i*5),2021+(i%4));}
ps("\nCyssus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyss_evaluation(t,c,1117+(i*13),1106+(i*10),1090+(i*7),1079+(i*4),2022+(i%3));}
ps("\nCyssus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyss_accessory(t,c,1109+(i*11),1100+(i*9),1086+(i*6),1076+(i*3),2023+(i%2));}
ps("\nCyssus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyss_market(t,c,1103+(i*9),1094+(i*7),1081+(i*5),1073+(i*3),2024);}
ps("\n");cyss_report();cyss_state();ps("\n=== Demo Complete ===\n");return 0;}
