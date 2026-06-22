/* adonidi_admin: Adonidi management technology administration (v1.0)
 * Adonidi planning, adonidi execution, adonidi evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} adon_t;
typedef struct{int n_adonp,n_adon,n_adon2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} adon_state_t;
static adon_t adons[N],adone[N-2],adonv[N-4],adonac[N-6],adonm[N-6]; static adon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(adon_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;adon_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ADON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int adon_init(void){if(init)return -1;st.n_adonp=0;st.n_adon=0;st.n_adon2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)adons[i].active=0;for(int i=0;i<N-2;i++)adone[i].active=0;for(int i=0;i<N-4;i++)adonv[i].active=0;for(int i=0;i<N-6;i++)adonac[i].active=0;for(int i=0;i<N-6;i++)adonm[i].active=0;init=1;ps("[ADON] Adonidi initialized\n");return 0;}
int adon_planning(int t,int c,int a,int b,int d,int e,int y){return add(adons,&st.n_adonp,&st.t_f1,N,t,c,a,b,d,e,y);}
int adon_execution(int t,int c,int a,int b,int d,int e,int y){return add(adone,&st.n_adon,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int adon_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(adonv,&st.n_adon2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int adon_accessory(int t,int c,int a,int b,int d,int e,int y){return add(adonac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int adon_market(int t,int c,int a,int b,int d,int e,int y){return add(adonm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void adon_report(void){ps("[ADON] Donp: ");pi(st.n_adonp);ps(" PCS=");pi(st.t_f1);ps("\nDone: ");pi(st.n_adon);ps(" PCS=");pi(st.t_f2);ps("\nDonv: ");pi(st.n_adon2);ps(" PCS=");pi(st.t_f3);ps("\nDonac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void adon_state(void){ps("[ADON] Donp=");pi(st.n_adonp);ps(" Done=");pi(st.n_adon);ps(" Donv=");pi(st.n_adon2);ps(" Donac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Adonidi Admin Demo ===\n\n");adon_init();
ps("Adonidi planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;adon_planning(t,c,1393+(i*17),1382+(i*14),1362+(i*10),1344+(i*6),2020+(i%5));}
ps("\nAdonidi execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;adon_execution(t,c,1382+(i*15),1371+(i*12),1353+(i*8),1340+(i*5),2021+(i%4));}
ps("\nAdonidi evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;adon_evaluation(t,c,1374+(i*13),1363+(i*10),1347+(i*7),1336+(i*4),2022+(i%3));}
ps("\nAdonidi accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adon_accessory(t,c,1366+(i*11),1357+(i*9),1343+(i*6),1333+(i*3),2023+(i%2));}
ps("\nAdonidi marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;adon_market(t,c,1360+(i*9),1351+(i*7),1338+(i*5),1330+(i*3),2024);}
ps("\n");adon_report();adon_state();ps("\n=== Demo Complete ===\n");return 0;}
