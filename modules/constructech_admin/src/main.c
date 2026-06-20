/* constructech_admin: Construction technology administration (v1.0)
 * Construction, building materials, architectural design, construction services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cst_t;
typedef struct{int n_cn,n_bm,n_ad,n_cs,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cst_state_t;
static cst_t cns[N],bms[N-2],ads[N-4],css[N-6],mks[N-6]; static cst_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cst_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cst_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CST] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cst_init(void){if(init)return -1;st.n_cn=0;st.n_bm=0;st.n_ad=0;st.n_cs=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cns[i].active=0;for(int i=0;i<N-2;i++)bms[i].active=0;for(int i=0;i<N-4;i++)ads[i].active=0;for(int i=0;i<N-6;i++)css[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CST] Constructech initialized\n");return 0;}
int cst_construct(int t,int c,int a,int b,int d,int e,int y){return add(cns,&st.n_cn,&st.t_f1,N,t,c,a,b,d,e,y);}
int cst_material(int t,int c,int a,int b,int d,int e,int y){return add(bms,&st.n_bm,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cst_design(int t,int c,int a,int b,int d,int e,int y){return add(ads,&st.n_ad,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cst_service(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cst_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cst_report(void){ps("[CST] Cn: ");pi(st.n_cn);ps(" PCS=");pi(st.t_f1);ps("\nBm: ");pi(st.n_bm);ps(" Ton=");pi(st.t_f2);ps("\nAd: ");pi(st.n_ad);ps(" PCS=");pi(st.t_f3);ps("\nCs: ");pi(st.n_cs);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cst_state(void){ps("[CST] Cn=");pi(st.n_cn);ps(" Bm=");pi(st.n_bm);ps(" Ad=");pi(st.n_ad);ps(" Cs=");pi(st.n_cs);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Construction Tech Admin Demo ===\n\n");cst_init();
ps("Construction...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cst_construct(t,c,243+(i*17),228+(i*14),208+(i*10),190+(i*6),2020+(i%5));}
ps("\nBuilding materials...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cst_material(t,c,232+(i*15),218+(i*12),200+(i*8),187+(i*5),2021+(i%4));}
ps("\nArchitectural design...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cst_design(t,c,224+(i*13),210+(i*10),194+(i*7),183+(i*4),2022+(i%3));}
ps("\nConstruction services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_service(t,c,216+(i*11),204+(i*9),190+(i*6),180+(i*3),2023+(i%2));}
ps("\nConstruction marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cst_market(t,c,210+(i*9),199+(i*7),186+(i*5),178+(i*3),2024);}
ps("\n");cst_report();cst_state();ps("\n=== Demo Complete ===\n");return 0;}
