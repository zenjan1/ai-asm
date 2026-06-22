/* blandfordia_admin: Blandfordia management technology administration (v1.0)
 * Blandfordia planning, blandfordia execution, blandfordia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} blan_t;
typedef struct{int n_blanp,n_blan,n_blan2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} blan_state_t;
static blan_t blans[N],blane[N-2],blan2[N-4],blanac[N-6],blanam[N-6]; static blan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(blan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;blan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BLAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int blan_init(void){if(init)return -1;st.n_blanp=0;st.n_blan=0;st.n_blan2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)blans[i].active=0;for(int i=0;i<N-2;i++)blane[i].active=0;for(int i=0;i<N-4;i++)blan2[i].active=0;for(int i=0;i<N-6;i++)blanac[i].active=0;for(int i=0;i<N-6;i++)blanam[i].active=0;init=1;ps("[BLAN] Blandfordia initialized\n");return 0;}
int blan_planning(int t,int c,int a,int b,int d,int e,int y){return add(blans,&st.n_blanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int blan_execution(int t,int c,int a,int b,int d,int e,int y){return add(blane,&st.n_blan,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int blan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(blan2,&st.n_blan2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int blan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(blanac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int blan_market(int t,int c,int a,int b,int d,int e,int y){return add(blanam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void blan_report(void){ps("[BLAN] Blap: ");pi(st.n_blanp);ps(" PCS=");pi(st.t_f1);ps("\nBlane: ");pi(st.n_blan);ps(" PCS=");pi(st.t_f2);ps("\nBlan2: ");pi(st.n_blan2);ps(" PCS=");pi(st.t_f3);ps("\nBlac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void blan_state(void){ps("[BLAN] Blap=");pi(st.n_blanp);ps(" Blan=");pi(st.n_blan);ps(" Blan2=");pi(st.n_blan2);ps(" Blac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Blandfordia Admin Demo ===\n\n");blan_init();
ps("Blandfordia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;blan_planning(t,c,1438+(i*17),1427+(i*14),1407+(i*10),1389+(i*6),2020+(i%5));}
ps("\nBlandfordia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;blan_execution(t,c,1427+(i*15),1416+(i*12),1398+(i*8),1385+(i*5),2021+(i%4));}
ps("\nBlandfordia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;blan_evaluation(t,c,1419+(i*13),1408+(i*10),1392+(i*7),1381+(i*4),2022+(i%3));}
ps("\nBlandfordia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blan_accessory(t,c,1411+(i*11),1402+(i*9),1388+(i*6),1378+(i*3),2023+(i%2));}
ps("\nBlandfordia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;blan_market(t,c,1405+(i*9),1396+(i*7),1383+(i*5),1375+(i*3),2024);}
ps("\n");blan_report();blan_state();ps("\n=== Demo Complete ===\n");return 0;}
