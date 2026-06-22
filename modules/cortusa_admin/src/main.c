/* cortusa_admin: Cortusa management technology administration (v1.0)
 * Cortusa planning, cortusa execution, cortusa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cort_t;
typedef struct{int n_cortp,n_cort,n_cort2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cort_state_t;
static cort_t corts[N],corte[N-2],cort2[N-4],cortac[N-6],cortam[N-6]; static cort_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cort_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cort_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CORT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cort_init(void){if(init)return -1;st.n_cortp=0;st.n_cort=0;st.n_cort2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)corts[i].active=0;for(int i=0;i<N-2;i++)corte[i].active=0;for(int i=0;i<N-4;i++)cort2[i].active=0;for(int i=0;i<N-6;i++)cortac[i].active=0;for(int i=0;i<N-6;i++)cortam[i].active=0;init=1;ps("[CORT] Cortusa initialized\n");return 0;}
int cort_planning(int t,int c,int a,int b,int d,int e,int y){return add(corts,&st.n_cortp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cort_execution(int t,int c,int a,int b,int d,int e,int y){return add(corte,&st.n_cort,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cort_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cort2,&st.n_cort2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cort_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cortac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cort_market(int t,int c,int a,int b,int d,int e,int y){return add(cortam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cort_report(void){ps("[CORT] Rtp: ");pi(st.n_cortp);ps(" PCS=");pi(st.t_f1);ps("\nRte: ");pi(st.n_cort);ps(" PCS=");pi(st.t_f2);ps("\nRt2: ");pi(st.n_cort2);ps(" PCS=");pi(st.t_f3);ps("\nRac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cort_state(void){ps("[CORT] Rtp=");pi(st.n_cortp);ps(" Rt=");pi(st.n_cort);ps(" Rt2=");pi(st.n_cort2);ps(" Rac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cortusa Admin Demo ===\n\n");cort_init();
ps("Cortusa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cort_planning(t,c,1481+(i*17),1470+(i*14),1450+(i*10),1432+(i*6),2020+(i%5));}
ps("\nCortusa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cort_execution(t,c,1470+(i*15),1459+(i*12),1441+(i*8),1428+(i*5),2021+(i%4));}
ps("\nCortusa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cort_evaluation(t,c,1462+(i*13),1451+(i*10),1435+(i*7),1424+(i*4),2022+(i%3));}
ps("\nCortusa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cort_accessory(t,c,1454+(i*11),1445+(i*9),1431+(i*6),1421+(i*3),2023+(i%2));}
ps("\nCortusa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cort_market(t,c,1448+(i*9),1439+(i*7),1426+(i*5),1418+(i*3),2024);}
ps("\n");cort_report();cort_state();ps("\n=== Demo Complete ===\n");return 0;}
