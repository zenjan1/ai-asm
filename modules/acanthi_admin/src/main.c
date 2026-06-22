/* acanthi_admin: Acanthi management technology administration (v1.0)
 * Acanthi planning, acanthi execution, acanthi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} acan_t;
typedef struct{int n_acanp,n_acan,n_acan2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} acan_state_t;
static acan_t acans[N],acane[N-2],acanv[N-4],acanac[N-6],acanm[N-6]; static acan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(acan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;acan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ACAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int acan_init(void){if(init)return -1;st.n_acanp=0;st.n_acan=0;st.n_acan2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)acans[i].active=0;for(int i=0;i<N-2;i++)acane[i].active=0;for(int i=0;i<N-4;i++)acanv[i].active=0;for(int i=0;i<N-6;i++)acanac[i].active=0;for(int i=0;i<N-6;i++)acanm[i].active=0;init=1;ps("[ACAN] Acanthi initialized\n");return 0;}
int acan_planning(int t,int c,int a,int b,int d,int e,int y){return add(acans,&st.n_acanp,&st.t_f1,N,t,c,a,b,d,e,y);}
int acan_execution(int t,int c,int a,int b,int d,int e,int y){return add(acane,&st.n_acan,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int acan_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(acanv,&st.n_acan2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int acan_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acanac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int acan_market(int t,int c,int a,int b,int d,int e,int y){return add(acanm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void acan_report(void){ps("[ACAN] Canp: ");pi(st.n_acanp);ps(" PCS=");pi(st.t_f1);ps("\nCane: ");pi(st.n_acan);ps(" PCS=");pi(st.t_f2);ps("\nCanv: ");pi(st.n_acan2);ps(" PCS=");pi(st.t_f3);ps("\nCanac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void acan_state(void){ps("[ACAN] Canp=");pi(st.n_acanp);ps(" Cane=");pi(st.n_acan);ps(" Canv=");pi(st.n_acan2);ps(" Canac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Acanthi Admin Demo ===\n\n");acan_init();
ps("Acanthi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;acan_planning(t,c,1388+(i*17),1377+(i*14),1357+(i*10),1339+(i*6),2020+(i%5));}
ps("\nAcanthi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;acan_execution(t,c,1377+(i*15),1366+(i*12),1348+(i*8),1335+(i*5),2021+(i%4));}
ps("\nAcanthi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;acan_evaluation(t,c,1369+(i*13),1358+(i*10),1342+(i*7),1331+(i*4),2022+(i%3));}
ps("\nAcanthi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acan_accessory(t,c,1361+(i*11),1352+(i*9),1338+(i*6),1328+(i*3),2023+(i%2));}
ps("\nAcanthi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;acan_market(t,c,1355+(i*9),1346+(i*7),1333+(i*5),1325+(i*3),2024);}
ps("\n");acan_report();acan_state();ps("\n=== Demo Complete ===\n");return 0;}
