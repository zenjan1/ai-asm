/* narcissus_admin: Narcissus management technology administration (v1.0)
 * Narcissus planning, narcissus execution, narcissus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nr_t;
typedef struct{int n_nrp,n_nre,n_nrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nr_state_t;
static nr_t nrps[N],nrss[N-2],nrvss[N-4],nras[N-6],nrmks[N-6]; static nr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NRS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nr_init(void){if(init)return -1;st.n_nrp=0;st.n_nre=0;st.n_nrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nrps[i].active=0;for(int i=0;i<N-2;i++)nrss[i].active=0;for(int i=0;i<N-4;i++)nrvss[i].active=0;for(int i=0;i<N-6;i++)nras[i].active=0;for(int i=0;i<N-6;i++)nrmks[i].active=0;init=1;ps("[NRS] Narcissus initialized\n");return 0;}
int nr_planning(int t,int c,int a,int b,int d,int e,int y){return add(nrps,&st.n_nrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nr_execution(int t,int c,int a,int b,int d,int e,int y){return add(nrss,&st.n_nre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nrvss,&st.n_nrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nr_market(int t,int c,int a,int b,int d,int e,int y){return add(nrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nr_report(void){ps("[NRS] Nrp: ");pi(st.n_nrp);ps(" PCS=");pi(st.t_f1);ps("\nNre: ");pi(st.n_nre);ps(" PCS=");pi(st.t_f2);ps("\nNrv: ");pi(st.n_nrv);ps(" PCS=");pi(st.t_f3);ps("\nNrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nr_state(void){ps("[NRS] Nrp=");pi(st.n_nrp);ps(" Nre=");pi(st.n_nre);ps(" Nrv=");pi(st.n_nrv);ps(" Nrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Narcissus Admin Demo ===\n\n");nr_init();
ps("Narcissus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nr_planning(t,c,648+(i*17),637+(i*14),617+(i*10),599+(i*6),2020+(i%5));}
ps("\nNarcissus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nr_execution(t,c,637+(i*15),626+(i*12),608+(i*8),595+(i*5),2021+(i%4));}
ps("\nNarcissus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nr_evaluation(t,c,629+(i*13),618+(i*10),602+(i*7),591+(i*4),2022+(i%3));}
ps("\nNarcissus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nr_accessory(t,c,621+(i*11),612+(i*9),598+(i*6),588+(i*3),2023+(i%2));}
ps("\nNarcissus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nr_market(t,c,615+(i*9),606+(i*7),593+(i*5),585+(i*3),2024);}
ps("\n");nr_report();nr_state();ps("\n=== Demo Complete ===\n");return 0;}
