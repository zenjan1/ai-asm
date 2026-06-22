/* arundinaria_admin: Arundinaria management technology administration (v1.0)
 * Arundinaria planning, arundinaria execution, arundinaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} arun_t;
typedef struct{int n_arunp,n_arun,n_arun2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} arun_state_t;
static arun_t aruns[N],arune[N-2],arun2[N-4],arunac[N-6],arunam[N-6]; static arun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(arun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;arun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ARUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int arun_init(void){if(init)return -1;st.n_arunp=0;st.n_arun=0;st.n_arun2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aruns[i].active=0;for(int i=0;i<N-2;i++)arune[i].active=0;for(int i=0;i<N-4;i++)arun2[i].active=0;for(int i=0;i<N-6;i++)arunac[i].active=0;for(int i=0;i<N-6;i++)arunam[i].active=0;init=1;ps("[ARUN] Arundinaria initialized\n");return 0;}
int arun_planning(int t,int c,int a,int b,int d,int e,int y){return add(aruns,&st.n_arunp,&st.t_f1,N,t,c,a,b,d,e,y);}
int arun_execution(int t,int c,int a,int b,int d,int e,int y){return add(arune,&st.n_arun,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int arun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(arun2,&st.n_arun2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int arun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(arunac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int arun_market(int t,int c,int a,int b,int d,int e,int y){return add(arunam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void arun_report(void){ps("[ARUN] Runp: ");pi(st.n_arunp);ps(" PCS=");pi(st.t_f1);ps("\nRune: ");pi(st.n_arun);ps(" PCS=");pi(st.t_f2);ps("\nRun2: ");pi(st.n_arun2);ps(" PCS=");pi(st.t_f3);ps("\nRunac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void arun_state(void){ps("[ARUN] Runp=");pi(st.n_arunp);ps(" Rune=");pi(st.n_arun);ps(" Run2=");pi(st.n_arun2);ps(" Runac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Arundinaria Admin Demo ===\n\n");arun_init();
ps("Arundinaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;arun_planning(t,c,1418+(i*17),1407+(i*14),1387+(i*10),1369+(i*6),2020+(i%5));}
ps("\nArundinaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;arun_execution(t,c,1407+(i*15),1396+(i*12),1378+(i*8),1365+(i*5),2021+(i%4));}
ps("\nArundinaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;arun_evaluation(t,c,1399+(i*13),1388+(i*10),1372+(i*7),1361+(i*4),2022+(i%3));}
ps("\nArundinaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arun_accessory(t,c,1391+(i*11),1382+(i*9),1368+(i*6),1358+(i*3),2023+(i%2));}
ps("\nArundinaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;arun_market(t,c,1385+(i*9),1376+(i*7),1363+(i*5),1355+(i*3),2024);}
ps("\n");arun_report();arun_state();ps("\n=== Demo Complete ===\n");return 0;}
