/* nuphar_admin: Nuphar management technology administration (v1.0)
 * Nuphar planning, nuphar execution, nuphar evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nup_t;
typedef struct{int n_nupp,n_nupe,n_nup2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} nup_state_t;
static nup_t nupps[N],nupss[N-2],nupvss[N-4],nupas[N-6],nupmks[N-6]; static nup_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nup_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nup_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NUP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nup_init(void){if(init)return -1;st.n_nupp=0;st.n_nupe=0;st.n_nup2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nupps[i].active=0;for(int i=0;i<N-2;i++)nupss[i].active=0;for(int i=0;i<N-4;i++)nupvss[i].active=0;for(int i=0;i<N-6;i++)nupas[i].active=0;for(int i=0;i<N-6;i++)nupmks[i].active=0;init=1;ps("[NUP] Nuphar initialized\n");return 0;}
int nup_planning(int t,int c,int a,int b,int d,int e,int y){return add(nupps,&st.n_nupp,&st.t_f1,N,t,c,a,b,d,e,y);}
int nup_execution(int t,int c,int a,int b,int d,int e,int y){return add(nupss,&st.n_nupe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nup_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(nupvss,&st.n_nup2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nup_accessory(int t,int c,int a,int b,int d,int e,int y){return add(nupas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nup_market(int t,int c,int a,int b,int d,int e,int y){return add(nupmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nup_report(void){ps("[NUP] Nupp: ");pi(st.n_nupp);ps(" PCS=");pi(st.t_f1);ps("\nNupe: ");pi(st.n_nupe);ps(" PCS=");pi(st.t_f2);ps("\nNupv: ");pi(st.n_nup2);ps(" PCS=");pi(st.t_f3);ps("\nNupc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void nup_state(void){ps("[NUP] Nupp=");pi(st.n_nupp);ps(" Nupe=");pi(st.n_nupe);ps(" Nupv=");pi(st.n_nup2);ps(" Nupc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Nuphar Admin Demo ===\n\n");nup_init();
ps("Nuphar planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nup_planning(t,c,1067+(i*17),1056+(i*14),1036+(i*10),1018+(i*6),2020+(i%5));}
ps("\nNuphar execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nup_execution(t,c,1056+(i*15),1045+(i*12),1027+(i*8),1014+(i*5),2021+(i%4));}
ps("\nNuphar evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nup_evaluation(t,c,1048+(i*13),1037+(i*10),1021+(i*7),1010+(i*4),2022+(i%3));}
ps("\nNuphar accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nup_accessory(t,c,1040+(i*11),1031+(i*9),1017+(i*6),1007+(i*3),2023+(i%2));}
ps("\nNuphar marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nup_market(t,c,1034+(i*9),1025+(i*7),1012+(i*5),1004+(i*3),2024);}
ps("\n");nup_report();nup_state();ps("\n=== Demo Complete ===\n");return 0;}
