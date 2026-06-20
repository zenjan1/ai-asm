/* cyclamen_admin: Cyclamen management technology administration (v1.0)
 * Cyclamen planning, cyclamen execution, cyclamen evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cyc_t;
typedef struct{int n_cyp,n_cye,n_cyv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cyc_state_t;
static cyc_t cyps[N],cyes[N-2],cyvs[N-4],acs[N-6],mks[N-6]; static cyc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cyc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cyc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CYC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cyc_init(void){if(init)return -1;st.n_cyp=0;st.n_cye=0;st.n_cyv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cyps[i].active=0;for(int i=0;i<N-2;i++)cyes[i].active=0;for(int i=0;i<N-4;i++)cyvs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CYC] Cyclamen initialized\n");return 0;}
int cyc_planning(int t,int c,int a,int b,int d,int e,int y){return add(cyps,&st.n_cyp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cyc_execution(int t,int c,int a,int b,int d,int e,int y){return add(cyes,&st.n_cye,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cyc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cyvs,&st.n_cyv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cyc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cyc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cyc_report(void){ps("[CYC] Cyp: ");pi(st.n_cyp);ps(" PCS=");pi(st.t_f1);ps("\nCye: ");pi(st.n_cye);ps(" PCS=");pi(st.t_f2);ps("\nCyv: ");pi(st.n_cyv);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cyc_state(void){ps("[CYC] Cyp=");pi(st.n_cyp);ps(" Cye=");pi(st.n_cye);ps(" Cyv=");pi(st.n_cyv);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cyclamen Admin Demo ===\n\n");cyc_init();
ps("Cyclamen planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cyc_planning(t,c,588+(i*17),577+(i*14),557+(i*10),539+(i*6),2020+(i%5));}
ps("\nCyclamen execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cyc_execution(t,c,577+(i*15),566+(i*12),548+(i*8),535+(i*5),2021+(i%4));}
ps("\nCyclamen evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cyc_evaluation(t,c,569+(i*13),558+(i*10),542+(i*7),531+(i*4),2022+(i%3));}
ps("\nCyclamen accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyc_accessory(t,c,561+(i*11),552+(i*9),538+(i*6),528+(i*3),2023+(i%2));}
ps("\nCyclamen marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cyc_market(t,c,555+(i*9),546+(i*7),533+(i*5),525+(i*3),2024);}
ps("\n");cyc_report();cyc_state();ps("\n=== Demo Complete ===\n");return 0;}
