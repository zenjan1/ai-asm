/* erica_admin: Erica management technology administration (v1.0)
 * Erica planning, erica execution, erica evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eri_t;
typedef struct{int n_erip,n_erie,n_eri2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eri_state_t;
static eri_t erips[N],eriss[N-2],erivss[N-4],erias[N-6],erimks[N-6]; static eri_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eri_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eri_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ERI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eri_init(void){if(init)return -1;st.n_erip=0;st.n_erie=0;st.n_eri2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)erips[i].active=0;for(int i=0;i<N-2;i++)eriss[i].active=0;for(int i=0;i<N-4;i++)erivss[i].active=0;for(int i=0;i<N-6;i++)erias[i].active=0;for(int i=0;i<N-6;i++)erimks[i].active=0;init=1;ps("[ERI] Erica initialized\n");return 0;}
int eri_planning(int t,int c,int a,int b,int d,int e,int y){return add(erips,&st.n_erip,&st.t_f1,N,t,c,a,b,d,e,y);}
int eri_execution(int t,int c,int a,int b,int d,int e,int y){return add(eriss,&st.n_erie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eri_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(erivss,&st.n_eri2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eri_accessory(int t,int c,int a,int b,int d,int e,int y){return add(erias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eri_market(int t,int c,int a,int b,int d,int e,int y){return add(erimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eri_report(void){ps("[ERI] Erip: ");pi(st.n_erip);ps(" PCS=");pi(st.t_f1);ps("\nErie: ");pi(st.n_erie);ps(" PCS=");pi(st.t_f2);ps("\nEriv: ");pi(st.n_eri2);ps(" PCS=");pi(st.t_f3);ps("\nEric: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eri_state(void){ps("[ERI] Erip=");pi(st.n_erip);ps(" Erie=");pi(st.n_erie);ps(" Eriv=");pi(st.n_eri2);ps(" Eric=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Erica Admin Demo ===\n\n");eri_init();
ps("Erica planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eri_planning(t,c,1278+(i*17),1267+(i*14),1247+(i*10),1229+(i*6),2020+(i%5));}
ps("\nErica execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eri_execution(t,c,1267+(i*15),1256+(i*12),1238+(i*8),1225+(i*5),2021+(i%4));}
ps("\nErica evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eri_evaluation(t,c,1259+(i*13),1248+(i*10),1232+(i*7),1221+(i*4),2022+(i%3));}
ps("\nErica accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eri_accessory(t,c,1251+(i*11),1242+(i*9),1228+(i*6),1218+(i*3),2023+(i%2));}
ps("\nErica marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eri_market(t,c,1245+(i*9),1236+(i*7),1223+(i*5),1215+(i*3),2024);}
ps("\n");eri_report();eri_state();ps("\n=== Demo Complete ===\n");return 0;}
