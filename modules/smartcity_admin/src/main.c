/* smartcity_admin: Smart city administration (v1.0)
 * Smart traffic, smart security, smart energy, smart governance, smart livelihood
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} smc_t;
typedef struct{int n_tr,n_se,n_en,n_gv,n_lv,t_f1,t_f2,t_f3,t_f4,t_f5;} smc_state_t;
static smc_t trs[N],ses[N-2],ens[N-4],gvs[N-6],lvs[N-6]; static smc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(smc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;smc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SMC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int smc_init(void){if(init)return -1;st.n_tr=0;st.n_se=0;st.n_en=0;st.n_gv=0;st.n_lv=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)trs[i].active=0;for(int i=0;i<N-2;i++)ses[i].active=0;for(int i=0;i<N-4;i++)ens[i].active=0;for(int i=0;i<N-6;i++)gvs[i].active=0;for(int i=0;i<N-6;i++)lvs[i].active=0;init=1;ps("[SMC] Smartcity initialized\n");return 0;}
int smc_traffic(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f1,N,t,c,a,b,d,e,y);}
int smc_security(int t,int c,int a,int b,int d,int e,int y){return add(ses,&st.n_se,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int smc_energy(int t,int c,int a,int b,int d,int e,int y){return add(ens,&st.n_en,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int smc_govern(int t,int c,int a,int b,int d,int e,int y){return add(gvs,&st.n_gv,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int smc_livelihood(int t,int c,int a,int b,int d,int e,int y){return add(lvs,&st.n_lv,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void smc_report(void){ps("[SMC] Traffic: ");pi(st.n_tr);ps(" V2X=");pi(st.t_f1);ps("\nSecure: ");pi(st.n_se);ps(" AI=");pi(st.t_f2);ps("\nEnergy: ");pi(st.n_en);ps(" Grid=");pi(st.t_f3);ps("\nGov: ");pi(st.n_gv);ps(" EGOV=");pi(st.t_f4);ps("\nLife: ");pi(st.n_lv);ps(" Serv=");pi(st.t_f5);ps("\n");}
void smc_state(void){ps("[SMC] Tr=");pi(st.n_tr);ps(" Se=");pi(st.n_se);ps(" En=");pi(st.n_en);ps(" Gv=");pi(st.n_gv);ps(" Lv=");pi(st.n_lv);ps("\n");}
int main(void){
ps("=== Smart City Admin Demo ===\n\n");smc_init();
ps("Smart traffic...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;smc_traffic(t,c,145+(i*17),130+(i*14),110+(i*10),93+(i*6),2020+(i%5));}
ps("\nSmart security...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;smc_security(t,c,134+(i*15),120+(i*12),102+(i*8),89+(i*5),2021+(i%4));}
ps("\nSmart energy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;smc_energy(t,c,126+(i*13),112+(i*10),96+(i*7),85+(i*4),2022+(i%3));}
ps("\nSmart governance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;smc_govern(t,c,118+(i*11),106+(i*9),92+(i*6),82+(i*3),2023+(i%2));}
ps("\nSmart livelihood...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;smc_livelihood(t,c,112+(i*9),101+(i*7),88+(i*5),80+(i*3),2024);}
ps("\n");smc_report();smc_state();ps("\n=== Demo Complete ===\n");return 0;}
