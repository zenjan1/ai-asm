/* passivationtech_admin: Passivation technology administration (v1.0)
 * Chromate passivation, organic passivation, electrochemical passivation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} psv_t;
typedef struct{int n_cr,n_or,n_ec,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} psv_state_t;
static psv_t crs[N],ors[N-2],ecs[N-4],acs[N-6],mks[N-6]; static psv_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(psv_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;psv_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PSV] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int psv_init(void){if(init)return -1;st.n_cr=0;st.n_or=0;st.n_ec=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)crs[i].active=0;for(int i=0;i<N-2;i++)ors[i].active=0;for(int i=0;i<N-4;i++)ecs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PSV] Passivationtech initialized\n");return 0;}
int psv_chromate(int t,int c,int a,int b,int d,int e,int y){return add(crs,&st.n_cr,&st.t_f1,N,t,c,a,b,d,e,y);}
int psv_organic(int t,int c,int a,int b,int d,int e,int y){return add(ors,&st.n_or,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int psv_electro(int t,int c,int a,int b,int d,int e,int y){return add(ecs,&st.n_ec,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int psv_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int psv_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void psv_report(void){ps("[PSV] Cr: ");pi(st.n_cr);ps(" PCS=");pi(st.t_f1);ps("\nOr: ");pi(st.n_or);ps(" PCS=");pi(st.t_f2);ps("\nEc: ");pi(st.n_ec);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void psv_state(void){ps("[PSV] Cr=");pi(st.n_cr);ps(" Or=");pi(st.n_or);ps(" Ec=");pi(st.n_ec);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Passivation Tech Admin Demo ===\n\n");psv_init();
ps("Chromate passivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;psv_chromate(t,c,336+(i*17),321+(i*14),301+(i*10),283+(i*6),2020+(i%5));}
ps("\nOrganic passivation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;psv_organic(t,c,325+(i*15),311+(i*12),293+(i*8),280+(i*5),2021+(i%4));}
ps("\nElectrochemical passivation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;psv_electro(t,c,317+(i*13),303+(i*10),287+(i*7),276+(i*4),2022+(i%3));}
ps("\nPassivation accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psv_accessory(t,c,309+(i*11),297+(i*9),283+(i*6),273+(i*3),2023+(i%2));}
ps("\nPassivation marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psv_market(t,c,303+(i*9),292+(i*7),279+(i*5),271+(i*3),2024);}
ps("\n");psv_report();psv_state();ps("\n=== Demo Complete ===\n");return 0;}
