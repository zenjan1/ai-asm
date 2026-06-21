/* opopanax_admin: Opopanax management technology administration (v1.0)
 * Opopanax planning, opopanax execution, opopanax evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} opo_t;
typedef struct{int n_opop,n_opoe,n_opo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} opo_state_t;
static opo_t opops[N],oposs[N-2],opovss[N-4],opoas[N-6],opomks[N-6]; static opo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(opo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;opo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OPO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int opo_init(void){if(init)return -1;st.n_opop=0;st.n_opoe=0;st.n_opo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)opops[i].active=0;for(int i=0;i<N-2;i++)oposs[i].active=0;for(int i=0;i<N-4;i++)opovss[i].active=0;for(int i=0;i<N-6;i++)opoas[i].active=0;for(int i=0;i<N-6;i++)opomks[i].active=0;init=1;ps("[OPO] Opopanax initialized\n");return 0;}
int opo_planning(int t,int c,int a,int b,int d,int e,int y){return add(opops,&st.n_opop,&st.t_f1,N,t,c,a,b,d,e,y);}
int opo_execution(int t,int c,int a,int b,int d,int e,int y){return add(oposs,&st.n_opoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int opo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(opovss,&st.n_opo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int opo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(opoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int opo_market(int t,int c,int a,int b,int d,int e,int y){return add(opomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void opo_report(void){ps("[OPO] Opopp: ");pi(st.n_opop);ps(" PCS=");pi(st.t_f1);ps("\nOpoe: ");pi(st.n_opoe);ps(" PCS=");pi(st.t_f2);ps("\nOpov: ");pi(st.n_opo2);ps(" PCS=");pi(st.t_f3);ps("\nOpoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void opo_state(void){ps("[OPO] Opopp=");pi(st.n_opop);ps(" Opoe=");pi(st.n_opoe);ps(" Opov=");pi(st.n_opo2);ps(" Opoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Opopanax Admin Demo ===\n\n");opo_init();
ps("Opopanax planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;opo_planning(t,c,1077+(i*17),1066+(i*14),1046+(i*10),1028+(i*6),2020+(i%5));}
ps("\nOpopanax execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;opo_execution(t,c,1066+(i*15),1055+(i*12),1037+(i*8),1024+(i*5),2021+(i%4));}
ps("\nOpopanax evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;opo_evaluation(t,c,1058+(i*13),1047+(i*10),1031+(i*7),1020+(i*4),2022+(i%3));}
ps("\nOpopanax accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opo_accessory(t,c,1050+(i*11),1041+(i*9),1027+(i*6),1017+(i*3),2023+(i%2));}
ps("\nOpopanax marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opo_market(t,c,1044+(i*9),1035+(i*7),1022+(i*5),1014+(i*3),2024);}
ps("\n");opo_report();opo_state();ps("\n=== Demo Complete ===\n");return 0;}
