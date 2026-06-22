/* aegopodi_admin: Aegopodi management technology administration (v1.0)
 * Aegopodi planning, aegopodi execution, aegopodi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aego_t;
typedef struct{int n_aegop,n_aego,n_aego2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aego_state_t;
static aego_t aegos[N],aegoe[N-2],aegov[N-4],aegoac[N-6],aegom[N-6]; static aego_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(aego_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aego_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AEGO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aego_init(void){if(init)return -1;st.n_aegop=0;st.n_aego=0;st.n_aego2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aegos[i].active=0;for(int i=0;i<N-2;i++)aegoe[i].active=0;for(int i=0;i<N-4;i++)aegov[i].active=0;for(int i=0;i<N-6;i++)aegoac[i].active=0;for(int i=0;i<N-6;i++)aegom[i].active=0;init=1;ps("[AEGO] Aegopodi initialized\n");return 0;}
int aego_planning(int t,int c,int a,int b,int d,int e,int y){return add(aegos,&st.n_aegop,&st.t_f1,N,t,c,a,b,d,e,y);}
int aego_execution(int t,int c,int a,int b,int d,int e,int y){return add(aegoe,&st.n_aego,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aego_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aegov,&st.n_aego2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aego_accessory(int t,int c,int a,int b,int d,int e,int y){return add(aegoac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aego_market(int t,int c,int a,int b,int d,int e,int y){return add(aegom,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aego_report(void){ps("[AEGO] Egop: ");pi(st.n_aegop);ps(" PCS=");pi(st.t_f1);ps("\nEgoe: ");pi(st.n_aego);ps(" PCS=");pi(st.t_f2);ps("\nEgov: ");pi(st.n_aego2);ps(" PCS=");pi(st.t_f3);ps("\nEgoac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aego_state(void){ps("[AEGO] Egop=");pi(st.n_aegop);ps(" Egoe=");pi(st.n_aego);ps(" Egov=");pi(st.n_aego2);ps(" Egoac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aegopodi Admin Demo ===\n\n");aego_init();
ps("Aegopodi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aego_planning(t,c,1394+(i*17),1383+(i*14),1363+(i*10),1345+(i*6),2020+(i%5));}
ps("\nAegopodi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aego_execution(t,c,1383+(i*15),1372+(i*12),1354+(i*8),1341+(i*5),2021+(i%4));}
ps("\nAegopodi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aego_evaluation(t,c,1375+(i*13),1364+(i*10),1348+(i*7),1337+(i*4),2022+(i%3));}
ps("\nAegopodi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aego_accessory(t,c,1367+(i*11),1358+(i*9),1344+(i*6),1334+(i*3),2023+(i%2));}
ps("\nAegopodi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aego_market(t,c,1361+(i*9),1352+(i*7),1339+(i*5),1331+(i*3),2024);}
ps("\n");aego_report();aego_state();ps("\n=== Demo Complete ===\n");return 0;}
