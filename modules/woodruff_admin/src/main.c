/* woodruff_admin: Woodruff management technology administration (v1.0)
 * Woodruff planning, woodruff execution, woodruff evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wd_t;
typedef struct{int n_wdp,n_wde,n_wdv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wd_state_t;
static wd_t wdps[N],wdss[N-2],wdvss[N-4],wdas[N-6],wdmks[N-6]; static wd_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wd_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wd_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WDB] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wd_init(void){if(init)return -1;st.n_wdp=0;st.n_wde=0;st.n_wdv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wdps[i].active=0;for(int i=0;i<N-2;i++)wdss[i].active=0;for(int i=0;i<N-4;i++)wdvss[i].active=0;for(int i=0;i<N-6;i++)wdas[i].active=0;for(int i=0;i<N-6;i++)wdmks[i].active=0;init=1;ps("[WDB] Woodruff initialized\n");return 0;}
int wd_planning(int t,int c,int a,int b,int d,int e,int y){return add(wdps,&st.n_wdp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wd_execution(int t,int c,int a,int b,int d,int e,int y){return add(wdss,&st.n_wde,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wd_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wdvss,&st.n_wdv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wd_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wdas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wd_market(int t,int c,int a,int b,int d,int e,int y){return add(wdmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wd_report(void){ps("[WDB] Wdp: ");pi(st.n_wdp);ps(" PCS=");pi(st.t_f1);ps("\nWde: ");pi(st.n_wde);ps(" PCS=");pi(st.t_f2);ps("\nWdv: ");pi(st.n_wdv);ps(" PCS=");pi(st.t_f3);ps("\nWdc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wd_state(void){ps("[WDB] Wdp=");pi(st.n_wdp);ps(" Wde=");pi(st.n_wde);ps(" Wdv=");pi(st.n_wdv);ps(" Wdc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Woodruff Admin Demo ===\n\n");wd_init();
ps("Woodruff planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wd_planning(t,c,695+(i*17),684+(i*14),664+(i*10),646+(i*6),2020+(i%5));}
ps("\nWoodruff execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wd_execution(t,c,684+(i*15),673+(i*12),655+(i*8),642+(i*5),2021+(i%4));}
ps("\nWoodruff evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wd_evaluation(t,c,676+(i*13),665+(i*10),649+(i*7),638+(i*4),2022+(i%3));}
ps("\nWoodruff accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wd_accessory(t,c,668+(i*11),659+(i*9),645+(i*6),635+(i*3),2023+(i%2));}
ps("\nWoodruff marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wd_market(t,c,662+(i*9),653+(i*7),640+(i*5),632+(i*3),2024);}
ps("\n");wd_report();wd_state();ps("\n=== Demo Complete ===\n");return 0;}
