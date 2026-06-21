/* parrotia_admin: Parrotia management technology administration (v1.0)
 * Parrotia planning, parrotia execution, parrotia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} prr_t;
typedef struct{int n_prp,n_pre,n_prv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} prr_state_t;
static prr_t prrps[N],prrss[N-2],prrvss[N-4],prras[N-6],prrmks[N-6]; static prr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(prr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;prr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PRR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int prr_init(void){if(init)return -1;st.n_prp=0;st.n_pre=0;st.n_prv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prrps[i].active=0;for(int i=0;i<N-2;i++)prrss[i].active=0;for(int i=0;i<N-4;i++)prrvss[i].active=0;for(int i=0;i<N-6;i++)prras[i].active=0;for(int i=0;i<N-6;i++)prrmks[i].active=0;init=1;ps("[PRR] Parrotia initialized\n");return 0;}
int prr_planning(int t,int c,int a,int b,int d,int e,int y){return add(prrps,&st.n_prp,&st.t_f1,N,t,c,a,b,d,e,y);}
int prr_execution(int t,int c,int a,int b,int d,int e,int y){return add(prrss,&st.n_pre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int prr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(prrvss,&st.n_prv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int prr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(prras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int prr_market(int t,int c,int a,int b,int d,int e,int y){return add(prrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void prr_report(void){ps("[PRR] Prp: ");pi(st.n_prp);ps(" PCS=");pi(st.t_f1);ps("\nPre: ");pi(st.n_pre);ps(" PCS=");pi(st.t_f2);ps("\nPrv: ");pi(st.n_prv);ps(" PCS=");pi(st.t_f3);ps("\nPrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void prr_state(void){ps("[PRR] Prp=");pi(st.n_prp);ps(" Pre=");pi(st.n_pre);ps(" Prv=");pi(st.n_prv);ps(" Prc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Parrotia Admin Demo ===\n\n");prr_init();
ps("Parrotia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;prr_planning(t,c,851+(i*17),840+(i*14),820+(i*10),802+(i*6),2020+(i%5));}
ps("\nParrotia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;prr_execution(t,c,840+(i*15),829+(i*12),811+(i*8),798+(i*5),2021+(i%4));}
ps("\nParrotia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;prr_evaluation(t,c,832+(i*13),821+(i*10),805+(i*7),794+(i*4),2022+(i%3));}
ps("\nParrotia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prr_accessory(t,c,824+(i*11),815+(i*9),801+(i*6),791+(i*3),2023+(i%2));}
ps("\nParrotia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;prr_market(t,c,818+(i*9),809+(i*7),796+(i*5),788+(i*3),2024);}
ps("\n");prr_report();prr_state();ps("\n=== Demo Complete ===\n");return 0;}
