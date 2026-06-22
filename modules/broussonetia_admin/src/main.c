/* broussonetia_admin: Broussonetia management technology administration (v1.0)
 * Broussonetia planning, broussonetia execution, broussonetia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} brou_t;
typedef struct{int n_broup,n_brou,n_brou2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} brou_state_t;
static brou_t brous[N],broue[N-2],brou2[N-4],brouac[N-6],brouam[N-6]; static brou_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(brou_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;brou_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[BROU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int brou_init(void){if(init)return -1;st.n_broup=0;st.n_brou=0;st.n_brou2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brous[i].active=0;for(int i=0;i<N-2;i++)broue[i].active=0;for(int i=0;i<N-4;i++)brou2[i].active=0;for(int i=0;i<N-6;i++)brouac[i].active=0;for(int i=0;i<N-6;i++)brouam[i].active=0;init=1;ps("[BROU] Broussonetia initialized\n");return 0;}
int brou_planning(int t,int c,int a,int b,int d,int e,int y){return add(brous,&st.n_broup,&st.t_f1,N,t,c,a,b,d,e,y);}
int brou_execution(int t,int c,int a,int b,int d,int e,int y){return add(broue,&st.n_brou,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int brou_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(brou2,&st.n_brou2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int brou_accessory(int t,int c,int a,int b,int d,int e,int y){return add(brouac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int brou_market(int t,int c,int a,int b,int d,int e,int y){return add(brouam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void brou_report(void){ps("[BROU] Oup: ");pi(st.n_broup);ps(" PCS=");pi(st.t_f1);ps("\nOue: ");pi(st.n_brou);ps(" PCS=");pi(st.t_f2);ps("\nOu2: ");pi(st.n_brou2);ps(" PCS=");pi(st.t_f3);ps("\nOuac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void brou_state(void){ps("[BROU] Oup=");pi(st.n_broup);ps(" Ou=");pi(st.n_brou);ps(" Ou2=");pi(st.n_brou2);ps(" Ouac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Broussonetia Admin Demo ===\n\n");brou_init();
ps("Broussonetia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;brou_planning(t,c,1445+(i*17),1434+(i*14),1414+(i*10),1396+(i*6),2020+(i%5));}
ps("\nBroussonetia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;brou_execution(t,c,1434+(i*15),1423+(i*12),1405+(i*8),1392+(i*5),2021+(i%4));}
ps("\nBroussonetia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;brou_evaluation(t,c,1426+(i*13),1415+(i*10),1399+(i*7),1388+(i*4),2022+(i%3));}
ps("\nBroussonetia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brou_accessory(t,c,1418+(i*11),1409+(i*9),1395+(i*6),1385+(i*3),2023+(i%2));}
ps("\nBroussonetia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;brou_market(t,c,1412+(i*9),1403+(i*7),1390+(i*5),1382+(i*3),2024);}
ps("\n");brou_report();brou_state();ps("\n=== Demo Complete ===\n");return 0;}
