/* formingtech_admin: Forming technology administration (v1.0)
 * Plastic forming, precision forming, incremental forming, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} frm_t;
typedef struct{int n_ps,n_pr,n_ic,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} frm_state_t;
static frm_t pss[N],prs[N-2],ics[N-4],acs[N-6],mks[N-6]; static frm_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(frm_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;frm_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int frm_init(void){if(init)return -1;st.n_ps=0;st.n_pr=0;st.n_ic=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pss[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)ics[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[FRM] Formingtech initialized\n");return 0;}
int frm_plastic(int t,int c,int a,int b,int d,int e,int y){return add(pss,&st.n_ps,&st.t_f1,N,t,c,a,b,d,e,y);}
int frm_precision(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int frm_incremental(int t,int c,int a,int b,int d,int e,int y){return add(ics,&st.n_ic,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int frm_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int frm_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void frm_report(void){ps("[FRM] Ps: ");pi(st.n_ps);ps(" PCS=");pi(st.t_f1);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f2);ps("\nIc: ");pi(st.n_ic);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void frm_state(void){ps("[FRM] Ps=");pi(st.n_ps);ps(" Pr=");pi(st.n_pr);ps(" Ic=");pi(st.n_ic);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Forming Tech Admin Demo ===\n\n");frm_init();
ps("Plastic forming...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;frm_plastic(t,c,284+(i*17),269+(i*14),249+(i*10),231+(i*6),2020+(i%5));}
ps("\nPrecision forming...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;frm_precision(t,c,273+(i*15),259+(i*12),241+(i*8),228+(i*5),2021+(i%4));}
ps("\nIncremental forming...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;frm_incremental(t,c,265+(i*13),251+(i*10),235+(i*7),224+(i*4),2022+(i%3));}
ps("\nForming accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frm_accessory(t,c,257+(i*11),245+(i*9),231+(i*6),221+(i*3),2023+(i%2));}
ps("\nForming marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;frm_market(t,c,251+(i*9),240+(i*7),227+(i*5),219+(i*3),2024);}
ps("\n");frm_report();frm_state();ps("\n=== Demo Complete ===\n");return 0;}
