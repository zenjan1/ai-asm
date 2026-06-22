/* anaphalis_admin: Anaphalis management technology administration (v1.0)
 * Anaphalis planning, anaphalis execution, anaphalis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anap_t;
typedef struct{int n_anapp,n_anap,n_anap2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anap_state_t;
static anap_t anaps[N],anape[N-2],anap2[N-4],anapac[N-6],anapam[N-6]; static anap_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(anap_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anap_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANAP] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anap_init(void){if(init)return -1;st.n_anapp=0;st.n_anap=0;st.n_anap2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)anaps[i].active=0;for(int i=0;i<N-2;i++)anape[i].active=0;for(int i=0;i<N-4;i++)anap2[i].active=0;for(int i=0;i<N-6;i++)anapac[i].active=0;for(int i=0;i<N-6;i++)anapam[i].active=0;init=1;ps("[ANAP] Anaphalis initialized\n");return 0;}
int anap_planning(int t,int c,int a,int b,int d,int e,int y){return add(anaps,&st.n_anapp,&st.t_f1,N,t,c,a,b,d,e,y);}
int anap_execution(int t,int c,int a,int b,int d,int e,int y){return add(anape,&st.n_anap,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anap_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(anap2,&st.n_anap2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anap_accessory(int t,int c,int a,int b,int d,int e,int y){return add(anapac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anap_market(int t,int c,int a,int b,int d,int e,int y){return add(anapam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anap_report(void){ps("[ANAP] Napp: ");pi(st.n_anapp);ps(" PCS=");pi(st.t_f1);ps("\nNape: ");pi(st.n_anap);ps(" PCS=");pi(st.t_f2);ps("\nNap2: ");pi(st.n_anap2);ps(" PCS=");pi(st.t_f3);ps("\nNapac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anap_state(void){ps("[ANAP] Napp=");pi(st.n_anapp);ps(" Nape=");pi(st.n_anap);ps(" Nap2=");pi(st.n_anap2);ps(" Napac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anaphalis Admin Demo ===\n\n");anap_init();
ps("Anaphalis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anap_planning(t,c,1405+(i*17),1394+(i*14),1374+(i*10),1356+(i*6),2020+(i%5));}
ps("\nAnaphalis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anap_execution(t,c,1394+(i*15),1383+(i*12),1365+(i*8),1352+(i*5),2021+(i%4));}
ps("\nAnaphalis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anap_evaluation(t,c,1386+(i*13),1375+(i*10),1359+(i*7),1348+(i*4),2022+(i%3));}
ps("\nAnaphalis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anap_accessory(t,c,1378+(i*11),1369+(i*9),1355+(i*6),1345+(i*3),2023+(i%2));}
ps("\nAnaphalis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anap_market(t,c,1372+(i*9),1363+(i*7),1350+(i*5),1342+(i*3),2024);}
ps("\n");anap_report();anap_state();ps("\n=== Demo Complete ===\n");return 0;}
