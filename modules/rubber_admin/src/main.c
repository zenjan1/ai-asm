/* rubber_admin: Rubber management technology administration (v1.0)
 * Rubber planning, rubber execution, rubber evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rub_t;
typedef struct{int n_rp,n_re,n_rv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rub_state_t;
static rub_t rps[N],res[N-2],rvs[N-4],acs[N-6],mks[N-6]; static rub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RUB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rub_init(void){if(init)return -1;st.n_rp=0;st.n_re=0;st.n_rv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rps[i].active=0;for(int i=0;i<N-2;i++)res[i].active=0;for(int i=0;i<N-4;i++)rvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RUB] Rubber initialized\n");return 0;}
int rub_planning(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rub_execution(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rub_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rvs,&st.n_rv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rub_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rub_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rub_report(void){ps("[RUB] Rp: ");pi(st.n_rp);ps(" PCS=");pi(st.t_f1);ps("\nRe: ");pi(st.n_re);ps(" PCS=");pi(st.t_f2);ps("\nRv: ");pi(st.n_rv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rub_state(void){ps("[RUB] Rp=");pi(st.n_rp);ps(" Re=");pi(st.n_re);ps(" Rv=");pi(st.n_rv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Rubber Admin Demo ===\n\n");rub_init();
ps("Rubber planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rub_planning(t,c,525+(i*17),514+(i*14),494+(i*10),476+(i*6),2020+(i%5));}
ps("\nRubber execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rub_execution(t,c,514+(i*15),503+(i*12),485+(i*8),472+(i*5),2021+(i%4));}
ps("\nRubber evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rub_evaluation(t,c,506+(i*13),495+(i*10),479+(i*7),468+(i*4),2022+(i%3));}
ps("\nRubber accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rub_accessory(t,c,498+(i*11),489+(i*9),475+(i*6),465+(i*3),2023+(i%2));}
ps("\nRubber marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rub_market(t,c,492+(i*9),483+(i*7),470+(i*5),462+(i*3),2024);}
ps("\n");rub_report();rub_state();ps("\n=== Demo Complete ===\n");return 0;}
