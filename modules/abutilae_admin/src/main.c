/* abutilae_admin: Abutilae management technology administration (v1.0)
 * Abutilae planning, abutilae execution, abutilae evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} abut_t;
typedef struct{int n_abutp,n_abut,n_abut2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} abut_state_t;
static abut_t abuts[N],abute[N-2],abutv[N-4],abutac[N-6],abutm[N-6]; static abut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(abut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;abut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ABUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int abut_init(void){if(init)return -1;st.n_abutp=0;st.n_abut=0;st.n_abut2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)abuts[i].active=0;for(int i=0;i<N-2;i++)abute[i].active=0;for(int i=0;i<N-4;i++)abutv[i].active=0;for(int i=0;i<N-6;i++)abutac[i].active=0;for(int i=0;i<N-6;i++)abutm[i].active=0;init=1;ps("[ABUT] Abutilae initialized\n");return 0;}
int abut_planning(int t,int c,int a,int b,int d,int e,int y){return add(abuts,&st.n_abutp,&st.t_f1,N,t,c,a,b,d,e,y);}
int abut_execution(int t,int c,int a,int b,int d,int e,int y){return add(abute,&st.n_abut,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int abut_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(abutv,&st.n_abut2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int abut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(abutac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int abut_market(int t,int c,int a,int b,int d,int e,int y){return add(abutm,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void abut_report(void){ps("[ABUT] Butp: ");pi(st.n_abutp);ps(" PCS=");pi(st.t_f1);ps("\nBute: ");pi(st.n_abut);ps(" PCS=");pi(st.t_f2);ps("\nButv: ");pi(st.n_abut2);ps(" PCS=");pi(st.t_f3);ps("\nButac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void abut_state(void){ps("[ABUT] Butp=");pi(st.n_abutp);ps(" Bute=");pi(st.n_abut);ps(" Butv=");pi(st.n_abut2);ps(" Butac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Abutilae Admin Demo ===\n\n");abut_init();
ps("Abutilae planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;abut_planning(t,c,1385+(i*17),1374+(i*14),1354+(i*10),1336+(i*6),2020+(i%5));}
ps("\nAbutilae execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;abut_execution(t,c,1374+(i*15),1363+(i*12),1345+(i*8),1332+(i*5),2021+(i%4));}
ps("\nAbutilae evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;abut_evaluation(t,c,1366+(i*13),1355+(i*10),1339+(i*7),1328+(i*4),2022+(i%3));}
ps("\nAbutilae accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abut_accessory(t,c,1358+(i*11),1349+(i*9),1335+(i*6),1325+(i*3),2023+(i%2));}
ps("\nAbutilae marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abut_market(t,c,1352+(i*9),1343+(i*7),1330+(i*5),1322+(i*3),2024);}
ps("\n");abut_report();abut_state();ps("\n=== Demo Complete ===\n");return 0;}
