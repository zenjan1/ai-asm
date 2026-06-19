/* public_finance_admin: Public finance administration system (v1.0)
 * Fiscal theory, tax theory, fiscal expenditure, debt management, fiscal system
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fin_t;
typedef struct{int n_ft,n_tx,n_fe,n_dm,n_fs,t_f1,t_f2,t_f3,t_f4,t_f5;} fin_state_t;
static fin_t fts[N],txs[N-2],fes[N-4],dms[N-6],fss[N-6]; static fin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fin_init(void){if(init)return -1;st.n_ft=0;st.n_tx=0;st.n_fe=0;st.n_dm=0;st.n_fs=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fts[i].active=0;for(int i=0;i<N-2;i++)txs[i].active=0;for(int i=0;i<N-4;i++)fes[i].active=0;for(int i=0;i<N-6;i++)dms[i].active=0;for(int i=0;i<N-6;i++)fss[i].active=0;init=1;ps("[FIN] Public finance initialized\n");return 0;}
int fin_theory(int t,int c,int a,int b,int d,int e,int y){return add(fts,&st.n_ft,&st.t_f1,N,t,c,a,b,d,e,y);}
int fin_tax(int t,int c,int a,int b,int d,int e,int y){return add(txs,&st.n_tx,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fin_expenditure(int t,int c,int a,int b,int d,int e,int y){return add(fes,&st.n_fe,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fin_debt(int t,int c,int a,int b,int d,int e,int y){return add(dms,&st.n_dm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fin_system(int t,int c,int a,int b,int d,int e,int y){return add(fss,&st.n_fs,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fin_report(void){ps("[FIN] Theory: ");pi(st.n_ft);ps(" Public=");pi(st.t_f1);ps("\nTax: ");pi(st.n_tx);ps(" Revenue=");pi(st.t_f2);ps("\nExpend: ");pi(st.n_fe);ps(" Spend=");pi(st.t_f3);ps("\nDebt: ");pi(st.n_dm);ps(" Bond=");pi(st.t_f4);ps("\nSystem: ");pi(st.n_fs);ps(" Budget=");pi(st.t_f5);ps("\n");}
void fin_state(void){ps("[FIN] Ft=");pi(st.n_ft);ps(" Tx=");pi(st.n_tx);ps(" Fe=");pi(st.n_fe);ps(" Dm=");pi(st.n_dm);ps(" Fs=");pi(st.n_fs);ps("\n");}
int main(void){
ps("=== Public Finance Admin Demo ===\n\n");fin_init();
ps("Fiscal theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fin_theory(t,c,86+(i*17),71+(i*14),51+(i*10),34+(i*6),2020+(i%5));}
ps("\nTax theory...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fin_tax(t,c,75+(i*15),61+(i*12),43+(i*8),30+(i*5),2021+(i%4));}
ps("\nFiscal expenditure...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fin_expenditure(t,c,67+(i*13),53+(i*10),37+(i*7),26+(i*4),2022+(i%3));}
ps("\nDebt management...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fin_debt(t,c,59+(i*11),47+(i*9),33+(i*6),23+(i*3),2023+(i%2));}
ps("\nFiscal system...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fin_system(t,c,53+(i*9),42+(i*7),29+(i*5),21+(i*3),2024);}
ps("\n");fin_report();fin_state();ps("\n=== Demo Complete ===\n");return 0;}
