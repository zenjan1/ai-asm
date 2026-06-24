/* nigella_admin: Nigella love-in-a-mist management (v1.0)
 * Nigella sativa (black cumin), N. damascena (love-in-a-mist) cultivation
 * Black seed oil, ornamental seed pods, spice production
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nig_t;
typedef struct{int n_nigo,n_nigs,n_nigp,n_nigm,n_nigb,t_f1,t_f2,t_f3,t_f4,t_f5;} nig_state_t;
static nig_t nigos[N],nigss[N-2],nigps[N-4],nigms[N-6],nigbs[N-6]; static nig_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nig_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nig_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NIG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nig_init(void){if(init)return -1;st.n_nigo=0;st.n_nigs=0;st.n_nigp=0;st.n_nigm=0;st.n_nigb=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nigos[i].active=0;for(int i=0;i<N-2;i++)nigss[i].active=0;for(int i=0;i<N-4;i++)nigps[i].active=0;for(int i=0;i<N-6;i++)nigms[i].active=0;for(int i=0;i<N-6;i++)nigbs[i].active=0;init=1;ps("[NIG] Nigella initialized\n");return 0;}
int nig_oil(int t,int c,int a,int b,int d,int e,int y){return add(nigos,&st.n_nigo,&st.t_f1,N,t,c,a,b,d,e,y);}
int nig_seed(int t,int c,int a,int b,int d,int e,int y){return add(nigss,&st.n_nigs,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nig_pod(int t,int c,int a,int b,int d,int e,int y){return add(nigps,&st.n_nigp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nig_medicinal(int t,int c,int a,int b,int d,int e,int y){return add(nigms,&st.n_nigm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nig_spice(int t,int c,int a,int b,int d,int e,int y){return add(nigbs,&st.n_nigb,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nig_report(void){ps("[NIG] Oil: ");pi(st.n_nigo);ps(" PCS=");pi(st.t_f1);ps("\nSeed: ");pi(st.n_nigs);ps(" PCS=");pi(st.t_f2);ps("\nPod: ");pi(st.n_nigp);ps(" PCS=");pi(st.t_f3);ps("\nMedicinal: ");pi(st.n_nigm);ps(" PCS=");pi(st.t_f4);ps("\nSpice: ");pi(st.n_nigb);ps(" USD=");pi(st.t_f5);ps("\n");}
void nig_state(void){ps("[NIG] Oil=");pi(st.n_nigo);ps(" Sed=");pi(st.n_nigs);ps(" Pod=");pi(st.n_nigp);ps(" Med=");pi(st.n_nigm);ps(" Spi=");pi(st.n_nigb);ps("\n");}
int main(void){
ps("=== Nigella Admin Demo ===\n\n");nig_init();
ps("Black seed oil...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nig_oil(t,c,985+(i*17),974+(i*14),954+(i*10),936+(i*6),2020+(i%5));}
ps("\nSeed production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nig_seed(t,c,974+(i*15),963+(i*12),945+(i*8),932+(i*5),2021+(i%4));}
ps("\nOrnamental pods...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nig_pod(t,c,966+(i*13),955+(i*10),939+(i*7),928+(i*4),2022+(i%3));}
ps("\nTraditional medicine...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nig_medicinal(t,c,958+(i*11),949+(i*9),935+(i*6),925+(i*3),2023+(i%2));}
ps("\nSpice production...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nig_spice(t,c,952+(i*9),943+(i*7),930+(i*5),922+(i*3),2024);}
ps("\n");nig_report();nig_state();ps("\n=== Demo Complete ===\n");return 0;}
