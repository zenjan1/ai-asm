/* standards_admin: Standards management technology administration (v1.0)
 * Standards development, standards implementation, standards management, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sdx_t;
typedef struct{int n_sd,n_si,n_sm,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sdx_state_t;
static sdx_t sds[N],sis[N-2],sms[N-4],acs[N-6],mks[N-6]; static sdx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sdx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sdx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SDX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sdx_init(void){if(init)return -1;st.n_sd=0;st.n_si=0;st.n_sm=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sds[i].active=0;for(int i=0;i<N-2;i++)sis[i].active=0;for(int i=0;i<N-4;i++)sms[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SDX] Standards initialized\n");return 0;}
int sdx_development(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f1,N,t,c,a,b,d,e,y);}
int sdx_implementation(int t,int c,int a,int b,int d,int e,int y){return add(sis,&st.n_si,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sdx_management(int t,int c,int a,int b,int d,int e,int y){return add(sms,&st.n_sm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sdx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sdx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sdx_report(void){ps("[SDX] Sd: ");pi(st.n_sd);ps(" PCS=");pi(st.t_f1);ps("\nSi: ");pi(st.n_si);ps(" PCS=");pi(st.t_f2);ps("\nSm: ");pi(st.n_sm);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sdx_state(void){ps("[SDX] Sd=");pi(st.n_sd);ps(" Si=");pi(st.n_si);ps(" Sm=");pi(st.n_sm);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Standards Admin Demo ===\n\n");sdx_init();
ps("Standards development...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sdx_development(t,c,448+(i*17),437+(i*14),417+(i*10),399+(i*6),2020+(i%5));}
ps("\nStandards implementation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sdx_implementation(t,c,437+(i*15),426+(i*12),408+(i*8),395+(i*5),2021+(i%4));}
ps("\nStandards management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sdx_management(t,c,429+(i*13),418+(i*10),402+(i*7),391+(i*4),2022+(i%3));}
ps("\nStandards accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sdx_accessory(t,c,421+(i*11),412+(i*9),398+(i*6),388+(i*3),2023+(i%2));}
ps("\nStandards marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sdx_market(t,c,415+(i*9),406+(i*7),393+(i*5),385+(i*3),2024);}
ps("\n");sdx_report();sdx_state();ps("\n=== Demo Complete ===\n");return 0;}
