/* linaria_admin: Linaria management technology administration (v1.0)
 * Linaria planning, linaria execution, linaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lz_t;
typedef struct{int n_lrp,n_lre,n_lrv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lz_state_t;
static lz_t lrps[N],lrss[N-2],lrvss[N-4],lras[N-6],lrmks[N-6]; static lz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lz_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lz_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LNR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lr_init(void){if(init)return -1;st.n_lrp=0;st.n_lre=0;st.n_lrv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lrps[i].active=0;for(int i=0;i<N-2;i++)lrss[i].active=0;for(int i=0;i<N-4;i++)lrvss[i].active=0;for(int i=0;i<N-6;i++)lras[i].active=0;for(int i=0;i<N-6;i++)lrmks[i].active=0;init=1;ps("[LNR] Linaria initialized\n");return 0;}
int lr_planning(int t,int c,int a,int b,int d,int e,int y){return add(lrps,&st.n_lrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lr_execution(int t,int c,int a,int b,int d,int e,int y){return add(lrss,&st.n_lre,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lrvss,&st.n_lrv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lr_market(int t,int c,int a,int b,int d,int e,int y){return add(lrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lr_report(void){ps("[LNR] Lrp: ");pi(st.n_lrp);ps(" PCS=");pi(st.t_f1);ps("\nLre: ");pi(st.n_lre);ps(" PCS=");pi(st.t_f2);ps("\nLrv: ");pi(st.n_lrv);ps(" PCS=");pi(st.t_f3);ps("\nLrc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lr_state(void){ps("[LNR] Lrp=");pi(st.n_lrp);ps(" Lre=");pi(st.n_lre);ps(" Lrv=");pi(st.n_lrv);ps(" Lrc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Linaria Admin Demo ===\n\n");lr_init();
ps("Linaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lr_planning(t,c,718+(i*17),707+(i*14),687+(i*10),669+(i*6),2020+(i%5));}
ps("\nLinaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lr_execution(t,c,707+(i*15),696+(i*12),678+(i*8),665+(i*5),2021+(i%4));}
ps("\nLinaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lr_evaluation(t,c,699+(i*13),688+(i*10),672+(i*7),661+(i*4),2022+(i%3));}
ps("\nLinaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lr_accessory(t,c,691+(i*11),682+(i*9),668+(i*6),658+(i*3),2023+(i%2));}
ps("\nLinaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lr_market(t,c,685+(i*9),676+(i*7),663+(i*5),655+(i*3),2024);}
ps("\n");lr_report();lr_state();ps("\n=== Demo Complete ===\n");return 0;}
