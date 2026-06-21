/* bacopa_admin: Bacopa management technology administration (v1.0)
 * Bacopa planning, bacopa execution, bacopa evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bc_t;
typedef struct{int n_bcp,n_bce,n_bcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bc_state_t;
static bc_t bcps[N],bcss[N-2],bcvss[N-4],bcas[N-6],bcmks[N-6]; static bc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bcn_init(void){if(init)return -1;st.n_bcp=0;st.n_bce=0;st.n_bcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bcps[i].active=0;for(int i=0;i<N-2;i++)bcss[i].active=0;for(int i=0;i<N-4;i++)bcvss[i].active=0;for(int i=0;i<N-6;i++)bcas[i].active=0;for(int i=0;i<N-6;i++)bcmks[i].active=0;init=1;ps("[BCN] Bacopa initialized\n");return 0;}
int bcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(bcps,&st.n_bcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(bcss,&st.n_bce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bcvss,&st.n_bcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bcn_market(int t,int c,int a,int b,int d,int e,int y){return add(bcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bcn_report(void){ps("[BCN] Bcp: ");pi(st.n_bcp);ps(" PCS=");pi(st.t_f1);ps("\nBce: ");pi(st.n_bce);ps(" PCS=");pi(st.t_f2);ps("\nBcv: ");pi(st.n_bcv);ps(" PCS=");pi(st.t_f3);ps("\nBcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bcn_state(void){ps("[BCN] Bcp=");pi(st.n_bcp);ps(" Bce=");pi(st.n_bce);ps(" Bcv=");pi(st.n_bcv);ps(" Bcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bacopa Admin Demo ===\n\n");bcn_init();
ps("Bacopa planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bcn_planning(t,c,742+(i*17),731+(i*14),711+(i*10),693+(i*6),2020+(i%5));}
ps("\nBacopa execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bcn_execution(t,c,731+(i*15),720+(i*12),702+(i*8),689+(i*5),2021+(i%4));}
ps("\nBacopa evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bcn_evaluation(t,c,723+(i*13),712+(i*10),696+(i*7),685+(i*4),2022+(i%3));}
ps("\nBacopa accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bcn_accessory(t,c,715+(i*11),706+(i*9),692+(i*6),682+(i*3),2023+(i%2));}
ps("\nBacopa marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bcn_market(t,c,709+(i*9),700+(i*7),687+(i*5),679+(i*3),2024);}
ps("\n");bcn_report();bcn_state();ps("\n=== Demo Complete ===\n");return 0;}
