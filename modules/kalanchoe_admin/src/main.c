/* kalanchoe_admin: Kalanchoe management technology administration (v1.0)
 * Kalanchoe planning, kalanchoe execution, kalanchoe evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} kc_t;
typedef struct{int n_kcp,n_kce,n_kcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} kc_state_t;
static kc_t kcps[N],kcss[N-2],kcvss[N-4],kcas[N-6],kcmks[N-6]; static kc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;kc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[KCN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int kcn_init(void){if(init)return -1;st.n_kcp=0;st.n_kce=0;st.n_kcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)kcps[i].active=0;for(int i=0;i<N-2;i++)kcss[i].active=0;for(int i=0;i<N-4;i++)kcvss[i].active=0;for(int i=0;i<N-6;i++)kcas[i].active=0;for(int i=0;i<N-6;i++)kcmks[i].active=0;init=1;ps("[KCN] Kalanchoe initialized\n");return 0;}
int kcn_planning(int t,int c,int a,int b,int d,int e,int y){return add(kcps,&st.n_kcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int kcn_execution(int t,int c,int a,int b,int d,int e,int y){return add(kcss,&st.n_kce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int kcn_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(kcvss,&st.n_kcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int kcn_accessory(int t,int c,int a,int b,int d,int e,int y){return add(kcas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int kcn_market(int t,int c,int a,int b,int d,int e,int y){return add(kcmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void kcn_report(void){ps("[KCN] Kcp: ");pi(st.n_kcp);ps(" PCS=");pi(st.t_f1);ps("\nKce: ");pi(st.n_kce);ps(" PCS=");pi(st.t_f2);ps("\nKcv: ");pi(st.n_kcv);ps(" PCS=");pi(st.t_f3);ps("\nKcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void kcn_state(void){ps("[KCN] Kcp=");pi(st.n_kcp);ps(" Kce=");pi(st.n_kce);ps(" Kcv=");pi(st.n_kcv);ps(" Kcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Kalanchoe Admin Demo ===\n\n");kcn_init();
ps("Kalanchoe planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;kcn_planning(t,c,745+(i*17),734+(i*14),714+(i*10),696+(i*6),2020+(i%5));}
ps("\nKalanchoe execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;kcn_execution(t,c,734+(i*15),723+(i*12),705+(i*8),692+(i*5),2021+(i%4));}
ps("\nKalanchoe evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;kcn_evaluation(t,c,726+(i*13),715+(i*10),699+(i*7),688+(i*4),2022+(i%3));}
ps("\nKalanchoe accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kcn_accessory(t,c,718+(i*11),709+(i*9),695+(i*6),685+(i*3),2023+(i%2));}
ps("\nKalanchoe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;kcn_market(t,c,712+(i*9),703+(i*7),690+(i*5),682+(i*3),2024);}
ps("\n");kcn_report();kcn_state();ps("\n=== Demo Complete ===\n");return 0;}
