/* tobaccotech_admin: Tobacco technology administration (v1.0)
 * Tobacco planting, tobacco processing, cigarette production, cigar production, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tbc_t;
typedef struct{int n_tp,n_tpc,n_cg,n_cgr,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tbc_state_t;
static tbc_t tps[N],tpcs[N-2],cgs[N-4],cgrs[N-6],mks[N-6]; static tbc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tbc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tbc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TBC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tbc_init(void){if(init)return -1;st.n_tp=0;st.n_tpc=0;st.n_cg=0;st.n_cgr=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tps[i].active=0;for(int i=0;i<N-2;i++)tpcs[i].active=0;for(int i=0;i<N-4;i++)cgs[i].active=0;for(int i=0;i<N-6;i++)cgrs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TBC] Tobaccotech initialized\n");return 0;}
int tbc_plant(int t,int c,int a,int b,int d,int e,int y){return add(tps,&st.n_tp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tbc_process(int t,int c,int a,int b,int d,int e,int y){return add(tpcs,&st.n_tpc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tbc_cigarette(int t,int c,int a,int b,int d,int e,int y){return add(cgs,&st.n_cg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tbc_cigar(int t,int c,int a,int b,int d,int e,int y){return add(cgrs,&st.n_cgr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tbc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tbc_report(void){ps("[TBC] Plant: ");pi(st.n_tp);ps(" ha=");pi(st.t_f1);ps("\nProc: ");pi(st.n_tpc);ps(" Ton=");pi(st.t_f2);ps("\nCig: ");pi(st.n_cg);ps(" PCS=");pi(st.t_f3);ps("\nCigar: ");pi(st.n_cgr);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tbc_state(void){ps("[TBC] Tp=");pi(st.n_tp);ps(" Tpc=");pi(st.n_tpc);ps(" Cg=");pi(st.n_cg);ps(" Cgr=");pi(st.n_cgr);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tobacco Tech Admin Demo ===\n\n");tbc_init();
ps("Tobacco planting...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tbc_plant(t,c,205+(i*17),190+(i*14),170+(i*10),152+(i*6),2020+(i%5));}
ps("\nTobacco processing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tbc_process(t,c,194+(i*15),180+(i*12),162+(i*8),149+(i*5),2021+(i%4));}
ps("\nCigarette production...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tbc_cigarette(t,c,186+(i*13),172+(i*10),156+(i*7),145+(i*4),2022+(i%3));}
ps("\nCigar production...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbc_cigar(t,c,178+(i*11),166+(i*9),152+(i*6),142+(i*3),2023+(i%2));}
ps("\nTobacco marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tbc_market(t,c,172+(i*9),161+(i*7),148+(i*5),140+(i*3),2024);}
ps("\n");tbc_report();tbc_state();ps("\n=== Demo Complete ===\n");return 0;}
