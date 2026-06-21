/* libertia_admin: Libertia management technology administration (v1.0)
 * Libertia planning, libertia execution, libertia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lbt_t;
typedef struct{int n_lbp,n_lbe,n_lbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lbt_state_t;
static lbt_t lbtps[N],lbtess[N-2],lbtvss[N-4],lbtas[N-6],lbtmks[N-6]; static lbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lbt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lbt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LBT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lbt_init(void){if(init)return -1;st.n_lbp=0;st.n_lbe=0;st.n_lbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lbtps[i].active=0;for(int i=0;i<N-2;i++)lbtess[i].active=0;for(int i=0;i<N-4;i++)lbtvss[i].active=0;for(int i=0;i<N-6;i++)lbtas[i].active=0;for(int i=0;i<N-6;i++)lbtmks[i].active=0;init=1;ps("[LBT] Libertia initialized\n");return 0;}
int lbt_planning(int t,int c,int a,int b,int d,int e,int y){return add(lbtps,&st.n_lbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lbt_execution(int t,int c,int a,int b,int d,int e,int y){return add(lbtess,&st.n_lbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lbt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lbtvss,&st.n_lbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lbt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lbtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lbt_market(int t,int c,int a,int b,int d,int e,int y){return add(lbtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lbt_report(void){ps("[LBT] Lbp: ");pi(st.n_lbp);ps(" PCS=");pi(st.t_f1);ps("\nLbe: ");pi(st.n_lbe);ps(" PCS=");pi(st.t_f2);ps("\nLbv: ");pi(st.n_lbv);ps(" PCS=");pi(st.t_f3);ps("\nLbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lbt_state(void){ps("[LBT] Lbp=");pi(st.n_lbp);ps(" Lbe=");pi(st.n_lbe);ps(" Lbv=");pi(st.n_lbv);ps(" Lbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Libertia Admin Demo ===\n\n");lbt_init();
ps("Libertia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lbt_planning(t,c,833+(i*17),822+(i*14),802+(i*10),784+(i*6),2020+(i%5));}
ps("\nLibertia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lbt_execution(t,c,822+(i*15),811+(i*12),793+(i*8),780+(i*5),2021+(i%4));}
ps("\nLibertia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lbt_evaluation(t,c,814+(i*13),803+(i*10),787+(i*7),776+(i*4),2022+(i%3));}
ps("\nLibertia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lbt_accessory(t,c,806+(i*11),797+(i*9),783+(i*6),773+(i*3),2023+(i%2));}
ps("\nLibertia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lbt_market(t,c,800+(i*9),791+(i*7),778+(i*5),770+(i*3),2024);}
ps("\n");lbt_report();lbt_state();ps("\n=== Demo Complete ===\n");return 0;}
