/* hydrastis_admin: Hydrastis management technology administration (v1.0)
 * Hydrastis planning, hydrastis execution, hydrastis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hydr_t;
typedef struct{int n_hydrp,n_hydr,n_hydr2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hydr_state_t;
static hydr_t hydra[N],hydře[N-2],hydr2[N-4],hydrac[N-6],hydram[N-6]; static hydr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hydr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hydr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HYDR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hydr_init(void){if(init)return -1;st.n_hydrp=0;st.n_hydr=0;st.n_hydr2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hydra[i].active=0;for(int i=0;i<N-2;i++)hydře[i].active=0;for(int i=0;i<N-4;i++)hydr2[i].active=0;for(int i=0;i<N-6;i++)hydrac[i].active=0;for(int i=0;i<N-6;i++)hydram[i].active=0;init=1;ps("[HYDR] Hydrastis initialized\n");return 0;}
int hydr_planning(int t,int c,int a,int b,int d,int e,int y){return add(hydra,&st.n_hydrp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hydr_execution(int t,int c,int a,int b,int d,int e,int y){return add(hydře,&st.n_hydr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hydr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hydr2,&st.n_hydr2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hydr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hydrac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hydr_market(int t,int c,int a,int b,int d,int e,int y){return add(hydram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hydr_report(void){ps("[HYDR] Hydrp: ");pi(st.n_hydrp);ps(" PCS=");pi(st.t_f1);ps("\nHydre: ");pi(st.n_hydr);ps(" PCS=");pi(st.t_f2);ps("\nHydr2: ");pi(st.n_hydr2);ps(" PCS=");pi(st.t_f3);ps("\nHac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hydr_state(void){ps("[HYDR] Hydrp=");pi(st.n_hydrp);ps(" Hydr=");pi(st.n_hydr);ps(" Hydr2=");pi(st.n_hydr2);ps(" Hac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hydrastis Admin Demo ===\n\n");hydr_init();
ps("Hydrastis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hydr_planning(t,c,1538+(i*17),1527+(i*14),1507+(i*10),1489+(i*6),2020+(i%5));}
ps("\nHydrastis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hydr_execution(t,c,1527+(i*15),1516+(i*12),1498+(i*8),1485+(i*5),2021+(i%4));}
ps("\nHydrastis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hydr_evaluation(t,c,1519+(i*13),1508+(i*10),1492+(i*7),1481+(i*4),2022+(i%3));}
ps("\nHydrastis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hydr_accessory(t,c,1511+(i*11),1502+(i*9),1488+(i*6),1478+(i*3),2023+(i%2));}
ps("\nHydrastis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hydr_market(t,c,1505+(i*9),1496+(i*7),1483+(i*5),1475+(i*3),2024);}
ps("\n");hydr_report();hydr_state();ps("\n=== Demo Complete ===\n");return 0;}
