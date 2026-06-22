/* wolfia_admin: Wolfia management technology administration (v1.0)
 * Wolfia planning, wolfia execution, wolfia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wolf_t;
typedef struct{int n_wolfp,n_wolf,n_wolf2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wolf_state_t;
static wolf_t wolfs[N],wolfe[N-2],wolv[N-4],wolfac[N-6],wolm[N-6]; static wolf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(wolf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wolf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WOLF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wolf_init(void){if(init)return -1;st.n_wolfp=0;st.n_wolf=0;st.n_wolf2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wolfs[i].active=0;for(int i=0;i<N-2;i++)wolfe[i].active=0;for(int i=0;i<N-4;i++)wolv[i].active=0;for(int i=0;i<N-6;i++)wolfac[i].active=0;for(int i=0;i<N-6;i++)wolm[i].active=0;init=1;ps("[WOLF] Wolfia initialized\n");return 0;}
int wolf_planning(int t,int c,int a,int b,int d,int e,int y){return add(wolfs,&st.n_wolfp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wolf_execution(int t,int c,int a,int b,int d,int e,int y){return add(wolfe,&st.n_wolf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wolf_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wolv,&st.n_wolf2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wolf_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wolfac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wolf_market(int t,int c,int a,int b,int d,int e,int y){return add(wolm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wolf_report(void){ps("[WOLF] Wolfp: ");pi(st.n_wolfp);ps(" PCS=");pi(st.t_f1);ps("\nWolfe: ");pi(st.n_wolf);ps(" PCS=");pi(st.t_f2);ps("\nWolv: ");pi(st.n_wolf2);ps(" PCS=");pi(st.t_f3);ps("\nWolac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wolf_state(void){ps("[WOLF] Wolfp=");pi(st.n_wolfp);ps(" Wolfe=");pi(st.n_wolf);ps(" Wv=");pi(st.n_wolf2);ps(" Wolac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wolfia Admin Demo ===\n\n");wolf_init();
ps("Wolfia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wolf_planning(t,c,1372+(i*17),1361+(i*14),1341+(i*10),1323+(i*6),2020+(i%5));}
ps("\nWolfia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wolf_execution(t,c,1361+(i*15),1350+(i*12),1332+(i*8),1319+(i*5),2021+(i%4));}
ps("\nWolfia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wolf_evaluation(t,c,1353+(i*13),1342+(i*10),1326+(i*7),1315+(i*4),2022+(i%3));}
ps("\nWolfia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wolf_accessory(t,c,1345+(i*11),1336+(i*9),1322+(i*6),1312+(i*3),2023+(i%2));}
ps("\nWolfia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wolf_market(t,c,1339+(i*9),1330+(i*7),1317+(i*5),1309+(i*3),2024);}
ps("\n");wolf_report();wolf_state();ps("\n=== Demo Complete ===\n");return 0;}
