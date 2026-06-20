/* railtech_admin: Railway technology administration (v1.0)
 * Vehicle manufacturing, track systems, signaling systems, rail services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rlt_t;
typedef struct{int n_rv,n_tr,n_sg,n_rs,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rlt_state_t;
static rlt_t rvs[N],trs[N-2],sgs[N-4],rss[N-6],mks[N-6]; static rlt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rlt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rlt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RLT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rlt_init(void){if(init)return -1;st.n_rv=0;st.n_tr=0;st.n_sg=0;st.n_rs=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rvs[i].active=0;for(int i=0;i<N-2;i++)trs[i].active=0;for(int i=0;i<N-4;i++)sgs[i].active=0;for(int i=0;i<N-6;i++)rss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RLT] Railtech initialized\n");return 0;}
int rlt_vehicle(int t,int c,int a,int b,int d,int e,int y){return add(rvs,&st.n_rv,&st.t_f1,N,t,c,a,b,d,e,y);}
int rlt_track(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rlt_signal(int t,int c,int a,int b,int d,int e,int y){return add(sgs,&st.n_sg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rlt_service(int t,int c,int a,int b,int d,int e,int y){return add(rss,&st.n_rs,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rlt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rlt_report(void){ps("[RLT] Rv: ");pi(st.n_rv);ps(" PCS=");pi(st.t_f1);ps("\nTr: ");pi(st.n_tr);ps(" km=");pi(st.t_f2);ps("\nSg: ");pi(st.n_sg);ps(" PCS=");pi(st.t_f3);ps("\nRs: ");pi(st.n_rs);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rlt_state(void){ps("[RLT] Rv=");pi(st.n_rv);ps(" Tr=");pi(st.n_tr);ps(" Sg=");pi(st.n_sg);ps(" Rs=");pi(st.n_rs);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Railway Tech Admin Demo ===\n\n");rlt_init();
ps("Rail vehicle manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rlt_vehicle(t,c,221+(i*17),206+(i*14),186+(i*10),168+(i*6),2020+(i%5));}
ps("\nTrack systems...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rlt_track(t,c,210+(i*15),196+(i*12),178+(i*8),165+(i*5),2021+(i%4));}
ps("\nSignaling systems...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rlt_signal(t,c,202+(i*13),188+(i*10),172+(i*7),161+(i*4),2022+(i%3));}
ps("\nRail services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rlt_service(t,c,194+(i*11),182+(i*9),168+(i*6),158+(i*3),2023+(i%2));}
ps("\nRail marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rlt_market(t,c,188+(i*9),177+(i*7),164+(i*5),156+(i*3),2024);}
ps("\n");rlt_report();rlt_state();ps("\n=== Demo Complete ===\n");return 0;}
