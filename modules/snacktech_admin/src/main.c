/* snacktech_admin: Snack technology administration (v1.0)
 * Puffed food, nuts, dried fruits, jerky, snack marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} snk_t;
typedef struct{int n_pf,n_nt,n_df,n_jk,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} snk_state_t;
static snk_t pfs[N],nts[N-2],dfs[N-4],jks[N-6],mks[N-6]; static snk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(snk_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;snk_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SNK] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int snk_init(void){if(init)return -1;st.n_pf=0;st.n_nt=0;st.n_df=0;st.n_jk=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pfs[i].active=0;for(int i=0;i<N-2;i++)nts[i].active=0;for(int i=0;i<N-4;i++)dfs[i].active=0;for(int i=0;i<N-6;i++)jks[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SNK] Snacktech initialized\n");return 0;}
int snk_puffed(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f1,N,t,c,a,b,d,e,y);}
int snk_nuts(int t,int c,int a,int b,int d,int e,int y){return add(nts,&st.n_nt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int snk_dried(int t,int c,int a,int b,int d,int e,int y){return add(dfs,&st.n_df,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int snk_jerky(int t,int c,int a,int b,int d,int e,int y){return add(jks,&st.n_jk,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int snk_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void snk_report(void){ps("[SNK] Puffed: ");pi(st.n_pf);ps(" kg=");pi(st.t_f1);ps("\nNuts: ");pi(st.n_nt);ps(" kg=");pi(st.t_f2);ps("\nDried: ");pi(st.n_df);ps(" kg=");pi(st.t_f3);ps("\nJerky: ");pi(st.n_jk);ps(" kg=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void snk_state(void){ps("[SNK] Pf=");pi(st.n_pf);ps(" Nt=");pi(st.n_nt);ps(" Df=");pi(st.n_df);ps(" Jk=");pi(st.n_jk);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Snack Tech Admin Demo ===\n\n");snk_init();
ps("Puffed food...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;snk_puffed(t,c,184+(i*17),169+(i*14),149+(i*10),131+(i*6),2020+(i%5));}
ps("\nNuts...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;snk_nuts(t,c,173+(i*15),159+(i*12),141+(i*8),128+(i*5),2021+(i%4));}
ps("\nDried fruits...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;snk_dried(t,c,165+(i*13),151+(i*10),135+(i*7),124+(i*4),2022+(i%3));}
ps("\nJerky...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;snk_jerky(t,c,157+(i*11),145+(i*9),131+(i*6),121+(i*3),2023+(i%2));}
ps("\nSnack marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;snk_market(t,c,151+(i*9),140+(i*7),127+(i*5),119+(i*3),2024);}
ps("\n");snk_report();snk_state();ps("\n=== Demo Complete ===\n");return 0;}
