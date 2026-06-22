/* cataria_admin: Cataria management technology administration (v1.0)
 * Cataria planning, cataria execution, cataria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cata_t;
typedef struct{int n_catap,n_cata,n_cata2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cata_state_t;
static cata_t catas[N],catae[N-2],cata2[N-4],cataac[N-6],catam[N-6]; static cata_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cata_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cata_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CATA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cata_init(void){if(init)return -1;st.n_catap=0;st.n_cata=0;st.n_cata2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)catas[i].active=0;for(int i=0;i<N-2;i++)catae[i].active=0;for(int i=0;i<N-4;i++)cata2[i].active=0;for(int i=0;i<N-6;i++)cataac[i].active=0;for(int i=0;i<N-6;i++)catam[i].active=0;init=1;ps("[CATA] Cataria initialized\n");return 0;}
int cata_planning(int t,int c,int a,int b,int d,int e,int y){return add(catas,&st.n_catap,&st.t_f1,N,t,c,a,b,d,e,y);}
int cata_execution(int t,int c,int a,int b,int d,int e,int y){return add(catae,&st.n_cata,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cata_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cata2,&st.n_cata2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cata_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cataac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cata_market(int t,int c,int a,int b,int d,int e,int y){return add(catam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cata_report(void){ps("[CATA] Atap: ");pi(st.n_catap);ps(" PCS=");pi(st.t_f1);ps("\nAtae: ");pi(st.n_cata);ps(" PCS=");pi(st.t_f2);ps("\nAta2: ");pi(st.n_cata2);ps(" PCS=");pi(st.t_f3);ps("\nAtac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cata_state(void){ps("[CATA] Atap=");pi(st.n_catap);ps(" Ata=");pi(st.n_cata);ps(" Ata2=");pi(st.n_cata2);ps(" Atac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cataria Admin Demo ===\n\n");cata_init();
ps("Cataria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cata_planning(t,c,1466+(i*17),1455+(i*14),1435+(i*10),1417+(i*6),2020+(i%5));}
ps("\nCataria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cata_execution(t,c,1455+(i*15),1444+(i*12),1426+(i*8),1413+(i*5),2021+(i%4));}
ps("\nCataria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cata_evaluation(t,c,1447+(i*13),1436+(i*10),1420+(i*7),1409+(i*4),2022+(i%3));}
ps("\nCataria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cata_accessory(t,c,1439+(i*11),1430+(i*9),1416+(i*6),1406+(i*3),2023+(i%2));}
ps("\nCataria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cata_market(t,c,1433+(i*9),1424+(i*7),1411+(i*5),1403+(i*3),2024);}
ps("\n");cata_report();cata_state();ps("\n=== Demo Complete ===\n");return 0;}
