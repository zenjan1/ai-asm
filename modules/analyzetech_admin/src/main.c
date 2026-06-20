/* analyzetech_admin: Analysis technology administration (v1.0)
 * Chemical analysis, physical analysis, material analysis, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anl_t;
typedef struct{int n_ch,n_ph,n_mt,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anl_state_t;
static anl_t chs[N],phs[N-2],mts[N-4],acs[N-6],mks[N-6]; static anl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(anl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anl_init(void){if(init)return -1;st.n_ch=0;st.n_ph=0;st.n_mt=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chs[i].active=0;for(int i=0;i<N-2;i++)phs[i].active=0;for(int i=0;i<N-4;i++)mts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ANL] Analyzetech initialized\n");return 0;}
int anl_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f1,N,t,c,a,b,d,e,y);}
int anl_physical(int t,int c,int a,int b,int d,int e,int y){return add(phs,&st.n_ph,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anl_material(int t,int c,int a,int b,int d,int e,int y){return add(mts,&st.n_mt,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anl_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anl_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anl_report(void){ps("[ANL] Ch: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f1);ps("\nPh: ");pi(st.n_ph);ps(" PCS=");pi(st.t_f2);ps("\nMt: ");pi(st.n_mt);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anl_state(void){ps("[ANL] Ch=");pi(st.n_ch);ps(" Ph=");pi(st.n_ph);ps(" Mt=");pi(st.n_mt);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Analysis Tech Admin Demo ===\n\n");anl_init();
ps("Chemical analysis...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anl_chemical(t,c,305+(i*17),290+(i*14),270+(i*10),252+(i*6),2020+(i%5));}
ps("\nPhysical analysis...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anl_physical(t,c,294+(i*15),280+(i*12),262+(i*8),249+(i*5),2021+(i%4));}
ps("\nMaterial analysis...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anl_material(t,c,286+(i*13),272+(i*10),256+(i*7),245+(i*4),2022+(i%3));}
ps("\nAnalysis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anl_accessory(t,c,278+(i*11),266+(i*9),252+(i*6),242+(i*3),2023+(i%2));}
ps("\nAnalysis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anl_market(t,c,272+(i*9),261+(i*7),248+(i*5),240+(i*3),2024);}
ps("\n");anl_report();anl_state();ps("\n=== Demo Complete ===\n");return 0;}
