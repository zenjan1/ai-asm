/* campanula_admin: Campanula management technology administration (v1.0)
 * Campanula planning, campanula execution, campanula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} camp_t;
typedef struct{int n_campp,n_camp,n_camp2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} camp_state_t;
static camp_t camps[N],campe[N-2],camp2[N-4],campac[N-6],campam[N-6]; static camp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(camp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;camp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CAMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int camp_init(void){if(init)return -1;st.n_campp=0;st.n_camp=0;st.n_camp2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)camps[i].active=0;for(int i=0;i<N-2;i++)campe[i].active=0;for(int i=0;i<N-4;i++)camp2[i].active=0;for(int i=0;i<N-6;i++)campac[i].active=0;for(int i=0;i<N-6;i++)campam[i].active=0;init=1;ps("[CAMP] Campanula initialized\n");return 0;}
int camp_planning(int t,int c,int a,int b,int d,int e,int y){return add(camps,&st.n_campp,&st.t_f1,N,t,c,a,b,d,e,y);}
int camp_execution(int t,int c,int a,int b,int d,int e,int y){return add(campe,&st.n_camp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int camp_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(camp2,&st.n_camp2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int camp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(campac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int camp_market(int t,int c,int a,int b,int d,int e,int y){return add(campam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void camp_report(void){ps("[CAMP] Mpp: ");pi(st.n_campp);ps(" PCS=");pi(st.t_f1);ps("\nMpe: ");pi(st.n_camp);ps(" PCS=");pi(st.t_f2);ps("\nMp2: ");pi(st.n_camp2);ps(" PCS=");pi(st.t_f3);ps("\nMac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void camp_state(void){ps("[CAMP] Mpp=");pi(st.n_campp);ps(" Mp=");pi(st.n_camp);ps(" Mp2=");pi(st.n_camp2);ps(" Mac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Campanula Admin Demo ===\n\n");camp_init();
ps("Campanula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;camp_planning(t,c,1456+(i*17),1445+(i*14),1425+(i*10),1407+(i*6),2020+(i%5));}
ps("\nCampanula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;camp_execution(t,c,1445+(i*15),1434+(i*12),1416+(i*8),1403+(i*5),2021+(i%4));}
ps("\nCampanula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;camp_evaluation(t,c,1437+(i*13),1426+(i*10),1410+(i*7),1399+(i*4),2022+(i%3));}
ps("\nCampanula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;camp_accessory(t,c,1429+(i*11),1420+(i*9),1406+(i*6),1396+(i*3),2023+(i%2));}
ps("\nCampanula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;camp_market(t,c,1423+(i*9),1414+(i*7),1401+(i*5),1393+(i*3),2024);}
ps("\n");camp_report();camp_state();ps("\n=== Demo Complete ===\n");return 0;}
