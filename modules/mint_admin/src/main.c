/* mint_admin: Mint management technology administration (v1.0)
 * Mint planning, mint execution, mint evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mt_t;
typedef struct{int n_mtp,n_mte,n_mtv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mt_state_t;
static mt_t mtps[N],mtss[N-2],mtvss[N-4],mtas[N-6],mtmks[N-6]; static mt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mt_init(void){if(init)return -1;st.n_mtp=0;st.n_mte=0;st.n_mtv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mtps[i].active=0;for(int i=0;i<N-2;i++)mtss[i].active=0;for(int i=0;i<N-4;i++)mtvss[i].active=0;for(int i=0;i<N-6;i++)mtas[i].active=0;for(int i=0;i<N-6;i++)mtmks[i].active=0;init=1;ps("[MTT] Mint initialized\n");return 0;}
int mt_planning(int t,int c,int a,int b,int d,int e,int y){return add(mtps,&st.n_mtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mt_execution(int t,int c,int a,int b,int d,int e,int y){return add(mtss,&st.n_mte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mtvss,&st.n_mtv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mt_market(int t,int c,int a,int b,int d,int e,int y){return add(mtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mt_report(void){ps("[MTT] Mtp: ");pi(st.n_mtp);ps(" PCS=");pi(st.t_f1);ps("\nMte: ");pi(st.n_mte);ps(" PCS=");pi(st.t_f2);ps("\nMtv: ");pi(st.n_mtv);ps(" PCS=");pi(st.t_f3);ps("\nMtc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mt_state(void){ps("[MTT] Mtp=");pi(st.n_mtp);ps(" Mte=");pi(st.n_mte);ps(" Mtv=");pi(st.n_mtv);ps(" Mtc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mint Admin Demo ===\n\n");mt_init();
ps("Mint planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mt_planning(t,c,653+(i*17),642+(i*14),622+(i*10),604+(i*6),2020+(i%5));}
ps("\nMint execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mt_execution(t,c,642+(i*15),631+(i*12),613+(i*8),600+(i*5),2021+(i%4));}
ps("\nMint evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mt_evaluation(t,c,634+(i*13),623+(i*10),607+(i*7),596+(i*4),2022+(i%3));}
ps("\nMint accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mt_accessory(t,c,626+(i*11),617+(i*9),603+(i*6),593+(i*3),2023+(i%2));}
ps("\nMint marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mt_market(t,c,620+(i*9),611+(i*7),598+(i*5),590+(i*3),2024);}
ps("\n");mt_report();mt_state();ps("\n=== Demo Complete ===\n");return 0;}
