/* robottech_admin: Robot technology administration (v1.0)
 * Industrial robots, service robots, core components, system integration, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rbt_t;
typedef struct{int n_ir,n_sr,n_cp,n_si,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rbt_state_t;
static rbt_t irs[N],srs[N-2],cps[N-4],sis[N-6],mks[N-6]; static rbt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rbt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rbt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[RBT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rbt_init(void){if(init)return -1;st.n_ir=0;st.n_sr=0;st.n_cp=0;st.n_si=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)irs[i].active=0;for(int i=0;i<N-2;i++)srs[i].active=0;for(int i=0;i<N-4;i++)cps[i].active=0;for(int i=0;i<N-6;i++)sis[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[RBT] Robottech initialized\n");return 0;}
int rbt_industrial(int t,int c,int a,int b,int d,int e,int y){return add(irs,&st.n_ir,&st.t_f1,N,t,c,a,b,d,e,y);}
int rbt_service(int t,int c,int a,int b,int d,int e,int y){return add(srs,&st.n_sr,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rbt_component(int t,int c,int a,int b,int d,int e,int y){return add(cps,&st.n_cp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rbt_integrate(int t,int c,int a,int b,int d,int e,int y){return add(sis,&st.n_si,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rbt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rbt_report(void){ps("[RBT] Ir: ");pi(st.n_ir);ps(" PCS=");pi(st.t_f1);ps("\nSr: ");pi(st.n_sr);ps(" PCS=");pi(st.t_f2);ps("\nCp: ");pi(st.n_cp);ps(" PCS=");pi(st.t_f3);ps("\nSi: ");pi(st.n_si);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rbt_state(void){ps("[RBT] Ir=");pi(st.n_ir);ps(" Sr=");pi(st.n_sr);ps(" Cp=");pi(st.n_cp);ps(" Si=");pi(st.n_si);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Robot Tech Admin Demo ===\n\n");rbt_init();
ps("Industrial robots...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rbt_industrial(t,c,224+(i*17),209+(i*14),189+(i*10),171+(i*6),2020+(i%5));}
ps("\nService robots...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rbt_service(t,c,213+(i*15),199+(i*12),181+(i*8),168+(i*5),2021+(i%4));}
ps("\nCore components...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rbt_component(t,c,205+(i*13),191+(i*10),175+(i*7),164+(i*4),2022+(i%3));}
ps("\nSystem integration...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbt_integrate(t,c,197+(i*11),185+(i*9),171+(i*6),161+(i*3),2023+(i%2));}
ps("\nRobot marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rbt_market(t,c,191+(i*9),180+(i*7),167+(i*5),159+(i*3),2024);}
ps("\n");rbt_report();rbt_state();ps("\n=== Demo Complete ===\n");return 0;}
