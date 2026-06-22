/* clematis_admin: Clematis management technology administration (v1.0)
 * Clematis planning, clematis execution, clematis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lmt_t;
typedef struct{int n_lmtp,n_lmte,n_lmt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lmt_state_t;
static lmt_t lmtps[N],lmtss[N-2],lmtvss[N-4],lmtas[N-6],lmtmks[N-6]; static lmt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lmt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lmt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LMT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lmt_init(void){if(init)return -1;st.n_lmtp=0;st.n_lmte=0;st.n_lmt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lmtps[i].active=0;for(int i=0;i<N-2;i++)lmtss[i].active=0;for(int i=0;i<N-4;i++)lmtvss[i].active=0;for(int i=0;i<N-6;i++)lmtas[i].active=0;for(int i=0;i<N-6;i++)lmtmks[i].active=0;init=1;ps("[LMT] Clematis initialized\n");return 0;}
int lmt_planning(int t,int c,int a,int b,int d,int e,int y){return add(lmtps,&st.n_lmtp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lmt_execution(int t,int c,int a,int b,int d,int e,int y){return add(lmtss,&st.n_lmte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lmt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lmtvss,&st.n_lmt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lmt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lmtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lmt_market(int t,int c,int a,int b,int d,int e,int y){return add(lmtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lmt_report(void){ps("[LMT] Clepp: ");pi(st.n_lmtp);ps(" PCS=");pi(st.t_f1);ps("\nClepe: ");pi(st.n_lmte);ps(" PCS=");pi(st.t_f2);ps("\nClev: ");pi(st.n_lmt2);ps(" PCS=");pi(st.t_f3);ps("\nClec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lmt_state(void){ps("[LMT] Clepp=");pi(st.n_lmtp);ps(" Clepe=");pi(st.n_lmte);ps(" Clev=");pi(st.n_lmt2);ps(" Clec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Clematis Admin Demo ===\n\n");lmt_init();
ps("Clematis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lmt_planning(t,c,1239+(i*17),1228+(i*14),1208+(i*10),1190+(i*6),2020+(i%5));}
ps("\nClematis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lmt_execution(t,c,1228+(i*15),1217+(i*12),1199+(i*8),1186+(i*5),2021+(i%4));}
ps("\nClematis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lmt_evaluation(t,c,1220+(i*13),1209+(i*10),1193+(i*7),1182+(i*4),2022+(i%3));}
ps("\nClematis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lmt_accessory(t,c,1212+(i*11),1203+(i*9),1189+(i*6),1179+(i*3),2023+(i%2));}
ps("\nClematis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lmt_market(t,c,1206+(i*9),1197+(i*7),1184+(i*5),1176+(i*3),2024);}
ps("\n");lmt_report();lmt_state();ps("\n=== Demo Complete ===\n");return 0;}
