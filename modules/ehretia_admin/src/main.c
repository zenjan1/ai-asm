/* ehretia_admin: Ehretia management technology administration (v1.0)
 * Ehretia planning, ehretia execution, ehretia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ehr_t;
typedef struct{int n_ehrp,n_ehre,n_ehr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ehr_state_t;
static ehr_t ehrps[N],ehrss[N-2],ehrvss[N-4],ehras[N-6],ehrmks[N-6]; static ehr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ehr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ehr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EHR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ehr_init(void){if(init)return -1;st.n_ehrp=0;st.n_ehre=0;st.n_ehr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ehrps[i].active=0;for(int i=0;i<N-2;i++)ehrss[i].active=0;for(int i=0;i<N-4;i++)ehrvss[i].active=0;for(int i=0;i<N-6;i++)ehras[i].active=0;for(int i=0;i<N-6;i++)ehrmks[i].active=0;init=1;ps("[EHR] Ehretia initialized\n");return 0;}
int ehr_planning(int t,int c,int a,int b,int d,int e,int y){return add(ehrps,&st.n_ehrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ehr_execution(int t,int c,int a,int b,int d,int e,int y){return add(ehrss,&st.n_ehre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ehr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ehrvss,&st.n_ehr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ehr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ehras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ehr_market(int t,int c,int a,int b,int d,int e,int y){return add(ehrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ehr_report(void){ps("[EHR] Ehrp: ");pi(st.n_ehrp);ps(" PCS=");pi(st.t_f1);ps("\nEhre: ");pi(st.n_ehre);ps(" PCS=");pi(st.t_f2);ps("\nEhrv: ");pi(st.n_ehr2);ps(" PCS=");pi(st.t_f3);ps("\nEhrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ehr_state(void){ps("[EHR] Ehrp=");pi(st.n_ehrp);ps(" Ehre=");pi(st.n_ehre);ps(" Ehrv=");pi(st.n_ehr2);ps(" Ehrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ehretia Admin Demo ===\n\n");ehr_init();
ps("Ehretia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ehr_planning(t,c,1280+(i*17),1269+(i*14),1249+(i*10),1231+(i*6),2020+(i%5));}
ps("\nEhretia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ehr_execution(t,c,1269+(i*15),1258+(i*12),1240+(i*8),1227+(i*5),2021+(i%4));}
ps("\nEhretia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ehr_evaluation(t,c,1261+(i*13),1250+(i*10),1234+(i*7),1223+(i*4),2022+(i%3));}
ps("\nEhretia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ehr_accessory(t,c,1253+(i*11),1244+(i*9),1230+(i*6),1220+(i*3),2023+(i%2));}
ps("\nEhretia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ehr_market(t,c,1247+(i*9),1238+(i*7),1225+(i*5),1217+(i*3),2024);}
ps("\n");ehr_report();ehr_state();ps("\n=== Demo Complete ===\n");return 0;}
