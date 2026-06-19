/* neuroeconomics_admin: Neuroeconomics administration (v1.0)
 * Neural basis, risk decisions, time preference, social decisions, consumer neuroscience
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} neu_t;
typedef struct{int n_nb,n_rd,n_tp,n_sd,n_cd,t_f1,t_f2,t_f3,t_f4,t_f5;} neu_state_t;
static neu_t nbs[N],rds[N-2],tps[N-4],sds[N-6],cds[N-6]; static neu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(neu_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;neu_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NEU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int neu_init(void){if(init)return -1;st.n_nb=0;st.n_rd=0;st.n_tp=0;st.n_sd=0;st.n_cd=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nbs[i].active=0;for(int i=0;i<N-2;i++)rds[i].active=0;for(int i=0;i<N-4;i++)tps[i].active=0;for(int i=0;i<N-6;i++)sds[i].active=0;for(int i=0;i<N-6;i++)cds[i].active=0;init=1;ps("[NEU] Neuroeconomics initialized\n");return 0;}
int neu_basis(int t,int c,int a,int b,int d,int e,int y){return add(nbs,&st.n_nb,&st.t_f1,N,t,c,a,b,d,e,y);}
int neu_risk(int t,int c,int a,int b,int d,int e,int y){return add(rds,&st.n_rd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int neu_time(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int neu_social(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int neu_consumer(int t,int c,int a,int b,int d,int e,int y){return add(cds,&st.n_cd,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void neu_report(void){ps("[NEU] Basis: ");pi(st.n_nb);ps(" Cortex=");pi(st.t_f1);ps("\nRisk: ");pi(st.n_rd);ps(" Averse=");pi(st.t_f2);ps("\nTime: ");pi(st.n_tp);ps(" Delay=");pi(st.t_f3);ps("\nSocial: ");pi(st.n_sd);ps(" Trust=");pi(st.t_f4);ps("\nConsumer: ");pi(st.n_cd);ps(" Value=");pi(st.t_f5);ps("\n");}
void neu_state(void){ps("[NEU] Nb=");pi(st.n_nb);ps(" Rd=");pi(st.n_rd);ps(" Tp=");pi(st.n_tp);ps(" Sd=");pi(st.n_sd);ps(" Cd=");pi(st.n_cd);ps("\n");}
int main(void){
ps("=== Neuroeconomics Admin Demo ===\n\n");neu_init();
ps("Neural basis...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;neu_basis(t,c,92+(i*17),77+(i*14),57+(i*10),40+(i*6),2020+(i%5));}
ps("\nRisk decisions...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;neu_risk(t,c,81+(i*15),67+(i*12),49+(i*8),36+(i*5),2021+(i%4));}
ps("\nTime preference...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;neu_time(t,c,73+(i*13),59+(i*10),43+(i*7),32+(i*4),2022+(i%3));}
ps("\nSocial decisions...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;neu_social(t,c,65+(i*11),53+(i*9),39+(i*6),29+(i*3),2023+(i%2));}
ps("\nConsumer neuroscience...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;neu_consumer(t,c,59+(i*9),48+(i*7),35+(i*5),27+(i*3),2024);}
ps("\n");neu_report();neu_state();ps("\n=== Demo Complete ===\n");return 0;}
