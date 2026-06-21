/* lemonbalm_admin: Lemonbalm management technology administration (v1.0)
 * Lemonbalm planning, lemonbalm execution, lemonbalm evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lb_t;
typedef struct{int n_lbp,n_lbe,n_lbv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lb_state_t;
static lb_t lbps[N],lbss[N-2],lbvss[N-4],lbas[N-6],lbmks[N-6]; static lb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LBM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lb_init(void){if(init)return -1;st.n_lbp=0;st.n_lbe=0;st.n_lbv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lbps[i].active=0;for(int i=0;i<N-2;i++)lbss[i].active=0;for(int i=0;i<N-4;i++)lbvss[i].active=0;for(int i=0;i<N-6;i++)lbas[i].active=0;for(int i=0;i<N-6;i++)lbmks[i].active=0;init=1;ps("[LBM] Lemonbalm initialized\n");return 0;}
int lb_planning(int t,int c,int a,int b,int d,int e,int y){return add(lbps,&st.n_lbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lb_execution(int t,int c,int a,int b,int d,int e,int y){return add(lbss,&st.n_lbe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lbvss,&st.n_lbv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lbas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lb_market(int t,int c,int a,int b,int d,int e,int y){return add(lbmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lb_report(void){ps("[LBM] Lbp: ");pi(st.n_lbp);ps(" PCS=");pi(st.t_f1);ps("\nLbe: ");pi(st.n_lbe);ps(" PCS=");pi(st.t_f2);ps("\nLbv: ");pi(st.n_lbv);ps(" PCS=");pi(st.t_f3);ps("\nLbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lb_state(void){ps("[LBM] Lbp=");pi(st.n_lbp);ps(" Lbe=");pi(st.n_lbe);ps(" Lbv=");pi(st.n_lbv);ps(" Lbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Lemonbalm Admin Demo ===\n\n");lb_init();
ps("Lemonbalm planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lb_planning(t,c,690+(i*17),679+(i*14),659+(i*10),641+(i*6),2020+(i%5));}
ps("\nLemonbalm execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lb_execution(t,c,679+(i*15),668+(i*12),650+(i*8),637+(i*5),2021+(i%4));}
ps("\nLemonbalm evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lb_evaluation(t,c,671+(i*13),660+(i*10),644+(i*7),633+(i*4),2022+(i%3));}
ps("\nLemonbalm accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lb_accessory(t,c,663+(i*11),654+(i*9),640+(i*6),630+(i*3),2023+(i%2));}
ps("\nLemonbalm marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lb_market(t,c,657+(i*9),648+(i*7),635+(i*5),627+(i*3),2024);}
ps("\n");lb_report();lb_state();ps("\n=== Demo Complete ===\n");return 0;}
