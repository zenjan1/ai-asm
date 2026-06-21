/* tanacetum_admin: Tanacetum management technology administration (v1.0)
 * Tanacetum planning, tanacetum execution, tanacetum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tnc_t;
typedef struct{int n_tcp,n_tce,n_tcv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} tnc_state_t;
static tnc_t tncps[N],tncss[N-2],tncvss[N-4],tncas[N-6],tncmks[N-6]; static tnc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tnc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tnc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TNC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tnc_init(void){if(init)return -1;st.n_tcp=0;st.n_tce=0;st.n_tcv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tncps[i].active=0;for(int i=0;i<N-2;i++)tncss[i].active=0;for(int i=0;i<N-4;i++)tncvss[i].active=0;for(int i=0;i<N-6;i++)tncas[i].active=0;for(int i=0;i<N-6;i++)tncmks[i].active=0;init=1;ps("[TNC] Tanacetum initialized\n");return 0;}
int tnc_planning(int t,int c,int a,int b,int d,int e,int y){return add(tncps,&st.n_tcp,&st.t_f1,N,t,c,a,b,d,e,y);}
int tnc_execution(int t,int c,int a,int b,int d,int e,int y){return add(tncss,&st.n_tce,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tnc_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(tncvss,&st.n_tcv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tnc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(tncas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tnc_market(int t,int c,int a,int b,int d,int e,int y){return add(tncmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tnc_report(void){ps("[TNC] Tcp: ");pi(st.n_tcp);ps(" PCS=");pi(st.t_f1);ps("\nTce: ");pi(st.n_tce);ps(" PCS=");pi(st.t_f2);ps("\nTcv: ");pi(st.n_tcv);ps(" PCS=");pi(st.t_f3);ps("\nTcc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void tnc_state(void){ps("[TNC] Tcp=");pi(st.n_tcp);ps(" Tce=");pi(st.n_tce);ps(" Tcv=");pi(st.n_tcv);ps(" Tcc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tanacetum Admin Demo ===\n\n");tnc_init();
ps("Tanacetum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tnc_planning(t,c,865+(i*17),854+(i*14),834+(i*10),816+(i*6),2020+(i%5));}
ps("\nTanacetum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tnc_execution(t,c,854+(i*15),843+(i*12),825+(i*8),812+(i*5),2021+(i%4));}
ps("\nTanacetum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tnc_evaluation(t,c,846+(i*13),835+(i*10),819+(i*7),808+(i*4),2022+(i%3));}
ps("\nTanacetum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tnc_accessory(t,c,838+(i*11),829+(i*9),815+(i*6),805+(i*3),2023+(i%2));}
ps("\nTanacetum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tnc_market(t,c,832+(i*9),823+(i*7),810+(i*5),802+(i*3),2024);}
ps("\n");tnc_report();tnc_state();ps("\n=== Demo Complete ===\n");return 0;}
