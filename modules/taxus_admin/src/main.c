/* taxus_admin: Taxus management technology administration (v1.0)
 * Taxus planning, taxus execution, taxus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tax_t;
typedef struct{int n_taxp,n_taxe,n_tax2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tax_state_t;
static tax_t taxps[N],taxss[N-2],taxvss[N-4],taxas[N-6],taxmks[N-6]; static tax_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tax_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tax_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TAX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tax_init(void){if(init)return -1;st.n_taxp=0;st.n_taxe=0;st.n_tax2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)taxps[i].active=0;for(int i=0;i<N-2;i++)taxss[i].active=0;for(int i=0;i<N-4;i++)taxvss[i].active=0;for(int i=0;i<N-6;i++)taxas[i].active=0;for(int i=0;i<N-6;i++)taxmks[i].active=0;init=1;ps("[TAX] Taxus initialized\n");return 0;}
int tax_planning(int t,int c,int a,int b,int d,int e,int y){return add(taxps,&st.n_taxp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tax_execution(int t,int c,int a,int b,int d,int e,int y){return add(taxss,&st.n_taxe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tax_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(taxvss,&st.n_tax2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tax_accessory(int t,int c,int a,int b,int d,int e,int y){return add(taxas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tax_market(int t,int c,int a,int b,int d,int e,int y){return add(taxmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tax_report(void){ps("[TAX] Taxpp: ");pi(st.n_taxp);ps(" PCS=");pi(st.t_f1);ps("\nTaxe: ");pi(st.n_taxe);ps(" PCS=");pi(st.t_f2);ps("\nTaxv: ");pi(st.n_tax2);ps(" PCS=");pi(st.t_f3);ps("\nTaxc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tax_state(void){ps("[TAX] Taxpp=");pi(st.n_taxp);ps(" Taxe=");pi(st.n_taxe);ps(" Taxv=");pi(st.n_tax2);ps(" Taxc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Taxus Admin Demo ===\n\n");tax_init();
ps("Taxus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tax_planning(t,c,1102+(i*17),1091+(i*14),1071+(i*10),1053+(i*6),2020+(i%5));}
ps("\nTaxus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tax_execution(t,c,1091+(i*15),1080+(i*12),1062+(i*8),1049+(i*5),2021+(i%4));}
ps("\nTaxus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tax_evaluation(t,c,1083+(i*13),1072+(i*10),1056+(i*7),1045+(i*4),2022+(i%3));}
ps("\nTaxus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tax_accessory(t,c,1075+(i*11),1066+(i*9),1052+(i*6),1042+(i*3),2023+(i%2));}
ps("\nTaxus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tax_market(t,c,1069+(i*9),1060+(i*7),1047+(i*5),1039+(i*3),2024);}
ps("\n");tax_report();tax_state();ps("\n=== Demo Complete ===\n");return 0;}
