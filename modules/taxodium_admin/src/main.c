/* taxodium_admin: Taxodium management technology administration (v1.0)
 * Taxodium planning, taxodium execution, taxodium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} taxo_t;
typedef struct{int n_taxop,n_taxoe,n_taxo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} taxo_state_t;
static taxo_t taxos[N],taxoess[N-2],taxovss[N-4],taxoas[N-6],taxomks[N-6]; static taxo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(taxo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;taxo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TAXO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int taxo_init(void){if(init)return -1;st.n_taxop=0;st.n_taxoe=0;st.n_taxo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)taxos[i].active=0;for(int i=0;i<N-2;i++)taxoess[i].active=0;for(int i=0;i<N-4;i++)taxovss[i].active=0;for(int i=0;i<N-6;i++)taxoas[i].active=0;for(int i=0;i<N-6;i++)taxomks[i].active=0;init=1;ps("[TAXO] Taxodium initialized\n");return 0;}
int taxo_planning(int t,int c,int a,int b,int d,int e,int y){return add(taxos,&st.n_taxop,&st.t_f1,N,t,c,a,b,d,e,y);}
int taxo_execution(int t,int c,int a,int b,int d,int e,int y){return add(taxoess,&st.n_taxoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int taxo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(taxovss,&st.n_taxo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int taxo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(taxoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int taxo_market(int t,int c,int a,int b,int d,int e,int y){return add(taxomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void taxo_report(void){ps("[TAXO] Taxop: ");pi(st.n_taxop);ps(" PCS=");pi(st.t_f1);ps("\nTaxoe: ");pi(st.n_taxoe);ps(" PCS=");pi(st.t_f2);ps("\nTaxo2: ");pi(st.n_taxo2);ps(" PCS=");pi(st.t_f3);ps("\nTaxoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void taxo_state(void){ps("[TAXO] Taxop=");pi(st.n_taxop);ps(" Taxoe=");pi(st.n_taxoe);ps(" Taxo2=");pi(st.n_taxo2);ps(" Taxoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Taxodium Admin Demo ===\n\n");taxo_init();
ps("Taxodium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;taxo_planning(t,c,1346+(i*17),1335+(i*14),1315+(i*10),1297+(i*6),2020+(i%5));}
ps("\nTaxodium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;taxo_execution(t,c,1335+(i*15),1324+(i*12),1306+(i*8),1293+(i*5),2021+(i%4));}
ps("\nTaxodium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;taxo_evaluation(t,c,1327+(i*13),1316+(i*10),1300+(i*7),1289+(i*4),2022+(i%3));}
ps("\nTaxodium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;taxo_accessory(t,c,1319+(i*11),1310+(i*9),1296+(i*6),1286+(i*3),2023+(i%2));}
ps("\nTaxodium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;taxo_market(t,c,1313+(i*9),1304+(i*7),1291+(i*5),1283+(i*3),2024);}
ps("\n");taxo_report();taxo_state();ps("\n=== Demo Complete ===\n");return 0;}
