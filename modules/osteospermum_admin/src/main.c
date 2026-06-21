/* osteospermum_admin: Osteospermum management technology administration (v1.0)
 * Osteospermum planning, osteospermum execution, osteospermum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ost_t;
typedef struct{int n_osp,n_ose,n_osv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ost_state_t;
static ost_t osps[N],osss[N-2],osvss[N-4],osas[N-6],osmks[N-6]; static ost_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ost_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ost_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ost_init(void){if(init)return -1;st.n_osp=0;st.n_ose=0;st.n_osv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)osps[i].active=0;for(int i=0;i<N-2;i++)osss[i].active=0;for(int i=0;i<N-4;i++)osvss[i].active=0;for(int i=0;i<N-6;i++)osas[i].active=0;for(int i=0;i<N-6;i++)osmks[i].active=0;init=1;ps("[OST] Osteospermum initialized\n");return 0;}
int ost_planning(int t,int c,int a,int b,int d,int e,int y){return add(osps,&st.n_osp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ost_execution(int t,int c,int a,int b,int d,int e,int y){return add(osss,&st.n_ose,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ost_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(osvss,&st.n_osv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ost_accessory(int t,int c,int a,int b,int d,int e,int y){return add(osas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ost_market(int t,int c,int a,int b,int d,int e,int y){return add(osmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ost_report(void){ps("[OST] Osp: ");pi(st.n_osp);ps(" PCS=");pi(st.t_f1);ps("\nOse: ");pi(st.n_ose);ps(" PCS=");pi(st.t_f2);ps("\nOsv: ");pi(st.n_osv);ps(" PCS=");pi(st.t_f3);ps("\nOsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ost_state(void){ps("[OST] Osp=");pi(st.n_osp);ps(" Ose=");pi(st.n_ose);ps(" Osv=");pi(st.n_osv);ps(" Osc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Osteospermum Admin Demo ===\n\n");ost_init();
ps("Osteospermum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ost_planning(t,c,794+(i*17),783+(i*14),763+(i*10),745+(i*6),2020+(i%5));}
ps("\nOsteospermum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ost_execution(t,c,783+(i*15),772+(i*12),754+(i*8),741+(i*5),2021+(i%4));}
ps("\nOsteospermum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ost_evaluation(t,c,775+(i*13),764+(i*10),750+(i*7),739+(i*4),2022+(i%3));}
ps("\nOsteospermum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ost_accessory(t,c,767+(i*11),758+(i*9),744+(i*6),734+(i*3),2023+(i%2));}
ps("\nOsteospermum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ost_market(t,c,761+(i*9),752+(i*7),739+(i*5),731+(i*3),2024);}
ps("\n");ost_report();ost_state();ps("\n=== Demo Complete ===\n");return 0;}
