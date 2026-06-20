/* service_admin: Service management technology administration (v1.0)
 * Service design, service delivery, service improvement, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} svx_t;
typedef struct{int n_sd,n_sdl,n_si,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} svx_state_t;
static svx_t sds[N],sdls[N-2],sis[N-4],acs[N-6],mks[N-6]; static svx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(svx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;svx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SVX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int svx_init(void){if(init)return -1;st.n_sd=0;st.n_sdl=0;st.n_si=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sds[i].active=0;for(int i=0;i<N-2;i++)sdls[i].active=0;for(int i=0;i<N-4;i++)sis[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SVX] Service initialized\n");return 0;}
int svx_design(int t,int c,int a,int b,int d,int e,int y){return add(sds,&st.n_sd,&st.t_f1,N,t,c,a,b,d,e,y);}
int svx_delivery(int t,int c,int a,int b,int d,int e,int y){return add(sdls,&st.n_sdl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int svx_improvement(int t,int c,int a,int b,int d,int e,int y){return add(sis,&st.n_si,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int svx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int svx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void svx_report(void){ps("[SVX] Sd: ");pi(st.n_sd);ps(" PCS=");pi(st.t_f1);ps("\nSdl: ");pi(st.n_sdl);ps(" PCS=");pi(st.t_f2);ps("\nSi: ");pi(st.n_si);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void svx_state(void){ps("[SVX] Sd=");pi(st.n_sd);ps(" Sdl=");pi(st.n_sdl);ps(" Si=");pi(st.n_si);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Service Admin Demo ===\n\n");svx_init();
ps("Service design...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;svx_design(t,c,459+(i*17),448+(i*14),428+(i*10),410+(i*6),2020+(i%5));}
ps("\nService delivery...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;svx_delivery(t,c,448+(i*15),437+(i*12),419+(i*8),406+(i*5),2021+(i%4));}
ps("\nService improvement...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;svx_improvement(t,c,440+(i*13),429+(i*10),413+(i*7),402+(i*4),2022+(i%3));}
ps("\nService accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;svx_accessory(t,c,432+(i*11),423+(i*9),409+(i*6),399+(i*3),2023+(i%2));}
ps("\nService marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;svx_market(t,c,426+(i*9),417+(i*7),404+(i*5),396+(i*3),2024);}
ps("\n");svx_report();svx_state();ps("\n=== Demo Complete ===\n");return 0;}
