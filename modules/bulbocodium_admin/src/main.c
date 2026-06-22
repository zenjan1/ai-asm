/* bulbocodium_admin: Bulbocodium management technology administration (v1.0)
 * Bulbocodium planning, bulbocodium execution, bulbocodium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} bulb_t;
typedef struct{int n_bulbp,n_bulb,n_bulb2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} bulb_state_t;
static bulb_t bulbs[N],bulbe[N-2],bulb2[N-4],bulbac[N-6],bulbam[N-6]; static bulb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(bulb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;bulb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BULB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int bulb_init(void){if(init)return -1;st.n_bulbp=0;st.n_bulb=0;st.n_bulb2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bulbs[i].active=0;for(int i=0;i<N-2;i++)bulbe[i].active=0;for(int i=0;i<N-4;i++)bulb2[i].active=0;for(int i=0;i<N-6;i++)bulbac[i].active=0;for(int i=0;i<N-6;i++)bulbam[i].active=0;init=1;ps("[BULB] Bulbocodium initialized\n");return 0;}
int bulb_planning(int t,int c,int a,int b,int d,int e,int y){return add(bulbs,&st.n_bulbp,&st.t_f1,N,t,c,a,b,d,e,y);}
int bulb_execution(int t,int c,int a,int b,int d,int e,int y){return add(bulbe,&st.n_bulb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int bulb_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(bulb2,&st.n_bulb2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int bulb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(bulbac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int bulb_market(int t,int c,int a,int b,int d,int e,int y){return add(bulbam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void bulb_report(void){ps("[BULB] Lp: ");pi(st.n_bulbp);ps(" PCS=");pi(st.t_f1);ps("\nLe: ");pi(st.n_bulb);ps(" PCS=");pi(st.t_f2);ps("\nLb2: ");pi(st.n_bulb2);ps(" PCS=");pi(st.t_f3);ps("\nLac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void bulb_state(void){ps("[BULB] Lp=");pi(st.n_bulbp);ps(" Lb=");pi(st.n_bulb);ps(" Lb2=");pi(st.n_bulb2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Bulbocodium Admin Demo ===\n\n");bulb_init();
ps("Bulbocodium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;bulb_planning(t,c,1448+(i*17),1437+(i*14),1417+(i*10),1399+(i*6),2020+(i%5));}
ps("\nBulbocodium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;bulb_execution(t,c,1437+(i*15),1426+(i*12),1408+(i*8),1395+(i*5),2021+(i%4));}
ps("\nBulbocodium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;bulb_evaluation(t,c,1429+(i*13),1418+(i*10),1402+(i*7),1391+(i*4),2022+(i%3));}
ps("\nBulbocodium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bulb_accessory(t,c,1421+(i*11),1412+(i*9),1398+(i*6),1388+(i*3),2023+(i%2));}
ps("\nBulbocodium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;bulb_market(t,c,1415+(i*9),1406+(i*7),1393+(i*5),1385+(i*3),2024);}
ps("\n");bulb_report();bulb_state();ps("\n=== Demo Complete ===\n");return 0;}
