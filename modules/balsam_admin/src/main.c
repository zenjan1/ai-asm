/* balsam_admin: Balsam management technology administration (v1.0)
 * Balsam planning, balsam execution, balsam evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bal_t;
typedef struct{int n_balp,n_bale,n_bal2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bal_state_t;
static bal_t balps[N],balss[N-2],balvss[N-4],balas[N-6],balmks[N-6]; static bal_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bal_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bal_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BAL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bal_init(void){if(init)return -1;st.n_balp=0;st.n_bale=0;st.n_bal2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)balps[i].active=0;for(int i=0;i<N-2;i++)balss[i].active=0;for(int i=0;i<N-4;i++)balvss[i].active=0;for(int i=0;i<N-6;i++)balas[i].active=0;for(int i=0;i<N-6;i++)balmks[i].active=0;init=1;ps("[BAL] Balsam initialized\n");return 0;}
int bal_planning(int t,int c,int a,int b,int d,int e,int y){return add(balps,&st.n_balp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bal_execution(int t,int c,int a,int b,int d,int e,int y){return add(balss,&st.n_bale,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bal_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(balvss,&st.n_bal2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bal_accessory(int t,int c,int a,int b,int d,int e,int y){return add(balas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bal_market(int t,int c,int a,int b,int d,int e,int y){return add(balmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bal_report(void){ps("[BAL] Balpp: ");pi(st.n_balp);ps(" PCS=");pi(st.t_f1);ps("\nBalpe: ");pi(st.n_bale);ps(" PCS=");pi(st.t_f2);ps("\nBalv: ");pi(st.n_bal2);ps(" PCS=");pi(st.t_f3);ps("\nBalc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bal_state(void){ps("[BAL] Balpp=");pi(st.n_balp);ps(" Balpe=");pi(st.n_bale);ps(" Balv=");pi(st.n_bal2);ps(" Balc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Balsam Admin Demo ===\n\n");bal_init();
ps("Balsam planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bal_planning(t,c,1219+(i*17),1208+(i*14),1188+(i*10),1170+(i*6),2020+(i%5));}
ps("\nBalsam execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bal_execution(t,c,1208+(i*15),1197+(i*12),1179+(i*8),1166+(i*5),2021+(i%4));}
ps("\nBalsam evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bal_evaluation(t,c,1200+(i*13),1189+(i*10),1173+(i*7),1162+(i*4),2022+(i%3));}
ps("\nBalsam accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bal_accessory(t,c,1192+(i*11),1183+(i*9),1169+(i*6),1159+(i*3),2023+(i%2));}
ps("\nBalsam marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bal_market(t,c,1186+(i*9),1177+(i*7),1164+(i*5),1156+(i*3),2024);}
ps("\n");bal_report();bal_state();ps("\n=== Demo Complete ===\n");return 0;}
