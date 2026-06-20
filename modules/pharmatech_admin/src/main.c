/* pharmatech_admin: Pharma technology administration (v1.0)
 * Drug R&D, drug production, quality control, clinical trials, drug regulation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pht_t;
typedef struct{int n_rd,n_pd,n_qu,n_tr,n_rg,t_f1,t_f2,t_f3,t_f4,t_f5;} pht_state_t;
static pht_t rds[N],pds[N-2],qus[N-4],trs[N-6],rgs[N-6]; static pht_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pht_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pht_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PHT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pht_init(void){if(init)return -1;st.n_rd=0;st.n_pd=0;st.n_qu=0;st.n_tr=0;st.n_rg=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rds[i].active=0;for(int i=0;i<N-2;i++)pds[i].active=0;for(int i=0;i<N-4;i++)qus[i].active=0;for(int i=0;i<N-6;i++)trs[i].active=0;for(int i=0;i<N-6;i++)rgs[i].active=0;init=1;ps("[PHT] Pharmatech initialized\n");return 0;}
int pht_rnd(int t,int c,int a,int b,int d,int e,int y){return add(rds,&st.n_rd,&st.t_f1,N,t,c,a,b,d,e,y);}
int pht_production(int t,int c,int a,int b,int d,int e,int y){return add(pds,&st.n_pd,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pht_quality(int t,int c,int a,int b,int d,int e,int y){return add(qus,&st.n_qu,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pht_trial(int t,int c,int a,int b,int d,int e,int y){return add(trs,&st.n_tr,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pht_regulation(int t,int c,int a,int b,int d,int e,int y){return add(rgs,&st.n_rg,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pht_report(void){ps("[PHT] R&D: ");pi(st.n_rd);ps(" Mol=");pi(st.t_f1);ps("\nProd: ");pi(st.n_pd);ps(" GMP=");pi(st.t_f2);ps("\nQual: ");pi(st.n_qu);ps(" QC=");pi(st.t_f3);ps("\nTrial: ");pi(st.n_tr);ps(" Ph=");pi(st.t_f4);ps("\nReg: ");pi(st.n_rg);ps(" FDA=");pi(st.t_f5);ps("\n");}
void pht_state(void){ps("[PHT] Rd=");pi(st.n_rd);ps(" Pd=");pi(st.n_pd);ps(" Qu=");pi(st.n_qu);ps(" Tr=");pi(st.n_tr);ps(" Rg=");pi(st.n_rg);ps("\n");}
int main(void){
ps("=== Pharma Tech Admin Demo ===\n\n");pht_init();
ps("Drug R&D...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pht_rnd(t,c,160+(i*17),145+(i*14),125+(i*10),108+(i*6),2020+(i%5));}
ps("\nDrug production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pht_production(t,c,149+(i*15),135+(i*12),117+(i*8),104+(i*5),2021+(i%4));}
ps("\nQuality control...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pht_quality(t,c,141+(i*13),127+(i*10),111+(i*7),100+(i*4),2022+(i%3));}
ps("\nClinical trials...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pht_trial(t,c,133+(i*11),121+(i*9),107+(i*6),97+(i*3),2023+(i%2));}
ps("\nDrug regulation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pht_regulation(t,c,127+(i*9),116+(i*7),103+(i*5),95+(i*3),2024);}
ps("\n");pht_report();pht_state();ps("\n=== Demo Complete ===\n");return 0;}
