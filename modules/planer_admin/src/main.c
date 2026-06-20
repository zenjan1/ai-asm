/* planer_admin: Planer machine technology administration (v1.0)
 * Shaper, planer, slotter, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} pln_t;
typedef struct{int n_sh,n_pl,n_sl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} pln_state_t;
static pln_t shs[N],pls[N-2],sls[N-4],acs[N-6],mks[N-6]; static pln_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pln_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;pln_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PLN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int pln_init(void){if(init)return -1;st.n_sh=0;st.n_pl=0;st.n_sl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)shs[i].active=0;for(int i=0;i<N-2;i++)pls[i].active=0;for(int i=0;i<N-4;i++)sls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PLN] Planer initialized\n");return 0;}
int pln_shaper(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f1,N,t,c,a,b,d,e,y);}
int pln_planer(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int pln_slotter(int t,int c,int a,int b,int d,int e,int y){return add(sls,&st.n_sl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int pln_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int pln_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void pln_report(void){ps("[PLN] Sh: ");pi(st.n_sh);ps(" PCS=");pi(st.t_f1);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f2);ps("\nSl: ");pi(st.n_sl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void pln_state(void){ps("[PLN] Sh=");pi(st.n_sh);ps(" Pl=");pi(st.n_pl);ps(" Sl=");pi(st.n_sl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Planer Admin Demo ===\n\n");pln_init();
ps("Shapers...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;pln_shaper(t,c,401+(i*17),386+(i*14),366+(i*10),348+(i*6),2020+(i%5));}
ps("\nPlaners...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;pln_planer(t,c,390+(i*15),376+(i*12),358+(i*8),345+(i*5),2021+(i%4));}
ps("\nSlotter...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;pln_slotter(t,c,382+(i*13),368+(i*10),352+(i*7),341+(i*4),2022+(i%3));}
ps("\nPlaner accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pln_accessory(t,c,374+(i*11),362+(i*9),348+(i*6),338+(i*3),2023+(i%2));}
ps("\nPlaner marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;pln_market(t,c,368+(i*9),357+(i*7),344+(i*5),336+(i*3),2024);}
ps("\n");pln_report();pln_state();ps("\n=== Demo Complete ===\n");return 0;}
