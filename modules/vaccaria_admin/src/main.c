/* vaccaria_admin: Vaccaria management technology administration (v1.0)
 * Vaccaria planning, vaccaria execution, vaccaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vcr_t;
typedef struct{int n_vcp,n_vce,n_vcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vcr_state_t;
static vcr_t vcrps[N],vcrss[N-2],vcrvss[N-4],vcras[N-6],vcrmks[N-6]; static vcr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vcr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vcr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VCR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vcr_init(void){if(init)return -1;st.n_vcp=0;st.n_vce=0;st.n_vcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vcrps[i].active=0;for(int i=0;i<N-2;i++)vcrss[i].active=0;for(int i=0;i<N-4;i++)vcrvss[i].active=0;for(int i=0;i<N-6;i++)vcras[i].active=0;for(int i=0;i<N-6;i++)vcrmks[i].active=0;init=1;ps("[VCR] Vaccaria initialized\n");return 0;}
int vcr_planning(int t,int c,int a,int b,int d,int e,int y){return add(vcrps,&st.n_vcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vcr_execution(int t,int c,int a,int b,int d,int e,int y){return add(vcrss,&st.n_vce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vcr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vcrvss,&st.n_vcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vcr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(vcras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vcr_market(int t,int c,int a,int b,int d,int e,int y){return add(vcrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vcr_report(void){ps("[VCR] Vcp: ");pi(st.n_vcp);ps(" PCS=");pi(st.t_f1);ps("\nVce: ");pi(st.n_vce);ps(" PCS=");pi(st.t_f2);ps("\nVcv: ");pi(st.n_vcv);ps(" PCS=");pi(st.t_f3);ps("\nVcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vcr_state(void){ps("[VCR] Vcp=");pi(st.n_vcp);ps(" Vce=");pi(st.n_vce);ps(" Vcv=");pi(st.n_vcv);ps(" Vcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vaccaria Admin Demo ===\n\n");vcr_init();
ps("Vaccaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vcr_planning(t,c,874+(i*17),863+(i*14),843+(i*10),825+(i*6),2020+(i%5));}
ps("\nVaccaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vcr_execution(t,c,863+(i*15),852+(i*12),834+(i*8),821+(i*5),2021+(i%4));}
ps("\nVaccaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vcr_evaluation(t,c,855+(i*13),844+(i*10),828+(i*7),817+(i*4),2022+(i%3));}
ps("\nVaccaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vcr_accessory(t,c,847+(i*11),838+(i*9),824+(i*6),814+(i*3),2023+(i%2));}
ps("\nVaccaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vcr_market(t,c,841+(i*9),832+(i*7),819+(i*5),811+(i*3),2024);}
ps("\n");vcr_report();vcr_state();ps("\n=== Demo Complete ===\n");return 0;}
