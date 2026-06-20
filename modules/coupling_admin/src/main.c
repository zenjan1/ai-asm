/* coupling_admin: Coupling technology administration (v1.0)
 * Rigid coupling, flexible coupling, safety coupling, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cpl_t;
typedef struct{int n_rg,n_fl,n_sf,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cpl_state_t;
static cpl_t rgs[N],fls[N-2],sfs[N-4],acs[N-6],mks[N-6]; static cpl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cpl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cpl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CPL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cpl_init(void){if(init)return -1;st.n_rg=0;st.n_fl=0;st.n_sf=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)rgs[i].active=0;for(int i=0;i<N-2;i++)fls[i].active=0;for(int i=0;i<N-4;i++)sfs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CPL] Coupling initialized\n");return 0;}
int cpl_rigid(int t,int c,int a,int b,int d,int e,int y){return add(rgs,&st.n_rg,&st.t_f1,N,t,c,a,b,d,e,y);}
int cpl_flexible(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cpl_safety(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cpl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cpl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cpl_report(void){ps("[CPL] Rg: ");pi(st.n_rg);ps(" PCS=");pi(st.t_f1);ps("\nFl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f2);ps("\nSf: ");pi(st.n_sf);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cpl_state(void){ps("[CPL] Rg=");pi(st.n_rg);ps(" Fl=");pi(st.n_fl);ps(" Sf=");pi(st.n_sf);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coupling Admin Demo ===\n\n");cpl_init();
ps("Rigid couplings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cpl_rigid(t,c,379+(i*17),364+(i*14),344+(i*10),326+(i*6),2020+(i%5));}
ps("\nFlexible couplings...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cpl_flexible(t,c,368+(i*15),354+(i*12),336+(i*8),323+(i*5),2021+(i%4));}
ps("\nSafety couplings...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cpl_safety(t,c,360+(i*13),346+(i*10),330+(i*7),319+(i*4),2022+(i%3));}
ps("\nCoupling accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpl_accessory(t,c,352+(i*11),340+(i*9),326+(i*6),316+(i*3),2023+(i%2));}
ps("\nCoupling marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cpl_market(t,c,346+(i*9),335+(i*7),322+(i*5),314+(i*3),2024);}
ps("\n");cpl_report();cpl_state();ps("\n=== Demo Complete ===\n");return 0;}
