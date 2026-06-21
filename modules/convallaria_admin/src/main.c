/* convallaria_admin: Convallaria management technology administration (v1.0)
 * Convallaria planning, convallaria execution, convallaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cvl_t;
typedef struct{int n_cvp,n_cve,n_cvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cvl_state_t;
static cvl_t cvps[N],cvss[N-2],cvvss[N-4],cvas[N-6],cvmks[N-6]; static cvl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cvl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cvl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CVL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cvl_init(void){if(init)return -1;st.n_cvp=0;st.n_cve=0;st.n_cvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cvps[i].active=0;for(int i=0;i<N-2;i++)cvss[i].active=0;for(int i=0;i<N-4;i++)cvvss[i].active=0;for(int i=0;i<N-6;i++)cvas[i].active=0;for(int i=0;i<N-6;i++)cvmks[i].active=0;init=1;ps("[CVL] Convallaria initialized\n");return 0;}
int cvl_planning(int t,int c,int a,int b,int d,int e,int y){return add(cvps,&st.n_cvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cvl_execution(int t,int c,int a,int b,int d,int e,int y){return add(cvss,&st.n_cve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cvl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvvss,&st.n_cvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cvl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cvas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cvl_market(int t,int c,int a,int b,int d,int e,int y){return add(cvmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cvl_report(void){ps("[CVL] Cvp: ");pi(st.n_cvp);ps(" PCS=");pi(st.t_f1);ps("\nCve: ");pi(st.n_cve);ps(" PCS=");pi(st.t_f2);ps("\nCvv: ");pi(st.n_cvv);ps(" PCS=");pi(st.t_f3);ps("\nCvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cvl_state(void){ps("[CVL] Cvp=");pi(st.n_cvp);ps(" Cve=");pi(st.n_cve);ps(" Cvv=");pi(st.n_cvv);ps(" Cvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Convallaria Admin Demo ===\n\n");cvl_init();
ps("Convallaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cvl_planning(t,c,815+(i*17),804+(i*14),784+(i*10),766+(i*6),2020+(i%5));}
ps("\nConvallaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cvl_execution(t,c,804+(i*15),793+(i*12),775+(i*8),762+(i*5),2021+(i%4));}
ps("\nConvallaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cvl_evaluation(t,c,796+(i*13),785+(i*10),771+(i*7),760+(i*4),2022+(i%3));}
ps("\nConvallaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvl_accessory(t,c,788+(i*11),779+(i*9),765+(i*6),755+(i*3),2023+(i%2));}
ps("\nConvallaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cvl_market(t,c,782+(i*9),773+(i*7),760+(i*5),752+(i*3),2024);}
ps("\n");cvl_report();cvl_state();ps("\n=== Demo Complete ===\n");return 0;}
