/* nostholinia_admin: Nostholinia management technology administration (v1.0)
 * Nostholinia planning, nostholinia execution, nostholinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nos_t;
typedef struct{int n_nosp,n_nose,n_nos2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nos_state_t;
static nos_t nosps[N],nosss[N-2],nosvss[N-4],nosas[N-6],nosmks[N-6]; static nos_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nos_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nos_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NOS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nos_init(void){if(init)return -1;st.n_nosp=0;st.n_nose=0;st.n_nos2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nosps[i].active=0;for(int i=0;i<N-2;i++)nosss[i].active=0;for(int i=0;i<N-4;i++)nosvss[i].active=0;for(int i=0;i<N-6;i++)nosas[i].active=0;for(int i=0;i<N-6;i++)nosmks[i].active=0;init=1;ps("[NOS] Nostholinia initialized\n");return 0;}
int nos_planning(int t,int c,int a,int b,int d,int e,int y){return add(nosps,&st.n_nosp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nos_execution(int t,int c,int a,int b,int d,int e,int y){return add(nosss,&st.n_nose,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nos_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nosvss,&st.n_nos2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nos_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nosas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nos_market(int t,int c,int a,int b,int d,int e,int y){return add(nosmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nos_report(void){ps("[NOS] Nospp: ");pi(st.n_nosp);ps(" PCS=");pi(st.t_f1);ps("\nNose: ");pi(st.n_nose);ps(" PCS=");pi(st.t_f2);ps("\nNosv: ");pi(st.n_nos2);ps(" PCS=");pi(st.t_f3);ps("\nNosc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nos_state(void){ps("[NOS] Nospp=");pi(st.n_nosp);ps(" Nose=");pi(st.n_nose);ps(" Nosv=");pi(st.n_nos2);ps(" Nosc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nostholinia Admin Demo ===\n\n");nos_init();
ps("Nostholinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nos_planning(t,c,1162+(i*17),1151+(i*14),1131+(i*10),1113+(i*6),2020+(i%5));}
ps("\nNostholinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nos_execution(t,c,1151+(i*15),1140+(i*12),1122+(i*8),1109+(i*5),2021+(i%4));}
ps("\nNostholinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nos_evaluation(t,c,1143+(i*13),1132+(i*10),1116+(i*7),1105+(i*4),2022+(i%3));}
ps("\nNostholinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nos_accessory(t,c,1135+(i*11),1126+(i*9),1112+(i*6),1102+(i*3),2023+(i%2));}
ps("\nNostholinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nos_market(t,c,1129+(i*9),1120+(i*7),1107+(i*5),1099+(i*3),2024);}
ps("\n");nos_report();nos_state();ps("\n=== Demo Complete ===\n");return 0;}
