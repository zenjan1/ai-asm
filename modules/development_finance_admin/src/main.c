/* development_finance_admin: Development finance administration (v1.0)
 * Financial development, microfinance, project finance, development institutions, financial stability
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} dfn_t;
typedef struct{int n_fd,n_mf,n_pf,n_di,n_fs,t_f1,t_f2,t_f3,t_f4,t_f5;} dfn_state_t;
static dfn_t fds[N],mfs[N-2],pfs[N-4],dis[N-6],fss[N-6]; static dfn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dfn_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;dfn_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DFN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dfn_init(void){if(init)return -1;st.n_fd=0;st.n_mf=0;st.n_pf=0;st.n_di=0;st.n_fs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fds[i].active=0;for(int i=0;i<N-2;i++)mfs[i].active=0;for(int i=0;i<N-4;i++)pfs[i].active=0;for(int i=0;i<N-6;i++)dis[i].active=0;for(int i=0;i<N-6;i++)fss[i].active=0;init=1;ps("[DFN] Development finance initialized\n");return 0;}
int dfn_develop(int t,int c,int a,int b,int d,int e,int y){return add(fds,&st.n_fd,&st.t_f1,N,t,c,a,b,d,e,y);}
int dfn_micro(int t,int c,int a,int b,int d,int e,int y){return add(mfs,&st.n_mf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dfn_project(int t,int c,int a,int b,int d,int e,int y){return add(pfs,&st.n_pf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dfn_institution(int t,int c,int a,int b,int d,int e,int y){return add(dis,&st.n_di,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dfn_stability(int t,int c,int a,int b,int d,int e,int y){return add(fss,&st.n_fs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dfn_report(void){ps("[DFN] Develop: ");pi(st.n_fd);ps(" Deepen=");pi(st.t_f1);ps("\nMicro: ");pi(st.n_mf);ps(" Credit=");pi(st.t_f2);ps("\nProject: ");pi(st.n_pf);ps(" CBA=");pi(st.t_f3);ps("\nInst: ");pi(st.n_di);ps(" WB=");pi(st.t_f4);ps("\nStabil: ");pi(st.n_fs);ps(" System=");pi(st.t_f5);ps("\n");}
void dfn_state(void){ps("[DFN] Fd=");pi(st.n_fd);ps(" Mf=");pi(st.n_mf);ps(" Pf=");pi(st.n_pf);ps(" Di=");pi(st.n_di);ps(" Fs=");pi(st.n_fs);ps("\n");}
int main(void){
ps("=== Development Finance Admin Demo ===\n\n");dfn_init();
ps("Financial development...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dfn_develop(t,c,114+(i*17),99+(i*14),79+(i*10),62+(i*6),2020+(i%5));}
ps("\nMicrofinance...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dfn_micro(t,c,103+(i*15),89+(i*12),71+(i*8),58+(i*5),2021+(i%4));}
ps("\nProject finance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dfn_project(t,c,95+(i*13),81+(i*10),65+(i*7),54+(i*4),2022+(i%3));}
ps("\nDevelopment institutions...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dfn_institution(t,c,87+(i*11),75+(i*9),61+(i*6),51+(i*3),2023+(i%2));}
ps("\nFinancial stability...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dfn_stability(t,c,81+(i*9),70+(i*7),57+(i*5),49+(i*3),2024);}
ps("\n");dfn_report();dfn_state();ps("\n=== Demo Complete ===\n");return 0;}
