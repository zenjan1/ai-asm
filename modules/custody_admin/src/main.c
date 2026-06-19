/* custody_admin: Custody administration (v1.0)
 * Asset custody, fund settlement, securities lending, corporate actions, custody services
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cus_t;
typedef struct{int n_ac,n_fs,n_sl,n_co,n_cs,t_f1,t_f2,t_f3,t_f4,t_f5;} cus_state_t;
static cus_t acs[N],fss[N-2],sls[N-4],cos[N-6],css[N-6]; static cus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cus_init(void){if(init)return -1;st.n_ac=0;st.n_fs=0;st.n_sl=0;st.n_co=0;st.n_cs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acs[i].active=0;for(int i=0;i<N-2;i++)fss[i].active=0;for(int i=0;i<N-4;i++)sls[i].active=0;for(int i=0;i<N-6;i++)cos[i].active=0;for(int i=0;i<N-6;i++)css[i].active=0;init=1;ps("[CUS] Custody initialized\n");return 0;}
int cus_asset(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f1,N,t,c,a,b,d,e,y);}
int cus_settle(int t,int c,int a,int b,int d,int e,int y){return add(fss,&st.n_fs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cus_lending(int t,int c,int a,int b,int d,int e,int y){return add(sls,&st.n_sl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cus_corporate(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cus_service(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cus_report(void){ps("[CUS] Asset: ");pi(st.n_ac);ps(" AUC=");pi(st.t_f1);ps("\nSettle: ");pi(st.n_fs);ps(" DVP=");pi(st.t_f2);ps("\nLend: ");pi(st.n_sl);ps(" Fee=");pi(st.t_f3);ps("\nCorp: ");pi(st.n_co);ps(" Div=");pi(st.t_f4);ps("\nServ: ");pi(st.n_cs);ps(" Val=");pi(st.t_f5);ps("\n");}
void cus_state(void){ps("[CUS] Ac=");pi(st.n_ac);ps(" Fs=");pi(st.n_fs);ps(" Sl=");pi(st.n_sl);ps(" Co=");pi(st.n_co);ps(" Cs=");pi(st.n_cs);ps("\n");}
int main(void){
ps("=== Custody Admin Demo ===\n\n");cus_init();
ps("Asset custody...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cus_asset(t,c,125+(i*17),110+(i*14),90+(i*10),73+(i*6),2020+(i%5));}
ps("\nFund settlement...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cus_settle(t,c,114+(i*15),100+(i*12),82+(i*8),69+(i*5),2021+(i%4));}
ps("\nSecurities lending...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cus_lending(t,c,106+(i*13),92+(i*10),76+(i*7),65+(i*4),2022+(i%3));}
ps("\nCorporate actions...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cus_corporate(t,c,98+(i*11),86+(i*9),72+(i*6),62+(i*3),2023+(i%2));}
ps("\nCustody services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cus_service(t,c,92+(i*9),81+(i*7),68+(i*5),60+(i*3),2024);}
ps("\n");cus_report();cus_state();ps("\n=== Demo Complete ===\n");return 0;}
