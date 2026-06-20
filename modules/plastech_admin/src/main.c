/* plastech_admin: Plastics technology administration (v1.0)
 * Plastic films, plastic pipes, plastic products, engineering plastics, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pls_t;
typedef struct{int n_pf,n_pp,n_pr,n_ep,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pls_state_t;
static pls_t pfs[N],pps[N-2],prs[N-4],eps[N-6],mks[N-6]; static pls_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pls_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pls_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pls_init(void){if(init)return -1;st.n_pf=0;st.n_pp=0;st.n_pr=0;st.n_ep=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pfs[i].active=0;for(int i=0;i<N-2;i++)pps[i].active=0;for(int i=0;i<N-4;i++)prs[i].active=0;for(int i=0;i<N-6;i++)eps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PLS] Plastech initialized\n");return 0;}
int pls_film(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f1,N,t,c,a,b,d,e,y);}
int pls_pipe(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pls_product(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pls_engineering(int t,int c,int a,int b,int d,int e,int y){return add(eps,&st.n_ep,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pls_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pls_report(void){ps("[PLS] Pf: ");pi(st.n_pf);ps(" PCS=");pi(st.t_f1);ps("\nPp: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f2);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f3);ps("\nEp: ");pi(st.n_ep);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pls_state(void){ps("[PLS] Pf=");pi(st.n_pf);ps(" Pp=");pi(st.n_pp);ps(" Pr=");pi(st.n_pr);ps(" Ep=");pi(st.n_ep);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Plastics Tech Admin Demo ===\n\n");pls_init();
ps("Plastic films...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pls_film(t,c,247+(i*17),232+(i*14),212+(i*10),194+(i*6),2020+(i%5));}
ps("\nPlastic pipes...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pls_pipe(t,c,236+(i*15),222+(i*12),204+(i*8),191+(i*5),2021+(i%4));}
ps("\nPlastic products...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pls_product(t,c,228+(i*13),214+(i*10),198+(i*7),187+(i*4),2022+(i%3));}
ps("\nEngineering plastics...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pls_engineering(t,c,220+(i*11),208+(i*9),194+(i*6),184+(i*3),2023+(i%2));}
ps("\nPlastics marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pls_market(t,c,214+(i*9),203+(i*7),190+(i*5),182+(i*3),2024);}
ps("\n");pls_report();pls_state();ps("\n=== Demo Complete ===\n");return 0;}
