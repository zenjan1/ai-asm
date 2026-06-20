/* justitech_admin: Justice and legal technology administration (v1.0)
 * Legal services, intellectual property, compliance, legal technology, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} jst_t;
typedef struct{int n_ls,n_ip,n_cp,n_lt,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} jst_state_t;
static jst_t lss[N],ips[N-2],cps[N-4],lts[N-6],mks[N-6]; static jst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;jst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[JST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int jst_init(void){if(init)return -1;st.n_ls=0;st.n_ip=0;st.n_cp=0;st.n_lt=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lss[i].active=0;for(int i=0;i<N-2;i++)ips[i].active=0;for(int i=0;i<N-4;i++)cps[i].active=0;for(int i=0;i<N-6;i++)lts[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[JST] Justitech initialized\n");return 0;}
int jst_legal(int t,int c,int a,int b,int d,int e,int y){return add(lss,&st.n_ls,&st.t_f1,N,t,c,a,b,d,e,y);}
int jst_ip(int t,int c,int a,int b,int d,int e,int y){return add(ips,&st.n_ip,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int jst_compliance(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int jst_tech(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int jst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void jst_report(void){ps("[JST] Ls: ");pi(st.n_ls);ps(" PCS=");pi(st.t_f1);ps("\nIp: ");pi(st.n_ip);ps(" PCS=");pi(st.t_f2);ps("\nCp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f3);ps("\nLt: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void jst_state(void){ps("[JST] Ls=");pi(st.n_ls);ps(" Ip=");pi(st.n_ip);ps(" Cp=");pi(st.n_cp);ps(" Lt=");pi(st.n_lt);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Justice Legal Tech Admin Demo ===\n\n");jst_init();
ps("Legal services...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;jst_legal(t,c,246+(i*17),231+(i*14),211+(i*10),193+(i*6),2020+(i%5));}
ps("\nIntellectual property...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;jst_ip(t,c,235+(i*15),221+(i*12),203+(i*8),190+(i*5),2021+(i%4));}
ps("\nCompliance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;jst_compliance(t,c,227+(i*13),213+(i*10),197+(i*7),186+(i*4),2022+(i%3));}
ps("\nLegal technology...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jst_tech(t,c,219+(i*11),207+(i*9),193+(i*6),183+(i*3),2023+(i%2));}
ps("\nLegal marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;jst_market(t,c,213+(i*9),202+(i*7),189+(i*5),181+(i*3),2024);}
ps("\n");jst_report();jst_state();ps("\n=== Demo Complete ===\n");return 0;}
