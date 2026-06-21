/* diascia_admin: Diascia management technology administration (v1.0)
 * Diascia planning, diascia execution, diascia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ds_t;
typedef struct{int n_dsp,n_dse,n_dsv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ds_state_t;
static ds_t dsps[N],dsss[N-2],dsvss[N-4],dsas[N-6],dsmks[N-6]; static ds_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ds_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ds_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[DSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int dsc_init(void){if(init)return -1;st.n_dsp=0;st.n_dse=0;st.n_dsv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dsps[i].active=0;for(int i=0;i<N-2;i++)dsss[i].active=0;for(int i=0;i<N-4;i++)dsvss[i].active=0;for(int i=0;i<N-6;i++)dsas[i].active=0;for(int i=0;i<N-6;i++)dsmks[i].active=0;init=1;ps("[DSC] Diascia initialized\n");return 0;}
int dsc_planning(int t,int c,int a,int b,int d,int e,int y){return add(dsps,&st.n_dsp,&st.t_f1,N,t,c,a,b,d,e,y);}
int dsc_execution(int t,int c,int a,int b,int d,int e,int y){return add(dsss,&st.n_dse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int dsc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(dsvss,&st.n_dsv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int dsc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(dsas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int dsc_market(int t,int c,int a,int b,int d,int e,int y){return add(dsmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void dsc_report(void){ps("[DSC] Dsp: ");pi(st.n_dsp);ps(" PCS=");pi(st.t_f1);ps("\nDse: ");pi(st.n_dse);ps(" PCS=");pi(st.t_f2);ps("\nDsv: ");pi(st.n_dsv);ps(" PCS=");pi(st.t_f3);ps("\nDsc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void dsc_state(void){ps("[DSC] Dsp=");pi(st.n_dsp);ps(" Dse=");pi(st.n_dse);ps(" Dsv=");pi(st.n_dsv);ps(" Dsc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Diascia Admin Demo ===\n\n");dsc_init();
ps("Diascia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;dsc_planning(t,c,766+(i*17),755+(i*14),735+(i*10),717+(i*6),2020+(i%5));}
ps("\nDiascia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;dsc_execution(t,c,755+(i*15),744+(i*12),726+(i*8),713+(i*5),2021+(i%4));}
ps("\nDiascia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;dsc_evaluation(t,c,747+(i*13),736+(i*10),722+(i*7),711+(i*4),2022+(i%3));}
ps("\nDiascia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsc_accessory(t,c,739+(i*11),730+(i*9),716+(i*6),706+(i*3),2023+(i%2));}
ps("\nDiascia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;dsc_market(t,c,733+(i*9),724+(i*7),711+(i*5),703+(i*3),2024);}
ps("\n");dsc_report();dsc_state();ps("\n=== Demo Complete ===\n");return 0;}
