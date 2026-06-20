/* vine_admin: Vine management technology administration (v1.0)
 * Vine planning, vine execution, vine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} vin_t;
typedef struct{int n_vp,n_ve,n_vv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} vin_state_t;
static vin_t vps[N],ves[N-2],vvs[N-4],acs[N-6],mks[N-6]; static vin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;vin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[VIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int vin_init(void){if(init)return -1;st.n_vp=0;st.n_ve=0;st.n_vv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)vps[i].active=0;for(int i=0;i<N-2;i++)ves[i].active=0;for(int i=0;i<N-4;i++)vvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[VIN] Vine initialized\n");return 0;}
int vin_planning(int t,int c,int a,int b,int d,int e,int y){return add(vps,&st.n_vp,&st.t_f1,N,t,c,a,b,d,e,y);}
int vin_execution(int t,int c,int a,int b,int d,int e,int y){return add(ves,&st.n_ve,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int vin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(vvs,&st.n_vv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int vin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int vin_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void vin_report(void){ps("[VIN] Vp: ");pi(st.n_vp);ps(" PCS=");pi(st.t_f1);ps("\nVe: ");pi(st.n_ve);ps(" PCS=");pi(st.t_f2);ps("\nVv: ");pi(st.n_vv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void vin_state(void){ps("[VIN] Vp=");pi(st.n_vp);ps(" Ve=");pi(st.n_ve);ps(" Vv=");pi(st.n_vv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Vine Admin Demo ===\n\n");vin_init();
ps("Vine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;vin_planning(t,c,534+(i*17),523+(i*14),503+(i*10),485+(i*6),2020+(i%5));}
ps("\nVine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;vin_execution(t,c,523+(i*15),512+(i*12),494+(i*8),481+(i*5),2021+(i%4));}
ps("\nVine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;vin_evaluation(t,c,515+(i*13),504+(i*10),488+(i*7),477+(i*4),2022+(i%3));}
ps("\nVine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vin_accessory(t,c,507+(i*11),498+(i*9),484+(i*6),474+(i*3),2023+(i%2));}
ps("\nVine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;vin_market(t,c,501+(i*9),492+(i*7),479+(i*5),471+(i*3),2024);}
ps("\n");vin_report();vin_state();ps("\n=== Demo Complete ===\n");return 0;}
