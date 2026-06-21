/* oxalis_admin: Oxalis management technology administration (v1.0)
 * Oxalis planning, oxalis execution, oxalis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} oxl_t;
typedef struct{int n_oxp,n_oxe,n_oxv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} oxl_state_t;
static oxl_t oxlps[N],oxlss[N-2],oxlvss[N-4],oxlas[N-6],oxlmks[N-6]; static oxl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(oxl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;oxl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OXL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int oxl_init(void){if(init)return -1;st.n_oxp=0;st.n_oxe=0;st.n_oxv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)oxlps[i].active=0;for(int i=0;i<N-2;i++)oxlss[i].active=0;for(int i=0;i<N-4;i++)oxlvss[i].active=0;for(int i=0;i<N-6;i++)oxlas[i].active=0;for(int i=0;i<N-6;i++)oxlmks[i].active=0;init=1;ps("[OXL] Oxalis initialized\n");return 0;}
int oxl_planning(int t,int c,int a,int b,int d,int e,int y){return add(oxlps,&st.n_oxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int oxl_execution(int t,int c,int a,int b,int d,int e,int y){return add(oxlss,&st.n_oxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int oxl_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(oxlvss,&st.n_oxv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int oxl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(oxlas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int oxl_market(int t,int c,int a,int b,int d,int e,int y){return add(oxlmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void oxl_report(void){ps("[OXL] Oxp: ");pi(st.n_oxp);ps(" PCS=");pi(st.t_f1);ps("\nOxe: ");pi(st.n_oxe);ps(" PCS=");pi(st.t_f2);ps("\nOxv: ");pi(st.n_oxv);ps(" PCS=");pi(st.t_f3);ps("\nOxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void oxl_state(void){ps("[OXL] Oxp=");pi(st.n_oxp);ps(" Oxe=");pi(st.n_oxe);ps(" Oxv=");pi(st.n_oxv);ps(" Oxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Oxalis Admin Demo ===\n\n");oxl_init();
ps("Oxalis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;oxl_planning(t,c,848+(i*17),837+(i*14),817+(i*10),799+(i*6),2020+(i%5));}
ps("\nOxalis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;oxl_execution(t,c,837+(i*15),826+(i*12),808+(i*8),795+(i*5),2021+(i%4));}
ps("\nOxalis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;oxl_evaluation(t,c,829+(i*13),818+(i*10),802+(i*7),791+(i*4),2022+(i%3));}
ps("\nOxalis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxl_accessory(t,c,821+(i*11),812+(i*9),798+(i*6),788+(i*3),2023+(i%2));}
ps("\nOxalis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;oxl_market(t,c,815+(i*9),806+(i*7),793+(i*5),785+(i*3),2024);}
ps("\n");oxl_report();oxl_state();ps("\n=== Demo Complete ===\n");return 0;}
