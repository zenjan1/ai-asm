/* compressor_admin: Compressor technology administration (v1.0)
 * Piston compressor, screw compressor, centrifugal compressor, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cmp_t;
typedef struct{int n_ps,n_sc,n_cf,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cmp_state_t;
static cmp_t pss[N],scs[N-2],cfs[N-4],acs[N-6],mks[N-6]; static cmp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmp_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cmp_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CMP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cmp_init(void){if(init)return -1;st.n_ps=0;st.n_sc=0;st.n_cf=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pss[i].active=0;for(int i=0;i<N-2;i++)scs[i].active=0;for(int i=0;i<N-4;i++)cfs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CMP] Compressor initialized\n");return 0;}
int cmp_piston(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f1,N,t,c,a,b,d,e,y);}
int cmp_screw(int t,int c,int a,int b,int d,int e,int y){return add(scs,&st.n_sc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cmp_centrifugal(int t,int c,int a,int b,int d,int e,int y){return add(cfs,&st.n_cf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cmp_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cmp_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cmp_report(void){ps("[CMP] Ps: ");pi(st.n_ps);ps(" PCS=");pi(st.t_f1);ps("\nSc: ");pi(st.n_sc);ps(" PCS=");pi(st.t_f2);ps("\nCf: ");pi(st.n_cf);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cmp_state(void){ps("[CMP] Ps=");pi(st.n_ps);ps(" Sc=");pi(st.n_sc);ps(" Cf=");pi(st.n_cf);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Compressor Admin Demo ===\n\n");cmp_init();
ps("Piston compressors...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cmp_piston(t,c,369+(i*17),354+(i*14),334+(i*10),316+(i*6),2020+(i%5));}
ps("\nScrew compressors...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cmp_screw(t,c,358+(i*15),344+(i*12),326+(i*8),313+(i*5),2021+(i%4));}
ps("\nCentrifugal compressors...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cmp_centrifugal(t,c,350+(i*13),336+(i*10),320+(i*7),309+(i*4),2022+(i%3));}
ps("\nCompressor accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_accessory(t,c,342+(i*11),330+(i*9),316+(i*6),306+(i*3),2023+(i%2));}
ps("\nCompressor marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cmp_market(t,c,336+(i*9),325+(i*7),312+(i*5),304+(i*3),2024);}
ps("\n");cmp_report();cmp_state();ps("\n=== Demo Complete ===\n");return 0;}
