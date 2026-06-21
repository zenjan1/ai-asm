/* chervil_admin: Chervil management technology administration (v1.0)
 * Chervil planning, chervil execution, chervil evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cv_t;
typedef struct{int n_cvp,n_cve,n_cvv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cv_state_t;
static cv_t cvps[N],cvss[N-2],cvvss[N-4],cvas[N-6],cvmks[N-6]; static cv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cv_init(void){if(init)return -1;st.n_cvp=0;st.n_cve=0;st.n_cvv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cvps[i].active=0;for(int i=0;i<N-2;i++)cvss[i].active=0;for(int i=0;i<N-4;i++)cvvss[i].active=0;for(int i=0;i<N-6;i++)cvas[i].active=0;for(int i=0;i<N-6;i++)cvmks[i].active=0;init=1;ps("[CHV] Chervil initialized\n");return 0;}
int cv_planning(int t,int c,int a,int b,int d,int e,int y){return add(cvps,&st.n_cvp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cv_execution(int t,int c,int a,int b,int d,int e,int y){return add(cvss,&st.n_cve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cv_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cvvss,&st.n_cvv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cvas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cv_market(int t,int c,int a,int b,int d,int e,int y){return add(cvmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cv_report(void){ps("[CHV] Cvp: ");pi(st.n_cvp);ps(" PCS=");pi(st.t_f1);ps("\nCve: ");pi(st.n_cve);ps(" PCS=");pi(st.t_f2);ps("\nCvv: ");pi(st.n_cvv);ps(" PCS=");pi(st.t_f3);ps("\nCvc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cv_state(void){ps("[CHV] Cvp=");pi(st.n_cvp);ps(" Cve=");pi(st.n_cve);ps(" Cvv=");pi(st.n_cvv);ps(" Cvc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chervil Admin Demo ===\n\n");cv_init();
ps("Chervil planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cv_planning(t,c,687+(i*17),676+(i*14),656+(i*10),638+(i*6),2020+(i%5));}
ps("\nChervil execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cv_execution(t,c,676+(i*15),665+(i*12),647+(i*8),634+(i*5),2021+(i%4));}
ps("\nChervil evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cv_evaluation(t,c,668+(i*13),657+(i*10),641+(i*7),630+(i*4),2022+(i%3));}
ps("\nChervil accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cv_accessory(t,c,660+(i*11),651+(i*9),637+(i*6),627+(i*3),2023+(i%2));}
ps("\nChervil marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cv_market(t,c,654+(i*9),645+(i*7),632+(i*5),624+(i*3),2024);}
ps("\n");cv_report();cv_state();ps("\n=== Demo Complete ===\n");return 0;}
