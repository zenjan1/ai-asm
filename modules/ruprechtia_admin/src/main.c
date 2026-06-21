/* ruprechtia_admin: Ruprechtia management technology administration (v1.0)
 * Ruprechtia planning, ruprechtia execution, ruprechtia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rup_t;
typedef struct{int n_rupp,n_rupe,n_rup2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rup_state_t;
static rup_t rupps[N],rupss[N-2],rupvss[N-4],rupas[N-6],rupmks[N-6]; static rup_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rup_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rup_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rup_init(void){if(init)return -1;st.n_rupp=0;st.n_rupe=0;st.n_rup2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rupps[i].active=0;for(int i=0;i<N-2;i++)rupss[i].active=0;for(int i=0;i<N-4;i++)rupvss[i].active=0;for(int i=0;i<N-6;i++)rupas[i].active=0;for(int i=0;i<N-6;i++)rupmks[i].active=0;init=1;ps("[RUP] Ruprechtia initialized\n");return 0;}
int rup_planning(int t,int c,int a,int b,int d,int e,int y){return add(rupps,&st.n_rupp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rup_execution(int t,int c,int a,int b,int d,int e,int y){return add(rupss,&st.n_rupe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rup_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rupvss,&st.n_rup2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rup_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rupas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rup_market(int t,int c,int a,int b,int d,int e,int y){return add(rupmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rup_report(void){ps("[RUP] rupp: ");pi(st.n_rupp);ps(" PCS=");pi(st.t_f1);ps("\nrupe: ");pi(st.n_rupe);ps(" PCS=");pi(st.t_f2);ps("\nrupv: ");pi(st.n_rup2);ps(" PCS=");pi(st.t_f3);ps("\nrupc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rup_state(void){ps("[RUP] rupp=");pi(st.n_rupp);ps(" rupe=");pi(st.n_rupe);ps(" rupv=");pi(st.n_rup2);ps(" rupc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ruprechtia Admin Demo ===\n\n");rup_init();
ps("Ruprechtia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rup_planning(t,c,1096+(i*17),1085+(i*14),1065+(i*10),1047+(i*6),2020+(i%5));}
ps("\nRuprechtia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rup_execution(t,c,1085+(i*15),1074+(i*12),1056+(i*8),1043+(i*5),2021+(i%4));}
ps("\nRuprechtia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rup_evaluation(t,c,1077+(i*13),1066+(i*10),1050+(i*7),1039+(i*4),2022+(i%3));}
ps("\nRuprechtia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rup_accessory(t,c,1069+(i*11),1060+(i*9),1046+(i*6),1036+(i*3),2023+(i%2));}
ps("\nRuprechtia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rup_market(t,c,1063+(i*9),1054+(i*7),1041+(i*5),1033+(i*3),2024);}
ps("\n");rup_report();rup_state();ps("\n=== Demo Complete ===\n");return 0;}
