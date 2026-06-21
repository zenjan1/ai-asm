/* parsley_admin: Parsley management technology administration (v1.0)
 * Parsley planning, parsley execution, parsley evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pr_t;
typedef struct{int n_prp,n_pre,n_prv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pr_state_t;
static pr_t prps[N],prss[N-2],prvss[N-4],pras[N-6],prmks[N-6]; static pr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pr_init(void){if(init)return -1;st.n_prp=0;st.n_pre=0;st.n_prv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prps[i].active=0;for(int i=0;i<N-2;i++)prss[i].active=0;for(int i=0;i<N-4;i++)prvss[i].active=0;for(int i=0;i<N-6;i++)pras[i].active=0;for(int i=0;i<N-6;i++)prmks[i].active=0;init=1;ps("[PRS] Parsley initialized\n");return 0;}
int pr_planning(int t,int c,int a,int b,int d,int e,int y){return add(prps,&st.n_prp,&st.t_f1,N,t,c,a,b,d,e,y);}
int pr_execution(int t,int c,int a,int b,int d,int e,int y){return add(prss,&st.n_pre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(prvss,&st.n_prv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pr_market(int t,int c,int a,int b,int d,int e,int y){return add(prmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pr_report(void){ps("[PRS] Prp: ");pi(st.n_prp);ps(" PCS=");pi(st.t_f1);ps("\nPre: ");pi(st.n_pre);ps(" PCS=");pi(st.t_f2);ps("\nPrv: ");pi(st.n_prv);ps(" PCS=");pi(st.t_f3);ps("\nPrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pr_state(void){ps("[PRS] Prp=");pi(st.n_prp);ps(" Pre=");pi(st.n_pre);ps(" Prv=");pi(st.n_prv);ps(" Prc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Parsley Admin Demo ===\n\n");pr_init();
ps("Parsley planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pr_planning(t,c,655+(i*17),644+(i*14),624+(i*10),606+(i*6),2020+(i%5));}
ps("\nParsley execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pr_execution(t,c,644+(i*15),633+(i*12),615+(i*8),602+(i*5),2021+(i%4));}
ps("\nParsley evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pr_evaluation(t,c,636+(i*13),625+(i*10),609+(i*7),598+(i*4),2022+(i%3));}
ps("\nParsley accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pr_accessory(t,c,628+(i*11),619+(i*9),605+(i*6),595+(i*3),2023+(i%2));}
ps("\nParsley marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pr_market(t,c,622+(i*9),613+(i*7),600+(i*5),592+(i*3),2024);}
ps("\n");pr_report();pr_state();ps("\n=== Demo Complete ===\n");return 0;}
