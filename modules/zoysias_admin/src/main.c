/* zoysias_admin: Zoysias management technology administration (v1.0)
 * Zoysias planning, zoysias execution, zoysias evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zoys_t;
typedef struct{int n_zoysp,n_zoys,n_zoys2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zoys_state_t;
static zoys_t zoys[N],zoye[N-2],zoysv[N-4],zoysac[N-6],zoysm[N-6]; static zoys_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(zoys_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zoys_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZOYS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zoys_init(void){if(init)return -1;st.n_zoysp=0;st.n_zoys=0;st.n_zoys2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zoys[i].active=0;for(int i=0;i<N-2;i++)zoye[i].active=0;for(int i=0;i<N-4;i++)zoysv[i].active=0;for(int i=0;i<N-6;i++)zoysac[i].active=0;for(int i=0;i<N-6;i++)zoysm[i].active=0;init=1;ps("[ZOYS] Zoysias initialized\n");return 0;}
int zoys_planning(int t,int c,int a,int b,int d,int e,int y){return add(zoys,&st.n_zoysp,&st.t_f1,N,t,c,a,b,d,e,y);}
int zoys_execution(int t,int c,int a,int b,int d,int e,int y){return add(zoye,&st.n_zoys,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zoys_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zoysv,&st.n_zoys2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zoys_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zoysac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zoys_market(int t,int c,int a,int b,int d,int e,int y){return add(zoysm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zoys_report(void){ps("[ZOYS] Zoysp: ");pi(st.n_zoysp);ps(" PCS=");pi(st.t_f1);ps("\nZoye: ");pi(st.n_zoys);ps(" PCS=");pi(st.t_f2);ps("\nZoysv: ");pi(st.n_zoys2);ps(" PCS=");pi(st.t_f3);ps("\nZoysac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zoys_state(void){ps("[ZOYS] Zoysp=");pi(st.n_zoysp);ps(" Zoye=");pi(st.n_zoys);ps(" Zoysv=");pi(st.n_zoys2);ps(" Zoysac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zoysias Admin Demo ===\n\n");zoys_init();
ps("Zoysias planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zoys_planning(t,c,1382+(i*17),1371+(i*14),1351+(i*10),1333+(i*6),2020+(i%5));}
ps("\nZoysias execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zoys_execution(t,c,1371+(i*15),1360+(i*12),1342+(i*8),1329+(i*5),2021+(i%4));}
ps("\nZoysias evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zoys_evaluation(t,c,1363+(i*13),1352+(i*10),1336+(i*7),1325+(i*4),2022+(i%3));}
ps("\nZoysias accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zoys_accessory(t,c,1355+(i*11),1346+(i*9),1332+(i*6),1322+(i*3),2023+(i%2));}
ps("\nZoysias marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zoys_market(t,c,1349+(i*9),1340+(i*7),1327+(i*5),1319+(i*3),2024);}
ps("\n");zoys_report();zoys_state();ps("\n=== Demo Complete ===\n");return 0;}
