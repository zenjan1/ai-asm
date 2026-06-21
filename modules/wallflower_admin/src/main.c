/* wallflower_admin: Wallflower management technology administration (v1.0)
 * Wallflower planning, wallflower execution, wallflower evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} wfr_t;
typedef struct{int n_wfp,n_wfe,n_wfv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} wfr_state_t;
static wfr_t wfrps[N],wfrss[N-2],wfrvss[N-4],wfras[N-6],wfrmks[N-6]; static wfr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(wfr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;wfr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[WFR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int wfr_init(void){if(init)return -1;st.n_wfp=0;st.n_wfe=0;st.n_wfv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)wfrps[i].active=0;for(int i=0;i<N-2;i++)wfrss[i].active=0;for(int i=0;i<N-4;i++)wfrvss[i].active=0;for(int i=0;i<N-6;i++)wfras[i].active=0;for(int i=0;i<N-6;i++)wfrmks[i].active=0;init=1;ps("[WFR] Wallflower initialized\n");return 0;}
int wfr_planning(int t,int c,int a,int b,int d,int e,int y){return add(wfrps,&st.n_wfp,&st.t_f1,N,t,c,a,b,d,e,y);}
int wfr_execution(int t,int c,int a,int b,int d,int e,int y){return add(wfrss,&st.n_wfe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int wfr_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(wfrvss,&st.n_wfv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int wfr_accessory(int t,int c,int a,int b,int d,int e,int y){return add(wfras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int wfr_market(int t,int c,int a,int b,int d,int e,int y){return add(wfrmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void wfr_report(void){ps("[WFR] Wfp: ");pi(st.n_wfp);ps(" PCS=");pi(st.t_f1);ps("\nWfe: ");pi(st.n_wfe);ps(" PCS=");pi(st.t_f2);ps("\nWfv: ");pi(st.n_wfv);ps(" PCS=");pi(st.t_f3);ps("\nWfc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void wfr_state(void){ps("[WFR] Wfp=");pi(st.n_wfp);ps(" Wfe=");pi(st.n_wfe);ps(" Wfv=");pi(st.n_wfv);ps(" Wfc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Wallflower Admin Demo ===\n\n");wfr_init();
ps("Wallflower planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;wfr_planning(t,c,879+(i*17),868+(i*14),848+(i*10),830+(i*6),2020+(i%5));}
ps("\nWallflower execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;wfr_execution(t,c,868+(i*15),857+(i*12),839+(i*8),826+(i*5),2021+(i%4));}
ps("\nWallflower evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;wfr_evaluation(t,c,860+(i*13),849+(i*10),833+(i*7),822+(i*4),2022+(i%3));}
ps("\nWallflower accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wfr_accessory(t,c,852+(i*11),843+(i*9),829+(i*6),819+(i*3),2023+(i%2));}
ps("\nWallflower marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;wfr_market(t,c,846+(i*9),837+(i*7),824+(i*5),816+(i*3),2024);}
ps("\n");wfr_report();wfr_state();ps("\n=== Demo Complete ===\n");return 0;}
