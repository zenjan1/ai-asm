/* risk_admin: Risk management technology administration (v1.0)
 * Risk identification, risk assessment, risk response, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rmk_t;
typedef struct{int n_ri,n_ra,n_rr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rmk_state_t;
static rmk_t ris[N],ras[N-2],rrs[N-4],acs[N-6],mks[N-6]; static rmk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rmk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rmk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RMK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rmk_init(void){if(init)return -1;st.n_ri=0;st.n_ra=0;st.n_rr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ris[i].active=0;for(int i=0;i<N-2;i++)ras[i].active=0;for(int i=0;i<N-4;i++)rrs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RMK] Risk initialized\n");return 0;}
int rmk_identification(int t,int c,int a,int b,int d,int e,int y){return add(ris,&st.n_ri,&st.t_f1,N,t,c,a,b,d,e,y);}
int rmk_assessment(int t,int c,int a,int b,int d,int e,int y){return add(ras,&st.n_ra,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rmk_response(int t,int c,int a,int b,int d,int e,int y){return add(rrs,&st.n_rr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rmk_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rmk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rmk_report(void){ps("[RMK] Ri: ");pi(st.n_ri);ps(" PCS=");pi(st.t_f1);ps("\nRa: ");pi(st.n_ra);ps(" PCS=");pi(st.t_f2);ps("\nRr: ");pi(st.n_rr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rmk_state(void){ps("[RMK] Ri=");pi(st.n_ri);ps(" Ra=");pi(st.n_ra);ps(" Rr=");pi(st.n_rr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Risk Admin Demo ===\n\n");rmk_init();
ps("Risk identification...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rmk_identification(t,c,451+(i*17),440+(i*14),420+(i*10),402+(i*6),2020+(i%5));}
ps("\nRisk assessment...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rmk_assessment(t,c,440+(i*15),429+(i*12),411+(i*8),398+(i*5),2021+(i%4));}
ps("\nRisk response...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rmk_response(t,c,432+(i*13),421+(i*10),405+(i*7),394+(i*4),2022+(i%3));}
ps("\nRisk accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rmk_accessory(t,c,424+(i*11),415+(i*9),401+(i*6),391+(i*3),2023+(i%2));}
ps("\nRisk marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rmk_market(t,c,418+(i*9),409+(i*7),396+(i*5),388+(i*3),2024);}
ps("\n");rmk_report();rmk_state();ps("\n=== Demo Complete ===\n");return 0;}
