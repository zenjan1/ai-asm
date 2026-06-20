/* reducer_admin: Reducer technology administration (v1.0)
 * Gear reducer, worm reducer, planetary reducer, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rdc_t;
typedef struct{int n_gr,n_wr,n_pr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rdc_state_t;
static rdc_t grs[N],wrs[N-2],prs[N-4],acs[N-6],mks[N-6]; static rdc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rdc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rdc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RDC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rdc_init(void){if(init)return -1;st.n_gr=0;st.n_wr=0;st.n_pr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)grs[i].active=0;for(int i=0;i<N-2;i++)wrs[i].active=0;for(int i=0;i<N-4;i++)prs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RDC] Reducer initialized\n");return 0;}
int rdc_gear(int t,int c,int a,int b,int d,int e,int y){return add(grs,&st.n_gr,&st.t_f1,N,t,c,a,b,d,e,y);}
int rdc_worm(int t,int c,int a,int b,int d,int e,int y){return add(wrs,&st.n_wr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rdc_planetary(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rdc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rdc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rdc_report(void){ps("[RDC] Gr: ");pi(st.n_gr);ps(" PCS=");pi(st.t_f1);ps("\nWr: ");pi(st.n_wr);ps(" PCS=");pi(st.t_f2);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rdc_state(void){ps("[RDC] Gr=");pi(st.n_gr);ps(" Wr=");pi(st.n_wr);ps(" Pr=");pi(st.n_pr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Reducer Admin Demo ===\n\n");rdc_init();
ps("Gear reducers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rdc_gear(t,c,376+(i*17),361+(i*14),341+(i*10),323+(i*6),2020+(i%5));}
ps("\nWorm reducers...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rdc_worm(t,c,365+(i*15),351+(i*12),333+(i*8),320+(i*5),2021+(i%4));}
ps("\nPlanetary reducers...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rdc_planetary(t,c,357+(i*13),343+(i*10),327+(i*7),316+(i*4),2022+(i%3));}
ps("\nReducer accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdc_accessory(t,c,349+(i*11),337+(i*9),323+(i*6),313+(i*3),2023+(i%2));}
ps("\nReducer marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rdc_market(t,c,343+(i*9),332+(i*7),319+(i*5),311+(i*3),2024);}
ps("\n");rdc_report();rdc_state();ps("\n=== Demo Complete ===\n");return 0;}
