/* fiscal_policy_admin: Fiscal policy administration system (v1.0)
 * Fiscal theory, budget management, tax administration, government debt, fiscal coordination
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fsc_t;
typedef struct{int n_ft,n_bm,n_ta,n_gd,n_fc,t_f1,t_f2,t_f3,t_f4,t_f5;} fsc_state_t;
static fsc_t fts[N],bms[N-2],tas[N-4],gds[N-6],fcs[N-6]; static fsc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fsc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fsc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fsc_init(void){if(init)return -1;st.n_ft=0;st.n_bm=0;st.n_ta=0;st.n_gd=0;st.n_fc=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fts[i].active=0;for(int i=0;i<N-2;i++)bms[i].active=0;for(int i=0;i<N-4;i++)tas[i].active=0;for(int i=0;i<N-6;i++)gds[i].active=0;for(int i=0;i<N-6;i++)fcs[i].active=0;init=1;ps("[FSC] Fiscal policy initialized\n");return 0;}
int fsc_theory(int t,int c,int a,int b,int d,int e,int y){return add(fts,&st.n_ft,&st.t_f1,N,t,c,a,b,d,e,y);}
int fsc_budget(int t,int c,int a,int b,int d,int e,int y){return add(bms,&st.n_bm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fsc_tax(int t,int c,int a,int b,int d,int e,int y){return add(tas,&st.n_ta,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fsc_debt(int t,int c,int a,int b,int d,int e,int y){return add(gds,&st.n_gd,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fsc_coord(int t,int c,int a,int b,int d,int e,int y){return add(fcs,&st.n_fc,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fsc_report(void){ps("[FSC] Theory: ");pi(st.n_ft);ps(" Keynes=");pi(st.t_f1);ps("\nBudget: ");pi(st.n_bm);ps(" Audit=");pi(st.t_f2);ps("\nTax: ");pi(st.n_ta);ps(" Collect=");pi(st.t_f3);ps("\nDebt: ");pi(st.n_gd);ps(" Bond=");pi(st.t_f4);ps("\nCoord: ");pi(st.n_fc);ps(" Reform=");pi(st.t_f5);ps("\n");}
void fsc_state(void){ps("[FSC] Ft=");pi(st.n_ft);ps(" Bm=");pi(st.n_bm);ps(" Ta=");pi(st.n_ta);ps(" Gd=");pi(st.n_gd);ps(" Fc=");pi(st.n_fc);ps("\n");}
int main(void){
ps("=== Fiscal Policy Admin Demo ===\n\n");fsc_init();
ps("Fiscal theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fsc_theory(t,c,87+(i*17),72+(i*14),52+(i*10),35+(i*6),2020+(i%5));}
ps("\nBudget management...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fsc_budget(t,c,76+(i*15),62+(i*12),44+(i*8),31+(i*5),2021+(i%4));}
ps("\nTax administration...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fsc_tax(t,c,68+(i*13),54+(i*10),38+(i*7),27+(i*4),2022+(i%3));}
ps("\nGovernment debt...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fsc_debt(t,c,60+(i*11),48+(i*9),34+(i*6),24+(i*3),2023+(i%2));}
ps("\nFiscal coordination...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fsc_coord(t,c,54+(i*9),43+(i*7),30+(i*5),22+(i*3),2024);}
ps("\n");fsc_report();fsc_state();ps("\n=== Demo Complete ===\n");return 0;}
