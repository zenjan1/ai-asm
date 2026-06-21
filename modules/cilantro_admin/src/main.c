/* cilantro_admin: Cilantro management technology administration (v1.0)
 * Cilantro planning, cilantro execution, cilantro evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ci_t;
typedef struct{int n_cip,n_cie,n_civ,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ci_state_t;
static ci_t cips[N],ciss[N-2],civss[N-4],cias[N-6],cimks[N-6]; static ci_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ci_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ci_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ci_init(void){if(init)return -1;st.n_cip=0;st.n_cie=0;st.n_civ=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cips[i].active=0;for(int i=0;i<N-2;i++)ciss[i].active=0;for(int i=0;i<N-4;i++)civss[i].active=0;for(int i=0;i<N-6;i++)cias[i].active=0;for(int i=0;i<N-6;i++)cimks[i].active=0;init=1;ps("[CIL] Cilantro initialized\n");return 0;}
int ci_planning(int t,int c,int a,int b,int d,int e,int y){return add(cips,&st.n_cip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ci_execution(int t,int c,int a,int b,int d,int e,int y){return add(ciss,&st.n_cie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ci_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(civss,&st.n_civ,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ci_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ci_market(int t,int c,int a,int b,int d,int e,int y){return add(cimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ci_report(void){ps("[CIL] Cip: ");pi(st.n_cip);ps(" PCS=");pi(st.t_f1);ps("\nCie: ");pi(st.n_cie);ps(" PCS=");pi(st.t_f2);ps("\nCiv: ");pi(st.n_civ);ps(" PCS=");pi(st.t_f3);ps("\nCic: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ci_state(void){ps("[CIL] Cip=");pi(st.n_cip);ps(" Cie=");pi(st.n_cie);ps(" Civ=");pi(st.n_civ);ps(" Cic=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cilantro Admin Demo ===\n\n");ci_init();
ps("Cilantro planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ci_planning(t,c,656+(i*17),645+(i*14),625+(i*10),607+(i*6),2020+(i%5));}
ps("\nCilantro execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ci_execution(t,c,645+(i*15),634+(i*12),616+(i*8),603+(i*5),2021+(i%4));}
ps("\nCilantro evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ci_evaluation(t,c,637+(i*13),626+(i*10),610+(i*7),599+(i*4),2022+(i%3));}
ps("\nCilantro accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ci_accessory(t,c,629+(i*11),620+(i*9),606+(i*6),596+(i*3),2023+(i%2));}
ps("\nCilantro marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ci_market(t,c,623+(i*9),614+(i*7),601+(i*5),593+(i*3),2024);}
ps("\n");ci_report();ci_state();ps("\n=== Demo Complete ===\n");return 0;}
