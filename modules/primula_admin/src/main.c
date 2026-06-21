/* primula_admin: Primula management technology administration (v1.0)
 * Primula planning, primula execution, primula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prl_t;
typedef struct{int n_prp,n_pre,n_prv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prl_state_t;
static prl_t prps[N],prss[N-2],prvss[N-4],pras[N-6],prmks[N-6]; static prl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prm_init(void){if(init)return -1;st.n_prp=0;st.n_pre=0;st.n_prv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prps[i].active=0;for(int i=0;i<N-2;i++)prss[i].active=0;for(int i=0;i<N-4;i++)prvss[i].active=0;for(int i=0;i<N-6;i++)pras[i].active=0;for(int i=0;i<N-6;i++)prmks[i].active=0;init=1;ps("[PRM] Primula initialized\n");return 0;}
int prm_planning(int t,int c,int a,int b,int d,int e,int y){return add(prps,&st.n_prp,&st.t_f1,N,t,c,a,b,d,e,y);}
int prm_execution(int t,int c,int a,int b,int d,int e,int y){return add(prss,&st.n_pre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prm_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(prvss,&st.n_prv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(pras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prm_market(int t,int c,int a,int b,int d,int e,int y){return add(prmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prm_report(void){ps("[PRM] Prp: ");pi(st.n_prp);ps(" PCS=");pi(st.t_f1);ps("\nPre: ");pi(st.n_pre);ps(" PCS=");pi(st.t_f2);ps("\nPrv: ");pi(st.n_prv);ps(" PCS=");pi(st.t_f3);ps("\nPrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prm_state(void){ps("[PRM] Prp=");pi(st.n_prp);ps(" Pre=");pi(st.n_pre);ps(" Prv=");pi(st.n_prv);ps(" Prc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Primula Admin Demo ===\n\n");prm_init();
ps("Primula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prm_planning(t,c,798+(i*17),787+(i*14),767+(i*10),749+(i*6),2020+(i%5));}
ps("\nPrimula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prm_execution(t,c,787+(i*15),776+(i*12),758+(i*8),745+(i*5),2021+(i%4));}
ps("\nPrimula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prm_evaluation(t,c,779+(i*13),768+(i*10),754+(i*7),743+(i*4),2022+(i%3));}
ps("\nPrimula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prm_accessory(t,c,771+(i*11),762+(i*9),748+(i*6),738+(i*3),2023+(i%2));}
ps("\nPrimula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prm_market(t,c,765+(i*9),756+(i*7),743+(i*5),735+(i*3),2024);}
ps("\n");prm_report();prm_state();ps("\n=== Demo Complete ===\n");return 0;}
