/* allium_admin: Allium management technology administration (v1.0)
 * Allium planning, allium execution, allium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alli_t;
typedef struct{int n_allip,n_alli,n_alli2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alli_state_t;
static alli_t allis[N],allie[N-2],alliv[N-4],alliac[N-6],allim[N-6]; static alli_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(alli_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alli_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALLI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alli_init(void){if(init)return -1;st.n_allip=0;st.n_alli=0;st.n_alli2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)allis[i].active=0;for(int i=0;i<N-2;i++)allie[i].active=0;for(int i=0;i<N-4;i++)alliv[i].active=0;for(int i=0;i<N-6;i++)alliac[i].active=0;for(int i=0;i<N-6;i++)allim[i].active=0;init=1;ps("[ALLI] Allium initialized\n");return 0;}
int alli_planning(int t,int c,int a,int b,int d,int e,int y){return add(allis,&st.n_allip,&st.t_f1,N,t,c,a,b,d,e,y);}
int alli_execution(int t,int c,int a,int b,int d,int e,int y){return add(allie,&st.n_alli,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alli_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(alliv,&st.n_alli2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alli_accessory(int t,int c,int a,int b,int d,int e,int y){return add(alliac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alli_market(int t,int c,int a,int b,int d,int e,int y){return add(allim,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alli_report(void){ps("[ALLI] Lip: ");pi(st.n_allip);ps(" PCS=");pi(st.t_f1);ps("\nLie: ");pi(st.n_alli);ps(" PCS=");pi(st.t_f2);ps("\nLiv: ");pi(st.n_alli2);ps(" PCS=");pi(st.t_f3);ps("\nLiac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alli_state(void){ps("[ALLI] Lip=");pi(st.n_allip);ps(" Lie=");pi(st.n_alli);ps(" Liv=");pi(st.n_alli2);ps(" Liac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Allium Admin Demo ===\n\n");alli_init();
ps("Allium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alli_planning(t,c,1398+(i*17),1387+(i*14),1367+(i*10),1349+(i*6),2020+(i%5));}
ps("\nAllium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alli_execution(t,c,1387+(i*15),1376+(i*12),1358+(i*8),1345+(i*5),2021+(i%4));}
ps("\nAllium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alli_evaluation(t,c,1379+(i*13),1368+(i*10),1352+(i*7),1341+(i*4),2022+(i%3));}
ps("\nAllium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alli_accessory(t,c,1371+(i*11),1362+(i*9),1348+(i*6),1338+(i*3),2023+(i%2));}
ps("\nAllium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alli_market(t,c,1365+(i*9),1356+(i*7),1343+(i*5),1335+(i*3),2024);}
ps("\n");alli_report();alli_state();ps("\n=== Demo Complete ===\n");return 0;}
