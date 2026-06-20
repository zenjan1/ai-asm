/* papertech_admin: Paper technology administration (v1.0)
 * Pulp production, paper making, coating, paper products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ppr_t;
typedef struct{int n_plp,n_ppm,n_ct,n_ppd,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ppr_state_t;
static ppr_t plps[N],ppms[N-2],cts[N-4],ppds[N-6],mks[N-6]; static ppr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ppr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ppr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PPR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ppr_init(void){if(init)return -1;st.n_plp=0;st.n_ppm=0;st.n_ct=0;st.n_ppd=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)plps[i].active=0;for(int i=0;i<N-2;i++)ppms[i].active=0;for(int i=0;i<N-4;i++)cts[i].active=0;for(int i=0;i<N-6;i++)ppds[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PPR] Papertech initialized\n");return 0;}
int ppr_pulp(int t,int c,int a,int b,int d,int e,int y){return add(plps,&st.n_plp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ppr_paper(int t,int c,int a,int b,int d,int e,int y){return add(ppms,&st.n_ppm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ppr_coat(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ppr_product(int t,int c,int a,int b,int d,int e,int y){return add(ppds,&st.n_ppd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ppr_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ppr_report(void){ps("[PPR] Pulp: ");pi(st.n_plp);ps(" Ton=");pi(st.t_f1);ps("\nPaper: ");pi(st.n_ppm);ps(" Ton=");pi(st.t_f2);ps("\nCoat: ");pi(st.n_ct);ps(" Ton=");pi(st.t_f3);ps("\nProd: ");pi(st.n_ppd);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ppr_state(void){ps("[PPR] Plp=");pi(st.n_plp);ps(" Ppm=");pi(st.n_ppm);ps(" Ct=");pi(st.n_ct);ps(" Ppd=");pi(st.n_ppd);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Paper Tech Admin Demo ===\n\n");ppr_init();
ps("Pulp production...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ppr_pulp(t,c,206+(i*17),191+(i*14),171+(i*10),153+(i*6),2020+(i%5));}
ps("\nPaper making...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ppr_paper(t,c,195+(i*15),181+(i*12),163+(i*8),150+(i*5),2021+(i%4));}
ps("\nCoating...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ppr_coat(t,c,187+(i*13),173+(i*10),157+(i*7),146+(i*4),2022+(i%3));}
ps("\nPaper products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ppr_product(t,c,179+(i*11),167+(i*9),153+(i*6),143+(i*3),2023+(i%2));}
ps("\nPaper marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ppr_market(t,c,173+(i*9),162+(i*7),149+(i*5),141+(i*3),2024);}
ps("\n");ppr_report();ppr_state();ps("\n=== Demo Complete ===\n");return 0;}
