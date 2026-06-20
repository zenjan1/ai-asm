/* assemble_admin: Assembly technology administration (v1.0)
 * Mechanical assembly, automated assembly, precision assembly, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} asb_t;
typedef struct{int n_mc,n_at,n_pr,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} asb_state_t;
static asb_t mcs[N],ats[N-2],prs[N-4],acs[N-6],mks[N-6]; static asb_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asb_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;asb_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ASB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int asb_init(void){if(init)return -1;st.n_mc=0;st.n_at=0;st.n_pr=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)ats[i].active=0;for(int i=0;i<N-4;i++)prs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ASB] Assemble initialized\n");return 0;}
int asb_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int asb_auto(int t,int c,int a,int b,int d,int e,int y){return add(ats,&st.n_at,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int asb_precision(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int asb_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int asb_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void asb_report(void){ps("[ASB] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nAt: ");pi(st.n_at);ps(" PCS=");pi(st.t_f2);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void asb_state(void){ps("[ASB] Mc=");pi(st.n_mc);ps(" At=");pi(st.n_at);ps(" Pr=");pi(st.n_pr);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Assemble Admin Demo ===\n\n");asb_init();
ps("Mechanical assembly...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;asb_mechanical(t,c,415+(i*17),404+(i*14),384+(i*10),366+(i*6),2020+(i%5));}
ps("\nAutomated assembly...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;asb_auto(t,c,404+(i*15),393+(i*12),375+(i*8),362+(i*5),2021+(i%4));}
ps("\nPrecision assembly...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;asb_precision(t,c,396+(i*13),385+(i*10),369+(i*7),358+(i*4),2022+(i%3));}
ps("\nAssembly accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asb_accessory(t,c,388+(i*11),379+(i*9),365+(i*6),355+(i*3),2023+(i%2));}
ps("\nAssembly marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;asb_market(t,c,382+(i*9),373+(i*7),360+(i*5),352+(i*3),2024);}
ps("\n");asb_report();asb_state();ps("\n=== Demo Complete ===\n");return 0;}
