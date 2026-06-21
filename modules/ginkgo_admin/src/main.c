/* ginkgo_admin: Ginkgo management technology administration (v1.0)
 * Ginkgo planning, ginkgo execution, ginkgo evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} gin_t;
typedef struct{int n_ginp,n_gine,n_gin2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} gin_state_t;
static gin_t ginps[N],ginss[N-2],ginvss[N-4],ginas[N-6],ginmks[N-6]; static gin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(gin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;gin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int gin_init(void){if(init)return -1;st.n_ginp=0;st.n_gine=0;st.n_gin2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ginps[i].active=0;for(int i=0;i<N-2;i++)ginss[i].active=0;for(int i=0;i<N-4;i++)ginvss[i].active=0;for(int i=0;i<N-6;i++)ginas[i].active=0;for(int i=0;i<N-6;i++)ginmks[i].active=0;init=1;ps("[GIN] Ginkgo initialized\n");return 0;}
int gin_planning(int t,int c,int a,int b,int d,int e,int y){return add(ginps,&st.n_ginp,&st.t_f1,N,t,c,a,b,d,e,y);}
int gin_execution(int t,int c,int a,int b,int d,int e,int y){return add(ginss,&st.n_gine,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int gin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ginvss,&st.n_gin2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int gin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ginas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int gin_market(int t,int c,int a,int b,int d,int e,int y){return add(ginmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void gin_report(void){ps("[GIN] Ginnp: ");pi(st.n_ginp);ps(" PCS=");pi(st.t_f1);ps("\nGine: ");pi(st.n_gine);ps(" PCS=");pi(st.t_f2);ps("\nGinv: ");pi(st.n_gin2);ps(" PCS=");pi(st.t_f3);ps("\nGinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void gin_state(void){ps("[GIN] Ginnp=");pi(st.n_ginp);ps(" Gine=");pi(st.n_gine);ps(" Ginv=");pi(st.n_gin2);ps(" Ginc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ginkgo Admin Demo ===\n\n");gin_init();
ps("Ginkgo planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;gin_planning(t,c,1011+(i*17),1000+(i*14),980+(i*10),962+(i*6),2020+(i%5));}
ps("\nGinkgo execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;gin_execution(t,c,1000+(i*15),989+(i*12),971+(i*8),958+(i*5),2021+(i%4));}
ps("\nGinkgo evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;gin_evaluation(t,c,992+(i*13),981+(i*10),965+(i*7),954+(i*4),2022+(i%3));}
ps("\nGinkgo accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gin_accessory(t,c,984+(i*11),975+(i*9),961+(i*6),951+(i*3),2023+(i%2));}
ps("\nGinkgo marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;gin_market(t,c,978+(i*9),969+(i*7),956+(i*5),948+(i*3),2024);}
ps("\n");gin_report();gin_state();ps("\n=== Demo Complete ===\n");return 0;}
