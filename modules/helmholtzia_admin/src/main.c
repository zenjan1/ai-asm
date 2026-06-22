/* helmholtzia_admin: Helmholtzia management technology administration (v1.0)
 * Helmholzia planning, helmholtzia execution, helmholtzia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} helm_t;
typedef struct{int n_helmp,n_helm,n_helm2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} helm_state_t;
static helm_t helms[N],helme[N-2],helm2[N-4],helmac[N-6],helmam[N-6]; static helm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(helm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;helm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HELM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int helm_init(void){if(init)return -1;st.n_helmp=0;st.n_helm=0;st.n_helm2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)helms[i].active=0;for(int i=0;i<N-2;i++)helme[i].active=0;for(int i=0;i<N-4;i++)helm2[i].active=0;for(int i=0;i<N-6;i++)helmac[i].active=0;for(int i=0;i<N-6;i++)helmam[i].active=0;init=1;ps("[HELM] Helmholzia initialized\n");return 0;}
int helm_planning(int t,int c,int a,int b,int d,int e,int y){return add(helms,&st.n_helmp,&st.t_f1,N,t,c,a,b,d,e,y);}
int helm_execution(int t,int c,int a,int b,int d,int e,int y){return add(helme,&st.n_helm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int helm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(helm2,&st.n_helm2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int helm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(helmac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int helm_market(int t,int c,int a,int b,int d,int e,int y){return add(helmam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void helm_report(void){ps("[HELM] Helmp: ");pi(st.n_helmp);ps(" PCS=");pi(st.t_f1);ps("\nHelme: ");pi(st.n_helm);ps(" PCS=");pi(st.t_f2);ps("\nHelm2: ");pi(st.n_helm2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void helm_state(void){ps("[HELM] Helmp=");pi(st.n_helmp);ps(" Helm=");pi(st.n_helm);ps(" Helm2=");pi(st.n_helm2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Helmholzia Admin Demo ===\n\n");helm_init();
ps("Helmholzia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;helm_planning(t,c,1527+(i*17),1516+(i*14),1496+(i*10),1478+(i*6),2020+(i%5));}
ps("\nHelmholzia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;helm_execution(t,c,1516+(i*15),1505+(i*12),1487+(i*8),1474+(i*5),2021+(i%4));}
ps("\nHelmholzia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;helm_evaluation(t,c,1508+(i*13),1497+(i*10),1481+(i*7),1470+(i*4),2022+(i%3));}
ps("\nHelmholzia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;helm_accessory(t,c,1500+(i*11),1491+(i*9),1477+(i*6),1467+(i*3),2023+(i%2));}
ps("\nHelmholzia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;helm_market(t,c,1494+(i*9),1485+(i*7),1472+(i*5),1464+(i*3),2024);}
ps("\n");helm_report();helm_state();ps("\n=== Demo Complete ===\n");return 0;}
