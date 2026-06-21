/* robinia_admin: Robinia management technology administration (v1.0)
 * Robinia planning, robinia execution, robinia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rbn_t;
typedef struct{int n_rbp,n_rbe,n_rbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rbn_state_t;
static rbn_t rbnps[N],rbnss[N-2],rbnvss[N-4],rbnas[N-6],rbnmks[N-6]; static rbn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rbn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rbn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RBN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rbn_init(void){if(init)return -1;st.n_rbp=0;st.n_rbe=0;st.n_rbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rbnps[i].active=0;for(int i=0;i<N-2;i++)rbnss[i].active=0;for(int i=0;i<N-4;i++)rbnvss[i].active=0;for(int i=0;i<N-6;i++)rbnas[i].active=0;for(int i=0;i<N-6;i++)rbnmks[i].active=0;init=1;ps("[RBN] Robinia initialized\n");return 0;}
int rbn_planning(int t,int c,int a,int b,int d,int e,int y){return add(rbnps,&st.n_rbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rbn_execution(int t,int c,int a,int b,int d,int e,int y){return add(rbnss,&st.n_rbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rbn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(rbnvss,&st.n_rbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rbn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(rbnas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rbn_market(int t,int c,int a,int b,int d,int e,int y){return add(rbnmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rbn_report(void){ps("[RBN] Rbp: ");pi(st.n_rbp);ps(" PCS=");pi(st.t_f1);ps("\nRbe: ");pi(st.n_rbe);ps(" PCS=");pi(st.t_f2);ps("\nRbv: ");pi(st.n_rbv);ps(" PCS=");pi(st.t_f3);ps("\nRbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rbn_state(void){ps("[RBN] Rbp=");pi(st.n_rbp);ps(" Rbe=");pi(st.n_rbe);ps(" Rbv=");pi(st.n_rbv);ps(" Rbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Robinia Admin Demo ===\n\n");rbn_init();
ps("Robinia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rbn_planning(t,c,858+(i*17),847+(i*14),827+(i*10),809+(i*6),2020+(i%5));}
ps("\nRobinia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rbn_execution(t,c,847+(i*15),836+(i*12),818+(i*8),805+(i*5),2021+(i%4));}
ps("\nRobinia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rbn_evaluation(t,c,839+(i*13),828+(i*10),812+(i*7),801+(i*4),2022+(i%3));}
ps("\nRobinia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbn_accessory(t,c,831+(i*11),822+(i*9),808+(i*6),798+(i*3),2023+(i%2));}
ps("\nRobinia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbn_market(t,c,825+(i*9),816+(i*7),803+(i*5),795+(i*3),2024);}
ps("\n");rbn_report();rbn_state();ps("\n=== Demo Complete ===\n");return 0;}
