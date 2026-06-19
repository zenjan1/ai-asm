/* logitech_admin: Logitech administration (v1.0)
 * Smart warehousing, smart transport, logistics platform, express service, supply chain finance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lgt_t;
typedef struct{int n_sw,n_st,n_lp,n_es,n_sf,t_f1,t_f2,t_f3,t_f4,t_f5;} lgt_state_t;
static lgt_t sws[N],sts[N-2],lps[N-4],ess[N-6],sfs[N-6]; static lgt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lgt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lgt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LGT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lgt_init(void){if(init)return -1;st.n_sw=0;st.n_st=0;st.n_lp=0;st.n_es=0;st.n_sf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sws[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)lps[i].active=0;for(int i=0;i<N-6;i++)ess[i].active=0;for(int i=0;i<N-6;i++)sfs[i].active=0;init=1;ps("[LGT] Logitech initialized\n");return 0;}
int lgt_warehouse(int t,int c,int a,int b,int d,int e,int y){return add(sws,&st.n_sw,&st.t_f1,N,t,c,a,b,d,e,y);}
int lgt_transport(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lgt_platform(int t,int c,int a,int b,int d,int e,int y){return add(lps,&st.n_lp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lgt_express(int t,int c,int a,int b,int d,int e,int y){return add(ess,&st.n_es,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lgt_finance(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lgt_report(void){ps("[LGT] WH: ");pi(st.n_sw);ps(" AGV=");pi(st.t_f1);ps("\nTrans: ");pi(st.n_st);ps(" IoT=");pi(st.t_f2);ps("\nPlat: ");pi(st.n_lp);ps(" TMS=");pi(st.t_f3);ps("\nExpr: ");pi(st.n_es);ps(" LDM=");pi(st.t_f4);ps("\nFin: ");pi(st.n_sf);ps(" SCF=");pi(st.t_f5);ps("\n");}
void lgt_state(void){ps("[LGT] Sw=");pi(st.n_sw);ps(" St=");pi(st.n_st);ps(" Lp=");pi(st.n_lp);ps(" Es=");pi(st.n_es);ps(" Sf=");pi(st.n_sf);ps("\n");}
int main(void){
ps("=== Logitech Admin Demo ===\n\n");lgt_init();
ps("Smart warehousing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lgt_warehouse(t,c,143+(i*17),128+(i*14),108+(i*10),91+(i*6),2020+(i%5));}
ps("\nSmart transport...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lgt_transport(t,c,132+(i*15),118+(i*12),100+(i*8),87+(i*5),2021+(i%4));}
ps("\nLogistics platform...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lgt_platform(t,c,124+(i*13),110+(i*10),94+(i*7),83+(i*4),2022+(i%3));}
ps("\nExpress service...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgt_express(t,c,116+(i*11),104+(i*9),90+(i*6),80+(i*3),2023+(i%2));}
ps("\nSupply chain finance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lgt_finance(t,c,110+(i*9),99+(i*7),86+(i*5),78+(i*3),2024);}
ps("\n");lgt_report();lgt_state();ps("\n=== Demo Complete ===\n");return 0;}
