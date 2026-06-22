/* caragana_admin: Caragana management technology administration (v1.0)
 * Caragana planning, caragana execution, caragana evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cara_t;
typedef struct{int n_carap,n_cara,n_cara2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cara_state_t;
static cara_t caras[N],carac_e[N-2],cara2[N-4],caraac[N-6],caram[N-6]; static cara_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cara_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cara_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CARA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cara_init(void){if(init)return -1;st.n_carap=0;st.n_cara=0;st.n_cara2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)caras[i].active=0;for(int i=0;i<N-2;i++)carac_e[i].active=0;for(int i=0;i<N-4;i++)cara2[i].active=0;for(int i=0;i<N-6;i++)caraac[i].active=0;for(int i=0;i<N-6;i++)caram[i].active=0;init=1;ps("[CARA] Caragana initialized\n");return 0;}
int cara_planning(int t,int c,int a,int b,int d,int e,int y){return add(caras,&st.n_carap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cara_execution(int t,int c,int a,int b,int d,int e,int y){return add(carac_e,&st.n_cara,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cara_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cara2,&st.n_cara2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cara_accessory(int t,int c,int a,int b,int d,int e,int y){return add(caraac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cara_market(int t,int c,int a,int b,int d,int e,int y){return add(caram,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cara_report(void){ps("[CARA] Arp: ");pi(st.n_carap);ps(" PCS=");pi(st.t_f1);ps("\nAre: ");pi(st.n_cara);ps(" PCS=");pi(st.t_f2);ps("\nAr2: ");pi(st.n_cara2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cara_state(void){ps("[CARA] Arp=");pi(st.n_carap);ps(" Ara=");pi(st.n_cara);ps(" Ar2=");pi(st.n_cara2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Caragana Admin Demo ===\n\n");cara_init();
ps("Caragana planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cara_planning(t,c,1459+(i*17),1448+(i*14),1428+(i*10),1410+(i*6),2020+(i%5));}
ps("\nCaragana execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cara_execution(t,c,1448+(i*15),1437+(i*12),1419+(i*8),1406+(i*5),2021+(i%4));}
ps("\nCaragana evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cara_evaluation(t,c,1440+(i*13),1429+(i*10),1413+(i*7),1402+(i*4),2022+(i%3));}
ps("\nCaragana accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cara_accessory(t,c,1432+(i*11),1423+(i*9),1409+(i*6),1399+(i*3),2023+(i%2));}
ps("\nCaragana marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cara_market(t,c,1426+(i*9),1417+(i*7),1404+(i*5),1396+(i*3),2024);}
ps("\n");cara_report();cara_state();ps("\n=== Demo Complete ===\n");return 0;}
