/* ophiopogon_admin: Ophiopogon management technology administration (v1.0)
 * Ophiopogon planning, ophiopogon execution, ophiopogon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oph_t;
typedef struct{int n_ophp,n_ophe,n_oph2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oph_state_t;
static oph_t ophps[N],ophss[N-2],ophvss[N-4],ophas[N-6],ophmks[N-6]; static oph_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oph_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oph_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OPH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oph_init(void){if(init)return -1;st.n_ophp=0;st.n_ophe=0;st.n_oph2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ophps[i].active=0;for(int i=0;i<N-2;i++)ophss[i].active=0;for(int i=0;i<N-4;i++)ophvss[i].active=0;for(int i=0;i<N-6;i++)ophas[i].active=0;for(int i=0;i<N-6;i++)ophmks[i].active=0;init=1;ps("[OPH] Ophiopogon initialized\n");return 0;}
int oph_planning(int t,int c,int a,int b,int d,int e,int y){return add(ophps,&st.n_ophp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oph_execution(int t,int c,int a,int b,int d,int e,int y){return add(ophss,&st.n_ophe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oph_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ophvss,&st.n_oph2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oph_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ophas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oph_market(int t,int c,int a,int b,int d,int e,int y){return add(ophmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oph_report(void){ps("[OPH] Ophpp: ");pi(st.n_ophp);ps(" PCS=");pi(st.t_f1);ps("\nOphe: ");pi(st.n_ophe);ps(" PCS=");pi(st.t_f2);ps("\nOphv: ");pi(st.n_oph2);ps(" PCS=");pi(st.t_f3);ps("\nOphc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oph_state(void){ps("[OPH] Ophpp=");pi(st.n_ophp);ps(" Ophe=");pi(st.n_ophe);ps(" Ophv=");pi(st.n_oph2);ps(" Ophc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ophiopogon Admin Demo ===\n\n");oph_init();
ps("Ophiopogon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oph_planning(t,c,1076+(i*17),1065+(i*14),1045+(i*10),1027+(i*6),2020+(i%5));}
ps("\nOphiopogon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oph_execution(t,c,1065+(i*15),1054+(i*12),1036+(i*8),1023+(i*5),2021+(i%4));}
ps("\nOphiopogon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oph_evaluation(t,c,1057+(i*13),1046+(i*10),1030+(i*7),1019+(i*4),2022+(i%3));}
ps("\nOphiopogon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oph_accessory(t,c,1049+(i*11),1040+(i*9),1026+(i*6),1016+(i*3),2023+(i%2));}
ps("\nOphiopogon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oph_market(t,c,1043+(i*9),1034+(i*7),1021+(i*5),1013+(i*3),2024);}
ps("\n");oph_report();oph_state();ps("\n=== Demo Complete ===\n");return 0;}
