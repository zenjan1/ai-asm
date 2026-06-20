/* plastictech_admin: Plastic technology administration (v1.0)
 * Plastic raw materials, plastic molding, plastic films, plastic products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pls_t;
typedef struct{int n_pr,n_pm,n_pf,n_pp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pls_state_t;
static pls_t prs[N],pms[N-2],pfs[N-4],pps[N-6],mks[N-6]; static pls_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pls_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pls_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pls_init(void){if(init)return -1;st.n_pr=0;st.n_pm=0;st.n_pf=0;st.n_pp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)prs[i].active=0;for(int i=0;i<N-2;i++)pms[i].active=0;for(int i=0;i<N-4;i++)pfs[i].active=0;for(int i=0;i<N-6;i++)pps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PLS] Plastictech initialized\n");return 0;}
int pls_raw(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f1,N,t,c,a,b,d,e,y);}
int pls_mold(int t,int c,int a,int b,int d,int e,int y){return add(pms,&st.n_pm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pls_film(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pls_product(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pls_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pls_report(void){ps("[PLS] Raw: ");pi(st.n_pr);ps(" kg=");pi(st.t_f1);ps("\nMold: ");pi(st.n_pm);ps(" PCS=");pi(st.t_f2);ps("\nFilm: ");pi(st.n_pf);ps(" m2=");pi(st.t_f3);ps("\nProd: ");pi(st.n_pp);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pls_state(void){ps("[PLS] Pr=");pi(st.n_pr);ps(" Pm=");pi(st.n_pm);ps(" Pf=");pi(st.n_pf);ps(" Pp=");pi(st.n_pp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Plastic Tech Admin Demo ===\n\n");pls_init();
ps("Plastic raw materials...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pls_raw(t,c,212+(i*17),197+(i*14),177+(i*10),159+(i*6),2020+(i%5));}
ps("\nPlastic molding...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pls_mold(t,c,201+(i*15),187+(i*12),169+(i*8),156+(i*5),2021+(i%4));}
ps("\nPlastic films...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pls_film(t,c,193+(i*13),179+(i*10),163+(i*7),152+(i*4),2022+(i%3));}
ps("\nPlastic products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pls_product(t,c,185+(i*11),173+(i*9),159+(i*6),149+(i*3),2023+(i%2));}
ps("\nPlastic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pls_market(t,c,179+(i*9),168+(i*7),155+(i*5),147+(i*3),2024);}
ps("\n");pls_report();pls_state();ps("\n=== Demo Complete ===\n");return 0;}
