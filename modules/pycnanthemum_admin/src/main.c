/* pycnanthemum_admin: Pycnanthemum management technology administration (v1.0)
 * Pycnanthemum planning, pycnanthemum execution, pycnanthemum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pyc_t;
typedef struct{int n_pycp,n_pyce,n_pyc2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pyc_state_t;
static pyc_t pycps[N],pycss[N-2],pycvss[N-4],pycas[N-6],pycmks[N-6]; static pyc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pyc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pyc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pyc_init(void){if(init)return -1;st.n_pycp=0;st.n_pyce=0;st.n_pyc2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pycps[i].active=0;for(int i=0;i<N-2;i++)pycss[i].active=0;for(int i=0;i<N-4;i++)pycvss[i].active=0;for(int i=0;i<N-6;i++)pycas[i].active=0;for(int i=0;i<N-6;i++)pycmks[i].active=0;init=1;ps("[PYC] Pycnanthemum initialized\n");return 0;}
int pyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(pycps,&st.n_pycp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(pycss,&st.n_pyce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(pycvss,&st.n_pyc2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pycas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pyc_market(int t,int c,int a,int b,int d,int e,int y){return add(pycmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pyc_report(void){ps("[PYC] pycp: ");pi(st.n_pycp);ps(" PCS=");pi(st.t_f1);ps("\npyce: ");pi(st.n_pyce);ps(" PCS=");pi(st.t_f2);ps("\npycv: ");pi(st.n_pyc2);ps(" PCS=");pi(st.t_f3);ps("\npycc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pyc_state(void){ps("[PYC] pycp=");pi(st.n_pycp);ps(" pyce=");pi(st.n_pyce);ps(" pycv=");pi(st.n_pyc2);ps(" pycc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pycnanthemum Admin Demo ===\n\n");pyc_init();
ps("Pycnanthemum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pyc_planning(t,c,1090+(i*17),1079+(i*14),1059+(i*10),1041+(i*6),2020+(i%5));}
ps("\nPycnanthemum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pyc_execution(t,c,1079+(i*15),1068+(i*12),1050+(i*8),1037+(i*5),2021+(i%4));}
ps("\nPycnanthemum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pyc_evaluation(t,c,1071+(i*13),1060+(i*10),1044+(i*7),1033+(i*4),2022+(i%3));}
ps("\nPycnanthemum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyc_accessory(t,c,1063+(i*11),1054+(i*9),1040+(i*6),1030+(i*3),2023+(i%2));}
ps("\nPycnanthemum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pyc_market(t,c,1057+(i*9),1048+(i*7),1035+(i*5),1027+(i*3),2024);}
ps("\n");pyc_report();pyc_state();ps("\n=== Demo Complete ===\n");return 0;}
