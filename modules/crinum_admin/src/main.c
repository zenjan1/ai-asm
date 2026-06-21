/* crinum_admin: Crinum management technology administration (v1.0)
 * Crinum planning, crinum execution, crinum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cri_t;
typedef struct{int n_crp,n_cre,n_crv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cri_state_t;
static cri_t crps[N],cres[N-2],crvs[N-4],acs[N-6],mks[N-6]; static cri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CRI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cri_init(void){if(init)return -1;st.n_crp=0;st.n_cre=0;st.n_crv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crps[i].active=0;for(int i=0;i<N-2;i++)cres[i].active=0;for(int i=0;i<N-4;i++)crvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CRI] Crinum initialized\n");return 0;}
int cri_planning(int t,int c,int a,int b,int d,int e,int y){return add(crps,&st.n_crp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cri_execution(int t,int c,int a,int b,int d,int e,int y){return add(cres,&st.n_cre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(crvs,&st.n_crv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cri_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cri_report(void){ps("[CRI] Crp: ");pi(st.n_crp);ps(" PCS=");pi(st.t_f1);ps("\nCre: ");pi(st.n_cre);ps(" PCS=");pi(st.t_f2);ps("\nCrv: ");pi(st.n_crv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cri_state(void){ps("[CRI] Crp=");pi(st.n_crp);ps(" Cre=");pi(st.n_cre);ps(" Crv=");pi(st.n_crv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Crinum Admin Demo ===\n\n");cri_init();
ps("Crinum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cri_planning(t,c,619+(i*17),608+(i*14),588+(i*10),570+(i*6),2020+(i%5));}
ps("\nCrinum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cri_execution(t,c,608+(i*15),597+(i*12),579+(i*8),566+(i*5),2021+(i%4));}
ps("\nCrinum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cri_evaluation(t,c,600+(i*13),589+(i*10),573+(i*7),562+(i*4),2022+(i%3));}
ps("\nCrinum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cri_accessory(t,c,592+(i*11),583+(i*9),569+(i*6),559+(i*3),2023+(i%2));}
ps("\nCrinum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cri_market(t,c,586+(i*9),577+(i*7),564+(i*5),556+(i*3),2024);}
ps("\n");cri_report();cri_state();ps("\n=== Demo Complete ===\n");return 0;}
