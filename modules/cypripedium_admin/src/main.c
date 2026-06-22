/* cypripedium_admin: Cypripedium management technology administration (v1.0)
 * Cypripedium planning, cypripedium execution, cypripedium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyp_t;
typedef struct{int n_cypp,n_cype,n_cyp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyp_state_t;
static cyp_t cypps[N],cypss[N-2],cypvss[N-4],cypas[N-6],cypmks[N-6]; static cyp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyp_init(void){if(init)return -1;st.n_cypp=0;st.n_cype=0;st.n_cyp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cypps[i].active=0;for(int i=0;i<N-2;i++)cypss[i].active=0;for(int i=0;i<N-4;i++)cypvss[i].active=0;for(int i=0;i<N-6;i++)cypas[i].active=0;for(int i=0;i<N-6;i++)cypmks[i].active=0;init=1;ps("[CYP] Cypripedium initialized\n");return 0;}
int cyp_planning(int t,int c,int a,int b,int d,int e,int y){return add(cypps,&st.n_cypp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyp_execution(int t,int c,int a,int b,int d,int e,int y){return add(cypss,&st.n_cype,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cypvss,&st.n_cyp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cypas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyp_market(int t,int c,int a,int b,int d,int e,int y){return add(cypmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyp_report(void){ps("[CYP] Cyppp: ");pi(st.n_cypp);ps(" PCS=");pi(st.t_f1);ps("\nCype: ");pi(st.n_cype);ps(" PCS=");pi(st.t_f2);ps("\nCypv: ");pi(st.n_cyp2);ps(" PCS=");pi(st.t_f3);ps("\nCypc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyp_state(void){ps("[CYP] Cyppp=");pi(st.n_cypp);ps(" Cype=");pi(st.n_cype);ps(" Cypv=");pi(st.n_cyp2);ps(" Cypc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cypripedium Admin Demo ===\n\n");cyp_init();
ps("Cypripedium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyp_planning(t,c,1135+(i*17),1124+(i*14),1104+(i*10),1086+(i*6),2020+(i%5));}
ps("\nCypripedium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyp_execution(t,c,1124+(i*15),1113+(i*12),1095+(i*8),1082+(i*5),2021+(i%4));}
ps("\nCypripedium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyp_evaluation(t,c,1116+(i*13),1105+(i*10),1089+(i*7),1078+(i*4),2022+(i%3));}
ps("\nCypripedium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyp_accessory(t,c,1108+(i*11),1099+(i*9),1085+(i*6),1075+(i*3),2023+(i%2));}
ps("\nCypripedium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyp_market(t,c,1102+(i*9),1093+(i*7),1080+(i*5),1072+(i*3),2024);}
ps("\n");cyp_report();cyp_state();ps("\n=== Demo Complete ===\n");return 0;}
