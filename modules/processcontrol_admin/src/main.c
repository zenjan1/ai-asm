/* processcontrol_admin: Process control administration (v1.0)
 * Temperature control, pressure control, flow control, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} psc_t;
typedef struct{int n_tm,n_pr,n_fl,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} psc_state_t;
static psc_t tms[N],prs[N-2],fls[N-4],acs[N-6],mks[N-6]; static psc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(psc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;psc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PSC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int psc_init(void){if(init)return -1;st.n_tm=0;st.n_pr=0;st.n_fl=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tms[i].active=0;for(int i=0;i<N-2;i++)prs[i].active=0;for(int i=0;i<N-4;i++)fls[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PSC] Processcontrol initialized\n");return 0;}
int psc_temperature(int t,int c,int a,int b,int d,int e,int y){return add(tms,&st.n_tm,&st.t_f1,N,t,c,a,b,d,e,y);}
int psc_pressure(int t,int c,int a,int b,int d,int e,int y){return add(prs,&st.n_pr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int psc_flow(int t,int c,int a,int b,int d,int e,int y){return add(fls,&st.n_fl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int psc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int psc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void psc_report(void){ps("[PSC] Tm: ");pi(st.n_tm);ps(" PCS=");pi(st.t_f1);ps("\nPr: ");pi(st.n_pr);ps(" PCS=");pi(st.t_f2);ps("\nFl: ");pi(st.n_fl);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void psc_state(void){ps("[PSC] Tm=");pi(st.n_tm);ps(" Pr=");pi(st.n_pr);ps(" Fl=");pi(st.n_fl);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Process Control Admin Demo ===\n\n");psc_init();
ps("Temperature control...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;psc_temperature(t,c,323+(i*17),308+(i*14),288+(i*10),270+(i*6),2020+(i%5));}
ps("\nPressure control...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;psc_pressure(t,c,312+(i*15),298+(i*12),280+(i*8),267+(i*5),2021+(i%4));}
ps("\nFlow control...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;psc_flow(t,c,304+(i*13),290+(i*10),274+(i*7),263+(i*4),2022+(i%3));}
ps("\nControl accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psc_accessory(t,c,296+(i*11),284+(i*9),270+(i*6),260+(i*3),2023+(i%2));}
ps("\nControl marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;psc_market(t,c,290+(i*9),279+(i*7),266+(i*5),258+(i*3),2024);}
ps("\n");psc_report();psc_state();ps("\n=== Demo Complete ===\n");return 0;}
