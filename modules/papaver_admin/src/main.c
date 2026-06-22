/* papaver_admin: Papaver management technology administration (v1.0)
 * Papaver planning, papaver execution, papaver evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pap_t;
typedef struct{int n_papp,n_pape,n_pap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pap_state_t;
static pap_t papps[N],papss[N-2],papvss[N-4],papas[N-6],papmks[N-6]; static pap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pap_init(void){if(init)return -1;st.n_papp=0;st.n_pape=0;st.n_pap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)papps[i].active=0;for(int i=0;i<N-2;i++)papss[i].active=0;for(int i=0;i<N-4;i++)papvss[i].active=0;for(int i=0;i<N-6;i++)papas[i].active=0;for(int i=0;i<N-6;i++)papmks[i].active=0;init=1;ps("[PAP] Papaver initialized\n");return 0;}
int pap_planning(int t,int c,int a,int b,int d,int e,int y){return add(papps,&st.n_papp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pap_execution(int t,int c,int a,int b,int d,int e,int y){return add(papss,&st.n_pape,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(papvss,&st.n_pap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(papas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pap_market(int t,int c,int a,int b,int d,int e,int y){return add(papmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pap_report(void){ps("[PAP] Pappp: ");pi(st.n_papp);ps(" PCS=");pi(st.t_f1);ps("\nPape: ");pi(st.n_pape);ps(" PCS=");pi(st.t_f2);ps("\nPapv: ");pi(st.n_pap2);ps(" PCS=");pi(st.t_f3);ps("\nPapc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pap_state(void){ps("[PAP] Pappp=");pi(st.n_papp);ps(" Pape=");pi(st.n_pape);ps(" Papv=");pi(st.n_pap2);ps(" Papc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Papaver Admin Demo ===\n\n");pap_init();
ps("Papaver planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pap_planning(t,c,1166+(i*17),1155+(i*14),1135+(i*10),1117+(i*6),2020+(i%5));}
ps("\nPapaver execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pap_execution(t,c,1155+(i*15),1144+(i*12),1126+(i*8),1113+(i*5),2021+(i%4));}
ps("\nPapaver evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pap_evaluation(t,c,1147+(i*13),1136+(i*10),1120+(i*7),1109+(i*4),2022+(i%3));}
ps("\nPapaver accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pap_accessory(t,c,1139+(i*11),1130+(i*9),1116+(i*6),1106+(i*3),2023+(i%2));}
ps("\nPapaver marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pap_market(t,c,1133+(i*9),1124+(i*7),1111+(i*5),1103+(i*3),2024);}
ps("\n");pap_report();pap_state();ps("\n=== Demo Complete ===\n");return 0;}
