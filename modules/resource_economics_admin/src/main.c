/* resource_economics_admin: Resource economics administration (v1.0)
 * Renewable resources, non-renewable resources, resource pricing, resource management, resource policy
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} res_t;
typedef struct{int n_rr,n_nr,n_rp,n_rm,n_ro,t_f1,t_f2,t_f3,t_f4,t_f5;} res_state_t;
static res_t rrs[N],nrs[N-2],rps[N-4],rms[N-6],ros[N-6]; static res_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(res_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;res_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RES] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int res_init(void){if(init)return -1;st.n_rr=0;st.n_nr=0;st.n_rp=0;st.n_rm=0;st.n_ro=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rrs[i].active=0;for(int i=0;i<N-2;i++)nrs[i].active=0;for(int i=0;i<N-4;i++)rps[i].active=0;for(int i=0;i<N-6;i++)rms[i].active=0;for(int i=0;i<N-6;i++)ros[i].active=0;init=1;ps("[RES] Resource economics initialized\n");return 0;}
int res_renew(int t,int c,int a,int b,int d,int e,int y){return add(rrs,&st.n_rr,&st.t_f1,N,t,c,a,b,d,e,y);}
int res_nonrenew(int t,int c,int a,int b,int d,int e,int y){return add(nrs,&st.n_nr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int res_pricing(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int res_manage(int t,int c,int a,int b,int d,int e,int y){return add(rms,&st.n_rm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int res_policy(int t,int c,int a,int b,int d,int e,int y){return add(ros,&st.n_ro,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void res_report(void){ps("[RES] Renew: ");pi(st.n_rr);ps(" Fish=");pi(st.t_f1);ps("\nNonRenew: ");pi(st.n_nr);ps(" Mineral=");pi(st.t_f2);ps("\nPricing: ");pi(st.n_rp);ps(" Tax=");pi(st.t_f3);ps("\nManage: ");pi(st.n_rm);ps(" Plan=");pi(st.t_f4);ps("\nPolicy: ");pi(st.n_ro);ps(" Right=");pi(st.t_f5);ps("\n");}
void res_state(void){ps("[RES] Rr=");pi(st.n_rr);ps(" Nr=");pi(st.n_nr);ps(" Rp=");pi(st.n_rp);ps(" Rm=");pi(st.n_rm);ps(" Ro=");pi(st.n_ro);ps("\n");}
int main(void){
ps("=== Resource Economics Admin Demo ===\n\n");res_init();
ps("Renewable resources...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;res_renew(t,c,111+(i*17),96+(i*14),76+(i*10),59+(i*6),2020+(i%5));}
ps("\nNon-renewable resources...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;res_nonrenew(t,c,100+(i*15),86+(i*12),68+(i*8),55+(i*5),2021+(i%4));}
ps("\nResource pricing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;res_pricing(t,c,92+(i*13),78+(i*10),62+(i*7),51+(i*4),2022+(i%3));}
ps("\nResource management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;res_manage(t,c,84+(i*11),72+(i*9),58+(i*6),48+(i*3),2023+(i%2));}
ps("\nResource policy...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;res_policy(t,c,78+(i*9),67+(i*7),54+(i*5),46+(i*3),2024);}
ps("\n");res_report();res_state();ps("\n=== Demo Complete ===\n");return 0;}
