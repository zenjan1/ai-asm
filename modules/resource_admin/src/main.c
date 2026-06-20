/* resource_admin: Resource management technology administration (v1.0)
 * Resource planning, resource allocation, resource protection, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rsx_t;
typedef struct{int n_rp,n_rl,n_rt,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rsx_state_t;
static rsx_t rps[N],rls[N-2],rts[N-4],acs[N-6],mks[N-6]; static rsx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rsx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rsx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RSX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rsx_init(void){if(init)return -1;st.n_rp=0;st.n_rl=0;st.n_rt=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rps[i].active=0;for(int i=0;i<N-2;i++)rls[i].active=0;for(int i=0;i<N-4;i++)rts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RSX] Resource initialized\n");return 0;}
int rsx_planning(int t,int c,int a,int b,int d,int e,int y){return add(rps,&st.n_rp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rsx_allocation(int t,int c,int a,int b,int d,int e,int y){return add(rls,&st.n_rl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rsx_protection(int t,int c,int a,int b,int d,int e,int y){return add(rts,&st.n_rt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rsx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rsx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rsx_report(void){ps("[RSX] Rp: ");pi(st.n_rp);ps(" PCS=");pi(st.t_f1);ps("\nRl: ");pi(st.n_rl);ps(" PCS=");pi(st.t_f2);ps("\nRt: ");pi(st.n_rt);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rsx_state(void){ps("[RSX] Rp=");pi(st.n_rp);ps(" Rl=");pi(st.n_rl);ps(" Rt=");pi(st.n_rt);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Resource Admin Demo ===\n\n");rsx_init();
ps("Resource planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rsx_planning(t,c,456+(i*17),445+(i*14),425+(i*10),407+(i*6),2020+(i%5));}
ps("\nResource allocation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rsx_allocation(t,c,445+(i*15),434+(i*12),416+(i*8),403+(i*5),2021+(i%4));}
ps("\nResource protection...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rsx_protection(t,c,437+(i*13),426+(i*10),410+(i*7),399+(i*4),2022+(i%3));}
ps("\nResource accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rsx_accessory(t,c,429+(i*11),420+(i*9),406+(i*6),396+(i*3),2023+(i%2));}
ps("\nResource marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rsx_market(t,c,423+(i*9),414+(i*7),401+(i*5),393+(i*3),2024);}
ps("\n");rsx_report();rsx_state();ps("\n=== Demo Complete ===\n");return 0;}
