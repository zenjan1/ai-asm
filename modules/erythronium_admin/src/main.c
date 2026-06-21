/* erythronium_admin: Erythronium management technology administration (v1.0)
 * Erythronium planning, erythronium execution, erythronium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ery_t;
typedef struct{int n_erp,n_ere,n_erv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ery_state_t;
static ery_t erps[N],eres[N-2],ervs[N-4],acs[N-6],mks[N-6]; static ery_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ery_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ery_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ERY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ery_init(void){if(init)return -1;st.n_erp=0;st.n_ere=0;st.n_erv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)erps[i].active=0;for(int i=0;i<N-2;i++)eres[i].active=0;for(int i=0;i<N-4;i++)ervs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ERY] Erythronium initialized\n");return 0;}
int ery_planning(int t,int c,int a,int b,int d,int e,int y){return add(erps,&st.n_erp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ery_execution(int t,int c,int a,int b,int d,int e,int y){return add(eres,&st.n_ere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ery_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ervs,&st.n_erv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ery_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ery_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ery_report(void){ps("[ERY] Erp: ");pi(st.n_erp);ps(" PCS=");pi(st.t_f1);ps("\nEre: ");pi(st.n_ere);ps(" PCS=");pi(st.t_f2);ps("\nErv: ");pi(st.n_erv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ery_state(void){ps("[ERY] Erp=");pi(st.n_erp);ps(" Ere=");pi(st.n_ere);ps(" Erv=");pi(st.n_erv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Erythronium Admin Demo ===\n\n");ery_init();
ps("Erythronium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ery_planning(t,c,614+(i*17),603+(i*14),583+(i*10),565+(i*6),2020+(i%5));}
ps("\nErythronium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ery_execution(t,c,603+(i*15),592+(i*12),574+(i*8),561+(i*5),2021+(i%4));}
ps("\nErythronium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ery_evaluation(t,c,595+(i*13),584+(i*10),568+(i*7),557+(i*4),2022+(i%3));}
ps("\nErythronium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ery_accessory(t,c,587+(i*11),578+(i*9),564+(i*6),554+(i*3),2023+(i%2));}
ps("\nErythronium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ery_market(t,c,581+(i*9),572+(i*7),559+(i*5),551+(i*3),2024);}
ps("\n");ery_report();ery_state();ps("\n=== Demo Complete ===\n");return 0;}
